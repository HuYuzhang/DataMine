# TextClassifier

## 网络
* 参考了Yoon Kim 在 “Convolutional Neural Networks for Sentence Classification”中提出的利用CNN进行文本分类的方法,
网络结构见下，不过简单起见最后没有加入dropout。
![Image text](http://d3kbpzbmcynnmx.cloudfront.net/wp-content/uploads/2015/11/Screen-Shot-2015-11-06-at-8.03.47-AM.png)

## 文件说明
### word2vec
* 预处理：调用gensim生成词向量，词向量的维数设定为50，将模型存放在word_vec当中。
### generate_data
* 根据预先训练好的词向量模型生成训练数据。在此处进行截断或者补全的操作，设定的长度是500。数据存储格式是hdf5。
### Apply_model
* 读取训练好的模型，对于test的每一个句子进行分类（需要调用caffe的python接口）。
### train
* 这个文件夹中放着caffe的网络结构文件以及solver文件。数据集文件合计10G，就不放上来了（
#### 另外几个文件都是没用的（
## 其他
* 使用了gensim的Word2Vec预先生成词向量，词向量维度设置成50维。
* 截短长度设置为500。统计了一下平均长度大概是1000左右，但是这样训练集太大了，就算截断到500也有8G+。
* 标点符号什么的完全没有处理（
