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
	int sys_tempVar;//�м�����±�
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
	void compressJump();//��������ת����ѹ��
	VarType getCurrUnitType();
	Unit pop_stack();
private:
	string rawInput;
	string programName;
	char currChar;
	int getNextToken();
	int pushTac(Unit u1, Unit u2, Unit u3, Unit u4);
	bool checkDefine();
	//���
	State sentence();
	//��������
	State Declare();//��������
	Unit valueDefine();//�ױ�����
	Unit remainValueDefine();//ʣ���������
	Unit TypeDefine();//��������
	State multiDeclare();//���ر���������
	//������ֵ
	State Assign_Exp();//��ֵ���
	State Cacl_Exp();//�������ʽ
	State Term();//��
	State factor();//����
	Unit generateTempVar();//�����м����
	//�������
	State multi_sentence();
	State sentenceList();
	//while���
	State while_Exp();
	//if���
	State if_Exp();
	//Repeat���
	State repeat_Exp();
	//�������ʽ
	State bool_Exp();
	State bool_Term();//������
	State bool_factor();//������
};
/* ɨ������е�DFA״̬���� */
typedef enum
{
	START, INASSIGN, INNUM, INID, INQUOTE, INSLASH, INCOMMENT, RIGHTCOMMENT, INGT, INLT, INDOT, DONE
}
StateType;
#endif