from cs50 import get_string
import re


def coleman_liau(text):
    letters_per_100_words = (num_letters(text) * 100) / num_words(text)
    sentences_per_100_words = (num_sentences(text) * 100) / num_words(text)

    grade_level = int(round(0.0588 * letters_per_100_words - 0.296 * sentences_per_100_words - 15.8, 0))

    if grade_level >= 16:
        return "Grade 16+"
    elif grade_level < 1:
        return "Before Grade 1"
    else:
        return f"Grade {grade_level}"


def num_letters(text):
    letter_count = 0

    for character in text:
        if re.search("[a-zA-Z]", character):
            letter_count += 1

    return letter_count


def num_words(text):
    word_count = 0

    for character in text:
        if re.search("\s", character):
            word_count += 1

    return word_count + 1


def num_sentences(text):
    sentence_count = 0

    for character in text:
        if character in ['.', '!', '?']:
            sentence_count += 1

    return sentence_count


if __name__ == "__main__":
    text = get_string("Text: ")

    print(coleman_liau(text))