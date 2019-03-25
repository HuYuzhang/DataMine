#pragma once
#include<vector>
#include<string>
using namespace std;

//用于保存一个网页的具体信息，其中fromPage用于存储所有的，指向当前这个网页的对象的指针。而toPage则用于存储那些，当前页面所指向的网页的指针。
class Page
{
private:
	vector<Page*> fromPage;
	vector<Page*> toPage;
	double oldPageRank;
	int fromCnt;
	int toCnt;
public:
	//addTo是发现这个页面指向另一个页面时，添加进入当前页面的指向列表
	//addFrom是另外某个页面指向当前页面时，添加进入当前页面的被指向列表
	//update用于对本页面的pageRank进行更新
	//preProcess用于将上一轮迭代之后得到的pageRank暂存入oldPageRank中，将在update函数之前被调用
	Page(string s);
	~Page();
	void addTo(Page* p);
	void addFrom(Page* p);
	void update();
	void preProcess();
	double pageRank;
	string name;
};

