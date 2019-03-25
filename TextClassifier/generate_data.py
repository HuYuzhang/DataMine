import h5py
import numpy as np
from gensim.models.word2vec import Word2Vec


model_path = '..\\raw_data\\word_vec'
train_path = '..\\raw_data\\train_data'
valid_path = '..\\raw_data\\valid_data'
h5_path = '..\\target\\train500.h5'
max_buf = 1000
pad_len = 500
vec_len = 50

model = Word2Vec.load(model_path)
buf_list = np.zeros([max_buf, 1, pad_len, vec_len])
buf_label = np.zeros([max_buf, 1, 1, 1])



def word_list2vec_list(words):
    length = len(words)
    if length > pad_len:
        target_list = words[0:pad_len - 1]
    else:
        target_list = words + ['<pad>' for i in range(pad_len - length)]

    ret_list = np.zeros([1, pad_len, vec_len])
    id = 0
    for word in target_list:
        if word in model:
            ret_list[0, id, :] = model[word]
        id += 1
    print('------'+str(id))
    return ret_list


line_id = 0
buf_cnt = 0

preRead = []
with open(train_path) as f:
    while True:
        line = f.readline()
        if line == '':
            break
        preRead.append(line)


preRead= preRead[0:59999]
line_id = 0
buf_cnt = 0
for line in preRead:
        content = line[1: -2].split(' ', 3)
        label = content[1][1:-2]
        text = content[3][1:-2]
        word_list = text.split()

        vec_list = word_list2vec_list(word_list)

        buf_list[buf_cnt,:,:,:] = vec_list
        buf_label[buf_cnt, 0, 0, 0] = int(label)
        buf_cnt += 1
        line_id += 1
        print(line_id)
        if buf_cnt == max_buf:

            if line_id == max_buf:
                f = h5py.File(h5_path, 'w')
                print(buf_list.shape)
                print(buf_label.shape)
                f.create_dataset('data', data=buf_list, maxshape=(None, 1, pad_len, vec_len), chunks=True, dtype='float32')
                f.create_dataset('label', data=buf_label, maxshape=(None, 1, 1, 1), chunks=True)
                f.close()
            else:
                f = h5py.File(h5_path, 'a')
                data = f['data']
                data.resize((line_id,1,pad_len,vec_len))
                label = f['label']
                label.resize((line_id,1,1,1))
                print(buf_list.shape)
                print(buf_label.shape)
                print(data[-max_buf:line_id,:,:,:].shape)
                data[-max_buf:line_id,:,:,:] = buf_list
                label[-max_buf:line_id,:,:,:] = buf_label
                f.close()
            buf_cnt = 0



