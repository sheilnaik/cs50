import sys
import csv
from cs50 import SQL

if len(sys.argv) != 2:
    print("Usage: python import.py filename.csv")
    sys.exit()

filename = sys.argv[1]
db = SQL("sqlite:///students.db")

with open(filename) as f:
    reader = csv.DictReader(f)

    for row in reader:
        name_split = row['name'].split(' ')

        if len(name_split) == 3:
            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)",
                       name_split[0], name_split[1], name_split[2], row["house"], int(row["birth"]))
        else:
            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)",
                       name_split[0], None, name_split[1], row["house"], int(row["birth"]))