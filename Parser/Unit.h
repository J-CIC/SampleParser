#ifndef UNIT_H
#define UNIT_H
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <iomanip>
#include "global.h"
using namespace std;
class Unit{
public:
	string err_msg;//������Ϣ
	int lineNum;//�к�
	int linePos;//����λ��
	int constant_id;//���������id
	Unit():content(""),type(-1){}
	Unit(string in_content, int in_type, int in_constant_id=0) :content(in_content), type(in_type),constant_id(in_constant_id){}
	void setInfo(int lineNum, int linePos, string err_msg);
	void setVarType(VarType v);
	void printInfo();
	void setType(int type);
	void setContent(string s);
	int getType();
	string getErrInfo();
	string getContent();
	VarType getVarType();
private:
	string content;
	VarType var_type;
	int type;
};
#endif