import copy


def initialize_board(n):
    board = [[] for _ in range(n)]
    for i in range(n):
        for _ in range(n):
            board[i].append(0)
    board[0][0] = 2
    board[-1][0] = 2
    return board


def print_board(board):
    print("+" + "-----+" * len(board))
    for row in board:
        print("|", end="")
        for elem in row:
            if elem == 0:
                print("     ", end="")
            else:
                print("%5d" % elem, end="")
            print("|", end="")
        print()
        print("+" + "-----+" * len(row))


def move_left(board):
    _board = copy.deepcopy(board)
    for i in range(len(_board)):
        aux_row = []
        result_row = []

        for elem in _board[i]:
            if elem != 0:
                aux_row.append(elem)

        idx = 0
        while idx < len(aux_row):
            if idx < len(aux_row) - 1 and aux_row[idx] == aux_row[idx + 1]:
                result_row.append(aux_row[idx] * 2)
                idx += 2
            else:
                result_row.append(aux_row[idx])
                idx += 1

        _board[i] = result_row + [0 for _ in range(len(_board[i]) - len(result_row))]
    return _board


def move_right(board):
    _board = copy.deepcopy(board)
    for i in range(len(_board)):
        aux_row = []
        result_row = []

        for elem in _board[i]:
            if elem != 0:
                aux_row.append(elem)

        idx = len(aux_row) - 1
        while idx >= 0:
            if idx > 0 and aux_row[idx] == aux_row[idx - 1]:
                result_row.append(aux_row[idx] * 2)
                idx -= 2
            else:
                result_row.append(aux_row[idx])
                idx -= 1

        _board[i] = [0 for _ in range(len(_board[i]) - len(result_row))] + result_row[
            ::-1
        ]
    return _board


def move_up(board):
    _board = copy.deepcopy(board)
    for i in range(len(_board)):
        aux_col = []
        result_col = []

        for j in range(len(_board)):
            if _board[j][i] != 0:
                aux_col.append(_board[j][i])

        idx = 0
        while idx < len(aux_col):
            if idx < len(aux_col) - 1 and aux_col[idx] == aux_col[idx + 1]:
                result_col.append(aux_col[idx] * 2)
                idx += 2
            else:
                result_col.append(aux_col[idx])
                idx += 1

        result_col = result_col + [0 for _ in range(len(_board[i]) - len(result_col))]
        for j in range(len(_board)):
            _board[j][i] = result_col[j]
    return _board


def move_down(board):
    _board = copy.deepcopy(board)
    for i in range(len(_board)):
        aux_col = []
        result_col = []

        for j in range(len(_board)):
            if _board[j][i] != 0:
                aux_col.append(_board[j][i])

        idx = len(aux_col) - 1
        while idx >= 0:
            if idx > 0 and aux_col[idx] == aux_col[idx - 1]:
                result_col.append(aux_col[idx] * 2)
                idx -= 2
            else:
                result_col.append(aux_col[idx])
                idx -= 1

        result_col = [0 for _ in range(len(_board[i]) - len(result_col))] + result_col[
            ::-1
        ]
        for j in range(len(_board)):
            _board[j][i] = result_col[j]
    return _board


def check_win(board):
    for row in board:
        for elem in row:
            if elem == 2048:
                return True
    return False


def check_lose(board, current_move):
    if not check_corners(board):
        print("No empty corners. You Lose...")
        return True

    _board = copy.deepcopy(board)
    _board = generate_new_tile(_board, current_move)
    next_board = move_up(_board)
    if next_board != _board:
        return False
    next_board = move_left(_board)
    if next_board != _board:
        return False
    next_board = move_down(_board)
    if next_board != _board:
        return False
    next_board = move_right(_board)
    if next_board != _board:
        return False
    print_board(_board)
    print()
    print("You Lose...")
    return True


def check_corners(board):
    # True: 사용 가능, False: 사용 불가능
    return not (
        board[0][0] != 0
        and board[0][-1] != 0
        and board[-1][0] != 0
        and board[-1][-1] != 0
    )


def generate_new_tile(board, current_move):
    # 현재 생성 가능한 타일이 있음을 가정
    if board[0][0] == 0:
        board[0][0] = 2 if current_move else 4
    elif board[0][-1] == 0:
        board[0][-1] = 2 if current_move else 4
    elif board[-1][-1] == 0:
        board[-1][-1] = 2 if current_move else 4
    elif board[-1][0] == 0:
        board[-1][0] = 2 if current_move else 4
    return board


def load_game(filename):
    with open(filename) as f:
        n = int(f.readline().strip())
        board = initialize_board(n)
        current_move = int(f.readline().strip())
        for i in range(n):
            segments = f.readline().strip().split()
            for j in range(len(segments)):
                board[i][j] = int(segments[j])
    return board, current_move


while True:
    print("[2048 Game]")
    print("----------------------------------------")
    print(" 1. New Game    2. Load Game    3. Exit")
    print("----------------------------------------")

    while True:
        print("Select a menu: ", end="")

        user_input = input().strip()

        if user_input != "1" and user_input != "2" and user_input != "3":
            print("Please select a valid option (1, 2, or 3).\n")
            continue
        else:
            break

    if user_input == "3":
        print("Exiting the game. Goodbye!")
        break

    if user_input == "1":
        print()

        while True:
            print("What size grid would you like to play 2048 on? (e.g., 4 for 4x4): ")
            user_input = input().strip()
            try:
                n = int(user_input)
            except:
                n = 0
            if n >= 8 or n < 3:
                print("Please enter a number between 3 and 7.")
            else:
                break

        board = initialize_board(n)
        current_move = 1
    elif user_input == "2":
        print()
        print("Enter the filename to load your game: ")

        user_input = input().strip()

        try:
            f = open(user_input)
        except:
            print("File not found.\n")
            continue
        else:
            f.close()

        board, current_move = load_game(user_input)

    game_status = False

    while True:
        print_board(board)
        print()
        print("Enter move (W/A/S/D to move, Q to quit): ", end="")

        user_input = input().strip().lower()

        if user_input not in ["w", "a", "s", "d", "q"]:
            print("Wrong input. Try again.")
            continue

        if user_input == "w":
            next_board = move_up(board)
        elif user_input == "a":
            next_board = move_left(board)
        elif user_input == "s":
            next_board = move_down(board)
        elif user_input == "d":
            next_board = move_right(board)
        elif user_input == "q":
            print("Enter filename to save and exit: ", end="")
            user_input = input().strip()
            with open(user_input, "w") as f:
                f.write(f"{n}\n")
                f.write(f"{current_move}\n")
                for row in board:
                    for elem in row:
                        f.write(f"{elem} ")
                    f.write("\n")
            print("Game saved. Goodbye!")
            game_status = True
            break

        if next_board == board:
            print("No tiles moved. Try again.")
            continue

        board = next_board

        if check_win(board):
            print_board(board)
            print()
            print("You Win!")
            game_status = True
            break

        if check_lose(board, current_move):
            game_status = True
            break

        board = generate_new_tile(board, current_move)
        current_move = int(not current_move)

    if game_status:
        break
