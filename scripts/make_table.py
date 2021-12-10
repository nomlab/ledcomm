from datetime import date
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import sys

data_dir = 'data'
data_list = ['5cm', '10cm', '20cm']

df = pd.read_csv(sys.argv[1], header=None)
df.columns = ['voltage']

value = df['voltage'][1]
prev_value = value
df_on_duration = pd.DataFrame(index=[], columns=['on_duration'])
df_off_duration = pd.DataFrame(index=[], columns=['off_duration'])
duration = 0

# print(df)

for i in range(1, len(df) - 1):
    duration += 0.2
    value = df['voltage'][i]
    # print("%0.2f %0.2f\n" % (value, prev_value))

    if (prev_value != 0) & (value == 0):
        record = pd.Series([duration], index=df_on_duration.columns)
        df_on_duration = df_on_duration.append(
            record, ignore_index=True)
        duration = 0

    if (prev_value == 0) & (value != 0):
        record = pd.Series([duration], index=df_off_duration.columns)
        df_off_duration = df_off_duration.append(
            record, ignore_index=True)
        duration = 0

    prev_value = value

print("%f & %f & %f & %f & %f\n"
      % (df_on_duration['on_duration'].max(),
         df_on_duration['on_duration'].min(),
         df_on_duration['on_duration'].mean(),
         df_on_duration['on_duration'].var(),
         df_on_duration['on_duration'].std())
      )

print(df_on_duration['on_duration'].idxmin())

print("%f & %f & %f & %f & %f\n"
      % (df_off_duration['off_duration'].max(),
         df_off_duration['off_duration'].min(),
         df_off_duration['off_duration'].mean(),
         df_off_duration['off_duration'].var(),
         df_off_duration['off_duration'].std())
      )
