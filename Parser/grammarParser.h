#ifndef GRAMMARPARSER_H
#define GRAMMARPARSER_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <stack>
#include <sstream>
#include <fstream>
#include <iomanip>
#include "global.h"
#include "Unit.h"
#include "State.h"
using namespace std;
class grammarParser{
public:
	//property
	static map<string, int, greater <string> > keywords;
	map<string, int> constantList;
	vector<string> inputList;
	vector<Unit> unitList;
	vector<Unit> errorList;
	vector<vector<Unit>> tacList;
	vector<Unit> customList;
	vector<Unit> varList;
	stack<Unit> cal_stack;
	int lineNum;
	int linePos;
	int unit_index;
	int addressNum;
	int sys_tempVar;//中间变量下标
	Unit* currUnit;
	//function
	grammarParser(){
		this->inputList = vector<string>();
		this->unitList = vector<Unit>();
		this->constantList = map<string, int>();
		this->errorList = vector<Unit>();
		sys_tempVar = 1;
	}
	static void printKeywords();
	void inputCode();
	void printListType();
	void error(string err_msg);
	int checkWords();
	int parse();
	int getChar();
	int getToken();
	void backtrackChar();
	void printTAC();
	void compressJump();//无条件跳转语句的压缩
	VarType getCurrUnitType();
	Unit pop_stack();
private:
	string rawInput;
	string programName;
	char currChar;
	int getNextToken();
	int pushTac(Unit u1, Unit u2, Unit u3, Unit u4);
	bool checkDefine();
	//语句
	State sentence();
	//变量声明
	State Declare();//变量声明
	Unit valueDefine();//首变量名
	Unit remainValueDefine();//剩余变量声明
	Unit TypeDefine();//类型声明
	State multiDeclare();//多重变量的声明
	//算术赋值
	State Assign_Exp();//赋值语句
	State Cacl_Exp();//算数表达式
	State Term();//项
	State factor();//因子
	Unit generateTempVar();//产生中间变量
	//复合语句
	State multi_sentence();
	State sentenceList();
	//while语句
	State while_Exp();
	//if语句
	State if_Exp();
	//Repeat语句
	State repeat_Exp();
	//布尔表达式
	State bool_Exp();
	State bool_Term();//布尔项
	State bool_factor();//布因子
};
/* 扫描程序中的DFA状态定义 */
typedef enum
{
	START, INASSIGN, INNUM, INID, INQUOTE, INSLASH, INCOMMENT, RIGHTCOMMENT, INGT, INLT, INDOT, DONE
}
StateType;
#endif