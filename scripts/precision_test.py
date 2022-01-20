ans = [1, 0, 1, 0, 0, 1, 0, 0, 1, 1]
ans_len = len(ans)
row_num = 0
data_dir = './precision_data/data_nexus/5cm/'
# data_dir = './precision_data/data_nexus/old/5cm/'

file_name = 'precision_data_10ms_5cm.txt'
file = open(f"{data_dir}{file_name}", "r")
total_success_num = 0
total_try_num = 0
while True:
    line = file.readline()
    # print(line)

    if line:
        row_num += 1
        if ('+' in line):
            # print(line)
            rate = line.replace('+', '')
        elif ('-' in line):
            data = file.readline().strip().split(',')
            print(data)

            data_len = len(data)
            fail_num = ans_len - data_len
            for i in range(0, data_len):
                try:
                    if int(data[i]) != ans[i]:
                        fail_num += 1
                except:
                    fail_num = ans_len
            success_num = ans_len - fail_num
            # print(f'success/ans_len = {success_num}/{ans_len}')
            total_success_num += success_num
            total_try_num += ans_len

    else:
        print(
            f"total_success / total_try = {total_success_num}/{total_try_num}")
        accuracy = total_success_num / total_try_num * 100
        print(f"accuracy : {accuracy}%")
        break
