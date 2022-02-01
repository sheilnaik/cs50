from cs50 import get_string


def is_amex(number):
    if number[:2] in ['34', '37'] and len(number) == 15:
        return True
    return False


def is_mastercard(number):
    if number[:2] in ['51', '52', '53', '54', '55'] and len(number) == 16:
        return True
    return False


def is_visa(number):
    if number[:1] == '4' and (len(number) == 13 or len(number) == 16):
        return True
    return False


def pass_lunh(number):
    # 1. Multiply every other digit by 2, starting with the number’s second-to-last digit, and then add those products’ digits together.
    digit_sum_1 = 0

    for i in range(len(number) - 2, -1, -2):
        # Double each number
        doubled = int(number[i]) * 2

        # Add each digit in the doubled number to the rolling sum
        for x in str(doubled):
            digit_sum_1 += int(x)

    # 2. Add the sum to the sum of the digits that weren’t multiplied by 2.
    digit_sum_2 = 0

    for i in range(len(number) - 1, -1, -2):
        digit_sum_2 += int(number[i])

    # 3. If the total’s last digit is 0 (or, put more formally, if the total modulo 10 is congruent to 0), the number is valid!
    digit_sums = digit_sum_1 + digit_sum_2

    if str(digit_sums)[1] == '0':
        return True

    return False


if __name__ == '__main__':
    number = get_string("Number: ")

    if pass_lunh(number):
        if is_amex(number):
            print('AMEX')
        elif is_mastercard(number):
            print('MASTERCARD')
        elif is_visa(number):
            print('VISA')
        else:
            print('INVALID')
    else:
        print('INVALID')