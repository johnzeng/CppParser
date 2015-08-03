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
	int operatorNum;
};
typedef map<string, OperatorType> OpTypeMap;
typedef map<int, OperatorType> OpTypeMarkMap;
enum AssociativityType
{
	Left2Right,
	Right2Left,
};
enum OperatorMark
{
	//priority -1
	ePriorityLargest = -1 ,

	eOpNotOp = 0,			//

	//brackets
	ePriorityLeftBracket,
	eOpLeftBracket ,		//(

	ePriority1 ,

	//priority 2
	ePriority2 ,
	eOpLogicalNegation,		//!
	eOpBitwiseComplement,	//~
	eOpUnaryMinus,			//- (like -1) ,I change it into #-
	eOpUnaryPlus,			//+ (like +1) ,I change it into #+ to diff it

	//priority 3
	ePriority3 ,
	eOpMultiplication,		//*
	eOpDivide,				// /
	eOpMoludus,				//%
	
	//priority 4
	ePriority4 ,
	eOpPlus,				//+
	eOpMinus,				//-

	//priority 5
	ePriority5 ,
	eOpBitwiseLeftShift,		//<<
	eOpBitwiseRihtShift,		//>>

	//priority 6
	ePriority6 ,
	eOpRelationalOperatorLess,			//<
	eOpRelationalOperatorLarger, 		//>
	eOpRelationalOperatorLessEqual,		//<=
	eOpRelationalOperatorLargerEqual, 	//>=

	//priority 7
	ePriority7 ,
	eOpRelationalOperatorEqual, 		//==
	eOpRelationalOperatorNotEqual, 		//!=
	
	//priority 8
	ePriority8 ,
	eOpBitwiseAnd,				//&
	//priority 9
	ePriority9 ,
	eOpBitwiseXOr,				//^
	//priority 10
	ePriority10 ,
	eOpBitwiseOr,				//|
	//priority 11
	ePriority11 ,
	eOpLogicalAnd,				//&&
	//priority 12
	ePriority12 ,
	eOpLogicalOr,				//||

	ePrioritySelect,
	eOpTernaryConditionalAsk,		//? in ?:
	eOpTernaryConditionalSelect,	//: in ?:


	ePriorityRightBracket ,
	eOpRightBracket,		//)

	//priority 999
	ePriorityLowest ,
	eOpLowestOperator,	//####
};
#define BASE_OPERATOR "#######"
#define STAND_WORD_PREFIX "#"
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

	uint32 getOperatorPriority(const string& input);

	const OperatorType* getOperatorData(const string& input);

	bool isConstNumber(const string& input);

	bool isDoulbeMeanWord(const string& input);

	string src2StandWord(const string& input);
private:

	//init func
	GrammarUtil ();
	void init();
	void insertOperatorToPreprocessOpSet(const string& op,uint32 priority, uint32 mark,int opNum,int associativity = Left2Right);
	void insertOperatorToOpSet(const string& op,uint32 priority, uint32 mark,int opNum,int associativity = Left2Right);
	void insertOperatorTo(OpTypeMap& insertSet, const string& op,uint32 priority, uint32 mark,int opNum,int associativity = Left2Right);

private:

	OpTypeMap mOperatorSet;
	OpTypeMap mPreprocessOperatorSet;
	StringSet mDoubleMeanWord;
	OpTypeMarkMap mOpMarkMap;
	StringSet mKeyWordSet;
};
#endif /* end of include guard: GRAMAUTIL_H */
