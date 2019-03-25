import collections
from operator import itemgetter
file_path = 'train_data'
line_id = 0
max_word = 20000
# 突然发现，直接调用Word2Vec就能够免去一大堆的预处理了，那么就直接采用这玩意吧


counter = collections.Counter()
with open(file_path) as file:
    while True:
        content = file.readline()
        if content == '':
            break
        line_id = line_id + 1
        content = content[1:-2].split(' ', 3)
        # 分成4个元素,所以分割次数是3，其中最后一个元素是这句话的文本内容
        label = content[1][1: -2]
        text = content[3][1: -2]
        for word in text.split():
            counter[word] += 1

# 目前不考虑标点符号的问题了
sorted_word_list = sorted(counter.items(), key=itemgetter(1), reverse=True)
cnt = [x[1] for x in sorted_word_list]

idx = 0
with open('count.txt', 'w') as f:
    for c in cnt:
        idx += 1
        f.write(str(idx) + ' '+ str(c) + '\n')
# print(sorted_word_list)
# sorted_word_list = [x[0] for x in sorted_word_list]
# print(sorted_word_list[0])
# tmp = [x[1] for x in sorted_word_list]
# with open('count.txt', 'w') as f:
#     for cnt in tmp:
#         f.write(str(cnt) + '\n')
# if len(sorted_word_list) > max_word:
#     sorted_word_list = sorted_word_list[:max_word]
# # 考虑进行预处理，得到词汇表切断之后进行重新浏览，将未出现在词汇表的全部用"UNKNOWN"来代替？
#
# with open('sorted', 'w') as f:
#     for word in sorted_word_list:
#         f.write(word + '\n')


