#include <iostream>
#include <vector>
using namespace std;
class State{
public:
	vector<int> true_chain;//�����������
	vector<int> false_chain;//������ļ���
	int codebegin;
	bool accept;
	State(){ accept = true; }
	void addTrueChain(State &st);
	void addFalseChain(State &st);
	void true_pushback(int a){ true_chain.push_back(a); }
	void false_pushback(int a){ false_chain.push_back(a); }
	void swapChain();//����notʱ���������
};