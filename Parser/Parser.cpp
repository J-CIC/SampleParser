// Parser.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "grammarParser.h"

int _tmain(int argc, _TCHAR* argv[])
{
	grammarParser parser = grammarParser();
	cout << "姚填佳" << endl;
	cout << "2015级计科2班" << endl;
	cout << "201530542309" << endl;
	parser.inputCode();
	parser.checkWords();
	parser.printListType();
	parser.parse();
	parser.printTAC();
	system("pause");
	return 0;
}

