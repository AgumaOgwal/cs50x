from cs50 import get_int


while True:
    n = get_int("Height: ")
    if n > 0 and n < 24:
        break
#print(f"Height is {n}")

spaces = n-1
for i in range(n):
    for j in range(spaces):
        print(" ", end="")
    
    for k in range(spaces, n):
        print("#", end="")
        
    print("#")
    spaces = spaces -1