import caffe
from gensim.models.word2vec import Word2Vec
import numpy as np

root_path = ''
solver_path = 'solver.prototxt'
deploy_path = 'deploy.prototxt'
model_path = '.caffemodel'
test_path = 'test_data'
word_vec_path = 'word_vec'
pad_len = 500
vec_len = 50


caffe.set_mode_cpu()
solver_path = root_path + solver_path
deploy_path = root_path + deploy_path
model_path = root_path + model_path
test_path = root_path + test_path
word_vec_path = root_path + word_vec_path

model = Word2Vec.load(word_vec_path)
net = caffe.Net(deploy_path, model_path, caffe.TEST)

# 接下来是输入的词向量的文件
sentences = []

with open(test_path) as f:
    while True:
        line = f.readline()
        if line == '':
            break
        content = line[1: -2].split(' ', 3)
        # 将当前行划分成四部分，分别是label, 0/1, text, 文本内容
        text = content[3][1:-2]
        word_list = text.split()
        sentences.append(word_list)


input = np.zeros([len(sentences, 1, pad_len, vec_len)])
cnt = 0
for word_list in sentences:
    target_list = np.zeros([1, pad_len, vec_len])
    length = len(word_list)
    if length > pad_len:
        word_list = word_list[:pad_len - 1]
    else:
        word_list = word_list + ['<pad>' for i in range(pad_len - length)]

    id = 0
    for word in word_list:
        if word in model:
            target_list[1, id, :] = model[word]
        id += 1
    input[cnt, :, :, :] = target_list
    cnt += 1
    print(cnt)


net.blobs['data'].data[...] = input
output = net.forward()
