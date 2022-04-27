"""
Student id: X2020I1007
Name: Rixing Liu
Date: 2021.12.03
"""

import threading
import time
import sys


#实现Map的功能
def mapper(readfile, writefile):
    newlist = []
    file = open(readfile)
    write = open(writefile, 'w')
    for line in file:
        line = line.strip()
        newlist.append(line.split(', '))
    with write as f:
        for words in newlist:
            for word in words:
                f.write(f"{word}, 1\n")


#实现Shuffle将各个文件内的单词按首字母进行归类并写入新的文件里
def shuffle(readfile):
    file = open(readfile)
    write1 = open('shuffle 01', 'a')
    write2 = open('shuffle 02', 'a')
    write3 = open('shuffle 03', 'a')
    for line in file:
        line = line.strip()
        word, count =line.split(',', 1)
        if word[0] == 'a' or word[0] == 'A' or word[0] == 'b' or word[0] == 'B' or word[0] == 'c' or word[0] == 'C':
            write1.write("{},{}\n".format(word, 1))
        elif word[0] == 'd' or word[0] == 'D' or word[0] == 'e' or word[0] == 'E' or word[0] == 'f' or word[0] == 'F':
            write1.write("{},{}\n".format(word, 1))
        elif word[0] == 'g' or word[0] == 'G':
            write1.write("{},{}\n".format(word, 1))
        elif word[0] == 'j' or word[0] == 'J' or word[0] == 'k' or word[0] == 'K' or word[0] == 'l' or word[0] == 'L':
            write2.write("{},{}\n".format(word, 1))
        elif word[0] == 'm' or word[0] == 'M' or word[0] == 'n' or word[0] == 'N' or word[0] == 'o' or word[0] == 'O':
            write2.write("{},{}\n".format(word, 1))
        elif word[0] == 'p' or word[0] == 'P' or word[0] == 'q' or word[0] == 'Q' or word[0] == 'r' or word[0] == 'R':
            write2.write("{},{}\n".format(word, 1))
        else:
            write3.write("{},{}\n".format(word, 1))


#实现reduce功能：输出每个单词重复的次数
def reduce(readfile, writefile):
    file = open(readfile)
    write = open(writefile, 'w')
    count_dict = {}
    for line in file:
        line = line.strip()
        word, count = line.split(',', 1)
        try:
            count = int(count)
        except ValueError:
            continue
        if word in count_dict.keys():
            count_dict[word] = count_dict[word] + count
        else:
            count_dict[word] = count

    count_dict = sorted(count_dict.items(), key=lambda x: x[0], reverse=False)
    for key, v in count_dict:
        write.write(f"{key}  {v}\n")


if __name__ == '__main__':
    t1 = threading.Thread(target=mapper('source01', 'map01'), args=("t1",))
    t2 = threading.Thread(target=mapper('source02', 'map02'), args=("t2",))
    t3 = threading.Thread(target=mapper('source03', 'map03'), args=("t3",))
    t4 = threading.Thread(target=mapper('source04', 'map04'), args=("t4",))
    t5 = threading.Thread(target=mapper('source05', 'map05'), args=("t5",))
    t6 = threading.Thread(target=mapper('source06', 'map06'), args=("t6",))
    t7 = threading.Thread(target=mapper('source07', 'map07'), args=("t7",))
    t8 = threading.Thread(target=mapper('source08', 'map08'), args=("t8",))
    t9 = threading.Thread(target=mapper('source09', 'map09'), args=("t9",))
    
    start = time.clock()
    t1.start()
    t2.start()
    t3.start()
    t4.start()
    t5.start()
    t6.start()
    t7.start()
    t8.start()
    t9.start()
    
    print('Time cost of map: ')
    t1.join()
    print(f't1 cost {(time.clock() - start)} s')
    t2.join()
    print(f't2 cost {(time.clock() - start)} s')
    t3.join()
    print(f't3 cost {(time.clock() - start)} s')
    t4.join()
    print(f't4 cost {(time.clock() - start)} s')
    t5.join()
    print(f't5 cost {(time.clock() - start)} s')
    t6.join()
    print(f't6 cost {(time.clock() - start)} s')
    t7.join()
    print(f't7 cost {(time.clock() - start)} s')
    t8.join()
    print(f't8 cost {(time.clock() - start)} s')
    t9.join()
    print(f't9 cost {(time.clock() - start)} s')
    
    
    t1 = threading.Thread(target=shuffle('map01'), args=("t1",))
    t2 = threading.Thread(target=shuffle('map02'), args=("t2",))
    t3 = threading.Thread(target=shuffle('map03'), args=("t3",))
    t4 = threading.Thread(target=shuffle('map04'), args=("t4",))
    t5 = threading.Thread(target=shuffle('map05'), args=("t5",))
    t6 = threading.Thread(target=shuffle('map06'), args=("t6",))
    t7 = threading.Thread(target=shuffle('map07'), args=("t7",))
    t8 = threading.Thread(target=shuffle('map08'), args=("t8",))
    t9 = threading.Thread(target=shuffle('map09'), args=("t9",))
    
    start = time.clock()
    t1.start()
    t2.start()
    t3.start()
    t4.start()
    t5.start()
    t6.start()
    t7.start()
    t8.start()
    t9.start()
    
    print('\nTime cost of shuffle: ')
    t1.join()
    print(f't1 cost {(time.clock() - start)} s')
    t2.join()
    print(f't2 cost {(time.clock() - start)} s')
    t3.join()
    print(f't3 cost {(time.clock() - start)} s')
    t4.join()
    print(f't4 cost {(time.clock() - start)} s')
    t5.join()
    print(f't5 cost {(time.clock() - start)} s')
    t6.join()
    print(f't6 cost {(time.clock() - start)} s')
    t7.join()
    print(f't7 cost {(time.clock() - start)} s')
    t8.join()
    print(f't8 cost {(time.clock() - start)} s')
    t9.join()
    print(f't9 cost {(time.clock() - start)} s')
    
    
    t1 = threading.Thread(target=reduce('shuffle 01', 'reduce01'), args=("t1",))
    t2 = threading.Thread(target=reduce('shuffle 02', 'reduce02'), args=("t2",))
    t3 = threading.Thread(target=reduce('shuffle 03', 'reduce03'), args=("t3",))
    
    start = time.clock()
    t1.start()
    t2.start()
    t3.start()
    
    print('\nTime cost of reduce: ')
    t1.join()
    print(f't1 cost {(time.clock() - start)} s')
    t2.join()
    print(f't2 cost {(time.clock() - start)} s')
    t3.join()
    print(f't3 cost {(time.clock() - start)} s')