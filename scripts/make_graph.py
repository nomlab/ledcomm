from datetime import date
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import sys

df = pd.read_csv(sys.argv[1], header=None)
# df.columns = ['time', 'voltage']
# print(df['time'][0])
# df['time'] -= df['time'][0]
# df['time'] /= 1000
df.columns = ['voltage']

i = 1
while i < len(df['voltage']):
    if (df['voltage'][i-1] == 0) & (df['voltage'][i] != 0):
        up_index = i - 1
        break
    i += 1

i = len(df['voltage'])-1
while 1 < i:
    if (df['voltage'][i-1] != 0) & (df['voltage'][i] == 0):
        down_index = i + 1
        break
    i -= 1

df['voltage'] = df['voltage'][up_index:down_index]

fig = plt.figure()
ax1 = fig.add_subplot()

# ax1.plot(df['time'], df['voltage'], color="red", label="voltage", marker="o")
# ax1.set_title("グラフ")  # タイトル
# ax1.set_xlabel("time[ms]")  # x軸ラベル
# ax1.set_ylabel("Volt")  # y1軸ラベル

ax1.plot(np.arange(0, 0.2 * len(df['voltage']), 0.2),
         df['voltage'], color="red", label="voltage", marker="o")
ax1.set_title("グラフ")  # タイトル
ax1.set_xlabel("time[ms]")  # x軸ラベル
ax1.set_ylabel("Volt")  # y1軸ラベル

y_min, y_max = ax1.get_ylim()
ax1.set_ylim(0, y_max*3)

plt.show()
