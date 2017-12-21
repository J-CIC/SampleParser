#ifndef MY_GLOBAL_H
#define MY_GLOBAL_H
#include <iostream>
#include <string>
#include <map>
#include <vector>
using namespace std;
typedef enum{
	AND = 1, ARRAY = 2, BEGIN = 3, BOOL = 4, CALL = 5, CASE = 6, CHAR = 7, CONSTANT = 8, DIM = 9, DO = 10, ELSE = 11, 
	END = 12, FALSE = 13, FOR = 14, IF = 15, INPUT = 16, INTEGER = 17, NOT = 18, OF = 19, OR = 20, OUTPUT = 21, 
	PROCEDURE = 22, PROGRAM = 23, READ = 24, REAL = 25, REPEAT = 26, SET = 27, STOP = 28, THEN = 29, TO = 30,
	TRUE = 31, UNTIL = 32, VAR = 33, WHILE = 34, WRITE = 35, ID = 36, NUM = 37, LPAREN = 39, RPAREN = 40, 
	TIMES = 41, PLUS = 43, MINUS = 45, OVER = 48, EQ = 50, ASSIGN = 51, SEMI = 52, LT = 53
} TokenType;
typedef enum{
	T_INTEGER, T_BOOL, T_CHAR
} VarType;
template <class T> struct greater : binary_function <T, T, bool> {
	bool operator() (const T& x, const T& y) const
	{
		return x>y;
	}
};
void SplitString(const string& s, vector<string>& v, const string& c);
#endif