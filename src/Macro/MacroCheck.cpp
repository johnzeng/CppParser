#include "LexData.h"
#include "MacroLex.h"
#include "GrammarUtil.h"
#include "StringUtil.h"
#include "JZLogger.h"
#include <unistd.h>
using namespace LexUtil;


uint32 MacroLex::isMacroSuccess(const LexRecList& logic, bool* ret)
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
	MacroCheckData baseData;

  baseData.word = BASE_OPERATOR;
  baseData.mark = baseOpPtr->mark;
  baseData.number = 0;
  baseData.priority = baseOpPtr->priority;
  baseData.opPtr = baseOpPtr;
  baseData.type = eMacroCheckDataOperator;

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
//      else
//      {
//        // it get a chance to be BASE_OPERATOR, but we don't need to handle it.
//      }
		}
		MacroCheckData data;
		uint32 setRet = GrmUtilPtr->setMacroCheckData(word, data);
		if (eLexNoError != setRet)
		{
			return setRet;
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
			//this is a symbol, cal1
			bool matchLeftBracket = false;
			while(!opStack.empty() && opStack.top().priority <= data.priority)
			{
				auto topOp = opStack.top();
				if (data.opPtr->associativity == eRight2Left && data.priority == topOp.priority)
				{
					break;
				}
				opStack.pop();
				int numberNum = topOp.opPtr->operatorNum;
				if (numStack.size() < numberNum )
				{
					JZWRITE_DEBUG("not rigth Number num");
					JZFUNC_END_LOG();
					return eLexUnknowError;
				}
				if (topOp.word == "(")
				{
					matchLeftBracket = true;
					if (data.word == ")")
					{
						break;
					}
				}
				switch(numberNum)
				{
					case 0:
						opStack.push(topOp);
						break;
					case 1:
						{
							auto topNum = numStack.top();
							numStack.pop();
							int64 retNum = GrmUtilPtr->singleOperatorNum(topOp.mark,topNum.number);	
							topNum.number = retNum;
							JZWRITE_DEBUG("push back num:%lld",topNum.number);
							numStack.push(topNum);
						}
						break;
					case 2:
						{
							auto topNum = numStack.top();
							numStack.pop();
							auto secondNum = numStack.top();
							numStack.pop();
							int retNum = GrmUtilPtr->doubleOperatorNum(topOp.mark, secondNum.number,topNum.number);

							topNum.number = retNum;
							JZWRITE_DEBUG("push back num:%lld",topNum.number);
							numStack.push(topNum);
						}						
						break;
					case 3:
						{
							if (opStack.size() < 1)
							{
								JZFUNC_END_LOG();
								return eLexUnknowError;
							}
							MacroCheckData curTopOp = topOp;
							MacroCheckDataStack tmpOp;
							MacroCheckDataStack tmpNum;
							while(!opStack.empty() && !numStack.empty() && opStack.top().mark != eOpTernaryConditionalAsk)
							{
								if (opStack.top().mark != eOpTernaryConditionalSelect)
								{
									JZFUNC_END_LOG();
									return eLexUnexpectedSeperator;
								}
								tmpOp.push(curTopOp);
								curTopOp = opStack.top();
								opStack.pop();
								tmpNum.push(numStack.top());
								numStack.pop();
							}
							if (opStack.empty())
							{
								JZFUNC_END_LOG();
								return eLexUnexpectedSeperator;
							}
							if (numStack.size() < 3)
							{
								JZFUNC_END_LOG();
								return eLexUnexpectedSeperator;
							}
							opStack.pop();
							auto topNum = numStack.top();
							numStack.pop();
							auto secondNum = numStack.top();
							numStack.pop();
							auto thirdNum = numStack.top();
							numStack.pop();
							if (false != thirdNum.number)
							{
								numStack.push(secondNum);
							}
							else
							{
								numStack.push(topNum);	
							}
							while(!tmpOp.empty())
							{
								opStack.push(tmpOp.top());	
								tmpOp.pop();
							}
							while(!tmpNum.empty())
							{
								numStack.push(tmpNum.top());
								tmpNum.pop();	
							}
							
						}
						break;
					default:
						JZWRITE_DEBUG("Unknow opnumber");
						break;
				}
				JZWRITE_DEBUG("handle operator end");
				if (true == matchLeftBracket)
				{
					break;
				}
			}
			if (")" == data.word)
			{
				if (false == matchLeftBracket)
				{
					JZFUNC_END_LOG();
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
	if (numStack.size() != 1)
	{
		JZFUNC_END_LOG();
		return eLexUnknowError;
	}
	
	JZWRITE_DEBUG("top number is :%lld", numStack.top().number);
	*ret = (false != numStack.top().number);
	JZWRITE_DEBUG("cal ret is :%s",*ret ? "true" : "false");
	JZFUNC_END_LOG();
	return eLexNoError;	
}

int64 GrammarUtil::singleOperatorNum(int mark, int64 number)
{
	switch(mark)
	{
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

int64 GrammarUtil::doubleOperatorNum(int mark,int64 left,int64 right)
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
			return left / right;
		case eOpBitwiseOr:
			return left | right;
		case eOpBitwiseAnd:
			return left & right;
		case eOpBitwiseXOr:
			return left ^ right;
		case eOpMoludus:
			return left % right;
		case eOpPlus:
			return left + right;
		case eOpMinus:
			return left - right;
		case eOpLogicalAnd:
			return left && right;
		case eOpLogicalOr:
			return left || right;
		case eOpRelationalOperatorEqual:
			return left == right;
		case eOpRelationalOperatorNotEqual:
			return left != right;
		case eOpRelationalOperatorLess:
			return left < right;
		case eOpRelationalOperatorLarger:
			return left > right;
		case eOpRelationalOperatorLessEqual:
			return left <= right;
		case eOpRelationalOperatorLargerEqual:
			return left >= right;
		case eOpBitwiseRihtShift:
			return left >> right;
		case eOpBitwiseLeftShift:
			return left << right;
		default:
		{
			return 0;
		}
	}
}

uint32 GrammarUtil::setMacroCheckData(string& word,MacroCheckData& data)
{
	JZFUNC_BEGIN_LOG();
	bool isOperator = GrmUtilPtr->isOperator(word);
	bool isNum = GrmUtilPtr->isConstIntNumber(word);
	data.word = word;
	data.opPtr = NULL;

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
			return LexBase::eLexUnknowError;
		}
		data.priority = opPtr->priority;
		data.mark = opPtr->mark;
		data.opPtr = opPtr;
	}
	else
	{
		data.mark = eOpNotOp;
		data.priority = 0;
		data.type = eMacroCheckDataNumber;
		data.number = GrmUtilPtr->str2int64(word);		
	}
	return LexBase::eLexNoError;
}
