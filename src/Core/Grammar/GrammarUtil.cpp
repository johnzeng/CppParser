#include "GrammarUtil.h"
#include "JZLogger.h"
#include "Lex.h"
#include <stdlib.h>

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

  /*********************************************************
    add key words 
   ********************************************************/
  
  mKeyWordSet.insert("int");
  mKeyWordSet.insert("typename");
  mKeyWordSet.insert("noexcept");
  mKeyWordSet.insert("new");
  mKeyWordSet.insert("delete");
  mKeyWordSet.insert("inline");
  mKeyWordSet.insert("explicit");
  mKeyWordSet.insert("virtual");
  mKeyWordSet.insert("dynamic_cast");
  mKeyWordSet.insert("case");
  mKeyWordSet.insert("static_assert");
  mKeyWordSet.insert("static_cast");
  mKeyWordSet.insert("const_cast");
  mKeyWordSet.insert("reinterpret_cast");
  mKeyWordSet.insert("operator");
  mKeyWordSet.insert("long");
  mKeyWordSet.insert("unsigned");
  mKeyWordSet.insert("class");
  mKeyWordSet.insert("union");
  mKeyWordSet.insert("enum");
  mKeyWordSet.insert("static");
  mKeyWordSet.insert("struct");
  mKeyWordSet.insert("char");
  mKeyWordSet.insert("double");
  mKeyWordSet.insert("float");
  mKeyWordSet.insert("bool");
  mKeyWordSet.insert("void");
  mKeyWordSet.insert("try");
  mKeyWordSet.insert("catch");
  mKeyWordSet.insert("const");
  mKeyWordSet.insert("volatile");
  mKeyWordSet.insert("override");
  mKeyWordSet.insert("final");
  mKeyWordSet.insert("typedef");
  mKeyWordSet.insert("private");
  mKeyWordSet.insert("public");
  mKeyWordSet.insert("protected");
  mKeyWordSet.insert("if");
  mKeyWordSet.insert("else");
  mKeyWordSet.insert("while");
  mKeyWordSet.insert("for");
  mKeyWordSet.insert("switch");
  mKeyWordSet.insert("goto");
  mKeyWordSet.insert("return");
  mKeyWordSet.insert("auto");
  mKeyWordSet.insert("NULL");
  mKeyWordSet.insert("mutable");
  mKeyWordSet.insert("template");
  mKeyWordSet.insert("thread_local");
  mKeyWordSet.insert("extern");
  mKeyWordSet.insert("register");
  mKeyWordSet.insert("+");
  mKeyWordSet.insert("-");
  mKeyWordSet.insert("*");
  mKeyWordSet.insert("/");
  mKeyWordSet.insert("%");
  mKeyWordSet.insert("++");
  mKeyWordSet.insert("--");
  mKeyWordSet.insert("+=");
  mKeyWordSet.insert("-=");
  mKeyWordSet.insert("==");
  mKeyWordSet.insert("!=");
  mKeyWordSet.insert("<<");
  mKeyWordSet.insert(">>");
  mKeyWordSet.insert(">=");
  mKeyWordSet.insert("<=");
  mKeyWordSet.insert(";");
  mKeyWordSet.insert("(");
  mKeyWordSet.insert(")");
  mKeyWordSet.insert("{");
  mKeyWordSet.insert("}");
  mKeyWordSet.insert("[");
  mKeyWordSet.insert("]");
  mKeyWordSet.insert("<");
  mKeyWordSet.insert(">");
  mKeyWordSet.insert(",");
  mKeyWordSet.insert(".");
  
//  mLoopBreakKey.insert(";");
  mLoopBreakKey.insert(")");
  mLoopBreakKey.insert("}");
  mLoopBreakKey.insert("]");
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
	OperatorType toInsertType;

  toInsertType.priority = priority;
  toInsertType.mark = mark;
  toInsertType.symbol = op;
  toInsertType.associativity = associativity;
  toInsertType.operatorNum = opNum;

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
		return input - 'A' + 10;
	}
	if (input >= 'a' && input <= 'f')
	{
		return input - 'a' + 10;
	}
	return 0;
}

bool GrammarUtil::isConstNumber(const string& input)
{
	return isDeci(input) || isHex(input) || isOcto(input) || isFloatNumber(input);
}

bool GrammarUtil::isConstIntNumber(const string& input)
{
	return isDeci(input) || isHex(input) || isOcto(input); 
}

bool GrammarUtil::isFloatNumber(const string& input)
{
	int pointNum = 0;
  bool isScienceNumber = false;
	for(int i = 0 ; i < input.size() ; i++)
	{
		if (LexUtil::isConstNumberChar(input[i]))
		{
      if(0 == i && '0' == input[i])
      {
        if(1 == input.size())
        {
          return true;
        }
        if (2 >= input.size() && input[1] != '.')
        {
          return false;
        }
      }
      continue;
		}
    if(true == isScienceNumber)
    {
      return false;
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
    if(i == input.size() - 1)
    {
      return 'f' == input[i] || 'F' == input[i];
    }
    else if(i != 0 && 'e' == input[i] && input.size() != i+1)
    {
      isScienceNumber = true;
      if('+' == input[i+1] || '-' == input[i + 1])
      {
        i++;
        if(input.size() == i + 1)
        {
          return false;
        }
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
    else if(i >= input.size() - 3)
    {
      return endWithDeciSuffix(input, i);
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
    else if(i >= input.size() - 3)
    {
      return endWithDeciSuffix(input, i);
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
    else if(i >= input.size() - 3)
    {
      return endWithDeciSuffix(input, i);
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
  if(false == isFloatNumber(input))
  {
    JZWRITE_DEBUG("return because false float number")
    return 0.0f;
  }
  char *endPointer = NULL;
	return strtof(input.c_str(), &endPointer);
}

double GrammarUtil::str2double(const string& input)
{
  if(false == isFloatNumber(input))
  {
    return 0.0;
  }
  char *endPointer = NULL;
  return strtod(input.c_str(), &endPointer);
}


template <typename type> type GrammarUtil::str2Num(const string& input)
{
	type ret = 0;
	type dim = 0;	
	int i = 0;
  JZWRITE_DEBUG("input is : %s" , input.c_str());
  if(true == isFloatNumber(input))
  {
    JZWRITE_ERROR("Should not use this function to trans double" );
    return (type)0;
  }
	if (true == isDeci(input))
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
//  JZWRITE_DEBUG("dim is:%lld, i is:%d" , dim, i);
	if (dim == 0)
	{
		return 0;
	}
	for (; i < input.size(); i++) 
	{
    if('L' == input[i] || 'l' == input[i] || 'U' == input[i] || 'u' == input[i])
    {
      break;
    }
		ret *= (type)dim;
		ret += (type)getNum(input[i]);
	}
	return ret;
	
}

bool GrammarUtil::isKeyWord(const string& input)
{
  auto it = mKeyWordSet.find(input);
  if(mKeyWordSet.end() != it)
  {
    return true;
  }

  return false;
}

bool GrammarUtil::endWithDeciSuffix(const string& input, int fromIndex)
{
  int32 size = input.size();
  if(size <= 1)
  {
    return false;
  }
  int32 lastIndex = size - 1;
  if(lastIndex == fromIndex){
    return input[lastIndex] == 'l' || input[lastIndex] == 'L' || input[lastIndex] == 'u'|| input[lastIndex] == 'U';
  }
  else if(lastIndex == fromIndex + 1)
  {
    return ( input[lastIndex] == 'l' || input[lastIndex] == 'L' || input[lastIndex] == 'U' || input[lastIndex] == 'u') &&
      ( input[lastIndex - 1] == 'l' || input[lastIndex - 1] == 'L' || input[lastIndex - 1] == 'U' || input[lastIndex - 1] == 'u');
  }
  else if(lastIndex == fromIndex + 2)
  {
    return ( input[lastIndex] == 'l' || input[lastIndex] == 'L'|| input[lastIndex] == 'U' || input[lastIndex] == 'u') &&
      ( input[lastIndex - 1] == 'l' || input[lastIndex - 1] == 'L') &&
      ( input[lastIndex - 2] == 'l' || input[lastIndex - 2] == 'L'|| input[lastIndex - 2] == 'U' || input[lastIndex - 2] == 'u');
  }
  
  return false;
}

bool GrammarUtil::isLoopBreakerKey(const string& key)
{
  auto it = mLoopBreakKey.find(key);
  return mLoopBreakKey.end() != it;
}
