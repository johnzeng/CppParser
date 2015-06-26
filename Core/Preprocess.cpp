#include "Preprocess.h"
#include "JZLogger.h"
#include "JZMacroFunc.h"
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
//	return;
	JZFUNC_BEGIN_LOG();
	const LexicalRecord* record = NULL;
	while((record = getNextRecordAndSkipEmptyRecord())!=NULL)
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
			handleDefine(record);	
		}
		else
		{
			//no need to handle
			this->mExpendedList.push_back(*record);	
		}
	
	};
//#if defined(DEBUG)
//	auto it = mExpendedList.begin();
//	for(;it != mExpendedList.end() ; it++ )
//	{
//		JZWRITE_DEBUG("%s",it->word.c_str());
//	}
//#endif
	JZFUNC_END_LOG();
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
			popLexReader();
			continue;
		}
		ret = lastLexReader.lexPtr->getLexiRecord(lastLexReader.curIndex);
		lastLexReader.curIndex++;

		if (NULL == ret)
		{
			popLexReader();
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
			popLexReader();
		}
		ret = lastLexReader.lexPtr->getLineRecordTillLineEnd(lastLexReader.curIndex);
		lastLexReader.curIndex += ret.size();
		//need to calculat the \\ num
		if (ret.size() > 0)
		{
			int beginLine = ret[0]->line;
			uint32 endIndex = ret.size() - 1;
			int endLine = ret[endIndex]->line;
			lastLexReader.curIndex += (endLine - beginLine);
		}
	}
	return ret;
}

void Preprocess::pushLexReader(LexReaderStruct reader)
{
	mLexStack.push(reader);
}

const LexicalRecord* Preprocess::getNextRecordAndSkipEmptyRecord()
{
	const LexicalRecord* ret = NULL;
	while ((ret = getNextRecord()))
	{
		if (ret->word != " ")
		{
			return ret;
		}
	}
	return ret;
}

void Preprocess::popLexReader()
{
	if (mLexStack.empty())
	{
		return;
	}
	auto lastLexReader = mLexStack.top();
	auto macroIt = mExpendingMacroSet.find(lastLexReader.expendingMacro);
	if (mExpendingMacroSet.end() != macroIt )
	{
		mExpendingMacroSet.erase(macroIt);
	}
	mLexStack.pop();

}
