#include "XMLParser.h"

XMLParser::XMLParser(string XMLName)
{
	fin = ifstream(XMLName);
#if DEBUG_MODE
	fout = ofstream("out.txt");
#endif
	pages.clear();
	name2id.clear();
	quote2id.clear();
	quoteTarget.clear();

	cur_id = 0;
	if (!fin)
	{
		cout << "Can't open the file " + XMLName << endl;
	}
}
XMLParser::~XMLParser()
{
	name2id.clear();
	quote2id.clear();
	quoteTarget.clear();
	pages.clear();
	fin.close();
#if DEBUG_MODE
	fout.close();
#endif
}
vector<string> XMLParser::splitString(string& s)
{
	//在这里我们认为分隔符只能是'|'，而且被'|'划分的两个部分左右是没有空格的。返回结果包括每个被"|"所划分的子串
	vector<string> ret;
	ret.clear();
	size_t st = 0;
	size_t ed = 0;
	while ((ed = s.find("|", st)) && ed != string::npos)
	{
		ret.push_back(s.substr(st, ed - st));
		st = ed + 1;
	}
	if (ed == 0)
	{
		ret.push_back(s);
	}
	else if (ed == string::npos)
	{
		ret.push_back(s.substr(st, s.length() - st));
	}
	return ret;
}
string XMLParser::getFirstTitle(string& s)
{
	size_t st = 0;
	size_t ed = s.find("|", st);
	if (ed == string::npos)
	{
		return s;
	}
	else
	{
		return s.substr(st, ed - st);
	}
}
Page* XMLParser::addItem(string& s)
{
	if (name2id.find(s) == name2id.end())
	{
		Page* tmp = new Page(s);
		pages.push_back(tmp);
		name2id[s] = cur_id;
		++cur_id;
		return tmp;
	}
	else
	{
		return NULL;
	}
}
bool XMLParser::deleteItem(string& s)
{
	if (name2id.find(s) != name2id.end())
	{
		name2id.erase(s);
		--cur_id;
		delete pages[cur_id];
		pages.pop_back();
		return true;
	}
	else
	{
		return false;
	}
}
void XMLParser::iter()
{
	//进行每一次的更新操作
	int cnt = (int)pages.size();

	for (int i = 0; i < cnt; ++i)
	{
		pages[i]->preProcess();
	}
	for (int i = 0; i < cnt; ++i)
	{
		pages[i]->update();
	}
}
void XMLParser::destroy()
{
	for (int i = 0; i < pages.size(); ++i)
	{
		delete pages[i];
	}
	pages.clear();
	name2id.clear();
	fin.close();
#if DEBUG_MODE
	fout.close();
#endif
}
bool comp(const Page* p1, const Page* p2)
{
	return p1->pageRank > p2->pageRank;
}
/*
之前挖的坑实在太大了，parse函数看来已经病入膏肓，放弃了，重写一个init。。。
应该可以断言，<page>和</page>都是单独占据一行的。于是外层循环可以进行网页计数
*/

void XMLParser::init()
{
	string tmpPage;
	string tmpLine;
	string tmpQuote;
	string tmpTitle;
	string tmpRedirect;
	string tmpTarget;
	smatch result;
	regex titlePattern(" *<title>(.*)</title>");
	regex redirectPattern(" *<redirect title=\"(.*)\" />");
	regex quotePattern("\\[\\[(.*?)\\]\\]");
	regex pageHeadPattern("( *?)<page>");
	regex pageTailPattern("( *?)</page>");
	regex textHeadPattern("( *)<text(.*?)>(.*)");
	regex textTailPattern("(.*)</text>");
	regex filterPattern("((.*)Category:(.*))|((.*)Template:(.*))|((.*)File:(.*))|((.*)Wikipedia:(.*))");
	bool break_tag;//break_tag是用于结束对于每一行的读取这个循环的
	bool filter_tag;

	int line_cnt = 0;
	int page_cnt = 0;

	//每次循环对应于每一个页面
	while (page_cnt <= max_id)
	{
		if (getline(fin, tmpLine))
		{
			++line_cnt;
#if DEBUG_MODE
			if (line_cnt % 10 == 0)
			{
				cout << line_cnt << endl;
			}
#endif
		}
		else
		{
#if DEBUG_MODE
			cout << "##############   FINISHED by EOF ##############" << endl;
			cout << "End of the file reached!" << endl;
			cout << "Cur_id: " << cur_id << " Line_cnt: " << line_cnt << endl;
#endif
			return;
		}
		if (regex_match(tmpLine, result, pageHeadPattern))
		{
			//找到了一个页面的开头，接着获取这个页面的标题
			break_tag = false;
			filter_tag = false;
			if (getline(fin, tmpLine))
			{
				++line_cnt;
#if DEBUG_MODE
				if (line_cnt % 10 == 0)
				{
					cout << line_cnt << endl;
				}
#endif
			}
			else
			{
#if DEBUG_MODE
				cout << "##############   FINISHED by EOF ##############" << endl;
				cout << "End of the file reached!" << endl;
				cout << "Cur_id: " << cur_id << " Line_cnt: " << line_cnt << endl;
#endif
				return;
			}

			//如果读到了代表标题的位置，得到页面的标题
			if (regex_match(tmpLine, result, titlePattern))
			{
				tmpTitle = result[1];
#if FILTER
				//现在加入的模式是过滤掉category和template的标题
				if (regex_match(tmpTitle, result, filterPattern))
				{
					//发现需要过滤的项目，于是直接过滤掉：狂读到</page>
					//cout << "************    Filter work!" << endl;
					//cout << tmpTitle << endl << endl;
					filter_tag = true;
					while (true)
					{
						//-------------------
						if (getline(fin, tmpLine))
						{
							++line_cnt;
#if DEBUG_MODE
							if (line_cnt % 10 == 0)
							{
								cout << line_cnt << endl;
							}
#endif
						}
						else
						{
#if DEBUG_MODE
							cout << "##############   FINISHED by EOF ##############" << endl;
							cout << "End of the file reached!" << endl;
							cout << "Cur_id: " << cur_id << " Line_cnt: " << line_cnt << endl;
#endif
							return;
						}
						//----------------------
						if (regex_match(tmpLine, pageTailPattern))
						{
							break;
						}
					}
				}
				if (filter_tag)
				{
					continue;
				}
#endif
				addItem(tmpTitle);
				++page_cnt;
#if DEBUG_MODE
				fout << "-----------------------Title found-------------" << endl;
				fout << tmpTitle << endl;
				fout << "-----------------------------------------------" << endl << endl;
#endif
			}
			else
			{
				//理论上不应该进入这个条件判断，这里是当对于标题的解析失败时输出错误信息
				cout << "******************************************************************" << endl;
				cout << "***************** ERROR while parsing title: " << tmpLine << endl;
				cout << "******************************************************************" << endl << endl;
			}

			//不断读取当前页面中的每一行
			//1、读到页面结尾行：跳出循环
			//2、读到重定向行：删除标题条目；检查目标条目是否存在；设置标记，跳出循环
			//3、text开始条目：进入内部循环分析接下来的所有引用
			//4、其他：忽略，进行下一行的读取
			while (true)
			{
				if (getline(fin, tmpLine))
				{
					++line_cnt;
#if DEBUG_MODE
					if (line_cnt % 10 == 0)
					{
						cout << line_cnt << endl;
					}
#endif
				}
				else
				{
#if DEBUG_MODE
					cout << "##############   FINISHED by EOF ##############" << endl;
					cout << "End of the file reached!" << endl;
					cout << "Cur_id: " << cur_id << " Line_cnt: " << line_cnt << endl;
#endif
					return;
				}
				//到达了网页的结尾，跳出循环
				if (regex_match(tmpLine, pageTailPattern))
				{
					break;
				}

				//发现当前网页是重定向网页，直接找到</page>的位置
				else if (regex_match(tmpLine, result, redirectPattern))
				{
					//设置跳出标记
					--page_cnt;
					break_tag = true;
					deleteItem(tmpTitle);
					tmpRedirect = result[1];
					//这里做了修改，即使重定向目标不存在，我们也不进行创建
#if DEBUG_MODE
					fout << "-----------------------Redirect found: " << endl;
					fout << tmpTitle << " -> " << tmpRedirect << endl;
					fout << "-----------------------------------------------" << endl << endl;
#endif
					//不断读取行直到读到页面的结尾
					//之后就会进入下一个页面的读取
					//实际上这里会忽略重定向页面中的<text>标签中的内容
					while (true)
					{
						if (getline(fin, tmpLine))
						{
							++line_cnt;
#if DEBUG_MODE
							if (line_cnt % 10 == 0)
							{
								cout << line_cnt << endl;
							}
#endif
						}
						else
						{
#if DEBUG_MODE
							cout << "##############   FINISHED by EOF ##############" << endl;
							cout << "End of the file reached!" << endl;
							cout << "Cur_id: " << cur_id << " Line_cnt: " << line_cnt << endl;
#endif
							return;
						}
						if (regex_match(tmpLine, pageTailPattern))
						{
							break;
						}
					}
				}
				else if (regex_match(tmpLine, textHeadPattern))
				{
					//--------------------------
					if (page_cnt % 100 == 0)
					{
						cout << "-------------------Page Cnt: " << page_cnt << endl;
					}
					//-------------------------
					//不断读取text中的每一行，检查其中是否存在形如[[(.*)]]的引用结构（注意过滤文件引用File::）
					while (true)
					{
						if (getline(fin, tmpLine))
						{
							++line_cnt;
#if DEBUG_MODE
							if (line_cnt % 10 == 0)
							{
								cout << line_cnt << endl;
							}
#endif
						}
						else
						{
#if DEBUG_MODE
							cout << "##############   FINISHED by EOF ##############" << endl;
							cout << "End of the file reached!" << endl;
							cout << "Cur_id: " << cur_id << " Line_cnt: " << line_cnt << endl;
#endif
							return;
						}
						//分析其中是否包含引用结构
						string::const_iterator st = tmpLine.begin();
						string::const_iterator ed = tmpLine.end();
						//遍历整个一行来不断检查是否存在多个形如[[(.*)]]的结构
						while (regex_search(st, ed, result, quotePattern))
						{
							tmpQuote = string(result[1].first, result[1].second);
							//首先的一个问题是会有file开头的文件，需要过滤掉形如File:开头的结构
							if (tmpQuote.substr(0, 5) == "File:")
							{
								st = result[0].second;
								continue;
							}

							//成功找到，首先获得第一个标题，还有当前标题的id
							string dstName = getFirstTitle(tmpQuote);
							int id = name2id[tmpTitle];

#if DEBUG_MODE
							fout << "---------------------------------Quote found" << endl;
							fout << "Here is Quote from: " << tmpTitle << " To " << dstName << endl;
							fout << "--------------------------------------------" << endl << endl;
#endif

							//原先这部分是检查引用页面是否存在，之后设置引用关系。
							//但是现在仅仅将这个引用记录存储下来。
							//首先检查一下指向对象是否以经有了记录
							quote2id.push_back(id);
							quoteTarget.push_back(dstName);
							//到这里就结束了！

							st = result[0].second;
						}

						//接下来需要注意的是引用结构后面可能紧跟着<text>结束的标志
						//另外，如果<text>的结束标志是单独的，那么在这里也能检测出来
						if (regex_match(tmpLine, textTailPattern))
						{
							break;
						}
					}
				}
				else{}//对应于普通正文的情况，暂时没有什么需要干的

				//如果是重定向网页，这里进行跳出循环的操作
				if (break_tag)
				{
					break;
				}
			}
		}
		/*
		对应312行的if，这里没有else语句，因为一旦每次遇到<page>语句，接下来会直接读取完这个页面的所有内容。
		因此其余的诸如<namespace>这类的语句，会自然而然地略过去。
		*/
	}
	cout << "******************* Parsing Finished  *****************" << endl << endl;
	
#if DUMP
	cout << "******************* Now Begin to Dump all Titles" << endl << endl;
	ofstream titleOut = ofstream("Title.txt");
	for (int i = 0; i < pages.size(); ++i)
	{
		titleOut << i << ": " << pages[i]->name << endl;
	}
	titleOut.close();
#endif
	//接下来，在创建完所有的Title条目之后，根据前面记录到的quote信息更新页面之间的相互引用关系
	size_t quoteCnt = quote2id.size();
	int tmpid;
	int targetid;
	for (size_t i = 0; i < quoteCnt; ++i)
	{
		tmpTarget = quoteTarget[i];
		tmpid = quote2id[i];
		m_it it = name2id.find(tmpTarget);
		if (it != name2id.end())
		{
			//这样就说明之前的遍历过程中已经发现过目标网页了
			targetid = (*it).second;
			pages[targetid]->addFrom(pages[tmpid]);
			pages[tmpid]->addTo(pages[targetid]);
		}
	}
	//完成了对于所有的引用结构的解析，因此释放空间。
	quote2id.clear();
	quoteTarget.clear();
	//完成了引用关系之间的更新，初始化完成
	cout << "#################### FINISHED SUCESSFULLY ############" << endl;
	cout << "Cur_id: " << cur_id << " Line_cnt: " << line_cnt << endl;
}

/*
调用这个函数之前已经完成了对于文件的解析，并且将引用关系列表全部初始完成。
首先需要进行的是pageRank的计算，包括迭代，准备等工作。
pageRank计算完成之后会对pages中的指针进行排序。当然，应该需要对指针的大小的比较进行定义
*/
void XMLParser::work()
{
	//首先对于每个页面的PageRank进行初始化？
	int tot = (int)pages.size();
	double init_pr = 1.0 / (double)tot;
	for (int i = 0; i < tot; ++i)
	{
		pages[i]->pageRank = init_pr;
	}

	cout << "--------------- FINISH PR initiation" << endl;
	for (int i = 0; i < iter_cnt; ++i)
	{
		for (int j = 0; j < tot; ++j)
		{
			pages[j]->preProcess();
		}
		for (int j = 0; j < pages.size(); ++j)
		{
			pages[j]->update();
		}
		if (i > 0 && i % 100 == 0)
		{
			cout << "--------------- FINISH PR calculation of iteration: " << i << endl;
			sort(pages.begin(), pages.end(), comp);
			cout << "--------------- FINISH PR sorting" << endl;
			ofstream out(to_string(i) + string("_sort.txt"));
			for (int i = 0; i < tot; ++i)
			{
				out << pages[i]->name << '\t' << pages[i]->pageRank << endl;
			}
			out.close();
		}
	}
	cout << "--------------- ALL FINISHED" << endl;
}