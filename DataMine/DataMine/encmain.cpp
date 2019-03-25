#include "XMLParser.h"
#include<fstream>

int main(int argc, char* argv[])
{
	//regex rr("((.*)Category:(.*))|((.*)Template:(.*))");
	//string ss("Template:Universities in the Republic of Ireland");
	//string sss("Category:Ethics");
	//cout << regex_match(ss, rr);
	//cout << regex_match(sss, rr);
	string path(argv[1]);
	XMLParser xmler(path);
	xmler.init();
	xmler.work();
	xmler.destroy();
	return 0;
}