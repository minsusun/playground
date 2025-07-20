import re
import matplotlib.pyplot as plt
import colorsys

with open("cursor-dark-theme-color-scheme.json", "r") as f:
    data = "".join(f.readlines())

expression = re.compile("#([0-9a-fA-F]{6})")
colors = set(expression.findall(data))
colors = list(
    sorted(
        colors,
        key=lambda x: colorsys.rgb_to_hsv(
            *tuple(int(x[i : i + 2], 16) for i in (0, 2, 4))
        ),
    )
)

num_colors = len(colors)
cols = 8
rows = (num_colors + cols - 1) // cols

fig, ax = plt.subplots(figsize=(cols * 2.5, rows * 2.5))
ax.set_xlim(0, cols)
ax.set_ylim(0, rows)
ax.axis("off")

for i, color in enumerate(colors):
    col = i % cols
    row = rows - (i // cols) - 1
    rect = plt.Rectangle((col, row), 1, 1, facecolor="#" + color)
    ax.add_patch(rect)
    ax.text(
        col + 0.5,
        row + 0.5,
        "#" + color,
        ha="center",
        va="center",
        fontsize=10,
        color="white" if int(color, 16) < 0x888888 else "black",
    )

plt.tight_layout()
plt.savefig("color_scheme.png")
