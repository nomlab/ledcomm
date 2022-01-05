from datetime import date
from string import Template
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import sys

fig = plt.figure()

data_dir = sys.argv[1]
# data_list = ['5cm', '10cm', '20cm']
data_list = ['5cm']

duration_data = pd.DataFrame(index=[], columns=[])

for data_dist in data_list:
    for rate in range(1, 11):
        file_template = Template(
            'row_data/${dir}/${dist}/data_${rate}ms_${dist}.csv')
        try:
            df = pd.read_csv(file_template.substitute(
                dir=data_dir, dist=data_dist, rate=rate), header=None)
        except FileNotFoundError:
            continue
        df.columns = ['voltage']

        value = df['voltage'][1]
        prev_value = value
        df_on_duration = pd.DataFrame(index=[], columns=['on_duration'])
        df_off_duration = pd.DataFrame(index=[], columns=['off_duration'])
        duration = 0

        # print(df)

        for i in range(1, len(df) - 1):
            duration += 1
            value = df['voltage'][i]

            if (prev_value != 0) & (value == 0):
                record = pd.Series(
                    [duration * 0.2], index=df_on_duration.columns)
                df_on_duration = df_on_duration.append(
                    record, ignore_index=True)
                duration = 0

            if (prev_value == 0) & (value != 0):
                record = pd.Series(
                    [duration * 0.2], index=df_off_duration.columns)
                df_off_duration = df_off_duration.append(
                    record, ignore_index=True)
                duration = 0

            prev_value = value

        df_on_duration['on_duration'] = df_on_duration['on_duration'].drop(
            df_on_duration['on_duration'].idxmin())
        print(rate)
        # duration_data[f'{rate}ms_on_duration'] = df_on_duration['on_duration']
        # duration_data[f'{rate}ms_off_duration'] = df_off_duration['off_duration']
        ax = fig.add_subplot(4, 3, rate)
        df_on_duration['on_duration'].hist(
            bins=20, color="blue", grid=True, alpha=0.6, ax=ax, label=f"{rate}ms_on")
        df_off_duration['off_duration'].hist(
            bins=20, color="red", grid=True, alpha=0.5, ax=ax, label=f"{rate}ms_off")
        axes = plt.gca()
        y_min, y_max = axes.get_ylim()
        plt.vlines(rate, y_min, y_max, colors='gold',
                   linestyles='dashed', linewidth=2)

        plt.vlines(df_on_duration.mean(), y_min, y_max,
                   colors='blue', linestyles='dashed', linewidth=2)

        plt.vlines(df_off_duration.mean(), y_min, y_max,
                   colors='red', linestyles='dashed', linewidth=2)
        plt.legend(loc="upper right", fontsize=10)
        plt.ylim(0, y_max)
        plt.xlim(0, 40)

fig.tight_layout()
plt.show()
plt.close()


# duration_data.hist()
