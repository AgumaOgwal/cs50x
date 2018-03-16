from cs50 import get_string
import sys

# simply to ensure a commandline argument has been supplied
if len(sys.argv) != 2:
    print("Usage: ./caesar k")
    # exit the program with code 1
    sys.exit(1)
key = int(sys.argv[1])

text = get_string("plaintext: ")

print("ciphertext: ", end="")

# start printing the ciphertext. using end="" to ensure no newline characters are used
for i in range(len(text)):
    letter = text[i]
    cipher = ""
    if letter.isupper():
        # ord gets int value of a char
        letter = ord(letter) - 65
        cipher = (letter + key) % 26
        cipher = cipher + 65
        print(chr(cipher), end="")
    elif letter.islower():
        letter = ord(letter) - 97
        cipher = (letter + key) % 26
        cipher = cipher + 97
        print(chr(cipher), end="")
    else:
        print(letter, end="")
    # the below is a simple way of printing a new line
print()