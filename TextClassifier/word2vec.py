import warnings
warnings.filterwarnings(action='ignore', category=UserWarning, module='gensim')
from gensim.models.word2vec import Word2Vec

file_path = 'train_data'

sentences = []

with open(file_path) as f:
    while True:
        line = f.readline()
        if line == '':
            break
        content = line[1: -2].split(' ', 3)
        # 将当前行划分成四部分，分别是label, 0/1, text, 文本内容
        label = content[1][1:-2]
        text = content[3][1:-2]
        word_list = text.split()
        sentences.append(word_list)

print('Finish reading data from ' + file_path + ', and data_line is: ' + str(len(sentences)))

model = Word2Vec(sentences, min_count=3, size=100)
print('Finish training the model')
model.save('word_vec')

# sentences = [['a', 'b'], ['c', 'a'], ['a', 'b'], ['a', 'b'],['a', 'b'],['a', 'b'],['a', 'b'],['c', 'a'],['c', 'a'],['c', 'a']]
# model = Word2Vec(sentences, min_count=2, size=100)
# # model.build_vocab(sentences)
# # model.train(sentences, min_count=2, size=10)
# model.save('my_model')
# print(model.most_similar(['a']))