import sys
from cs50 import SQL

db = SQL("sqlite:///students.db")

if len(sys.argv) != 2:
    print("Usage: python roster.py [Name of House]")
    sys.exit()


students = db.execute(f"SELECT * FROM students WHERE house = '{sys.argv[1]}' ORDER BY last, first ASC")

for row in students:
    if row['middle']:
        print(f"{row['first']} {row['middle']} {row['last']}, born {str(row['birth'])}")
    else:
        print(f"{row['first']} {row['last']}, born {str(row['birth'])}")