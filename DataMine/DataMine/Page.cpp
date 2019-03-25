#include "Page.h"


Page::Page(string s)
{
	name = s;
	fromCnt = 0;
	toCnt = 0;
	pageRank = 0.0;
	oldPageRank = 0.0;
	fromPage.clear();
	toPage.clear();
}


Page::~Page()
{
	fromPage.clear();
	toPage.clear();
}

void Page::addFrom(Page* p)
{
	fromPage.push_back(p);
	++fromCnt;
}
void Page::addTo(Page* p)
{
	toPage.push_back(p);
	++toCnt;
}

void Page::update()
{
	for (int i = 0; i < fromCnt; ++i)
	{
		pageRank += fromPage[i]->oldPageRank / ((double)fromPage[i]->toCnt);
	}
}

void Page::preProcess()
{
	oldPageRank = pageRank;
	pageRank = 0.0;
}