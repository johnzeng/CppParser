#include "Preprocess.h"
#include "JZLogger.h"
#include "JZMarcoFunc.h"
#include "IncludeHandler.h"
#include "JZFileUtil.h"
#include "ErrorCode.h"
#include "KeyWordDefine.h"

using namespace JZErrorCode;

int Preprocess::handleSharp()
{
	JZFUNC_BEGIN_LOG();
	const LexicalRecord* record = getNextRecord();
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
		return errSharDefineFollowWithNothing;
	}
	std::string keyWord = "";
	if (NULL != recordList[0])
	{
		keyWord = recordList[0]->word;
	}
	else
	{
		JZWRITE_ERROR("return with NULL ptr");
		return errUnknow;
	}
	std::string defineWord = "";
	for(int j = 1; j < recordList.size(); j ++)
	{
		if (NULL == recordList[j])
		{
			continue;
		}
		defineWord += (recordList[j])->word;
	}
	LexicalAnalyzer *analyzer = new LexicalAnalyzer();
	bool inStringFlag = false,inIncludeFlag = false,inCommentBlockFlag = false,inCommentLineFlag = false, inCharFlag = false, backSlantEndFlag =false;
	analyzer->analyzeALine(defineWord, -1,&inStringFlag, &inIncludeFlag, & inCommentBlockFlag, &inCommentLineFlag, &inCharFlag, &backSlantEndFlag);
	mDefinemanager.addDefineMap(keyWord, analyzer->getRecordList());	

	JZWRITE_DEBUG("you define word : %s, context is :%s",keyWord.c_str(), defineWord.c_str() );
	JZFUNC_END_LOG();
	return errNoError;	
}

int Preprocess::handleSharpInclude()
{
	JZFUNC_BEGIN_LOG();
	string fileName = "";
	auto suroundMarkA = getNextRecord(); 
	JZIF_NULL_RETURN(suroundMarkA, errSharpIncludeNotSurroundWithRightSeperator);
	if ("\"" != suroundMarkA->word && "<" != suroundMarkA->word)
	{
		return errSharpIncludeNotSurroundWithRightSeperator;
	}

	auto includeRecord = getNextRecord();
	JZIF_NULL_RETURN(includeRecord,errSharpIncludeFollowNoFile);
	
	auto suroundMarkB = getNextRecord();
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
	while ((lex = getNextRecord() ) != NULL)
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
	return errNoError;
}
