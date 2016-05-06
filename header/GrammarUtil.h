#ifndef GRAMAUTIL_H
#define GRAMAUTIL_H

#include "JZCommonDefine.h"
using namespace std;

#define BASE_OPERATOR "#######"
#define STAND_WORD_PREFIX "#"
#define GrmUtilPtr GrammarUtil::getInstance()


enum MacroCheckDataType
{
	eMacroCheckDataNumber,
	eMacroCheckDataOperator,
};

struct OperatorType
{
	int associativity;
	uint32 priority;
	uint32 mark;
	string symbol;
	int operatorNum;
};

struct MacroCheckData
{
	string word;
	int32 type;
	uint32 mark;		//if this is a operator, this will be symbol mar
	int64 number;		//if this is a number, this will set
	uint32 priority;	
	const OperatorType* opPtr;
};

typedef stack<MacroCheckData> MacroCheckDataStack;

typedef map<string, OperatorType> OpTypeMap;
typedef map<int, OperatorType> OpTypeMarkMap;
enum AssociativityType
{
	eLeft2Right,
	eRight2Left,
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
class GrammarUtil {
public:
	
	enum GrammarUtilReturn
	{
		eGrammarNotOperator = 999,	
	};

	virtual ~GrammarUtil ();

	static GrammarUtil* getInstance();

  bool isKeyWord(const string& input);

	bool isOperator(const string& input);

	bool isPreprocessOperator(const string& input);

	uint32 getOperatorPriority(const string& input);

	const OperatorType* getOperatorData(const string& input);

	bool isConstNumber(const string& input);

	bool isConstIntNumber(const string& input);

	bool isFloatNumber(const string& input);

	bool isDoulbeMeanWord(const string& input);

	string src2StandWord(const string& input);

	uint64 str2Uint64(const string& input);
	int64 str2int64(const string& input);
	uint32 str2Uint32(const string& input);
	int str2int32(const string& input);

	float str2float(const string& input);
	double str2double(const string& input);
  
  uint32 setMacroCheckData(string& word,MacroCheckData& ret);
  int64 singleOperatorNum(int mark, int64 number);
  int64 doubleOperatorNum(int mark,int64 left,int64 right);

	bool isHex(const string& input);
	bool isOcto(const string& input);
	bool isDeci(const string& input);
  bool endWithDeciSuffix(const string& input,int fromIndex);
	int getNum(const char input);

	template <typename type> type str2Num(const string& input);

private:

	//init func
	GrammarUtil ();
	void init();
	void insertOperatorToPreprocessOpSet(const string& op,uint32 priority, uint32 mark,int opNum,int associativity = eLeft2Right);
	void insertOperatorToOpSet(const string& op,uint32 priority, uint32 mark,int opNum,int associativity = eLeft2Right);
	void insertOperatorTo(OpTypeMap& insertSet, const string& op,uint32 priority, uint32 mark,int opNum,int associativity = eLeft2Right);

private:

	OpTypeMap mOperatorSet;
	OpTypeMap mPreprocessOperatorSet;
	StringSet mDoubleMeanWord;
	OpTypeMarkMap mOpMarkMap;
	StringSet mKeyWordSet;
};

#endif /* end of include guard: GRAMAUTIL_H */
