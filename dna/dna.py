import sys
import csv


def count_repeats(repeat, sequence):
    repeat_count = 0
    repeat_conseq = repeat

    while True:
        if repeat_conseq in sequence:
            repeat_count += 1
        else:
            break
        repeat_conseq += repeat

    return repeat_count


if __name__ == "__main__":
    # Check for the correct number of arguments
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")

    database_filename = sys.argv[1]
    sequence_filename = sys.argv[2]
    people = []

    with open(sequence_filename) as sequence_file:
        sequence = sequence_file.read()

    with open(database_filename) as database_file:
        reader = csv.DictReader(database_file)
        for row in reader:
            people.append(row)

    list_of_repeats = [key for key in people[0].keys()]
    list_of_repeats.remove('name')

    for person in people:
        num_of_matched_repeats = 0

        for repeat in list_of_repeats:
            repeat_count = count_repeats(repeat, sequence)

            if int(person[repeat]) == repeat_count:
                num_of_matched_repeats += 1

        if num_of_matched_repeats == len(list_of_repeats):
            print(person['name'])
            sys.exit()

    print("No match")
    sys.exit()
