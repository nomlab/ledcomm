from datetime import date
from string import Template
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import sys

# data_dir = 'data'
data_dir = sys.argv[1]
data_list = ['5cm', '10cm', '20cm']
# data_list = ['5cm']s

ave_value = 0
value_num = 0

for data_dist in data_list:
    for i in range(1, 11):
        file_template = Template(
            'row_data/${dir}/${dist}/data_${rate}ms_${dist}.csv')
        try:
            df = pd.read_csv(file_template.substitute(
                dir=data_dir, dist=data_dist, rate=i), header=None)
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
            if 0 < value:
                ave_value += value
                value_num += 1
            # print("%0.2f %0.2f\n" % (value, prev_value))

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

    # print("%0.2f & %0.2f & %0.2f & %0.2f"
    #       % (df_on_duration['on_duration'].max(),
    #          df_on_duration['on_duration'].min(),
    #          df_on_duration['on_duration'].mean(),
    #          df_on_duration['on_duration'].var())
    #       )

    # print("%0.2f & %0.2f & %0.2f & %0.2f"
    #       % (df_off_duration['off_duration'].max(),
    #          df_off_duration['off_duration'].min(),
    #          df_off_duration['off_duration'].mean(),
    #          df_off_duration['off_duration'].var())
    #       )

    # print("%0.2f, %0.2f, %0.2f, %0.2f, %0.2f, %0.2f, %0.2f, %0.2f"
    #       % (df_on_duration['on_duration'].max(),
    #          df_off_duration['off_duration'].max(),
    #          df_on_duration['on_duration'].min(),
    #          df_off_duration['off_duration'].min(),
    #          df_on_duration['on_duration'].mean(),
    #          df_off_duration['off_duration'].mean(),
    #          df_on_duration['on_duration'].var(),
    #          df_off_duration['off_duration'].var())
    #       )

        # print("%0.2f\t%0.2f\t%0.2f\t%0.2f\t%0.2f\t%0.2f\t%0.2f\t%0.2f"
        #   % (df_on_duration['on_duration'].max(),
        #  df_off_duration['off_duration'].max(),
        #  df_on_duration['on_duration'].min(),
        #  df_off_duration['off_duration'].min(),
        #  df_on_duration['on_duration'].mean(),
        #  df_off_duration['off_duration'].mean(),
        #  df_on_duration['on_duration'].var(),
        #  df_off_duration['off_duration'].var())
        #   )
    print("%d, %0.2f" % (value_num, (ave_value / value_num)))
