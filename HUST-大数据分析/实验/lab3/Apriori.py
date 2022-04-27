# coding:utf-8
import numpy as np
import pandas as pd
import itertools
import time
from funcs import combine, reduceFreq, PCY, generateRules

'''定义全局变量'''
debug = False
min_support = 0.005         # 定义最小支持度为0.005
min_confidence = 0.5        # 定义最小置信度为0.5
upgrade = True
nBuckets = 1000

'''从文件中提取数据并格式化输入'''
data = pd.read_csv('Groceries.csv')['items']
data = [d.lstrip('{').rstrip('}').split(',') for d in data]
if debug:
    data = data[:500]
data_num = len(data)

'''计算物品个数并赋予编号'''
items = list(set(list(itertools.chain(*data))))
item_num = len(items)

id2item = {}    # id2item: 将id转化为物品名称的字典
for id, item in enumerate(items):
    id2item[id] = item
item2id = {v:k for k,v in id2item.items()}  # item2id: 将物品名称转化为id的字典

for i in range(data_num):
    for j in range(len(data[i])):
        data[i][j] = item2id[data[i][j]]    # 将数据中的物品名称转化为标号
    data[i].sort()      # 将数据中的物品名称id按升序排序

'''计算1阶频繁项集并按支持度升序进行排序'''
groups_1 = [i for i in range(item_num)]
support_1 = reduceFreq(groups_1, 1, data, data_num, min_support)
support_1 = sorted(support_1.items(), key=lambda x:x[-1], reverse=False)
support_1 = dict(support_1)
# print(support_1)
print("1阶频繁项及支持度计算完成！\n")

'''计算2阶频繁项集并按支持度升序进行排序'''
groups_2 = combine(list(support_1.keys()), 2)
start = time.time()
if upgrade:
    support_2 = PCY(list(support_1.keys()), 2, data, data_num, min_support, nBuckets)
else:
    support_2 = reduceFreq(groups_2, 2, data, data_num, min_support)
support_2 = sorted(support_2.items(), key=lambda x:x[-1], reverse=False)
support_2 = dict(support_2)
end = time.time()
print(end-start)
print("2阶频繁项及支持度计算完成！\n")
# print(support_2)

'''计算3阶频繁项集并按支持度升序进行排序'''
groups_3 = combine(list(support_2.keys()), 3)
support_3 = reduceFreq(groups_3, 3, data, data_num, min_support)
support_3 = sorted(support_3.items(), key=lambda x:x[-1], reverse=False)
support_3 = dict(support_3)
print("3阶频繁项及支持度计算完成！\n")
# print(support_3)

'''计算4阶频繁项集并按支持度升序进行排序'''
groups_4 = combine(list(support_3.keys()), 4)
support_4 = reduceFreq(groups_4, 4, data, data_num, min_support)
support_4 = sorted(support_4.items(), key=lambda x:x[-1], reverse=False)
support_4 = dict(support_4)
print("4阶频繁项及支持度计算完成！\n")
# print(support_4)

'''输出各阶频繁项集数和所有阶频繁项集总数'''
print(f'1阶频繁项集数: {len(support_1)}')
print(f'2阶频繁项集数: {len(support_2)}')
print(f'3阶频繁项集数: {len(support_3)}')
print(f'4阶频繁项集数: {len(support_4)}\n')
print(f'总频繁项集数: {len(support_1) + len(support_2) + len(support_3) + len(support_4)}')

'''计算关联规则并按置信度升序进行排序'''
support = {**support_1, **support_2, **support_3, **support_4}
rules = sorted(generateRules(support, min_confidence).items(), key=lambda x:x[1], reverse=False)
rules = dict(rules)
print(f'关联规则数: {len(rules)}\n')

'''将输出结果写入名为output.txt文件内'''
with open('output.txt', 'w') as f:
    # 1阶频繁项目集
    f.write(f'----- 1阶频繁项集数: {len(support_1)} -----\n')
    for k, v in support_1.items():
        k = id2item[k]
        f.write(k + ' : ' + str(v) + '\n')

    # 2阶频繁项目集
    f.write(f'\n----- 2阶频繁项集数: {len(support_2)} -----\n')
    for k, v in support_2.items():
        for i in k:
            f.write(id2item[i] + ', ')
        f.write(' : ' + str(v) + '\n')

    # 3阶频繁项目集
    f.write(f'\n----- 3阶频繁项集数: {len(support_3)} -----\n')
    for k, v in support_3.items():
        for i in k:
            f.write(id2item[i] + ', ')
        f.write(' : ' + str(v) + '\n')

    # 4阶频繁项目集
    f.write(f'\n----- 4阶频繁项集数: {len(support_4)} -----\n')
    for k, v in support_4.items():
        for i in k:
            f.write(id2item[i] + ', ')
        f.write(' : ' + str(v) + '\n')

    # 关联规则
    f.write(f'\n----- 关联规则数: {len(rules)} -----\n' )
    for k, v in rules.items():
        if type(k[0]) == int:
            f.write(id2item[k[0]])
        else:
            for i in k[0]:
                f.write(id2item[i] + ', ')
        f.write(' -> ')
        if type(k[1]) == int:
            f.write(id2item[k[1]])
        else:
            for i in k[1]:
                f.write(id2item[i] + ', ')     
        f.write(' : ' + str(v) + '\n')

print('完成！')