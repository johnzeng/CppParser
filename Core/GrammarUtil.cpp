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
	
	insertOperatorToPreprocessOpSet("(",1,eOpLeftBracket);
	insertOperatorToPreprocessOpSet(")",1,eOpRightBracket);

	insertOperatorToPreprocessOpSet("!",2,eOpLogicalNegation);
	insertOperatorToPreprocessOpSet("~",2,eOpBitwiseComplement);
	insertOperatorToPreprocessOpSet("-",2,eOpUnaryMinus,Right2Left);
	insertOperatorToPreprocessOpSet("+",2,eOpUnaryPlus,Right2Left);


	insertOperatorToPreprocessOpSet("*",3,eOpMultiplication);
	insertOperatorToPreprocessOpSet("/",3,eOpDivide);
	insertOperatorToPreprocessOpSet("%",3,eOpMoludus);

	insertOperatorToPreprocessOpSet("+",4,eOpPlus);
	insertOperatorToPreprocessOpSet("-",4,eOpMinus);

	insertOperatorToPreprocessOpSet("<<",5,eOpBitwiseLeftShift);
	insertOperatorToPreprocessOpSet(">>",5,eOpBitwiseRihtShift);

	insertOperatorToPreprocessOpSet("<",6,eOpRelationalOperatorLess);
	insertOperatorToPreprocessOpSet("<=",6,eOpRelationalOperatorLessEqual);
	insertOperatorToPreprocessOpSet(">",6,eOpRelationalOperatorLarger);
	insertOperatorToPreprocessOpSet(">=",6,eOpRelationalOperatorLargerEqual);

	insertOperatorToPreprocessOpSet("==",7,eOpRelationalOperatorEqual);
	insertOperatorToPreprocessOpSet("!=",7,eOpRelationalOperatorNotEqual);

	insertOperatorToPreprocessOpSet("&",8,eOpBitwiseAnd);

	insertOperatorToPreprocessOpSet("^",9,eOpBitwiseXOr);

	insertOperatorToPreprocessOpSet("|",10,eOpBitwiseOr);

	insertOperatorToPreprocessOpSet("&&",11,eOpLogicalAnd);

	insertOperatorToPreprocessOpSet("||",12,eOpLogicalOr);


	insertOperatorToPreprocessOpSet("?",13,eOpTernaryConditionalAsk,Right2Left);
	insertOperatorToPreprocessOpSet(":",13,eOpTernaryConditionalSelect,Right2Left);

	/*********************************************************
		operator useful not only in preprocess 
	 ********************************************************/
	
}

void GrammarUtil::insertOperatorToPreprocessOpSet(const string& op,uint32 priority, uint32 mark, int associativity)
{
	if (Left2Right == associativity)
	{
		
		insertOperatorTo(mPreprocessOperatorSetL2R,op,priority,mark,associativity);
	}
	else
	{
		insertOperatorTo(mPreprocessOperatorSetR2L,op,priority,mark,associativity);
	}
}


void GrammarUtil::insertOperatorTo(OpTypeMap &insertMap, const string& op,uint32 priority, uint32 mark, int associativity)
{
	OperatorType toInsertType = 
	{
		.priority = priority,
		.mark = mark,
		.symbol = op,
		.associativity = associativity,
	};
	insertMap[op] = toInsertType;
}

const OperatorType* GrammarUtil::getOperatorData(const string& input,int associativity)
{
	OperatorType* ret = NULL;
	if (Left2Right == associativity)
	{
		auto it = mOperatorSetL2R.find(input);
		if (it != mOperatorSetL2R.end())
		{
			ret = &it->second;
		}
		if (NULL == ret)
		{
			auto it = mPreprocessOperatorSetL2R.find(input);
			if (it != mPreprocessOperatorSetL2R.end())
			{
				ret = &it->second;
			}
		}
	}
	else
	{
		auto it = mOperatorSetR2L.find(input);
		if (it != mOperatorSetR2L.end())
		{
			ret = &it->second;
		}
		if (NULL == ret)
		{
			auto it = mPreprocessOperatorSetR2L.find(input);
			if (it != mPreprocessOperatorSetR2L.end())
			{
				ret = &it->second;
			}
		}
		
	}
	return ret;
}

uint32 GrammarUtil::getOperatorPriority(const string& input,int associativity)
{
	auto dataPtr = getOperatorData(input, associativity); 
	if (NULL == dataPtr)
	{
		return eGrammarNotOperator;
	}
	return dataPtr->priority;
}

void GrammarUtil::insertOperatorToOpSet(const string& op,uint32 priority, uint32 mark,int associativity)
{
	if (Left2Right == associativity)
	{
		insertOperatorTo(mOperatorSetL2R,op,priority,mark,associativity);
	}
	else
	{
		insertOperatorTo(mOperatorSetR2L,op,priority,mark, associativity);
	}
}

bool GrammarUtil::isOperator(const string& input)
{
	auto L2Rptr = getOperatorData(input, Left2Right);
	if (NULL != L2Rptr)
	{
		return true ;
	}
	auto R2Lptr = getOperatorData(input, Right2Left);
	if (NULL != R2Lptr)
	{
		return true;
	}
	return false;
}

bool GrammarUtil::isPreprocessOperator(const string& input)
{
	auto itL2R = mPreprocessOperatorSetL2R.find(input);
	if (itL2R != mPreprocessOperatorSetL2R.end())
	{
		return true;
	}
	auto itR2L = mPreprocessOperatorSetR2L.find(input);
	if (itR2L != mPreprocessOperatorSetR2L.end())
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
