from gensim.models.word2vec import Word2Vec

model = Word2Vec.load('word_vec')

print(model.most_similar('we'))