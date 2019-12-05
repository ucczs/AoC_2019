# 6 digit
# two same neighbours (3462_33_8)
# no decreasing numbers

min_password = 109_165
max_password = 576_723

max_numbers = max_password - min_password + 1

def check_adjacent_digits(number):
    number_str = str(number)
    for idx in range(len(number_str)):
        if idx == 0:
            continue
        else:
            if number_str[idx] == number_str[idx-1]:
                return True

    return False

def check_no_decrease(number):
    number_str = str(number)
    no_decrease = True
    for idx in range(len(number_str)):
        if idx == 0:
            continue
        else:
            if int(number_str[idx]) < int(number_str[idx - 1]):
                no_decrease = False

    return no_decrease

def check_for_big_group(number):
    number_str = str(number)
    for idx in range(len(number_str)):
        if idx == 0:
            continue
        else:
            if number_str[idx] == number_str[idx-1]:
                if idx == 1:
                    if number_str[idx] != number_str[idx+1]:
                        return True
                elif idx == 5:
                    if number_str[idx] != number_str[idx-2]:
                        return True
                else:
                    if number_str[idx] != number_str[idx+1] and number_str[idx] != number_str[idx-2]:
                        return True
    return False


passed_adjacent = []
passed_adjacent_decrease = []
passed_adjacent_decrease_bigGroup = []

for number in range(min_password, max_password + 1):
#for number in range(10, 1000):
    if check_adjacent_digits(number):
        passed_adjacent.append(number)

for number in passed_adjacent:
    if check_no_decrease(number):
        passed_adjacent_decrease.append(number)

for number in passed_adjacent_decrease:
    if check_for_big_group(number):
        passed_adjacent_decrease_bigGroup.append(number)

print(len(passed_adjacent_decrease_bigGroup))