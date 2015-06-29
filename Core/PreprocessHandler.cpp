#include "Preprocess.h"
#include "JZLogger.h"
#include "JZMacroFunc.h"
#include "IncludeHandler.h"
#include "JZFileUtil.h"
#include "ErrorCode.h"
#include "KeyWordDefine.h"

using namespace JZErrorCode;

int Preprocess::handleSharp()
{
	JZFUNC_BEGIN_LOG();
	const LexicalRecord* record = getNextRecordAndSkipEmptyRecord();
	JZIF_NULL_RETURN(record,errSharpFollowNothing);

	if (C_MACRO_WORD_DEFINE == record->word)
	{
		JZFUNC_END_LOG();
		return handleSharpDefine();
	}
	else if (C_MACRO_WORD_INCLUDE == record->word)
	{
		JZFUNC_END_LOG();
		return handleSharpInclude();
	}
	else
	{
		//no key word is found!
		JZFUNC_END_LOG();
		return errSharpFollowWithUnknowKeyWord;
	}
}

int Preprocess::handleSharpDefine()
{
	JZFUNC_BEGIN_LOG();
	auto recordList = getLineRecordTillLineEnd();
	if (recordList.size() == 0)
	{
		JZWRITE_DEBUG("define following nothing!");
		return errSharpDefineFollowWithNothing;
	}
	std::string keyWord = "";
	int i = 0;
	while (i < recordList.size() && " " == recordList[i]->word )
	{
		i++;
	}
	if (i >= recordList.size())
	{
		JZWRITE_ERROR("define end with nothing");
		return errSharpDefineFollowWithNothing;
	}

	keyWord = recordList[i]->word;
	i++;

	LexRecordList list;
	while(i < recordList.size())
	{
		list.push_back(*recordList[i]);	
		i++;
	}
	mDefinemanager.addDefineMap(keyWord, list);	

	JZWRITE_DEBUG("you define word : %s",keyWord.c_str());

	JZFUNC_END_LOG();
	return errNoError;	
}

int Preprocess::handleSharpInclude()
{
	JZFUNC_BEGIN_LOG();
	string fileName = "";
	auto suroundMarkA = getNextRecordAndSkipEmptyRecord(); 
	JZIF_NULL_RETURN(suroundMarkA, errSharpIncludeNotSurroundWithRightSeperator);
	if ("\"" != suroundMarkA->word && "<" != suroundMarkA->word)
	{
		return errSharpIncludeNotSurroundWithRightSeperator;
	}

	auto includeRecord = getNextRecordAndSkipEmptyRecord();
	JZIF_NULL_RETURN(includeRecord,errSharpIncludeFollowNoFile);
	
	auto suroundMarkB = getNextRecordAndSkipEmptyRecord();
	JZIF_NULL_RETURN(suroundMarkB,errSharpIncludeNotSurroundWithRightSeperator);

	if ("\"" == suroundMarkA->word && "\"" != suroundMarkB->word)
	{
		return errSharpIncludeNotSurroundWithRightSeperator;
	}
	else if ("<" == suroundMarkA->word && ">" != suroundMarkB->word)
	{
		return errSharpIncludeNotSurroundWithRightSeperator;
	}

	fileName = includeRecord->word;
	string fullPath = IncludeHandler::getInstance()->getFullPathForIncludeFile(fileName);
	if (false == JZFileAccessable(fullPath.c_str()))
	{
		JZWRITE_ERROR("can not access to file : %s",fileName.c_str());
		return errNoSuchPath;
	}

	//expand include file
	LexicalAnalyzer* includeLex = AnalyzerCollector::getInstance()->getAnalyzer(fullPath);
	if (NULL == includeLex)
	{
		includeLex = new LexicalAnalyzer();
		includeLex->setSourceCodeDir(fullPath);
		includeLex->doAnalyze();

		AnalyzerCollector::getInstance()->addAnalyzer(fullPath, includeLex);
	}
	LexReaderStruct includeReaderStruct;
	includeReaderStruct.lexPtr = includeLex;
//	to debug,don't push it yet
	pushLexReader(includeReaderStruct);
	JZFUNC_END_LOG();
	return errNoError;
}

int Preprocess::handleLineComment()
{
	JZFUNC_BEGIN_LOG();
	getLineRecordTillLineEnd();	
	JZFUNC_END_LOG();
	return errNoError;
}

int Preprocess::handleBlockComment()
{
	JZFUNC_BEGIN_LOG();
	const LexicalRecord* lex = NULL;
	while ((lex = getNextRecordAndSkipEmptyRecord() ) != NULL)
	{
		if(lex->word == "*/")
		{
			JZFUNC_END_LOG();
			return errNoError;	
		}
	}
	JZFUNC_END_LOG();
	return errCommentBlockDoEnd;
}

int Preprocess::handleDefine(const LexicalRecord* record)
{
	//find out the deine record list
	//match the current func
	//then you can find out the define
	JZFUNC_BEGIN_LOG();	
	if (NULL == record)
	{
		return errUnknow;
	}

	if (mExpendingMacroSet.find(record->word) != mExpendingMacroSet.end())
	{
		JZWRITE_ERROR("double expend marco,but I handle it as no error");
		return errNoError;
	}

	JZWRITE_DEBUG("now handle define : %s",record->word.c_str() );
	auto defineLex = mDefinemanager.findDefineMap(record->word);
	if (NULL == defineLex)
	{
		return errNoDefineIsFind;
	}

	if (defineLex->size() == 0)
	{
		//this is possible,when it is an empty macro
		return errNoError;
	}
	if ((*defineLex)[0].word != "(")
	{
		//not a func macro !,so just replace it!
		LexicalAnalyzer *tmpLex = new LexicalAnalyzer();
		tmpLex->setRecordList(*defineLex);
		AnalyzerCollector::getInstance()->addTmpAnalyzer(tmpLex);
		LexReaderStruct reader;
		reader.curIndex = 0;
		reader.lexPtr = tmpLex;
		reader.expendingMacro = record->word;
		mExpendingMacroSet.insert(record->word);
		this->pushLexReader(reader);
		return errNoError;
	}
	else
	{
		//a func macro !!!
		//now check match !
		//actually, actual param can be a little strange,cause it may be another lex list
		vector<LexRecordList> actualParamList; 
		map<string ,int> formalParamMap;

		//check the formal param first
		int paramIndex = 1;
		int paramPos = 0;

		bool expectParam = true;
		bool isVariableParam = false;
		for(; paramIndex < (*defineLex).size(); paramIndex++)
		{
			string curWord = (*defineLex)[paramIndex].word;
			if ("" ==  curWord || " " == curWord)
			{
				//empty input or unused input
				continue;
			}

			//handle ")"
			if (")" == curWord)
			{
				//reach next ")", formal param end
				if (paramPos == 0 && true == expectParam)
				{
					break;
				}
				else if (false == expectParam)
				{
					break;
				}
				else
				{
				JZWRITE_ERROR("miss ing seperator");
					return errMissingSeperator;	
				}
			}

			//handle other input
			if (true == expectParam)
			{
				if (C_KEY_WORD_VAR_LENGTH_PARAM != curWord && true == LexicalAnalyzer::isInterpunction(
							(curWord[0])
							))
				{
					JZWRITE_ERROR("function-like macro expect ')'before other seperator join");
					return errMissingSeperator;
				}
				JZWRITE_DEBUG("push back a new param : %s", curWord.c_str());
				formalParamMap[curWord] = paramPos;
				paramPos++;
				expectParam = false;
				if (C_KEY_WORD_VAR_LENGTH_PARAM == curWord)
				{
					isVariableParam = true;
				}
				continue;
			}

			//here comes the false == expectParam state
			if (curWord == ",")
			{
				//not expecting param,catch ","
				expectParam = true;
				continue;
			}
			else
			{
				JZWRITE_ERROR("unexpect input!");
				return errFuncLikeMacroParamError;
			}
		}

		if (true == isVariableParam)
		{
			if (formalParamMap[C_KEY_WORD_VAR_LENGTH_PARAM] != formalParamMap.size() - 1)
			{
				JZWRITE_ERROR("var param should be the last param");
				return errVarParamAtWrongPose;
			}
		}
		JZWRITE_DEBUG("now get marco actural param list");	
		int32 errNo = getMacroParams(actualParamList);
		if (errNoError != errNo)
		{
			return errNo;
		}
#ifdef DEBUG
		else
		{
			auto actualParmIt = actualParamList.begin();
			for(; actualParmIt != actualParamList.end(); actualParmIt++)
			{
				auto recordListIt = actualParmIt->begin();
				for(; recordListIt != actualParmIt->end(); recordListIt++)
				{
					JZWRITE_DEBUG("parm:%s",recordListIt->word.c_str());
				}
				JZWRITE_DEBUG("a param end");
			}		
		}
#endif
		if (false == isVariableParam)
		{
			if (actualParamList.size() != formalParamMap.size())
			{
				JZWRITE_ERROR("actual param num doesn't match formal param map size");
				return errParamNumDoseNotMatch;
			}
		}
	}
	JZFUNC_END_LOG();	
	return errNoError;
}

int32 Preprocess::getMacroParams(vector<LexRecordList>& ret)
{
	JZFUNC_BEGIN_LOG();
	stack<string> symbolStack;

	//init symbol stack
	const LexicalRecord* record = getNextRecordAndSkipEmptyRecord();
	if (NULL == record)
	{
		JZWRITE_ERROR("function like macro didn't follow with anything");
		return errMissingSeperator;
	}
	if ("(" != record->word)
	{
		JZWRITE_ERROR("function like macro didn't follow with \"(\"");
		return errMissingSeperator;
	}
	symbolStack.push(record->word);

	//now read the symbol
	LexRecordList curParams;
	while (!symbolStack.empty())
	{
		auto nextRecord = getNextRecordAndSkipEmptyRecord();
		if (NULL == nextRecord)
		{
			return errMissingSeperator;
		}
		if ("\"" == nextRecord->word || "'" == nextRecord->word)
		{
			/*********************************************************
				what a happy time... 
				another special handle about \" and '
			 ********************************************************/
		
			curParams.push_back(*nextRecord);	
			auto stringRecord = getNextRecordAndSkipEmptyRecord();
			if (NULL == stringRecord)
			{
				JZWRITE_ERROR("no string follow the \"");
				return errMissingSeperator;
			}

			auto anotherComa = getNextRecordAndSkipEmptyRecord();
			if (NULL == anotherComa)
			{
				return errExpectAnotherSeperator;
			}
			if (nextRecord->word != anotherComa->word)
			{
				return errExpectAnotherSeperator;
			}
			curParams.push_back(*stringRecord);
			curParams.push_back(*anotherComa);
			continue;
		}

		if ("(" == nextRecord->word || "[" == nextRecord->word)
		{
			symbolStack.push(nextRecord->word);
		}
		else if (")" == nextRecord->word)
		{
			if (symbolStack.top() == "(")
			{
				symbolStack.pop();
			}
			else
			{
				JZWRITE_ERROR("unexpected ')'");	
				return errUnexpectSeperator;
			}
		}
		else if ("]" == nextRecord->word)
		{
			if (symbolStack.top() == "[")
			{
				symbolStack.pop();
			}
			else
			{
				JZWRITE_ERROR("unexpected ']'" );
			}
		}

		if (symbolStack.empty())
		{
			if (curParams.size() > 0)
			{
				ret.push_back(curParams);
			}
			break;
		}

		if ("," == nextRecord->word && 1 == symbolStack.size())
		{
			ret.push_back(curParams);
			curParams.clear();
			continue;
		}
		curParams.push_back(*nextRecord);
		
	}
	JZFUNC_END_LOG();
	return errNoError;
}
