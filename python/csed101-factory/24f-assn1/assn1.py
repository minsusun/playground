import random

random.seed(42)

RANDOM_COIN_PRICES = [i * 100 for i in range(1, 11)]  # immutable

def play_investment():
    # initilizing
    loan_status = False
    initial_loan_amount = 0

    coin_prices = [0, 0, 0]  # 현재 코인의 시세
    current_coin_counts = [0, 0, 0]  # 현재 가지고 있는 코인의 개수

    cur_day = 1
    balance = 3000
    loan_amount = 0  # 현재 대출 총액

    while True:
        update_prices(coin_prices)
        while True:
            print_display(cur_day, balance, loan_amount, coin_prices, current_coin_counts)
            inputs = input().split()  # list
            if not inputs[0].isdigit() or int(inputs[0]) not in [1, 2, 3, 4, 5]:
                print("잘못된 입력입니다.")
                continue
            select = int(inputs[0])
            if select == 1:
                if len(inputs) != 3:
                    print("잘못된 입력입니다.")
                    continue
                if inputs[1] not in ['p', 'P', 'n', 'N', 'l', 'L']:
                    print("잘못된 입력입니다.")
                    continue
                if not inputs[2].isdigit() or int(inputs[2]) <= 0:
                    print("잘못된 입력입니다.")
                    continue
                count = int(inputs[2])
                if inputs[1] in ['p', 'P'] and balance < coin_prices[0] * count:
                    print("잔고가 부족합니다.")
                    continue
                if inputs[1] in ['n', 'N'] and balance < coin_prices[1] * count:
                    print("잔고가 부족합니다.")
                    continue
                if inputs[1] in ['l', 'L'] and balance < coin_prices[2] * count:
                    print("잔고가 부족합니다.")
                    continue
                balance = purchase_coin(
                    balance, inputs[1], count, coin_prices, current_coin_counts)
            if select == 2:
                if len(inputs) != 3:
                    print("잘못된 입력입니다.")
                    continue
                if inputs[1] not in ['p', 'P', 'n', 'N', 'l', 'L']:
                    print("잘못된 입력입니다.")
                    continue
                if not inputs[2].isdigit() or int(inputs[2]) <= 0:
                    print("잘못된 입력입니다.")
                    continue
                count = int(inputs[2])
                if inputs[1] in ['p', 'P'] and count > current_coin_counts[0]:
                    print("코인 개수가 부족합니다.")
                    continue
                if inputs[1] in ['n', 'N'] and count > current_coin_counts[1]:
                    print("코인 개수가 부족합니다.")
                    continue
                if inputs[1] in ['l', 'L'] and count > current_coin_counts[2]:
                    print("코인 개수가 부족합니다.")
                    continue
                balance = sell_coin(balance, inputs[1], count, coin_prices, current_coin_counts)
            if select == 3:
                if len(inputs) != 2:
                    print("잘못된 입력입니다.")
                    continue
                if not inputs[1].isdigit() or int(inputs[1]) <= 0:
                    print("잘못된 입력입니다.")
                    continue
                amount = int(inputs[1])  # 대출하고자하는 금액
                if amount == 0:
                    print("잘못된 입력입니다.")
                    continue
                if loan_status:
                    print("대출은 한 번만 가능합니다.")
                    continue
                if amount > 5000:
                    print("한도를 초과하였습니다.")
                    continue
                if not can_loan(loan_status, amount):
                    print("대출은 100원 단위로 가능합니다.")
                    continue
                balance, loan_amount = make_loan(amount, balance, loan_amount)
                loan_status = True
                initial_loan_amount = amount
            if select == 4:
                if len(inputs) != 2:
                    print("잘못된 입력입니다.")
                    continue
                if not inputs[1].isdigit() or int(inputs[1]) <= 0:
                    print("잘못된 입력입니다.")
                    continue
                repay_amount = int(inputs[1])
                if repay_amount == 0:
                    print("잘못된 입력입니다.")
                    continue
                if not can_repay(balance, loan_amount, repay_amount, loan_status):
                    continue
                balance, loan_amount = make_repay(
                    repay_amount, balance, loan_amount)
            if select == 5:
                if len(inputs) != 1:
                    print("잘못된 입력입니다.")
                    continue
                break
        cur_day += 1
        if cur_day == 6:
            break
        if loan_status and loan_amount > 0:
            loan_amount += initial_loan_amount // 10
    coin_total = coin_prices[0] * current_coin_counts[0] + coin_prices[1] * \
        current_coin_counts[1] + coin_prices[2] * current_coin_counts[2]
    check_result(balance, coin_total, loan_amount)


def update_prices(coin_prices):
    # coin_prices -> list[int]
    # [100, 500, 300]
    for i in range(len(coin_prices)):
        coin_prices[i] = random.choice(RANDOM_COIN_PRICES)


def print_display(cur_day, balance, loan_amount, coin_prices, current_coin_counts):
    # cur_day -> int
    # balance -> int
    # loan_amount -> int
    # current_coin_counts <- 포닉스, 넙죽이, 리오 순
    print("=======================================")
    print("[Coin Investment Simulation]")
    print("=======================================")
    print(f"[{cur_day}일차] 현재 잔액 : {balance}원, 대출 금액 : {loan_amount} 원")
    print(
        f"포닉스 코인 : {current_coin_counts[0]} ({coin_prices[0]} 원) - {current_coin_counts[0] * coin_prices[0]} 원")
    print(
        f"넙죽이 코인 : {current_coin_counts[1]} ({coin_prices[1]} 원) - {current_coin_counts[1] * coin_prices[1]} 원")
    print(
        f"리오  코인 : {current_coin_counts[2]} ({coin_prices[2]} 원) - {current_coin_counts[2] * coin_prices[2]} 원")
    print("=======================================")
    print("포닉스 코인: " + "■ " * (coin_prices[0]//100))
    print("넙죽이 코인: " + "■ " * (coin_prices[1]//100))
    print("리오 코인: " + "■ " * (coin_prices[2]//100))
    print("=======================================")
    print("1. 구매 2. 판매 3. 대출 4. 상환 5. 스킵")
    print("선택 : ", end='')


def purchase_coin(balance, code, count, coin_prices, current_coin_counts):
    # 이미 사용자가 해당 개수만큼 코인을 살 수 있다는 가정 하에 이 함수가 호출된다
    # code -> [nNpPlL]
    coin_price = 0
    if code == 'p' or code == 'P':
        coin_price = coin_prices[0]
        current_coin_counts[0] += count
    if code == 'n' or code == 'N':
        coin_price = coin_prices[1]
        current_coin_counts[1] += count
    if code == 'l' or code == 'L':
        coin_price = coin_prices[2]
        current_coin_counts[2] += count
    balance -= coin_price * count
    return balance


def sell_coin(balance, code, count, coin_prices, current_coin_counts):
    coin_price = 0
    if code == 'p' or code == 'P':
        coin_price = coin_prices[0]
        current_coin_counts[0] -= count
    if code == 'n' or code == 'N':
        coin_price = coin_prices[1]
        current_coin_counts[1] -= count
    if code == 'l' or code == 'L':
        coin_price = coin_prices[2]
        current_coin_counts[2] -= count
    balance += coin_price * count
    return balance


def can_loan(loan_status, amount):
    # loan_status -> bool 형 파라미터 false -> 대출을 아직 안한거 / true -> 대출을 이미 한거
    # 100원 단위
    # not / and or -> 낮은 우선순위
    # == !=  -> 더 높은 우선순위
    return (not loan_status) and (amount % 100 == 0)


def make_loan(amount, balance, loan_amount):
    balance += amount
    loan_amount += amount
    return balance, loan_amount


def can_repay(balance, loan_amount, amount, loan_status):
    if not loan_status:
        print("대출 이력이 없습니다.")
        return False
    if amount > balance or amount > loan_amount:
        print("잘못된 입력입니다.")
        return False
    if amount % 10 != 0:
        print("상환은 10원 단위로 가능합니다.")
        return False
    return True


def make_repay(amount, balance, loan_amount):
    balance -= amount
    loan_amount -= amount
    return balance, loan_amount


def check_result(balance, coin_total, loan_amount):
    final_amount = balance + coin_total - loan_amount
    print(f"최종 금액: {final_amount}")
    if final_amount > 3000:
        print("You Win!")
    else:
        print("You Lost...")

while True:
    play_investment()
    if input("Do you want to play another game? (yes/no): ") == "no":
        break
