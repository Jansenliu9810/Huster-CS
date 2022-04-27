import numpy as np
import pandas as pd


def main():
    fileopen = open('sent_receive.csv')
    edges = [line.strip('\n').split(',') for line in fileopen]
    edges = edges[2:]

    #将读到的节点按顺序放入nodes数组里
    nodes = []
    for edge in edges:
        if edge[1] not in nodes:
            nodes.append(edge[1])
        if edge[2] not in nodes:
            nodes.append(edge[2])

    print(nodes)

    #节点数和边长
    N = len(nodes)
    L = len(edges)
    print(f'Number of Nodes: {N}')
    print(f'Length of Edges: {L}')

    #初始化矩阵
    matrix = np.zeros([N, N])
    for edge in edges:
        start = nodes.index(edge[1])
        end = nodes.index(edge[2])
        matrix[end, start] = 1

    #构造矩阵
    for j in range(N):
        col_sum = sum(matrix[:, j])
        for i in range(N):
            if matrix[i, j]:
                matrix[i, j] /= col_sum

    r = np.ones(N) / N
    next_r = np.zeros(N)
    einit = 300000          #初始化误差大小
    iteration = 0          #迭代次数初始为0
    beta = 0.85             #Beta值常为0.85

    while einit > 1e-8:
        next_r = np.dot(matrix, r) * beta + (1-beta) / N * np.ones(N)   # 迭代公式
        col_sum = sum(next_r)
        next_r = next_r / col_sum
        einit = next_r - r
        einit = max(map(abs, einit))  # 计算误差
        r = next_r
        iteration += 1

    print('iteration %s:\n' % str(iteration), r)
    ans = pd.DataFrame({'id:': nodes, 'Pagerank:': r})
    # print('iteration %s:\n' % str(iteration), ans)
    ans.to_csv('E:\HUST\HUST-大数据分析\实验\Lab 2\实验二\数据\Pagerank.csv', index=False)


main()
