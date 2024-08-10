import random

random_seed = 2024

WORD_LENGTH = 5
DEFAULT_LIFE = HISTORY_LENGTH = 6

COLOR_CODE_GREEN = 42
COLOR_CODE_YELLOW = 43
COLOR_CODE_GRAY = 47
COLOR_CODE_RESET = 0

def color_word(word, color):
    word = " " + word + " "
    return f"\033[{color}m{word}\033[{COLOR_CODE_RESET}m"

def print_status(history, life, answer):
    print("--------------------")
    print("History:")
    for word in history:
        print(get_colored_string(word, answer))
    for _ in range(HISTORY_LENGTH - len(history)):
        print(" _ " * WORD_LENGTH)
    print()
    print("Life:", life)
    print("--------------------")

# word, answer -> len == 5
# lower case
def get_colored_string(word, answer):
    assert len(word) == len(answer) == WORD_LENGTH

    result = ""
    for i, j in zip(word, answer):
        if i == j:
            result += color_word(i.upper(), COLOR_CODE_GREEN)
        elif i in answer:
            result += color_word(i.upper(), COLOR_CODE_YELLOW)
        else:
            result += color_word(i.upper(), COLOR_CODE_GRAY)
    return result

def is_valid(word, word_list, history):
    if len(word) != WORD_LENGTH:
        print("Error: Input word is not a five-letter word!")
        print("Choose another word!")
        return False
    if word not in word_list:
        print("Error: Input word is not in the word list!")
        print("Choose another word!")
        return False
    if word in history:
        print("Error: Input word has already been typed!")
        print("Choose another word!")
        return False
    return True

def is_correct(word, answer):
    return word == answer

#------------------------------------------------------------------------------------------------------------
# Below function is a reference.
# Do not fix the reference function below.
# Use this function to implement the wordle game.
# Refer to comments in this function when you write your own comments.

def load_word_list(path):
    """
    Args:
        path: path to text file which contains answers
    Return:
        word_list: list of words which contains answers
    """
    
    # Load the text file from path
    f = open(path, "r")
    lines = f.readlines()
    f.close()
    
    # Delete newline ('\n') in the list
    word_list = []
    for line in lines:
        word_list.append(line.strip())
    
    # Return list of words
    return word_list


#------------------------------------------------------------------------------------------------------------

random.seed(random_seed)

word_list = load_word_list("./word_list.txt")

print("Wordle game starts!")
print()

wordle = random.choice(word_list)
life = DEFAULT_LIFE
history = []

while True:

    print_status(history, life, wordle)

    word = ""

    while True:
        word = input("Type a word: ").strip().lower()
        if is_valid(word, word_list, history):
            break

    history.append(word)

    if word == wordle:
        print_status(history, life, wordle)
        print("Great! You got the answer")
        print("The answer was", wordle)
    else:
        life -= 1
        if life == 0:
            print_status(history, life, wordle)
            print("Too bad! You miss the answer")
            print("The answer was", wordle)
        else:
            continue

    play = input("Do you want to play more games? (yes/no): ").strip()
    print()
    if play == "yes":
        wordle = random.choice(word_list)
        life = DEFAULT_LIFE
        history = []
        continue
    else:
        break

print("See you next!")