import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import random

'''定义全局参数'''
clustering = 3
randomSelect = False
maxTime = 1000

'''预处理数据'''
columns = ['label'] + ['dim' + str(i) for i in range(13)]
data = pd.read_csv("归一化数据.csv", names = columns)
label = data['label']
del data['label']

'''返回X和Y的欧氏距离'''
def dist(x, y):
    return np.sqrt(np.sum(np.square(x-y)))

'''随机选择三个起始位置'''
data_num = len(data)
dim_num = len(data.loc[0])
if randomSelect:
    initialChoices = random.sample([i for i in range(data_num)], clustering)
else:
    initialChoices = [20, 70, 150]
centralPoints = [data.loc[initialChoices[i]] for i in range(clustering)]

'''kmeans算法迭代开始'''
ans = np.zeros(data_num)
for i in range(clustering):
    ans[initialChoices[i]] = i + 1  # 分配初始编号1-n

i = 0
while True:
    change = False
    tmp = [np.zeros(dim_num)] * clustering
    cnt = [0] * clustering
    for j in range(data_num):
        dists = []
        for k in range(clustering):
            dists.append(dist(data.loc[j], centralPoints[k]))

        min_index = dists.index(min(dists))
        tmp[min_index] += data.loc[j]
        cnt[min_index] += 1
        if min_index + 1 != ans[j]:
            ans[j] = min_index + 1
            change = True

    # np.seterr(all='ignore')
    for j in range(clustering):
        centralPoints[j] = tmp[j] / cnt[j]

    i += 1
    if i > maxTime or not change:  # 超过最大次数或聚类结果不再改变时退出
        break

'''将结果输出到result.csv文件内'''
print(f'{i} 个回合后.....')
acc = np.sum(ans == label) / data_num * 100
print('分类准确率 = %.2f%%' % acc)
dists = []
for i in range(data_num):
    dists.append(dist(data.loc[i], centralPoints[int(ans[i]) - 1]))
sse = np.sum(dists)

output = pd.DataFrame(dists, columns=['dist'])
output.to_csv('result.csv')

'''以图形形式显示'''
while(1):
    firs = int(input("请选择第一个dim（1~6）: "))
    if firs <= 6:
        first_dim = f'dim{firs}'
        while(1):
            seco = int(input("请选择第二个dim（1~6）: "))
            if seco <= 6:
                second_dim = f'dim{seco}'
                break
            else:
                print("所选择的第二个dim超出范围，请重新输入!")
                seco
        break
    else:
        print("所选择的第一个dim超出范围，请重新选择!")
        firs

fd, sd = data[first_dim], data[second_dim]
map_dict = {0: 'r', 1: 'g', 2: 'b'}  # clustering=3
ans = [map_dict[ans[i] - 1] for i in range(data_num)]

plt.xlabel(first_dim)
plt.ylabel(second_dim)
plt.title('SSE=%.2f Acc=%.2f%%' % (sse, acc))
plt.scatter(fd, sd, c=ans)
for i in range(clustering):
    tmp = centralPoints[i]
    x, y = tmp[int(first_dim[-1])], tmp[int(second_dim[-1])]
    plt.plot(x, y, color='gold', marker='*', markersize=10)
plt.show()
