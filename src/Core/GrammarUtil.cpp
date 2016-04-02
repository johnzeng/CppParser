#include "GrammarUtil.h"
#include "JZLogger.h"
#include "Lex.h"

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
	

	insertOperatorToPreprocessOpSet("!",ePriority2,eOpLogicalNegation,1,eRight2Left);
	insertOperatorToPreprocessOpSet("~",ePriority2,eOpBitwiseComplement,1,eRight2Left);
	insertOperatorToPreprocessOpSet(STAND_WORD_PREFIX"-",ePriority2,eOpUnaryMinus,1,eRight2Left);
	insertOperatorToPreprocessOpSet(STAND_WORD_PREFIX"+",ePriority2,eOpUnaryPlus,1,eRight2Left);


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

	insertOperatorToPreprocessOpSet("?",ePrioritySelect,eOpTernaryConditionalAsk,0,eRight2Left);
	insertOperatorToPreprocessOpSet(":",ePrioritySelect,eOpTernaryConditionalSelect,3,eRight2Left);

	insertOperatorToPreprocessOpSet("(",ePriorityLeftBracket,eOpLeftBracket,0,eRight2Left);
	insertOperatorToPreprocessOpSet(")",ePriorityRightBracket,eOpRightBracket,1);

	insertOperatorToPreprocessOpSet(BASE_OPERATOR,ePriorityLowest,eOpLowestOperator,0,eRight2Left);
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

uint64 GrammarUtil::str2Uint64(const string& input)
{
	return str2Num<uint64>(input);
}

int64 GrammarUtil::str2int64(const string& input)
{
	return str2Num<int64>(input);
}
uint32 GrammarUtil::str2Uint32(const string& input)
{
	return str2Num<uint32>(input);
}
int GrammarUtil::str2int32(const string& input)
{
	return str2Num<int>(input);
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

int GrammarUtil::getNum(const char input)
{
	if (input >= '0' && input <= '9')
	{
		return input - '0';
	}
	if (input >= 'A' && input <= 'F')
	{
		return input - 'A' + 9;
	}
	if (input >= 'a' && input <= 'f')
	{
		return input - 'a' + 9;
	}
	return 0;
}

bool GrammarUtil::isConstNumber(const string& input)
{
	return isDeci(input) || isHex(input) || isOcto(input);
}

bool GrammarUtil::isFloatNumber(const string& input)
{
	int pointNum;
	for(int i = 0 ; i < input.size() ; i++)
	{
		if (LexUtil::isConstNumberChar(input[i]))
		{
      if(0 == i && '0' == input[i])
      {
        return false;
      }
			continue;
		}
		if ('.' == input[i])
		{
			pointNum++;
			if (pointNum > 1)
			{
				return false;
			}
			continue;
		}
		return false;
	}
	return pointNum == 1;
}

bool GrammarUtil::isDeci(const string& input)
{
	if (input.size() == 0)
	{
		return false;
	}
	if (input.size() >1 && input[0] == '0')
	{
		return false;
	}
	for (int i = 0; i < input.size(); i++) 
	{
		if (true == LexUtil::isConstNumberChar(input[i]))
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

bool GrammarUtil::isHex(const string& input)
{
	if (input.size() < 3)
	{
		return false;
	}
	if (input[0] != '0' || input[1] != 'x')
	{
		return false;
	}
	for (int i = 2; i < input.size(); i++) 
	{
		if (true == LexUtil::isConstNumberChar(input[i]))
		{
			continue;
		}
		else if(input[i] >='A' && input[i] <= 'F')
		{
			continue;	
		}
		else if(input[i] >='a' && input[i] <= 'f')
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

bool GrammarUtil::isOcto(const string& input)
{
	if (input.size() < 2)
	{
		return false;
	}
	if (input[0] != '0')
	{
		return false;
	}
	for (int i = 2; i < input.size(); i++) 
	{
		//for octo,you can use only 0 to 7
		if (input[i] >= '0' && input[i] <= '7')
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

float GrammarUtil::str2float(const string& input)
{
	return str2Num<float>(input);
}

double GrammarUtil::str2double(const string& input)
{
	return str2Num<double>(input);
}

template <typename type> type GrammarUtil::str2Num(const string& input)
{
	type ret = 0;
	type dim = 0;	
	int i = 0;
	if (true == isOcto(input) || true == isFloatNumber(input))
	{
		dim = 10;
	}
	else if (true == isHex(input))
	{
		dim = 16;
		i = 2;
	}
	else if (true == isOcto(input))
	{
		dim = 8;
		i = 1;
	}
	if (dim == 0)
	{
		return 0;
	}
	for (; i < input.size(); i++) 
	{
		ret *= (type)dim;
		ret += (type)getNum(input[i]);
	}
	return ret;
	
}
