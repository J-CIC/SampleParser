#include "stdafx.h"
#include "grammarParser.h"
map<string, int, greater<string> > grammarParser::keywords = map<string, int, greater<string> >({
	{ "and", 1 }, { "array", 2 }, { "begin", 3 }, { "bool", 4 }, { "call", 5 },
	{ "case", 6 }, { "char", 7 }, { "constant", 8 }, { "dim", 9 }, { "do", 10 },
	{ "else", 11 }, { "end", 12 }, { "false", 13 }, { "for", 14 }, { "if", 15 },
	{ "input", 16 }, { "integer", 17 }, { "not", 18 }, { "of", 19 }, { "or", 20 },
	{ "output", 21 }, { "procedure", 22 }, { "program", 23 }, { "read", 24 }, { "real", 25 },
	{ "repeat", 26 }, { "set", 27 }, { "stop", 28 }, { "then", 29 }, { "to", 30 },
	{ "true", 31 }, { "until", 32 }, { "var", 33 }, { "while", 34 }, { "write", 35 },
	{ "(", 39 }, { ")", 40 }, { "*", 41 }, { "*/", 42 }, { "+", 43 }, { ",", 44 },
	{ "-", 45 }, { ".", 46 }, { "..", 47 }, { "/", 48 }, { "/*", 49 }, { ":", 50 },
	{ ":=", 51 }, { ";", 52 }, { "<", 53 }, { "<=", 54 }, { "<>", 55 }, { "=", 56 },
	{ ">", 57 }, { ">=", 58 }, { "[", 59 }, { "]", 60 }
});
void walkhere(string input)
{
	cout << "walk here " << input << endl;
}
void grammarParser::error(string err_msg)
{
	cout << err_msg << " at line " << currUnit->lineNum << " position " << currUnit->linePos << endl;
	system("pause");
	exit(0);
}
void grammarParser::printKeywords()
{
	for (auto i : keywords){
		cout << i.first<<" "<<i.second << endl;
	}
}
void grammarParser::inputCode()
{
	this->rawInput = "";
	this->inputList.resize(0);
	this->unitList.resize(0);
	cout << "Input file name:\n";
	cin >> this->programName;
	std::ifstream fin(this->programName, std::ios::in);
	string temp;
	if (!fin){
		cout << "File not exists!" << endl;
		exit(0);
	}
	while (getline(fin, temp))
	{
		vector<string> tempList = vector<string>();
		this->rawInput.append(temp+"\n");
		this->inputList.push_back(temp+"\n");
	}
}
int grammarParser::checkWords()
{
	lineNum = 0;
	linePos = 0;
	while (getToken() != 0);
	return this->errorList.size();
}
int grammarParser::getToken(){
	StateType state = START;
	int currTokenType = 0;
	bool is_comment = false, is_err = false;
	string currToken = "",err_msg="";
	while (state != DONE)
	{
		bool save_flag = true;
		int currChar = getChar();
		switch (state)
		{
			case START:
			{
				if (isdigit(currChar))
				{
					state = INNUM;
				}
				else if (isalpha(currChar)){
					state = INID;
				}
				else if (currChar == ':'){
					state = INASSIGN;
				}
				else if (currChar == '\''){
					save_flag = false;
					state = INQUOTE;
				}
				else if (currChar == '/'){
					state = INSLASH;
				}
				else if (currChar == '>'){
					state = INGT;
				}
				else if (currChar == '<'){
					state = INLT;
				}
				else if (currChar == '.'){
					state = INDOT;
				}
				else if (currChar == 0){
					//end of file
					state = DONE;
					currTokenType = 0;
					save_flag = false;
				}
				else if ((currChar == ' ') || (currChar == '\t') || (currChar == '\n')){
					save_flag = false;
				}
				else{
					state = DONE;
					string temp_key = "";
					temp_key += (char)currChar;
					if (this->keywords.find(temp_key) != this->keywords.end()){
						currTokenType = this->keywords[temp_key];
					}
					else{
						currTokenType = -1;
						is_err = true;
						err_msg = "invalid Token";
					}
				}
				break;
			}
			case INASSIGN:
			{
				state = DONE;
				if (currChar == '='){
					currTokenType = this->keywords[":="];
				}
				else{
					currTokenType = this->keywords[":"];
					save_flag = false;
					backtrackChar();
				}
				break;
			}
			case INGT:
			{
				state = DONE;
				if (currChar == '='){
					currTokenType = this->keywords[">="];
				}
				else{
					currTokenType = this->keywords[">"];
					save_flag = false;
					backtrackChar();
				}
				break;
			}
			case INLT:
			{
				state = DONE;
				if (currChar == '='){
					currTokenType = this->keywords["<="];
				}
				else if (currChar == '>'){
					currTokenType = this->keywords["<>"];
				}
				else{
					currTokenType = this->keywords["<"];
					save_flag = false;
					backtrackChar();
				}
				break;
			}
			case INDOT:
			{
				state = DONE;
				if (currChar == '.'){
					currTokenType = this->keywords[".."];
				}
				else{
					currTokenType = this->keywords["."];
					save_flag = false;
					backtrackChar();
				}
				break;
			}
			case INQUOTE:
			{
				if (currChar == '\''){
					save_flag = false;
					state = DONE;
					currTokenType = 38;
				}
				else if (currChar == '\n'){
					state = DONE;
					currTokenType = -1;
					is_err = true;
					err_msg = "Syntax ERROR: Missing '\n";
				}
				break;
			}
			case INSLASH:
			{
				if (currChar == '*'){
					state = INCOMMENT;
				}
				else{
					state = DONE;
					save_flag = false;
					backtrackChar();
					currTokenType = this->keywords["/"];
				}
				break;
			}
			case INCOMMENT:
			{
				if (currChar == '*')
				{
					state = RIGHTCOMMENT;
				}
				else if (currChar == '\n')
				{
					is_comment = true;
					state = DONE;
					currTokenType = -1;
					is_err = true;
					err_msg = "Syntax error: Comment format incorrect, missing */\n";
				}
				break;
			}
			case RIGHTCOMMENT:
			{
				if (currChar == '/')
				{
					is_comment = true;
					state = DONE;
					currTokenType = this->keywords["*/"];
				}
				else if (currChar == '\n')
				{
					is_comment = true;
					state = DONE;
					currTokenType = -1;
					is_err = true;
					err_msg = "Syntax error: Comment format incorrect, missing /\n";
				}
				else{
					is_comment = true;
					state = INCOMMENT;
				}
				break;
			}
			case INNUM:
			{
				if (!isdigit(currChar) && !isalpha(currChar)){
					backtrackChar();
					save_flag = false;
					state = DONE;
					currTokenType = 37;
				}
				else if (!isdigit(currChar) && isalpha(currChar))
				{
					backtrackChar();
					save_flag = false;
					state = DONE;
					currTokenType = 37;
					is_err = true;
					err_msg = "error: wrong constant num";
				}
				break;
			}
			case INID:
			{
				if (!isdigit(currChar)&&!isalpha(currChar))
				{
					backtrackChar();
					save_flag = false;
					state = DONE;
					currTokenType = 36;
				}
				break;
			}
		}
		if (save_flag){
			currToken += currChar;
		}
		if (state == DONE){
			//check whether ID is reserved keywords
			if (currTokenType == 36){
				if (this->keywords.find(currToken) != this->keywords.end()){
					currTokenType = this->keywords[currToken];
				}
			}
			//save ID, num, constant
			int id = 0;
			Unit unit = Unit(currToken, currTokenType,id);
			unit.setInfo(lineNum+1, linePos-currToken.length()+1, err_msg);//lineNum and linePos starts from 0
			if (currTokenType >= 36 && currTokenType <= 38){
				if (this->constantList.find(currToken) == this->constantList.end()){
					id = constantList.size() + 1;
					unit.constant_id = id;
					constantList.insert({ currToken, id });
					customList.push_back(unit);
				}
				else{
					unit.constant_id = this->constantList[currToken];
				}
			}
			if (is_err)
			{
				this->errorList.push_back(unit);
				//should put in errlist;
			}else if (!is_comment&&!currTokenType==0){
				this->unitList.push_back(unit);
			}
		}
	}
	return currTokenType;
}
void grammarParser::backtrackChar()
{
	linePos--;
}
int grammarParser::getChar()
{
	if (linePos >= inputList[lineNum].length())
	{
		lineNum++;
		linePos = 0;
	}
	if (lineNum >= this->inputList.size()){
		//over max size
		currChar = 0;
		return currChar;
	}
	currChar = inputList[lineNum][linePos++];
	return currChar;
}
void grammarParser::printListType()
{
	int i = 0;
	for (auto element : this->unitList){
		i++;
		element.printInfo();
		if (i % 5 == 0)
			cout << endl;
	}
	cout << endl;
	for (auto element : this->errorList){
		cout << element.getErrInfo();
	}
}
Unit grammarParser::pop_stack()
{
	Unit ret = cal_stack.top();
	cal_stack.pop();
	return ret;
}
int grammarParser::pushTac(Unit u1, Unit u2, Unit u3, Unit u4)
{
	vector<Unit> tempTac = { u1, u2, u3, u4 };
	tacList.push_back(tempTac);
	addressNum++;
	return addressNum - 1;
}
VarType grammarParser::getCurrUnitType()
{
	int idx = constantList[currUnit->getContent()];
	return varList[idx].getVarType();
}
Unit grammarParser::generateTempVar()
{
	string content = "T" + to_string(sys_tempVar++);
	Unit temp = Unit(content, 36, constantList.size());
	constantList.insert({ content, constantList.size() });
	return temp;
}
int grammarParser::parse()
{
	unit_index = 0;
	getNextToken();
	sentence();
	return 0;
}
int grammarParser::getNextToken()
{
	if (unit_index < unitList.size())
		currUnit = &unitList[unit_index++];
	return unit_index - 1;
}
State grammarParser::sentence(){
	State temp;
	switch (currUnit->getType())
	{
		case 23:{
			//program 
			Unit* unit_temp = currUnit;
			getNextToken();
			if (currUnit->getType() != 36)
			{
				error("Error: missing program name");
			}
			pushTac(*unit_temp, *currUnit, *currUnit, *currUnit);
			getNextToken();
			if (currUnit->getType() != keywords[";"]){
				error("Error: missing keywords ;");
			}
			getNextToken();
			temp = Declare();//变量说明解析
			getNextToken();
			sentence();//复合语句
			if (currUnit->getType() != 46){// .
				error("Error: without . ending");
			}
			pushTac(*currUnit, *currUnit, *currUnit, *currUnit);
			break;
		}
		case 36:{
			temp = Assign_Exp();
			break;
		}
		case 3:{//begin   
			temp = multi_sentence();
			break;
		}
		case 34:{// while
			temp = while_Exp();
			break;
		}
		case 15:{//if   
			temp = if_Exp();
			break;
		}
		case 26:{//repeat
			temp = repeat_Exp();
			break;
		}
		default:{
			error("Error: unexpected token " + currUnit->getContent());
		}
	}
	return temp;
}
//变量声明
State grammarParser::Declare(){
	State temp;
	switch (currUnit->getType()){
		case 33:{//var
			getNextToken();
			valueDefine();
			if (currUnit->getType() != 52){
				error("Error: missing keywords ;");
			}
			getNextToken();
			while (currUnit->getType() == 36){// id
				temp = multiDeclare();
				getNextToken();
			}
			unit_index--;//drawback
			break;
		}
		default:break;
	}
	return temp;
}
Unit grammarParser::valueDefine(){
	//标识符声明
	Unit temp = *currUnit;
	if (currUnit->getType() != 36)
	{
		error("Error: missing keywords ;" );
	}
	getNextToken();

	temp.setVarType((remainValueDefine()).getVarType());//标识符后续声明, 并保存变量类型
	constantList[temp.getContent()] = varList.size();//更新指向的下标
	varList.push_back(temp);
	return temp;
}
Unit grammarParser::remainValueDefine(){
	Unit temp = *currUnit;
	switch (currUnit->getType()){
		case 44:{// ,
			getNextToken();
			temp.setVarType((valueDefine()).getVarType());//变量名
			break;
		}
		case 50:{// :
			getNextToken();
			temp.setVarType((TypeDefine()).getVarType());//变量类型声明
			break;
		}
		default:{
			error("Error: syntax error " + currUnit->getContent());
		}
	}
	return temp;
}
Unit grammarParser::TypeDefine(){
	Unit temp;
	switch (currUnit->getType()){
		case 17:{// integer
			temp.setVarType(T_INTEGER);
			getNextToken();
			break;
		}
		case 4:{// bool
			temp.setVarType(T_BOOL);
			getNextToken();
			break;
		}
		case 7:{//char
			temp.setVarType(T_CHAR);
			getNextToken();
			break;
		}
		default: {
			error("Error: syntax error " + currUnit->getContent());
		}
	}
	return temp;
}
State grammarParser::multiDeclare()
{
	State temp;
	switch (currUnit->getType()){
		case 36:{
			valueDefine();
			if (currUnit->getType() != 52){
				error("Error: without ;");
			}
			break;
		}
		default:break;
	}
	return temp;
}
//复合语句
State grammarParser::multi_sentence()
{
	State temp;
	if (currUnit->getType() == 3){//begin
		getNextToken();
		temp = sentenceList();
		if (currUnit->getType() != 12){//end
			error("Error: syntax error: no end");
		}
		getNextToken();
	}
	return temp;
}
State grammarParser::sentenceList(){
	State temp = sentence();
	if (currUnit->getType() == 52){// ;
		getNextToken();
		sentenceList();
	}
	return temp;
}
//赋值语句
State grammarParser::Assign_Exp()
{
	State temp;
	temp.codebegin = addressNum;
	if (!checkDefine()){
		error("Error: undefined identifier" + currUnit->getContent());
	}
	//类型检查，但是实验要求中没有布尔的赋值所以只检查INTEGER类型
	if (varList[constantList[currUnit->getContent()]].getVarType() != T_INTEGER){
		error("Error : wrong type ,expected integer variable, current variable is "+currUnit->getContent());
	}
	cal_stack.push(*currUnit);
	getNextToken();
	if (currUnit->getType() != keywords[":="]){
		error("Error: missing token :=");
	}
	Unit assign_op = *currUnit;
	temp = Cacl_Exp();
	//赋值四元式
	if (cal_stack.size() >= 2)
	{
		Unit u1 = pop_stack();
		Unit u2 = pop_stack();
		Unit null_unit = Unit("-", 0, 0);
		pushTac(assign_op, u1, null_unit, u2);

	}
	return temp;
}
State grammarParser::Cacl_Exp(){
	State temp;
	temp.codebegin = addressNum;
	temp = Term();
	while (currUnit->getType() == 45 || currUnit->getType() == 43){//+ -
		Unit op = *currUnit;
		temp = Term();
		if (cal_stack.size() >= 2){//表达式从栈提出成四元式
			Unit u1 = pop_stack();
			Unit u2 = pop_stack();
			Unit temp_var = generateTempVar();
			pushTac(op, u2, u1, temp_var);
			cal_stack.push(temp_var);
		}
	}
	return temp;
}
State grammarParser::Term(){
	State temp;
	temp.codebegin = addressNum;
	temp = factor();//因子*或/项，项可以没有
	getNextToken();
	if (currUnit->getType() == 41 || currUnit->getType() == 48){// * /
		Unit op = *currUnit;
		temp = Term();
		Unit u1 = pop_stack();
		Unit u2 = pop_stack();
		Unit temp_var = generateTempVar(); 
		pushTac(op, u2, u1, temp_var);
		//在这里写入乘除的结果中间变量
		cal_stack.push(temp_var);
	}
	return temp;
}
State grammarParser::factor(){
	State temp;
	temp.codebegin = addressNum;
	getNextToken();
	switch (currUnit->getType()){
		case 45:{// -
			Unit op = *currUnit;
			factor();
			Unit u1 = pop_stack();
			Unit temp_var = generateTempVar();
			cal_stack.push(temp_var);//取负号后推回栈内
			pushTac(op, u1, op, temp_var);
			break;
		}
		case 37:{// num
			cal_stack.push(*currUnit);
			break;
		}
		case 36:{// ID
			if (!checkDefine()){
				error("Error: undefined identifier" + currUnit->getContent());
			}
			else if (getCurrUnitType() != T_INTEGER){
				error("Error: Wrong Type, expected integer");
			}
			cal_stack.push(*currUnit);
			break;
		}
		case 39:{// (
			Cacl_Exp();
			if (currUnit->getType() != 40){
				error("Error: without )");
			}
			break;
		}
		default:{
			error("Error: unexpected token " + currUnit->getContent());
		}
	}
	return temp;
}
bool grammarParser::checkDefine()
{
	bool flag = false;
	for (auto element : varList){
		if (element.getContent() == currUnit->getContent()){
			flag = true;
			return flag;
		}
	}
	return flag;
}
//while语句
State grammarParser::while_Exp()
{
	State temp;
	temp.codebegin = addressNum;
	getNextToken();
	temp = bool_Exp();
	if (currUnit->getType()==10){// do
		getNextToken();
		sentence();
	}
	else{
		error("Error: missing keyword do");
	}
	pushTac(Unit("jump", 0), Unit("-", 0), Unit("-", 0), Unit(to_string(temp.codebegin), 0));
	for (int i = 0; i < temp.false_chain.size() - 1; i++){
		tacList[temp.false_chain[i]][3].setContent(to_string(addressNum));
	}
	int idx = temp.false_chain[temp.false_chain.size() - 1];
	tacList[idx][3].setContent(to_string(addressNum));
	return temp;
}
//if语句
State grammarParser::if_Exp()
{
	State temp;
	temp.codebegin = addressNum;
	getNextToken();
	temp = bool_Exp();
	if (currUnit->getType() == 29){// then
		getNextToken();
		sentence();
		temp.true_pushback(pushTac(Unit("jump", 0), Unit("-", 0), Unit("-", 0), Unit("0", 0)));
		int idx = temp.false_chain[temp.false_chain.size() - 1];
		tacList[idx][3].setContent(to_string(addressNum));
	}
	else{
		error("Error: missing keyword then");
	}
	if (currUnit->getType() == 11){//else
		getNextToken();
		for (auto i : temp.false_chain){
			tacList[i][3].setContent(to_string(addressNum));
		}
		sentence();
		int idx = temp.true_chain[temp.true_chain.size() - 1];
		tacList[idx][3].setContent(to_string(addressNum));
	}
	return temp;
}
//repeat语句
State grammarParser::repeat_Exp()
{
	State temp;
	temp.codebegin = addressNum;
	getNextToken();
	sentence();
	if (currUnit->getType() == 32){// until
		getNextToken();
		State temp_state = bool_Exp();
		int idx = temp_state.false_chain[temp_state.false_chain.size() - 1];
		tacList[idx][3].setContent(to_string(temp.codebegin));
	}
	else{
		error("Error: missing keyword then");
	}
	return temp;
}
//布尔表达式
State grammarParser::bool_Exp()
{
	State temp;
	temp.codebegin = addressNum;
	temp = bool_Term();
	if (currUnit->getType() == 20){//or
		Unit op = *currUnit;
		getNextToken();
		State temp_state = bool_Exp();
		temp.addTrueChain(temp_state);//or的真链直接保存等待回填
		//将上一个bool_Term中需要回填的假链填上现在bool_Exp的code_begin
		for (int i = 0; i < temp.false_chain.size(); i++){
			tacList[temp.false_chain[i]][3].setContent(to_string(temp_state.codebegin));
		}
		//将当前的bool_Exp中的最后那个需要回填的假链保存在State中
		temp.addFalseChain(temp_state);
		if (temp.false_chain.size()>=1){
			temp.false_chain[0] = temp.false_chain[temp.false_chain.size() - 1];
			temp.false_chain.resize(1);
		}
	}
	//布尔表达式结束，回填真链到当前最新地址
	for (int i = 0; i < temp.true_chain.size(); i++){
		tacList[temp.true_chain[i]][3].setContent(to_string(addressNum));
	}
	return temp;
}
State grammarParser::bool_Term()
{
	State temp;
	temp.codebegin = addressNum;
	temp = bool_factor();
	if (currUnit->getType() == 1)
	{//and
		Unit op = *currUnit;
		getNextToken();
		State temp_state = bool_Term();
		temp.addFalseChain(temp_state);//假链直接保存等待回填
		for (int i = 0; i < temp.true_chain.size(); i++){
			//将上一个bool_factor的真链回填到现在的bool_Term的code_begin
			tacList[temp.true_chain[i]][3].setContent(to_string(temp_state.codebegin));
		}
		//将当前的bool_Term中的最后那个需要回填的真链保存在State中
		temp.addTrueChain(temp_state);
		if (temp.true_chain.size()>=1){
			temp.true_chain[0] = temp.true_chain[temp.true_chain.size() - 1];
			temp.true_chain.resize(1);
		}
	}
	return temp;
}
State grammarParser::bool_factor()
{
	State temp;
	temp.codebegin = addressNum;
	switch (currUnit->getType()){
		case 45:{//-和整型走同一个case，所以没有break
		}
		case 37:{
			unit_index--;//回退一个Token以进行算术表达式翻译
			temp = Cacl_Exp();
			Unit u1 = pop_stack();
			int type = currUnit->getType();
			Unit op = *currUnit;
			//这里要翻译类似a>b的语句
			if (type >= 53 && type <= 58)//匹配关系符
			{
				temp = Cacl_Exp();
				Unit u2 = pop_stack();//获取最后的计算结果
				//推入四元式
				temp.true_pushback(pushTac(Unit("j" + op.getContent(), 0), u1, u2, Unit("-", 0)));
				temp.false_pushback(pushTac(Unit("jump", 0), Unit("-", 0), Unit("-", 0), Unit("-", 0)));
			}
			else{
				error("Error: incomplete Expression");
			}
			break;
		}
		case 36:{
			if (!checkDefine()){
				error("Error: undefined identifier " + currUnit->getContent());
			}
			if (getCurrUnitType() == T_INTEGER){
				unit_index--;//回退一个Token以进行算术表达式翻译
				temp = Cacl_Exp();
				Unit u1 = pop_stack();
				int type = currUnit->getType();
				Unit op = *currUnit;
				if (type >= 53 && type <= 58)//匹配关系符
				{
					temp = Cacl_Exp();
					if (!temp.accept)
					{
						return temp;
					}
					Unit u2 = pop_stack();//获取最后的计算结果
					//这里要翻译a>b的语句
					temp.true_pushback(pushTac(Unit("j" + op.getContent(), 0), u1, u2, Unit("-", 0)));
					temp.false_pushback(pushTac(Unit("jump", 0), Unit("-", 0), Unit("-", 0), Unit("-", 0)));
				}
				else{
					error("Error: incomplete Expression");
				}
			}
			else if (getCurrUnitType() == T_BOOL){
				temp.true_pushback(pushTac(Unit("jnz" , 0), *currUnit, Unit("-", 0), Unit("-", 0)));
				temp.false_pushback(pushTac(Unit("jump", 0), Unit("-", 0), Unit("-", 0), Unit("-", 0)));
				getNextToken();
				return temp;
			}
			else{
				error("Error: unexpected token " + currUnit->getContent());
			}
			break;
		}
		case 18:{//not
			Unit op = *currUnit;
			getNextToken();
			temp = bool_factor();
			temp.swapChain();
			break;
		}
		case 31:{// true
			temp.true_pushback(pushTac(Unit("jump", 0), Unit("-", 0), Unit("-", 0), Unit("-", 0)));
			cal_stack.push(*currUnit);
			getNextToken();
			break;
		}
		case 13:{// false
			temp.false_pushback(pushTac(Unit("jump", 0), Unit("-", 0), Unit("-", 0), Unit("-", 0)));
			cal_stack.push(*currUnit);
			getNextToken();
			break;
		}
		case 39:{// (
			getNextToken();
			bool_Exp();
			if (currUnit->getType() != 40){
				error("Error: without )");
			}
			getNextToken();
			break;
		}
		default:{
			error("Error: unexpected token " + currUnit->getContent());
		}
	}
	return temp;
}
void grammarParser::compressJump()
{
	for (auto &element : tacList){
		int idx;
		auto nextJump = element;
		while (nextJump[0].getContent() == "jump"){
			stringstream ss;
			ss << nextJump[3].getContent();
			ss >> idx;
			nextJump = tacList[idx];
		}
		if (element[0].getContent() == "jump"){
			element[3].setContent(to_string(idx));
		}
	}
}
void grammarParser::printTAC()
{
	compressJump();
	int i = 0;
	for (auto element : tacList){
		string lineNum = "(" + to_string(i) + ")";
		if (element[0].getType() == 23)
		{//program
			cout << setw(5) << lineNum << "(" << element[0].getContent() << "," 
				<< element[1].getContent() << ",-,-)" << endl;
		}
		else if (element[0].getType() == 46){//程序结束的.
			cout << setw(5) << lineNum << "(sys,-,-,-)" << endl;
		}
		else{// 二元语句*
			cout << setw(5) << lineNum << "("<<element[0].getContent() << "," << element[1].getContent() << ","
				<< element[2].getContent() << "," << element[3].getContent() << ")" << endl;
		}
		i++;
	}
}
