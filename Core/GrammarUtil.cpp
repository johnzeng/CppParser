#include "GrammarUtil.h"
#include "JZLogger.h"

GrammarUtil::GrammarUtil()
{
}

GrammarUtil::~GrammarUtil()
{
}

GrammarUtil* GrammarUtil::getInstance()
{
	static GrammarUtil* instance = NULL;
	if (NULL == instance)
	{
		instance = new GrammarUtil();
		instance->init();
	}
	return instance;
}

void GrammarUtil::init()
{
	/*********************************************************
		operator only useful in preprocess 
	 ********************************************************/
	
	insertOperatorToPreprocessOpSet("(",ePriority1,eOpLeftBracket,0,Right2Left);
	insertOperatorToPreprocessOpSet(")",ePriority1,eOpRightBracket,1);

	insertOperatorToPreprocessOpSet("!",ePriority2,eOpLogicalNegation,1,Right2Left);
	insertOperatorToPreprocessOpSet("~",ePriority2,eOpBitwiseComplement,1,Right2Left);
	insertOperatorToPreprocessOpSet(STAND_WORD_PREFIX"-",ePriority2,eOpUnaryMinus,1,Right2Left);
	insertOperatorToPreprocessOpSet(STAND_WORD_PREFIX"+",ePriority2,eOpUnaryPlus,1,Right2Left);


	insertOperatorToPreprocessOpSet("*",ePriority3,eOpMultiplication,2);
	insertOperatorToPreprocessOpSet("/",ePriority3,eOpDivide,2);
	insertOperatorToPreprocessOpSet("%",ePriority3,eOpMoludus,2);

	insertOperatorToPreprocessOpSet("+",ePriority4,eOpPlus,2);
	insertOperatorToPreprocessOpSet("-",ePriority4,eOpMinus,2);

	insertOperatorToPreprocessOpSet("<<",ePriority5,eOpBitwiseLeftShift,2);
	insertOperatorToPreprocessOpSet(">>",ePriority5,eOpBitwiseRihtShift,2);

	insertOperatorToPreprocessOpSet("<",ePriority6,eOpRelationalOperatorLess,2);
	insertOperatorToPreprocessOpSet("<=",ePriority6,eOpRelationalOperatorLessEqual,2);
	insertOperatorToPreprocessOpSet(">",ePriority6,eOpRelationalOperatorLarger,2);
	insertOperatorToPreprocessOpSet(">=",ePriority6,eOpRelationalOperatorLargerEqual,2);

	insertOperatorToPreprocessOpSet("==",ePriority7,eOpRelationalOperatorEqual,2);
	insertOperatorToPreprocessOpSet("!=",ePriority7,eOpRelationalOperatorNotEqual,2);

	insertOperatorToPreprocessOpSet("&",ePriority8,eOpBitwiseAnd,2);

	insertOperatorToPreprocessOpSet("^",ePriority9,eOpBitwiseXOr,2);

	insertOperatorToPreprocessOpSet("|",ePriority10,eOpBitwiseOr,2);

	insertOperatorToPreprocessOpSet("&&",ePriority11,eOpLogicalAnd,2);

	insertOperatorToPreprocessOpSet("||",ePriority12,eOpLogicalOr,2);


	insertOperatorToPreprocessOpSet("?",ePriority13,eOpTernaryConditionalAsk,0,Right2Left);
	insertOperatorToPreprocessOpSet(":",ePriority13,eOpTernaryConditionalSelect,3,Right2Left);

	insertOperatorToPreprocessOpSet(BASE_OPERATOR,ePriorityLowest,eOpLowestOperator,0);
	/*********************************************************
		operator useful not only in preprocess 
	 ********************************************************/
	
}

void GrammarUtil::insertOperatorToPreprocessOpSet(const string& op,uint32 priority, uint32 mark,int opNum, int associativity)
{
	insertOperatorTo(mPreprocessOperatorSet,op,priority,mark,opNum,associativity);
}


bool GrammarUtil::isDoulbeMeanWord(const string& input)
{
	string standWord = STAND_WORD_PREFIX ;
	standWord += input;
	auto it = mDoubleMeanWord.find(standWord);
	if (it != mDoubleMeanWord.end())
	{
		return true;
	}
	return false;
}

string GrammarUtil::src2StandWord(const string& input)
{
	if (false == isDoulbeMeanWord(input))
	{
		return "";
	}
	string ret = STAND_WORD_PREFIX ;
	ret += input;
	return ret;
}

void GrammarUtil::insertOperatorTo(OpTypeMap &insertMap, const string& op,uint32 priority, uint32 mark,int opNum, int associativity)
{
	if (op == "")
	{
		JZWRITE_DEBUG("no input!");
		return;
	}
	OperatorType toInsertType = 
	{
		.priority = priority,
		.mark = mark,
		.symbol = op,
		.associativity = associativity,
		.operatorNum = opNum,
	};
	insertMap[op] = toInsertType;
	//any way , mark is unique
	mOpMarkMap[mark] = toInsertType;
	if ('#' == op[0])
	{
		mDoubleMeanWord.insert(op);
	}
}

const OperatorType* GrammarUtil::getOperatorData(const string& input)
{
	OperatorType* ret = NULL;
	auto it = mOperatorSet.find(input);
	if (it != mOperatorSet.end())
	{
		ret = &it->second;
	}
	if (NULL == ret)
	{
		auto it = mPreprocessOperatorSet.find(input);
		if (it != mPreprocessOperatorSet.end())
		{
			ret = &it->second;
		}
	}
	return ret;
}

uint32 GrammarUtil::getOperatorPriority(const string& input)
{
	auto dataPtr = getOperatorData(input); 
	if (NULL == dataPtr)
	{
		return eGrammarNotOperator;
	}
	return dataPtr->priority;
}

void GrammarUtil::insertOperatorToOpSet(const string& op,uint32 priority, uint32 mark,int opNum,int associativity)
{
	insertOperatorTo(mOperatorSet,op,priority,mark,opNum,associativity);
}

bool GrammarUtil::isOperator(const string& input)
{
	auto ptr = getOperatorData(input);
	if (NULL != ptr)
	{
		return true ;
	}
	return false;
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

bool GrammarUtil::isConstNumber(const string& input)
{
	for (int i = 0; i < input.size(); i++) 
	{
		if (input[i] > '0' && input[i] < '9')
		{
			continue;
		}
		else
		{
			return false;	
		}
	}
	return true;
}
