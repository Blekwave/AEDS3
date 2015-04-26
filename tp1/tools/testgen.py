from random import randint, choice, random
from operator import itemgetter
import sys
import string
import argparse

def gen_valid():
    return [randint(1,NUM_TANKS), randint(0, MAX_VALID_COORD),
        randint(0, MAX_VALID_COORD)]

def attack_to_str(tanks, x, y):
    return ("p" + "o" * tanks + "int(", str(x) + "," + str(y) + ")")

def add_comma(p2_str):
    index = randint(0, len(p2_str) - 1)
    if index == 0:
        return ","
    return p2_str[:index] + "," + p2_str[index:p2_str.find(',') + 1]

def add_open_parens(p2_str):
    index = randint(0, len(p2_str) - 1)
    return p2_str[:index] + "("

def valid_noise_charsets():
    p1_charsets = [list(string.printable), list(string.printable),
                   list(string.printable), list(string.printable),
                   list(string.printable), list(string.printable)]
    p1_charsets[0].remove('p')
    p1_charsets[1].remove('o')
    p1_charsets[2].remove('o')
    p1_charsets[2].remove('i')
    p1_charsets[3].remove('n')
    p1_charsets[4].remove('t')
    p1_charsets[5].remove('(')
    p2_charset = list(string.printable)
    p2_charset.remove('(')
    p2_charset.remove(')')
    p2_charset.remove(',')
    for i in range(0,10):
        p2_charset.remove(str(i))
    return p1_charsets, p2_charset


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("num_attacks", type=int,
        help="number of valid attacks to be generated")
    parser.add_argument("-m", "--memory", type=int, default=30,
        help="maximum amount of memory to be used, in MB (default: %(default)sMB)")
    parser.add_argument("-o", "--output", action="store_true",
        help="generates and prints the expected output to stderr")
    parser.add_argument("-n", "--noise", type=float, default=0.5,
        help="proportion of noise to content (default: %(default)s)")
    parser.add_argument("-i", "--invalid", type=float, default=0.2,
        help="proportion of invalid inputs to valid ones (default: %(default)s)")
    parser.add_argument("-t", "--tanks", type=int, default=40,
        help="maximum amount of tanks per input (default: %(default)s)")
    parser.add_argument("-c", "--coordmax", type=int, default=99999,
        help="maximum value for coordinates (default: %(default)s)")
    args = parser.parse_args()


    MAX_VALID_COORD = args.coordmax
    NUM_TANKS = args.tanks
    GEN_OUTPUT = args.output
    INVALID_PROP = args.invalid
    NOISE_PROP = args.noise
    P1_CHARSETS, P2_CHARSET = valid_noise_charsets()

    num_attacks = args.num_attacks

    print(args.memory) # available memory

    base = (randint(0, MAX_VALID_COORD), randint(0, MAX_VALID_COORD))
    print(*base) # base coordinates

    if GEN_OUTPUT:
        attacks = []

    while num_attacks:
        attack = gen_valid()

        if random() < INVALID_PROP: # invalid
            inv_type = randint(1,4)
            if inv_type == 1:
                attack[1] = randint(MAX_VALID_COORD + 1, 999999999999)
            elif inv_type == 2:
                attack[2] = randint(MAX_VALID_COORD + 1, 999999999999)
            p1_str, p2_str = attack_to_str(*attack)
            if inv_type == 1:
                p2_str = p2_str[:6] # 123098|23,2430857
            elif inv_type == 2:
                p2_str = p2_str[:p2_str.find(',') + 7] #99999,812394|2094
            elif inv_type == 3:
                p2_str = add_comma(p2_str)
            else: # type == 4
                p2_str = add_open_parens(p2_str)
        else:
            if GEN_OUTPUT:
                attacks.append(attack)
            p1_str, p2_str = attack_to_str(*attack)
            num_attacks -= 1
        pos = 0

        while pos < len(p1_str):
            if random() < NOISE_PROP:
                if pos < 3:
                    curr_charset = P1_CHARSETS[pos]
                elif pos < (2 + attack[0]):
                    curr_charset = P1_CHARSETS[2]
                else:
                    curr_charset = P1_CHARSETS[pos - attack[0] + 1]
                sys.stdout.write(choice(curr_charset))
            else:
                sys.stdout.write(p1_str[pos])
                pos += 1

        pos = 0
        while pos < len(p2_str):
            if random() < NOISE_PROP:
                sys.stdout.write(choice(P2_CHARSET))
            else:
                sys.stdout.write(p2_str[pos])
                pos += 1

    sys.stdout.write("\n")

    if GEN_OUTPUT:
        attacks.sort(key=lambda a: (base[0] - a[1])**2 + (base[1] - a[2])**2)
        attacks.sort(key=itemgetter(0), reverse=True)
        for attack in attacks:
            print('{0};({1},{2})'.format(*attack), file=sys.stderr)