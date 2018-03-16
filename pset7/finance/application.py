from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
from werkzeug.security import check_password_hash, generate_password_hash

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
    portfolio = db.execute("SELECT company, sum(shares) shares, sum(price) price FROM portfolio where userid = :userid group by company", userid=session["user_id"])
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
        port = {"company":row["company"], "shares":row["shares"], "total":totalest, "price":quotation["price"]}
        stocks.append(port)
    
    grandtotal = grandtotal + cash
    return render_template("index.html", username=username, cash=cash, stocks=stocks, total=grandtotal)
    
    return apology("TODO")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        #ensure a symbol was submitted
        if not request.form.get("symbol"):
            return apology("enter a company symbol",403)
        
        #ensure number of shares was submitted    
        elif not request.form.get("numberofshares"):
            return apology("must provide number of shares",403)
        
        #get quote
        quote = lookup(request.form.get("symbol"))
       
        #check if user can afford to buy the stock   
        rows = db.execute("SELECT * FROM users WHERE id = :userid", userid=session["user_id"])
        cash = rows[0]["cash"]
        
        stockvalue = int(quote["price"]) * int(request.form.get("numberofshares"))
        
        if stockvalue > cash:
            return apology("insufficient funds",403)
        
        #add stock to portfolio
        add_portfolio = db.execute("INSERT INTO portfolio(userid, username, company, shares, price) VALUES(:userid, :username, :company, :shares, :price)",userid=session["user_id"],username=rows[0]["username"], company=request.form.get("symbol"), shares=int(request.form.get("numberofshares")), price=stockvalue)
        
        #reduce cash accordingly
        update_row = db.execute("UPDATE users SET cash = cash - :cash where id = :id",cash=stockvalue, id=session["user_id"])
        
        #return render_template("success.html", name=rows[0]["username"], symbol=request.form.get("symbol"), price=stockvalue, shares=request.form.get("numberofshares"))
        return redirect("/")
        
    else:
        return render_template("buy.html")
        
    return apology("TODO")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
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
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

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
            return apology("must provide symbol", 403)
        
        quote = lookup(request.form.get("symbol"))
        
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
            return apology("must provide username", 403)
        elif not request.form.get("password"):
            return apology("must provide password", 403)
        elif not request.form.get("password2"):
            return apology("must re-enter password",403)
        elif request.form.get("password") != request.form.get("password2",403):
            return apology("passwords dont match", 403)
            
        password = generate_password_hash(request.form.get("password"))
        #uname = request.form.get("username")
        
        #db.execute("INSERT INTO users (username, hash) VALUES(%s,%s)",(username,password))
        #db.commit()
        
         # Query database for username
        #rows = db.execute("SELECT * FROM users WHERE username = :username",
        #                  username=username)

        # Ensure username exists and password is correct
        #if len(rows) != 1 or not check_password_hash(rows[0]["hash"], password):
        #    return apology("invalid username and/or password", 403)

        #sql = ''' INSERT INTO users(username, hash) VALUES(?,?) '''
        #items = [(uname, password)]
        # Remember which user has logged in
        key = db.execute("INSERT INTO users(username, hash) VALUES(:username, :hash)",username=request.form.get("username"), hash=password)
        #db.commit()
        
        session["user_id"] = key

        # Redirect user to home page
        return redirect("/")
        
    else:
        return render_template("register.html")        
    
        
    #return apology("TODO")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        #todo
        boss = "Okay"
    else:
        render_template("sell.html")
    return apology("TODO")


def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)


# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
