// Parser.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "grammarParser.h"

int _tmain(int argc, _TCHAR* argv[])
{
	grammarParser parser = grammarParser();
	cout << "Ҧ���" << endl;
	cout << "2015���ƿ�2��" << endl;
	cout << "201530542309" << endl;
	parser.inputCode();
	parser.checkWords();
	parser.printListType();
	parser.parse();
	parser.printTAC();
	system("pause");
	return 0;
}

