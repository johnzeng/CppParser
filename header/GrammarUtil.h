#ifndef GRAMAUTIL_H
#define GRAMAUTIL_H

#include "JZCommonDefine.h"
using namespace std;

struct OperatorType
{
	int associativity;
	uint32 priority;
	uint32 mark;
	string symbol;
};
typedef map<string, OperatorType> OpTypeMap;
enum AssociativityType
{
	Left2Right,
	Right2Left,
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
	eOpMultiplication,		//*
	eOpDivide,				// /
	eOpMoludus,				//%
	
	//priority 4
	eOpPlus,				//+
	eOpMinus,				//-

	//priority 5
	eOpBitwiseLeftShift,		//<<
	eOpBitwiseRihtShift,		//>>

	//priority 6
	eOpRelationalOperatorLess,			//<
	eOpRelationalOperatorLarger, 		//>
	eOpRelationalOperatorLessEqual,		//<=
	eOpRelationalOperatorLargerEqual, 	//>=

	//priority 7
	eOpRelationalOperatorEqual, 		//==
	eOpRelationalOperatorNotEqual, 		//!=
	
	//priority 8
	eOpBitwiseAnd,				//&
	//priority 9
	eOpBitwiseXOr,				//^
	//priority 10
	eOpBitwiseOr,				//|
	//priority 11
	eOpLogicalAnd,				//&&
	//priority 12
	eOpLogicalOr,				//||
	//priority 13
	eOpTernaryConditionalAsk,	//? in ?:
	eOpTernaryConditionalSelect,	//? in ?:
};

#define GrmUtilPtr GrammarUtil::getInstance()
class GrammarUtil {
public:
	
	enum GrammarUtilReturn
	{
		eGrammarNotOperator = 999,	
	};

	virtual ~GrammarUtil ();

	static GrammarUtil* getInstance();

	bool isOperator(const string& input);

	bool isPreprocessOperator(const string& input);

	uint32 getOperatorPriority(const string& input,int associativity = Left2Right);

	const OperatorType* getOperatorData(const string& input, int associativity = Left2Right);

	bool isConstNumber(const string& input);
	
private:

	//init func
	GrammarUtil ();
	void init();
	void insertOperatorToPreprocessOpSet(const string& op,uint32 priority, uint32 mark,int associativity = Left2Right);
	void insertOperatorToOpSet(const string& op,uint32 priority, uint32 mark,int associativity = Left2Right);
	void insertOperatorTo(OpTypeMap& insertSet, const string& op,uint32 priority, uint32 mark,int associativity = Left2Right);

private:

	OpTypeMap mOperatorSetR2L;
	OpTypeMap mPreprocessOperatorSetR2L;
	OpTypeMap mOperatorSetL2R;
	OpTypeMap mPreprocessOperatorSetL2R;
	StringSet mKeyWordSet;
};
#endif /* end of include guard: GRAMAUTIL_H */
