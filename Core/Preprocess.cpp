#include "Preprocess.h"
#include "JZLogger.h"
#include "JZMarcoFunc.h"
#include "IncludeHandler.h"
#include "JZFileUtil.h"
#include "ErrorCode.h"
#include "KeyWordDefine.h"

using namespace JZErrorCode;

Preprocess::Preprocess()
{
	mDefinemanager.init();
}

Preprocess::~Preprocess()
{
}

void Preprocess::init(LexicalAnalyzer* rootLex)
{
	if (rootLex == NULL)
	{
		JZWRITE_DEBUG("no lex is set");
		return;
	}
	LexReaderStruct rootReader;
	rootReader.lexPtr = rootLex;
	pushLexReader(rootReader);
}

void Preprocess::analyze()
{
	while (!mLexStack.empty())
	{
		auto curReader = mLexStack.top();
		mLexStack.pop();
		auto curLex = curReader.lexPtr;
		
		const LexicalRecord* record = NULL;
		while((record = getNextRecord())!=NULL)
		{
			//now analyze this record
			if ("#" == record->word)
			{
				handleSharp();
			}
			else if("//" == record->word)
			{
				handleLineComment();
			}
			else if("/*" == record->word)
			{
				handleBlockComment();	
			}
			else if(true == mDefinemanager.isDefined(record->word))
			{
				//if this is a defined marco,handle it
				
			}
			else
			{
				//no need to handle
				this->mExpendedList.push_back(*record);	
			}
		
		};
	}
}

const LexicalRecord* Preprocess::getNextRecord()
{
	const LexicalRecord* ret = NULL;
	while(!mLexStack.empty())
	{
		LexReaderStruct &lastLexReader = mLexStack.top();
		if (lastLexReader.lexPtr == NULL)
		{
			JZWRITE_ERROR("no ptr is set! Now pop this reader");
			mLexStack.pop();
			continue;
		}
		ret = lastLexReader.lexPtr->getLexiRecord(lastLexReader.curIndex);
		lastLexReader.curIndex++;

		if (NULL == ret)
		{
			//this lex reach the end
			mLexStack.pop();
		}
		else
		{
			return ret;	
		}
	}
	return ret;
}

std::vector<LexicalRecord*> Preprocess::getLineRecordTillLineEnd()
{
	std::vector<LexicalRecord*> ret;
	if (!mLexStack.empty())
	{
		LexReaderStruct &lastLexReader = mLexStack.top();
		if (lastLexReader.lexPtr == NULL)
		{
			JZWRITE_ERROR("no ptr is set! now pop this reader");
			mLexStack.pop();
		}
		ret = lastLexReader.lexPtr->getLineRecordTillLineEnd(lastLexReader.curIndex);
		lastLexReader.curIndex += ret.size();
	}
	return ret;
}

void Preprocess::pushLexReader(LexReaderStruct reader)
{
	mLexStack.push(reader);
}


int Preprocess::handleSharp()
{
	const LexicalRecord* record = getNextRecord();
	JZIF_NULL_RETURN(record,errSharpFollowNothing);

	if (C_MACRO_WORD_DEFINE == record->word)
	{
		return handleSharpDefine();
	}
	else if (C_MACRO_WORD_INCLUDE == record->word)
	{
		return handleSharpInclude();
	}
	else
	{
		//no key word is found!
		return errSharpFollowWithUnknowKeyWord;
	}
}

int Preprocess::handleSharpDefine()
{
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
	mDefinemanager.addDefineMap(keyWord, defineWord);	
	JZWRITE_DEBUG("you define word : %s, context is :%s",keyWord.c_str(), defineWord.c_str() );
	return errNoError;	
}

int Preprocess::handleSharpInclude()
{
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
	pushLexReader(includeReaderStruct);
	return errNoError;
}

int Preprocess::handleLineComment()
{
	getLineRecordTillLineEnd();	
	return errNoError;
}

int Preprocess::handleBlockComment()
{
	getNextRecord();
	return errNoError;
}
