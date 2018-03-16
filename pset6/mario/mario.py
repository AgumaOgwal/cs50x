from cs50 import get_int
import sys

# looping to make sure correct height is supplied
while True:
    n = get_int("Height: ")
    if n == 0:
        sys.exit(0)
    if n > 0 and n < 24:
        break
# print(f"Height is {n}")

spaces = n - 1
for i in range(n):
    for j in range(spaces):
        print(" ", end="")
    for k in range(spaces, n):
        print("#", end="")
    print("#")
    spaces = spaces - 1