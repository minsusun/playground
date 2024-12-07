import tkinter as tk
import random


class Block:
    # 테트리스 블록 모양 정의
    SHAPES = [
        [[1, 1, 1, 1]],  # I
        [[1, 1], [1, 1]],  # O
        [[1, 1, 1], [0, 1, 0]],  # T
        [[1, 1, 0], [0, 1, 1]],  # Z
        [[0, 1, 1], [1, 1, 0]],  # S
        [[1, 1, 1], [1, 0, 0]],  # L
        [[1, 1, 1], [0, 0, 1]],  # J
    ]
    # 테트리스 블록 회전 모양 정의
    ROTATE_SHAPES = {
        "I": [[[1, 1, 1, 1]], [[1], [1], [1], [1]]],
        "O": [[[1, 1], [1, 1]]],
        "T": [
            [[1, 1, 1], [0, 1, 0]],
            [[0, 1], [1, 1], [0, 1]],
            [[0, 1, 0], [1, 1, 1]],
            [[1, 0], [1, 1], [1, 0]],
        ],
        "Z": [[[1, 1, 0], [0, 1, 1]], [[0, 1], [1, 1], [1, 0]]],
        "S": [[[0, 1, 1], [1, 1, 0]], [[1, 0], [1, 1], [0, 1]]],
        "L": [
            [[1, 1, 1], [1, 0, 0]],
            [[1, 1], [0, 1], [0, 1]],
            [[0, 0, 1], [1, 1, 1]],
            [[1, 0], [1, 0], [1, 1]],
        ],
        "J": [
            [[1, 1, 1], [0, 0, 1]],
            [[0, 1], [0, 1], [1, 1]],
            [[1, 0, 0], [1, 1, 1]],
            [[1, 1], [1, 0], [1, 0]],
        ],
    }
    # 색상 정의
    COLORS = ["cyan", "yellow", "purple", "red", "green", "orange", "blue"]
    # 색상 역인덱싱
    COLORS_DICT = {
        "cyan": 0,
        "yellow": 1,
        "purple": 2,
        "red": 3,
        "green": 4,
        "orange": 5,
        "blue": 6,
    }

    # 생성자
    def __init__(self):
        # TODO shape와 color를 SHAPES와 COLORS에서 랜덤하게 설정
        # self.shape = random.sample(self.SHAPES)
        self.shape_alphabet = random.choice(list(self.ROTATE_SHAPES.keys()))
        self.rotate_pos = 0
        self.shape = self.ROTATE_SHAPES[self.shape_alphabet][self.rotate_pos]
        self.color = random.choice(self.COLORS)
        # TODO 주어진 shape에 맞춰 적절한 초기위치 설정
        # 20 x 10 board
        self.pos = (0, (10 - len(self.shape[0])) // 2)

    # 블록을 시계방향으로 90도 회전
    def rotate(self):
        self.rotate_pos = (self.rotate_pos + 1) % len(
            self.ROTATE_SHAPES[self.shape_alphabet]
        )
        self.shape = self.ROTATE_SHAPES[self.shape_alphabet][self.rotate_pos]

    # 블록의 위치를 (dx, dy)만큼 이동
    def move(self, dx, dy):
        self.pos = (self.pos[0] + dx, self.pos[1] + dy)


class Tetris:
    # 생성자
    def __init__(self, root):
        self.root = root

        # 디폴트 값 배정 (행, 열, 각 칸의 크기, 게임판 크기, 게임 종료 여부(game over), 점수)
        self.ROWS = 20
        self.COLS = 10
        self.WIDTH = 300
        self.HEIGHT = 600
        self.INFO_SIZE = 150
        self.CELL_SIZE = 30

        # 게임 상태
        self.game_over = False
        self.score = 0

        # TODO 게임 화면 설정
        # (1) 게임 전체 화면 크기 설정
        self.root.geometry(f"{self.WIDTH + self.INFO_SIZE}x{self.HEIGHT}")  #
        # (2) 게임 진행 화면 - 요구 크기에 맞게 캔버스 설정 및 위치 설정 - self.canvas
        self.canvas = tk.Canvas(
            self.root, width=self.WIDTH, height=self.HEIGHT, bg="black"
        )
        self.canvas.pack(side="left")
        # (3) 게임 정보 화면 - 요구 크기에 맞게 프레임 설정 및 위치 설정 - self.frame
        self.frame = tk.Frame(
            self.root, width=self.INFO_SIZE, height=self.HEIGHT, bg="gray"
        )
        self.frame.pack(expand=True, fill='both')
        # (4) 게임 점수 - 현재 게임 점수 표시 관련 설정 및 위치 설정 - self.score_label
        self.score_label = tk.Label(
            self.frame, text="Score: 000", font=("Arial", 16), fg="white"
        )
        self.score_label.pack(pady=20)
        # (5) 다음 블록 창 - 요구 크기에 맞게 캔버스 설정 및 위치 설정 - self.next_canvas
        self.next_canvas = tk.Canvas(
            self.frame, width=self.INFO_SIZE, height=self.INFO_SIZE, bg="black"
        )
        self.next_canvas.pack(pady=10)

        # TODO 게임 보드와 블록 초기화
        # self.board, self.current_block, self.next_block
        self.board = [[0 for _ in range(self.COLS)] for _ in range(self.ROWS)]
        self.current_block = None
        self.next_block = Block()

        # 초기 게임 화면 구성
        self.new_shape()  # 현재 블록 및 다음 블록 업데이트 후, 다음 블록을 화면에 표시
        self.draw_board()  # 게임 보드와 블록을 화면에 그리기
        self.root.bind("<Key>", self.handle_key)  # 키보드 키와 바인딩
        self.update()  # 게임 진행을 주기적으로 업데이트 함

    # 새로운 블록 생성하고 다음 블록 창에 표시
    def new_shape(self):
        # 1) 현재 블록 정보를 다음 불록 정보로 갱신
        # 2) 다음 블록 정보를 생성
        # 3) 다음 블록 정보를 '다음 블록 창'에 표시
        self.current_block = self.next_block
        self.next_block = Block()
        top_left = (
            (self.INFO_SIZE - len(self.next_block.shape) * self.CELL_SIZE) // 2,
            (self.INFO_SIZE - len(self.next_block.shape[0]) * self.CELL_SIZE) // 2,
        )
        self.next_canvas.delete("all")
        for i in range(len(self.next_block.shape)):
            for j in range(len(self.next_block.shape[0])):
                if self.next_block.shape[i][j]:
                    self.next_canvas.create_rectangle(
                        top_left[1] + j * self.CELL_SIZE,
                        top_left[0] + i * self.CELL_SIZE,
                        top_left[1] + (j + 1) * self.CELL_SIZE,
                        top_left[0] + (i + 1) * self.CELL_SIZE,
                        fill=self.next_block.color,
                    )
        self.next_canvas.pack()

    # 현재 게임판과 현재 블록을 캔버스에 그리기
    def draw_board(self):
        self.canvas.delete("all")
        for i in range(self.ROWS):
            for j in range(self.COLS):
                if self.board[i][j]:
                    self.canvas.create_rectangle(
                        j * self.CELL_SIZE,
                        i * self.CELL_SIZE,
                        (j + 1) * self.CELL_SIZE,
                        (i + 1) * self.CELL_SIZE,
                        fill=Block.COLORS[self.board[i][j] - 1],
                    )
        for i in range(len(self.current_block.shape)):
            for j in range(len(self.current_block.shape[0])):
                if self.current_block.shape[i][j]:
                    self.canvas.create_rectangle(
                        (self.current_block.pos[1] + j) * self.CELL_SIZE,
                        (self.current_block.pos[0] + i) * self.CELL_SIZE,
                        (self.current_block.pos[1] + j + 1) * self.CELL_SIZE,
                        (self.current_block.pos[0] + i + 1) * self.CELL_SIZE,
                        fill=self.current_block.color,
                    )
        self.canvas.pack(side="left")

    # 블록의 이동/회전 가능 여부 체크
    def check_collision(self, dx=0, dy=0, rotate=False):
        next_pos = (self.current_block.pos[0] + dx, self.current_block.pos[1] + dy)
        next_shape = (
            Block.ROTATE_SHAPES[self.current_block.shape_alphabet][
                (self.current_block.rotate_pos + 1)
                % len(Block.ROTATE_SHAPES[self.current_block.shape_alphabet])
            ]
            if rotate
            else self.current_block.shape
        )
        if (
            next_pos[0] + len(next_shape) > self.ROWS
            or next_pos[1] + len(next_shape[0]) > self.COLS
            or next_pos[0] < 0
            or next_pos[1] < 0
        ):
            return True
        for i in range(len(next_shape)):
            for j in range(len(next_shape[0])):
                if next_shape[i][j] and self.board[next_pos[0] + i][next_pos[1] + j]:
                    return True
        return False

    # 블록 이동 및 회전 처리
    def move_block(self, dx=0, dy=0, rotate=False):
        self.current_block.move(dx, dy)
        if rotate:
            self.current_block.rotate()

    # 현재 블록을 게임 보드에 고정
    def freeze_shape(self):
        for i in range(len(self.current_block.shape)):
            for j in range(len(self.current_block.shape[0])):
                if self.current_block.shape[i][j]:
                    self.board[self.current_block.pos[0] + i][
                        self.current_block.pos[1] + j
                    ] = (Block.COLORS_DICT[self.current_block.color] + 1)
        self.clear_lines()

    # 완성된 줄 제거 및 점수 업데이트
    def clear_lines(self):
        for i in range(self.ROWS):
            if all(self.board[i]):
                self.board.pop(i)
                self.board.insert(0, [0 for _ in range(self.COLS)])
                self.score += 100
                self.score_label.config(text=f"Score: {self.score:03d}")

    # 게임 진행 상황을 업데이트
    def update(self):
        if self.game_over:
            self.canvas.create_text(
                self.WIDTH / 2, self.HEIGHT / 2, text="Game Over", font=("Arial", 24), justify="center", fill='white'
            )
            self.canvas.pack(side="left")
        else:
            if not self.check_collision(1, 0, False):
                self.move_block(1, 0, False)
            else:
                self.freeze_shape()
                self.new_shape()
                if self.check_collision(0, 0, False):
                    self.game_over = True
            self.draw_board()
        self.root.after(500, self.update)

    # 키 입력 이벤트 처리 - 화살표 키로 블록을 이동하거나 회전함
    def handle_key(self, event):
        if self.game_over:
            return

        dx = 0
        dy = 0
        rotate = False

        if event.keysym == "Up":  # upper arrow
            rotate = True
        elif event.keysym == "Right":  # right arrow
            dy = 1
        elif event.keysym == "Left":
            dy = -1
        elif event.keysym == "Down":
            dx = 1

        if not self.check_collision(dx, dy, rotate):
            self.move_block(dx, dy, rotate)

        pass


# Tetris 게임 실행
if __name__ == "__main__":
    root = tk.Tk()
    root.title("Assignment4_XXXXXXXXX")  # TODO 각자 학번 입력

    game = Tetris(root)

    root.mainloop()
