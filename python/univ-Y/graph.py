import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches

df = pd.read_csv('result.csv')

x = []
y = []
c = []

for idx in range(len(df)):
    R = df.loc[idx,'Radius of the sphere (R)']
    data = [
        df.loc[idx, 'F(R) for BCC'],
        df.loc[idx, 'F(R) for FCC'],
        df.loc[idx, 'F(R) for HCP']
    ]

    result = min(data)

    if result == data[0]:
        color = 'r'
    elif result == data[1]:
        color = 'g'
    else:
        color = 'b'

    x.append(R)
    y.append(result)
    c.append(color)

    fig, ax = plt.subplots()

red_patch = mpatches.Patch(color = 'red', label = 'BCC')
green_patch = mpatches.Patch(color = 'green', label = 'FCC')
blue_patch = mpatches.Patch(color = 'blue', label = 'HCP')

for idx in range(len(x) - 1):
    seg_x = [x[idx], x[idx + 1]]
    seg_y = [y[idx], y[idx + 1]]
    ax.plot(seg_x, seg_y, color = c[idx])

plt.title("Result")
plt.legend(handles=[red_patch, green_patch, blue_patch])
plt.show()

fig.savefig('graph.png')
