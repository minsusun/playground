import random


# Imported from hint.py
def print_custom_map(enemies, helpers, mystery, pos):
    # Input
    # 1) enemies: Enemy 위치들의 리스트 (2 이상 14 사이의 자연수로 구성)
    # 2) helpers: Helper 위치들의 리스트 (2 이상 14 사이의 자연수로 구성)
    # 3) mystery: Mystery 위치 (2 이상 14 사이의 자연수)
    # 4) pos: 플레이어의 위치 (1 이상 15 사이의 자연수)
    code_dic = {"Enemy": 31, "Helper": 34, "Mystery": 32, "ESC": 0}

    def get_colored_text(code, text):
        return f"\033[1;{code}m{text}\033[0m"

    # 플레이어 현재 위치에 따른 색상 코드 설정
    pos_code = (
        code_dic["Enemy"]
        if pos in enemies
        else (
            code_dic["Helper"]
            if pos in helpers
            else code_dic["Mystery"] if pos == mystery else code_dic["ESC"]
        )
    )

    # 플레이어 행별 텍스트 정의
    elements = [
        (" \\o/  ", "  o   "),  # 머리
        ("  |   ", " /|\\  "),  # 몸통
        (" / \\ ", " / \\  "),  # 다리
    ]

    # 출력을 위한 행 리스트 생성
    rows = [[" " * 6 for _ in range(15)] for _ in range(3)]

    # 플레이어의 현재 위치에 따른 요소 출력 설정
    for i, (current, default) in enumerate(elements):
        rows[i][pos - 1] = get_colored_text(pos_code, current if pos == 15 else default)

    # Enemy, Helper, Mystery 캐릭터 출력 설정
    for p in enemies:
        if p != pos:
            rows[1][p - 1] = get_colored_text(code_dic["Enemy"], " |E|  ")
    for p in helpers:
        if p != pos:
            rows[1][p - 1] = get_colored_text(code_dic["Helper"], " |H|  ")
    if mystery != pos:
        rows[1][mystery - 1] = get_colored_text(code_dic["Mystery"], " |?|  ")

    # 각 행 출력
    for row in rows:
        print("".join(row))

    # 바닥 출력
    print("===== " * 15)


class Dashboard:
    # (Player, Map)
    def __init__(self):
        self.info = []

    def show_dashboard(self):
        print("========================== DASH BOARD ==========================")
        print("Name            Map             Life            Status")
        print("================================================================")
        for row in self.info:
            map_name = "Hard" if isinstance(row[1], HardMap) else "Easy"
            print(
                "%-16s%-16s%-16d%s"
                % (row[0].name, map_name, row[0].life, row[0].status)
            )
        print("================================================================")

    def run(self):
        while True:
            self.show_dashboard()

            print("다음 중 하나를 고르시오.")
            print("1. 새로운 플레이어 및 맵 생성")
            print("2. 플레이어 선택 및 게임 진행")
            print("3. 종료하기")
            user_input = input("> ").strip()
            print()

            if user_input == "1":
                print("플레이어 이름을 입력하세요.")
                user_name = input("> ").strip()
                print()

                result = list(filter(lambda x: x[0].name == user_name, self.info))
                if len(result) != 0:
                    print("이미 존재하는 플레이어입니다.")
                    print()
                    continue

                print("맵을 선택하세요.")
                print("1. Easy")
                print("2. Hard")
                user_map = input("> ").strip()
                print()

                player = Player(user_name)
                if user_map == "1":
                    map = EasyMap(player)
                elif user_map == "2":
                    map = HardMap(player)

                self.info.append((player, map))

                print(
                    f"플레이어 {user_name}(맵 : {'Easy' if user_map == '1' else 'Hard'})가 추가 되었습니다."
                )
            elif user_input == "2":
                print("플레이어 이름을 입력하세요.")
                user_name = input("> ").strip()
                print()

                result = list(filter(lambda x: x[0].name == user_name, self.info))
                if len(result) == 0:
                    print("존재하지 않는 플레이어입니다.")
                    print()
                    continue

                result[0][1].run()

            elif user_input == "3":
                break


class Enemy:
    def __init__(self, name, pos):
        self.name = name
        self.pos = pos

    def attack(self, player):
        a, b = [random.randint(10, 99) for _ in range(2)]
        print(f"{self.name} 이 문제를 냅니다.. {a} + {b} = ?")
        c = int(input().strip())
        if a + b != c:
            print(f"[Wrong] {self.name}에게 라이프 2개를 빼앗겼습니다.")
            player - 2
        else:
            print(f"[Correct] {self.name}이 인사하며 떠납니다.")


class Helper:
    def __init__(self, name, pos):
        self.name = name
        self.pos = pos

    def help(self, player):
        a, b = [random.randint(10, 99) for _ in range(2)]
        print(f"{self.name} 이 문제를 냅니다.. {a} + {b} = ?")
        c = int(input().strip())
        if a + b == c:
            print(f"[Correct] {self.name}으로부터 라이프 1개를 획득하였습니다.")
            player + 1
        else:
            print(f"[Wrong] {self.name}이 인사하며 떠납니다.")


class EasyMap:
    def __init__(self, player):
        tmp = random.sample([i for i in range(2, 15)], 5)
        self.player = player
        self.enemies = [
            Enemy(f"Enemy_{i}", idx) for i, idx in enumerate(sorted(tmp[:2]))
        ]
        self.enemy_pos_list = [enemy.pos for enemy in self.enemies]
        self.helpers = [
            Helper(f"Helper_{i}", idx) for i, idx in enumerate(sorted(tmp[2:4]))
        ]
        self.helper_pos_list = [helper.pos for helper in self.helpers]
        self.mystery = Mystery(f"Mystery", tmp[-1])
        self.mode = "Easy"

    def show_map(self):
        print("=" * 90)
        print(" " * 29 + f"Race to the Finish! ({self.mode} mode)")
        print("-" * 90)
        print(
            f"%30s%30s%30s"
            % (
                f"Player: {self.player.name}",
                f"Position: {self.player.pos}",
                f"Life: {self.player.life}",
            )
        )
        print("=" * 90)
        print_custom_map(
            self.enemy_pos_list, self.helper_pos_list, self.mystery.pos, self.player.pos
        )
        print()

    def run(self):
        prev_msg = ""
        while True:
            self.show_map()

            if prev_msg:
                print(prev_msg)
                print()

            print("다음 중 하나를 고르시오.")
            print("1. 한 칸 전진")
            print("2. 대시보드로 나가기")

            user_input = input("> ").strip()
            print()

            if user_input == "1":
                if self.player.status == "성공":
                    prev_msg = "[성공] 마지막 칸에 도달하셨습니다!"
                    continue
                elif self.player.status == "실패":
                    prev_msg = "[실패] 더 이상 전진할 수 없습니다."
                    continue

                self.player.pos = min(self.player.pos + 1, 15)

                for helper in self.helpers:
                    if self.player.pos == helper.pos:
                        helper.help(self.player)
                for enemy in self.enemies:
                    if self.player.pos == enemy.pos:
                        enemy.attack(self.player)
                if self.player.pos == self.mystery.pos:
                    self.mystery.attack_or_help(self.player)
                
                if self.player.pos == 15:
                    self.player.status = "승리"
                    prev_msg = "[성공]마지막 칸에 도달하셨습니다!"
                elif self.player.life == 0:
                    self.player.status = "실패"
                    prev_msg = "[실패] 더 이상 전진할 수 없습니다."
                else:
                    prev_msg = ""
            elif user_input == "2":
                break


class HardMap(EasyMap):
    def __init__(self, player):
        tmp = random.sample([i for i in range(2, 15)], 6)
        self.player = player
        self.enemies = [
            Enemy(f"Enemy_{i+1}", idx) for i, idx in enumerate(sorted(tmp[:3]))
        ]
        self.enemy_pos_list = [enemy.pos for enemy in self.enemies]
        self.helpers = [
            Helper(f"Helper_{i+1}", idx) for i, idx in enumerate(sorted(tmp[3:5]))
        ]
        self.helper_pos_list = [helper.pos for helper in self.helpers]
        self.mystery = Mystery(f"Mystery", tmp[-1])
        self.mode = "Hard"


class Player:
    def __init__(self, name):
        self.name = name
        self.pos = 1
        self.status = "진행"
        self.life = 3

    def __add__(self, n):
        self.life += n

    def __sub__(self, n):
        self.life = max(0, self.life - n)


class Mystery(Enemy, Helper):
    def __init__(self, name, pos):
        self.name = name
        self.pos = pos

    def attack_or_help(self, player):
        if bool(random.randint(0, 1)):
            super().attack(player)
        else:
            super().help(player)

if __name__ == "__main__":
    board = Dashboard()
    board.run()
