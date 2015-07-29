#ifndef GRAMAUTIL_H
#define GRAMAUTIL_H

#include "JZCommonDefine.h"
using namespace std;

struct OperatorType
{
	int priority;
	int mark;
	string symbol;
};

enum OperatorMark
{
	//priority 1
	eOpLeftBracket,			//(
	eOpRightBracket,		//)

	//priority 2
	eOpLogicalNegation,		//!
	eOpBitwiseComplement,	//~
	eOpUnaryMinus,			//- (like -1)
	eOpUnaryPlus,			//+ (like +1)

	//priority 3
	//not useful yet
	
	//priority 4
	eOpMultiplication,		//*
	eOpDivide,				// /
	eOpMoludus,				//%

	eOpComparisonEqualTo,	//==
};

class GrammarUtil {
public:
	GrammarUtil ();
	virtual ~GrammarUtil ();

	void init();
	static GrammarUtil* getInstance();

	bool isOperator(const string& input);
	bool isPreprocessOperator(const string& input);

private:
	StringSet mOperatorSet;
	StringSet mPreprocessOperatorSet;
	StringSet mKeyWordSet;
};
#endif /* end of include guard: GRAMAUTIL_H */
