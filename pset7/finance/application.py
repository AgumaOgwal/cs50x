from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
from werkzeug.security import check_password_hash, generate_password_hash
import sqlite3

#the below for debugging
import sys

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

#to help with front end formating of currency
app.jinja_env.globals.update(usd=usd)

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    
    #first, get all necessary info
    userinfo = db.execute("SELECT * FROM users WHERE id = :userid", userid=session["user_id"])
    #select company, sum(shares) shares, sum(price) from portfolio group by company;
    #portfolio = db.execute("SELECT company, companyname, sum(shares) shares, sum(price) price FROM portfolio where userid = :userid group by company", userid=session["user_id"])
    #portfolio = db.execute("SELECT company, totalshares shares, totalcost price FROM totals where userid = :userid", userid=session["user_id"])
    
    portfolio = db.execute("SELECT company, totalshares shares, totalcost price FROM totals where userid = :userid and totalshares > 0", userid=session["user_id"])
    
    username = userinfo[0]["username"]
    cash = userinfo[0]["cash"]
    stocks = []
    grandtotal = 0
    for row in portfolio:
        quotation = lookup(row["company"])
        current_price = quotation["price"]
        total = float(current_price) * int(row["shares"])
        totalest = round(total, 2)
        grandtotal = grandtotal + totalest
        port = {"company":row["company"], "companyname":row["company"], "shares":row["shares"], "total":totalest, "price":quotation["price"]}
        stocks.append(port)
    
    grandtotal = grandtotal + cash
    grandtotalest = round(grandtotal, 2)
    return render_template("index.html", username=username, cash=cash, stocks=stocks, total=grandtotalest)
    
    return apology("TODO")
    

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        # ensure a symbol was submitted
        if not request.form.get("symbol"):
            return apology("enter a company symbol",400)
        
        #ensure number of shares was submitted    
        elif not request.form.get("shares"):
            return apology("must provide number of shares",400)
        
        
        #get quote
        quote = lookup(request.form.get("symbol"))
        numshares = request.form.get("shares")
        
        #the below to help with check50 entering invalid share number (fractions, negatives)
        try:
            value = int(numshares)
        except ValueError:
            return apology("Please Enter numerical shares",400)
        
        if int(numshares) < 0:
            return apology("Please Enter positive value",400)
            
        if not quote:
            return apology("Company not found", 400)
            
        #check if user can afford to buy the stock   
        rows = db.execute("SELECT * FROM users WHERE id = :userid", userid=session["user_id"])
        cash = rows[0]["cash"]
        
        stockvalue = int(quote["price"]) * int(numshares)
        
        if stockvalue > cash:
            return apology("insufficient funds",400)
        
        #add stock to portfolio
        add_portfolio = db.execute("INSERT INTO portfolio(userid, username, company, companyname, shares, price) VALUES(:userid, :username, :company, :companyname, :shares, :price)",userid=session["user_id"], username=rows[0]["username"], company=quote["name"], companyname=quote["name"], shares=numshares, price=stockvalue)
        
        #add stock to collated table.
        #first check if symbol already exists in table, then update. if not insert
        check_if_already_exists_collated = db.execute("SELECT * FROM totals WHERE userid = :userid and company = :company",userid=session["user_id"], company=request.form.get("symbol"))
        shares=int(request.form.get("shares"))
        
        if not check_if_already_exists_collated:
            add_collated = db.execute("INSERT INTO totals(userid, username, company, totalshares, totalcost) VALUES(:userid, :username, :company, :totalshares, :totalcost)",userid=session["user_id"], username=rows[0]["username"], company=quote["name"], totalshares=shares, totalcost=stockvalue)
        else:
            print("Rows to SHow", file=sys.stdout)
            update_collated = db.execute("UPDATE totals set totalshares = totalshares + :totalshares, totalcost = totalcost + :totalcost where userid = :userid and company = :company",totalshares=shares, totalcost=stockvalue, userid=session["user_id"], company=quote["name"])
        
        #reduce cash accordingly
        update_row = db.execute("UPDATE users SET cash = cash - :cash where id = :id",cash=stockvalue, id=session["user_id"])
        
        #log transaction
        log_transaction = db.execute("INSERT INTO history(userid, username, company, shares, price, transactiontype) VALUES(:userid, :username, :company, :shares, :price, :type)",userid=session["user_id"], username=rows[0]["username"], company=quote["name"], shares=numshares, price=quote["price"], type="BUY")
        
        #return render_template("success.html", name=rows[0]["username"], symbol=request.form.get("symbol"), price=stockvalue, shares=request.form.get("numberofshares"))
        return redirect("/")
        
    else:
        return render_template("buy.html")
        
    return apology("TODO")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    queryHistory = db.execute("SELECT * FROM history where userid = :userid order by date desc", userid=session["user_id"])
    
    if not queryHistory:
        return apology("No History Found")
    else:
        history = []
        for row in queryHistory:
            port = {"company":row["company"], "shares":row["shares"], "price":row["price"], "date":row["date"], "type":row["transactiontype"]}
            history.append(port)
        
        return render_template("history.html", history=history)
    return apology("TODO")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 400)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)
        
        quote = lookup(request.form.get("symbol"))
        
        if not quote:
            return apology("Symbol not found", 400)
        else:
            return render_template("stock.html",name=quote["name"], price=quote["price"], symbol=quote["symbol"])
        
    else:
        return render_template("quote.html")
        
    return apology("TODO")


@app.route("/register", methods=["GET", "POST"])
def register():
    
    session.clear()
    """Register user"""
    if request.method == "POST":
        if not request.form.get("username"):
            return apology("must provide username", 400)
        elif not request.form.get("password"):
            return apology("must provide password", 400)
        elif not request.form.get("confirmation"):
            return apology("must re-enter password", 400)
        elif request.form.get("password") != request.form.get("confirmation", 400):
            return apology("passwords dont match", 400)
        password = generate_password_hash(request.form.get("password"))
        
        userExist = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))
        
        if userExist:
            return apology("User Already Exists", 400)
        key = db.execute("INSERT INTO users(username, hash) VALUES(:username, :hash)",username=request.form.get("username"), hash=password)
        #db.commit()
        
        session["user_id"] = key

        # Redirect user to home page
        return redirect("/")
        
    else:
        return render_template("register.html")       


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        #todo
        if not request.form.get("symbol"):
            return apology("must select company", 403)
        elif not request.form.get("shares"):
            return apology("must provide number of shares", 403)
            
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        rows = db.execute("SELECT * FROM portfolio where userid = :userid and company = :company group by company", userid=session["user_id"], company=symbol)
        rowdy = db.execute("SELECT sum(shares) as shares FROM portfolio where userid = :userid and company = :company group by company",userid=session["user_id"], company=request.form.get("symbol"))
        
        if int(rowdy[0]["shares"]) < int(shares) or int(shares) < 0:
            return apology("Not enough shares to sell", 403)
        
        else:
            quote = lookup(symbol)
            price = quote["price"]
            symbol = quote["symbol"]
            print(quote["symbol"], file=sys.stdout)
            print(quote["price"], file=sys.stdout)
            cost = int(shares) * price
            print(cost, file=sys.stdout)
            update_collated = db.execute("UPDATE totals set totalshares = totalshares - :shares, totalcost = totalcost - :cost where userid = :userid and company = :company",shares=shares, cost=cost, userid=session["user_id"], company=quote["name"])    
            
            # reduce cash accordingly
            update_row = db.execute("UPDATE users SET cash = cash + :cost where id = :id",cost=cost, id=session["user_id"])
            
            #log transaction
            log_transaction = db.execute("INSERT INTO history(userid, username, company, shares, price, transactiontype) VALUES(:userid, :username, :company, :shares, :price, :type)",userid=session["user_id"], username=rows[0]["username"], company=quote["name"], shares=shares, price=quote["price"], type="SELL")
        
            return redirect("/")
        
    else:
        # load all stocks currently held
        portfolio = db.execute("SELECT * FROM totals where userid = :userid and totalshares > 0", userid=session["user_id"])
        stocky = []
        for row in portfolio:
            stocky.append(row["company"])
        return render_template("sell.html", stocks=stocky)
    return apology("TODO")


def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)


# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
