import sys
import random

# Generates a name by combining two pseudo-random adjectives/nouns from a static list
def generate_names(n):
    names = []

    for i in range(0, n):
        firstname = random.choice([f for f in open('adjectives.txt')]).replace('\n', '')
        lastname = random.choice([f for f in open('nouns.txt')]).replace('\n', '')
        names.append(firstname.title() + ' ' + lastname.title())

    return names


def print_names(names):
    for i in range(0, len(names)):
        print(names[i])
        

if __name__ == "__main__":
    if len(sys.argv[1:]) < 1:
        print('Please enter the number of names you\'d like to generate.')
        exit(1)
    else:
        try:
            print_names(generate_names(int(sys.argv[1])))
        except ValueError as e:
            print('Please input a single integer.')
            exit(1)
