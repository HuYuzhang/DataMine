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
	//������������Ϊ�ָ���ֻ����'|'�����ұ�'|'���ֵ���������������û�пո�ġ����ؽ������ÿ����"|"�����ֵ��Ӵ�
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
	//����ÿһ�εĸ��²���
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
֮ǰ�ڵĿ�ʵ��̫���ˣ�parse���������Ѿ���������������ˣ���дһ��init������
Ӧ�ÿ��Զ��ԣ�<page>��</page>���ǵ���ռ��һ�еġ��������ѭ�����Խ�����ҳ����
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
	bool break_tag;//break_tag�����ڽ�������ÿһ�еĶ�ȡ���ѭ����
	bool filter_tag;

	int line_cnt = 0;
	int page_cnt = 0;

	//ÿ��ѭ����Ӧ��ÿһ��ҳ��
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
			//�ҵ���һ��ҳ��Ŀ�ͷ�����Ż�ȡ���ҳ��ı���
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

			//��������˴�������λ�ã��õ�ҳ��ı���
			if (regex_match(tmpLine, result, titlePattern))
			{
				tmpTitle = result[1];
#if FILTER
				//���ڼ����ģʽ�ǹ��˵�category��template�ı���
				if (regex_match(tmpTitle, result, filterPattern))
				{
					//������Ҫ���˵���Ŀ������ֱ�ӹ��˵��������</page>
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
				//�����ϲ�Ӧ�ý�����������жϣ������ǵ����ڱ���Ľ���ʧ��ʱ���������Ϣ
				cout << "******************************************************************" << endl;
				cout << "***************** ERROR while parsing title: " << tmpLine << endl;
				cout << "******************************************************************" << endl << endl;
			}

			//���϶�ȡ��ǰҳ���е�ÿһ��
			//1������ҳ���β�У�����ѭ��
			//2�������ض����У�ɾ��������Ŀ�����Ŀ����Ŀ�Ƿ���ڣ����ñ�ǣ�����ѭ��
			//3��text��ʼ��Ŀ�������ڲ�ѭ����������������������
			//4�����������ԣ�������һ�еĶ�ȡ
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
				//��������ҳ�Ľ�β������ѭ��
				if (regex_match(tmpLine, pageTailPattern))
				{
					break;
				}

				//���ֵ�ǰ��ҳ���ض�����ҳ��ֱ���ҵ�</page>��λ��
				else if (regex_match(tmpLine, result, redirectPattern))
				{
					//�����������
					--page_cnt;
					break_tag = true;
					deleteItem(tmpTitle);
					tmpRedirect = result[1];
					//���������޸ģ���ʹ�ض���Ŀ�겻���ڣ�����Ҳ�����д���
#if DEBUG_MODE
					fout << "-----------------------Redirect found: " << endl;
					fout << tmpTitle << " -> " << tmpRedirect << endl;
					fout << "-----------------------------------------------" << endl << endl;
#endif
					//���϶�ȡ��ֱ������ҳ��Ľ�β
					//֮��ͻ������һ��ҳ��Ķ�ȡ
					//ʵ�������������ض���ҳ���е�<text>��ǩ�е�����
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
					//���϶�ȡtext�е�ÿһ�У���������Ƿ��������[[(.*)]]�����ýṹ��ע������ļ�����File::��
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
						//���������Ƿ�������ýṹ
						string::const_iterator st = tmpLine.begin();
						string::const_iterator ed = tmpLine.end();
						//��������һ�������ϼ���Ƿ���ڶ������[[(.*)]]�Ľṹ
						while (regex_search(st, ed, result, quotePattern))
						{
							tmpQuote = string(result[1].first, result[1].second);
							//���ȵ�һ�������ǻ���file��ͷ���ļ�����Ҫ���˵�����File:��ͷ�Ľṹ
							if (tmpQuote.substr(0, 5) == "File:")
							{
								st = result[0].second;
								continue;
							}

							//�ɹ��ҵ������Ȼ�õ�һ�����⣬���е�ǰ�����id
							string dstName = getFirstTitle(tmpQuote);
							int id = name2id[tmpTitle];

#if DEBUG_MODE
							fout << "---------------------------------Quote found" << endl;
							fout << "Here is Quote from: " << tmpTitle << " To " << dstName << endl;
							fout << "--------------------------------------------" << endl << endl;
#endif

							//ԭ���ⲿ���Ǽ������ҳ���Ƿ���ڣ�֮���������ù�ϵ��
							//�������ڽ�����������ü�¼�洢������
							//���ȼ��һ��ָ������Ƿ��Ծ����˼�¼
							quote2id.push_back(id);
							quoteTarget.push_back(dstName);
							//������ͽ����ˣ�

							st = result[0].second;
						}

						//��������Ҫע��������ýṹ������ܽ�����<text>�����ı�־
						//���⣬���<text>�Ľ�����־�ǵ����ģ���ô������Ҳ�ܼ�����
						if (regex_match(tmpLine, textTailPattern))
						{
							break;
						}
					}
				}
				else{}//��Ӧ����ͨ���ĵ��������ʱû��ʲô��Ҫ�ɵ�

				//������ض�����ҳ�������������ѭ���Ĳ���
				if (break_tag)
				{
					break;
				}
			}
		}
		/*
		��Ӧ312�е�if������û��else��䣬��Ϊһ��ÿ������<page>��䣬��������ֱ�Ӷ�ȡ�����ҳ����������ݡ�
		������������<namespace>�������䣬����Ȼ��Ȼ���Թ�ȥ��
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
	//���������ڴ��������е�Title��Ŀ֮�󣬸���ǰ���¼����quote��Ϣ����ҳ��֮����໥���ù�ϵ
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
			//������˵��֮ǰ�ı����������Ѿ����ֹ�Ŀ����ҳ��
			targetid = (*it).second;
			pages[targetid]->addFrom(pages[tmpid]);
			pages[tmpid]->addTo(pages[targetid]);
		}
	}
	//����˶������е����ýṹ�Ľ���������ͷſռ䡣
	quote2id.clear();
	quoteTarget.clear();
	//��������ù�ϵ֮��ĸ��£���ʼ�����
	cout << "#################### FINISHED SUCESSFULLY ############" << endl;
	cout << "Cur_id: " << cur_id << " Line_cnt: " << line_cnt << endl;
}

/*
�����������֮ǰ�Ѿ�����˶����ļ��Ľ��������ҽ����ù�ϵ�б�ȫ����ʼ��ɡ�
������Ҫ���е���pageRank�ļ��㣬����������׼���ȹ�����
pageRank�������֮����pages�е�ָ��������򡣵�Ȼ��Ӧ����Ҫ��ָ��Ĵ�С�ıȽϽ��ж���
*/
void XMLParser::work()
{
	//���ȶ���ÿ��ҳ���PageRank���г�ʼ����
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