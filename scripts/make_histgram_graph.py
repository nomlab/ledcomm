from datetime import date
from string import Template
from matplotlib.backends.backend_pdf import PdfPages
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import sys


# data_dir = sys.argv[1]
# data_list = ['5cm', '10cm', '20cm']
data_phone_list = ['nexus', 'galaxy', 'xperia', 'pixel']
# data_phone_list = ['data_galaxy', 'data_nexus']

data_list = ['5cm']
graph_row = 2
graph_column = 5
graph_page = 1
graph_num = 10

# pp = PdfPages('./glaphs/flashlight_histgram.pdf')

for data_dir in data_phone_list:
    for data_dist in data_list:
        fig = plt.figure(figsize=(16, 9), dpi=120)
        fig_pos = 0
        for rate in range(graph_page, graph_page + graph_num):
            file_template = Template(
                'row_data/data_${dir}/${dist}/data_${rate}ms_${dist}.csv')
            try:
                df = pd.read_csv(file_template.substitute(
                    dir=data_dir, dist=data_dist, rate=rate), header=None)
            except FileNotFoundError:
                continue
            fig_pos += 1
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
            df_on_duration['on_duration'] -= rate
            df_off_duration['off_duration'] -= rate
            print(rate)
            # duration_data[f'{rate}ms_on_duration'] = df_on_duration['on_duration']
            # duration_data[f'{rate}ms_off_duration'] = df_off_duration['off_duration']
            ax = fig.add_subplot(graph_row, graph_column, fig_pos)
            df_on_duration['on_duration'].hist(
                bins=20, color="blue", grid=True, alpha=0.6, ax=ax, label=f"{rate}ms_on")
            df_off_duration['off_duration'].hist(
                bins=20, color="red", grid=True, alpha=0.5, ax=ax, label=f"{rate}ms_off")
            # y_lim の取得
            axes = plt.gca()
            y_min, y_max = axes.get_ylim()

            # rate msのところに破線を引く
            # plt.vlines(rate, y_min, y_max, colors='orange',
            #    linestyles='dashed', linewidth=2)

            plt.vlines(df_on_duration.mean(), y_min, y_max,
                       colors='blue', linestyles='dashed', linewidth=2)

            plt.vlines(df_off_duration.mean(), y_min, y_max,
                       colors='red', linestyles='dashed', linewidth=2)

            plt.legend(loc="upper right", fontsize=10)
            plt.ylim(0, y_max)
            plt.xlim(-5, 40)
        # pp.savefig(fig)
    fig.savefig(f'./glaphs/{data_dir}_{data_dist}_all_histgram.png')

fig.tight_layout()
# pp.close()
# plt.show()
# plt.close()


# duration_data.hist()
