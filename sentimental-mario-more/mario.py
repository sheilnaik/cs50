from cs50 import get_int

while True:
    height = get_int("Height: ")
    if height >= 1 and height <= 8:
        break

# Create 'height' number of lines for the pyramid
for y in range(1, height + 1):

    # Create first blank spaces
    print(" " * (height - y), end="")

    # Create first hash signs
    print("#" * y, end="")

    # Leave two spaces in between
    print("  ", end="")

    # Create second hash marks
    print("#" * y, end="")

    # Go to next line
    print()