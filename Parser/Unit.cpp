#include "stdafx.h"
#include "Unit.h"
void Unit::setInfo(int lineNum, int linePos, string err_msg){
	this->lineNum = lineNum;
	this->linePos = linePos;
	this->err_msg = err_msg;
}
void Unit::printInfo()
{
	string temp;
	if (type >= 36 && type <= 38){
		temp = "(" + to_string(type) + "," + to_string(constant_id) + ")";
	}
	else{
		temp = "(" + to_string(type) + "," + "-" + ")";
	}
	cout <<setw(10) << temp;
}
string Unit::getErrInfo()
{
	return  "At line " + to_string(lineNum) + " pos " + to_string(linePos) + " : " + err_msg +  "\n";
}
int Unit::getType()
{
	return type;
}
void Unit::setType(int type)
{
	this->type = type;
}
string Unit::getContent()
{
	return content;
}
void Unit::setVarType(VarType v){
	var_type = v;
}
VarType Unit::getVarType(){
	return var_type;
}
void Unit::setContent(string s){
	content = s;
}