from cs50 import get_float

while True:
    change = get_float("Change owed: ")
    if change > 0:
        break
cents = change * 100
#print(f"cents are {cents}")
quarters = 0
dimes = 0
coins = 0

while cents >= 25:
    quarters = cents // 25
    coins = coins + quarters
    cents = cents % 25
    #print(f"First: cents are {cents}")
while cents >= 10:
    dimes = cents // 10
    coins = coins + dimes
    cents = cents % 10
    #print(f"Second: cents are {cents}")
while cents >= 5:
    dimes = cents // 5
    coins = coins + dimes
    cents = cents % 5
    #print(f"Third: cents are {cents}")
while cents >= 1:
    dimes = cents // 1
    coins = coins + dimes
    cents = cents % 1
    #print(f"Fourth: cents are {cents}")
print("{:.0f}".format(coins))
# print(round(coins,0))