#include "stdafx.h"
#include "State.h"
void State::addTrueChain(State &st)
{
	true_chain.insert(true_chain.end(),st.true_chain.begin(), st.true_chain.end());
}
void State::addFalseChain(State &st)
{
	false_chain.insert(false_chain.end(), st.false_chain.begin(), st.false_chain.end());
}
void State::swapChain()
{
	vector<int> temp = true_chain;
	true_chain = false_chain;
	false_chain = temp;
}