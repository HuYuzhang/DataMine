#pragma once
#include<vector>
#include<string>
using namespace std;

//���ڱ���һ����ҳ�ľ�����Ϣ������fromPage���ڴ洢���еģ�ָ��ǰ�����ҳ�Ķ����ָ�롣��toPage�����ڴ洢��Щ����ǰҳ����ָ�����ҳ��ָ�롣
class Page
{
private:
	vector<Page*> fromPage;
	vector<Page*> toPage;
	double oldPageRank;
	int fromCnt;
	int toCnt;
public:
	//addTo�Ƿ������ҳ��ָ����һ��ҳ��ʱ����ӽ��뵱ǰҳ���ָ���б�
	//addFrom������ĳ��ҳ��ָ��ǰҳ��ʱ����ӽ��뵱ǰҳ��ı�ָ���б�
	//update���ڶԱ�ҳ���pageRank���и���
	//preProcess���ڽ���һ�ֵ���֮��õ���pageRank�ݴ���oldPageRank�У�����update����֮ǰ������
	Page(string s);
	~Page();
	void addTo(Page* p);
	void addFrom(Page* p);
	void update();
	void preProcess();
	double pageRank;
	string name;
};

