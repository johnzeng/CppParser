#include "GrammarUtil.h"

GrammarUtil::GrammarUtil()
{
	init();
}

GrammarUtil::~GrammarUtil()
{
}

void GrammarUtil::init()
{
	/*********************************************************
		operator only useful in preprocess 
	 ********************************************************/
	
	mPreprocessOperatorSet.insert("(");
	mPreprocessOperatorSet.insert(")");
	mPreprocessOperatorSet.insert("<");
	mPreprocessOperatorSet.insert(">");
	mPreprocessOperatorSet.insert(">=");
	mPreprocessOperatorSet.insert("<=");
	mPreprocessOperatorSet.insert("+");
	mPreprocessOperatorSet.insert("-");
	mPreprocessOperatorSet.insert("*");
	mPreprocessOperatorSet.insert("/");
	mPreprocessOperatorSet.insert("%");
	mPreprocessOperatorSet.insert("|");
	mPreprocessOperatorSet.insert("||");
	mPreprocessOperatorSet.insert("&");
	mPreprocessOperatorSet.insert("&&");
	mPreprocessOperatorSet.insert("==");
	mPreprocessOperatorSet.insert("!=");
	mPreprocessOperatorSet.insert("!");
	mPreprocessOperatorSet.insert("~");  //~01001 = 10110
	mPreprocessOperatorSet.insert("^");  //01^11 = 10 ,xor
	mPreprocessOperatorSet.insert("<<");
	mPreprocessOperatorSet.insert(">>");

	// isTrue() ? 1:0
	mPreprocessOperatorSet.insert("?");
	mPreprocessOperatorSet.insert(":");

	/*********************************************************
		operator useful not only in preprocess 
	 ********************************************************/
	
	mOperatorSet.insert("++");
	mOperatorSet.insert("+=");
	mOperatorSet.insert("--");
	mOperatorSet.insert("-=");
	mOperatorSet.insert("%=");
	mOperatorSet.insert("->");
	mOperatorSet.insert("=");
	mOperatorSet.insert("<<=");
	mOperatorSet.insert(">>=");
	mOperatorSet.insert(".");
	mOperatorSet.insert("{");
	mOperatorSet.insert("}");
	mOperatorSet.insert("[");
	mOperatorSet.insert("]");
	mOperatorSet.insert(",");
	mOperatorSet.insert(".");

	//# and ## is useful when expending macro,but not useful in #if,
	//so I insert it here,normally,you will not meet this operator 
	//after the expending
	mOperatorSet.insert("#");
	mOperatorSet.insert("##");
}

bool GrammarUtil::isOperator(const string& input)
{
	auto it = mOperatorSet.find(input);
	if (it != mOperatorSet.end())
	{
		return true;
	}
	return isPreprocessOperator(input);
}

bool GrammarUtil::isPreprocessOperator(const string& input)
{
	auto it = mPreprocessOperatorSet.find(input);
	if (it != mPreprocessOperatorSet.end())
	{
		return true;
	}
	return false;
}
