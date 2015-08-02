#include "LexData.h"
#include "Lex.h"
#include "GrammarUtil.h"
#include "StringUtil.h"
#include "JZLogger.h"
#include <unistd.h>
using namespace LexUtil;

enum MacroCheckDataType
{
	eMacroCheckDataNumber,
	eMacroCheckDataOperator,
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

int64 singleOperatorNum(int mark, int64 number);
int64 doubleOperatorNum(int mark,int64 left,int64 right);
typedef stack<MacroCheckData> MacroCheckDataStack;

uint32 Lex::isMacroSuccess(const LexRecList& logic, bool* ret)
{

	JZFUNC_BEGIN_LOG();
	*ret = false;
	MacroCheckDataStack symStack;
	MacroCheckDataStack numStack;
	MacroCheckDataStack opStack;
	
	auto baseOpPtr = GrmUtilPtr->getOperatorData(BASE_OPERATOR);
	if (NULL == baseOpPtr)
	{
		JZFUNC_END_LOG();
		return eLexUnknowError;
	}
	MacroCheckData baseData = 
	{
		.word = BASE_OPERATOR,
		.mark = baseOpPtr->mark,
		.number = 0,
		.priority = baseOpPtr->priority,
		.opPtr = baseOpPtr,
		.type = eMacroCheckDataOperator,
	};
	symStack.push(baseData);
	opStack.push(baseData);


	LexRecList localLogic = logic;
	LexRec lastRec;
	lastRec.word = BASE_OPERATOR;
	localLogic.push_back(lastRec);

	for(int i = 0 ; i < localLogic.size(); i ++)
	{
		auto rec = localLogic[i];
		string word = rec.word;	
		if (true == GrmUtilPtr->isDoulbeMeanWord(word))
		{
			JZWRITE_DEBUG("if need to change to stand word");
			if ("+" == word || "-" == word)
			{
				if (!symStack.empty() && (symStack.top().mark == eOpRightBracket || symStack.top().mark == eOpNotOp))
				{
					//no need to change!
					JZWRITE_DEBUG("normal plus and minus");
				}
				else
				{
					word = GrmUtilPtr->src2StandWord(word);	
				}
			}
			else if(BASE_OPERATOR == word)
			{
				//do nothing	
			}
			else
			{
				JZWRITE_DEBUG("unhandled double mean word:%s",word.c_str());	
				JZFUNC_END_LOG();
				return eLexUnknowError;
			}
		}
		bool isOperator = GrmUtilPtr->isOperator(word);
		bool isNum = GrmUtilPtr->isConstNumber(word);
		MacroCheckData data = 
		{
			.word = word,
			.opPtr = NULL,
		};
		if (false == isOperator && false == isNum)
		{
			//this is a undefine word,means ,0
			data.type = eMacroCheckDataNumber;
			data.mark = eOpNotOp;
			data.number = 0;
			data.priority = 0;
		}
		else if (true == isOperator)
		{
			data.type = eMacroCheckDataOperator;
			data.number = 0;
			const OperatorType* opPtr = GrmUtilPtr->getOperatorData(word);
			if (NULL == opPtr)
			{
				JZFUNC_END_LOG();
				return eLexUnknowError;
			}
			data.priority = opPtr->priority;
			data.mark = opPtr->mark;
			data.opPtr = opPtr;
		}
		else
		{
			data.priority = 0;
			data.type = eMacroCheckDataNumber;
			data.number = StringUtil::str2int64(word);		
		}
		symStack.push(data);
		JZWRITE_DEBUG("data analyze fin,now cal");


		if (data.type == eMacroCheckDataNumber)
		{
			//this is a number ,just push into the stack
			numStack.push(data);
		}
		else
		{
			//this is a symbol, cal
			bool matchLeftBracket = false;
			while(!opStack.empty() && opStack.top().priority <= data.priority)
			{
				auto topOp = opStack.top();
				opStack.pop();
				int numberNum = topOp.opPtr->operatorNum;
				if (topOp.word == "(")
				{
					matchLeftBracket = true;
				}
				switch(numberNum)
				{
					case 0:
						break;
					case 1:
						{
							if (numStack.size() < 1)
							{
								return eLexUnknowError;
							}
							auto topNum = numStack.top();
							numStack.pop();
							int64 retNum = singleOperatorNum(topOp.mark,topNum.number);	
							topNum.number = retNum;
							numStack.push(topNum);
						}
						break;
					case 2:
						{
							if (numStack.size() < 2)
							{
								return eLexUnknowError;
							}
							auto topNum = numStack.top();
							numStack.pop();
							auto secondNum = numStack.top();
							int retNum = doubleOperatorNum(topOp.mark, secondNum.number,topNum.number);
							topNum.number = retNum;
							numStack.push(topNum);
						}						
						break;
					case 3:
						{
							if (numStack.size() < 3 && numStack.size() < 1)
							{
								return eLexUnknowError;
							}
							auto topNum = numStack.top();
							numStack.pop();
							auto secondNum = numStack.top();
							numStack.pop();
							auto thirdNum = numStack.top();
							numStack.pop();
							auto secondOp = opStack.top();
							opStack.pop();
							if (secondOp.mark != eOpTernaryConditionalAsk)
							{
								JZFUNC_END_LOG();
								return eLexUnknowError;
							}
							if (true == thirdNum.number)
							{
								numStack.push(secondNum);
							}
							else
							{
								numStack.push(topNum);	
							}
						}
						break;
					default:
						JZWRITE_DEBUG("Unknow opnumber");
						break;
				}
				JZWRITE_DEBUG("handle operator end");
			}	
			if (")" == data.word)
			{
				if (false == matchLeftBracket)
				{
					return eLexUnexpectedSeperator;
				}
				continue;
			}
			else
			{
				opStack.push(data);
			}
		}
	}
	if (numStack.empty())
	{
		JZFUNC_END_LOG();
		return eLexUnknowError;
	}
	
	*ret = (true == numStack.top().number);
	JZWRITE_DEBUG("cal ret is :%s",*ret ? "true" : "false");
	JZFUNC_END_LOG();
	return eLexNoError;	
}

int64 singleOperatorNum(int mark, int64 number)
{
	switch(mark)
	{
		case eOpRightBracket:
			return number;
		case eOpLogicalNegation:
			return !number;
		case eOpBitwiseComplement:
			return ~number;
		case eOpUnaryMinus:
			return -number;
		case eOpUnaryPlus:
			return number;
		default:
		{
			return number;
			break;	
		}
	}

}

int64 doubleOperatorNum(int mark,int64 left,int64 right)
{
	switch(mark)
	{
		case eOpMultiplication:
			return left*right;
		case eOpDivide:
			if (right == 0)
			{
				JZWRITE_DEBUG("divide zero");
				return 0;
			}
			return left/right;
		case eOpMoludus:
			return left%right;
		case eOpPlus:
			return left+right;
		case eOpMinus:
			return left-right;
		case eOpLogicalAnd:
			return left&&right;
		default:
		{
			return 0;
		}
	}
}
