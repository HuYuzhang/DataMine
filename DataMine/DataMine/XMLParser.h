#pragma once
#include"Page.h"
#include<regex>
#include<string>
#include<vector>
#include<iostream>
#include<fstream>
#include<map>
#include<set>
#include<functional>
#include<algorithm>
using namespace std;
typedef size_t hash_id;
typedef map<string, int> nameDic;
typedef map<string, int>::iterator m_it;
#define DEBUG_MODE 0
#define DUMP 1
#define FILTER 1

class XMLParser
{
	
private:
	ifstream fin;
	hash<string> hasher;
	map<hash_id, int> hash2id;
	map<string, int>name2id;
	vector<Page*> pages;
	vector<int> quote2id;
	vector<string> quoteTarget;
	int cur_id;
	const int max_id = 1000000;
	const int iter_cnt = 10000;
#if DEBUG_MODE
	ofstream fout;
#endif
public:
	XMLParser(string XMLName);
	~XMLParser();

	//**************  Tool Function  *****************
	vector<string> splitString(string& s);
	Page* addItem(string& s);
	bool deleteItem(string& s);
	string getFirstTitle(string& s);
	//**************  Tool Function  *****************

	//************** Call Function   *****************
	//void parse();
	void iter();
	void init();
	void destroy();
	void work();
};
