#include "LexBase.h"
#include "JZLogger.h"
#include "LexUtil.h"
#include "JZMacroFunc.h"
#include "LexPatternTable.h"
#include "JZFileUtil.h"

LexBase::LexBase()
{
  mPatternTable = new LexPatternTable();
	mPatternTable->insertPattern('\'', &LexBase::handleSingleQuotation);
	mPatternTable->insertPattern('"',  &LexBase::handleDoubleQuotation);
}
LexBase::~LexBase()
{
  JZSAFE_DELETE(mPatternTable)
}

uint32 LexBase::analyzeAFile(const string& fileName)
{
	JZFUNC_BEGIN_LOG();
	JZWRITE_DEBUG("now analyze file : %s", fileName.c_str());
	uint64 bufSize;
	unsigned char* buff = JZGetFileData(fileName.c_str(), &bufSize);
	const char* buffWithOutBackSlant = LexUtil::eraseLineSeperator((const char*)buff,&bufSize);
	JZSAFE_DELETE(buff);

	const char* buffWithOutComment = LexUtil::eraseComment(buffWithOutBackSlant,&bufSize);
	JZSAFE_DELETE(buffWithOutBackSlant);

	pushReaderRecord(buffWithOutComment,bufSize,fileName,eFileTypeFile);
	uint32 ret = doLex();
  //buffWithOutComment will be delete in popReaderRecord
	popReaderRecord();
  JZSAFE_DELETE(buffWithOutComment);
	JZWRITE_DEBUG("analyze file end");
	JZFUNC_END_LOG();
	return ret;
}
uint32 LexBase::doLex()
{
	JZFUNC_BEGIN_LOG();
	uint32 ret = eLexNoError;
	string word;
	do
  {
    ret = heartBeat(word);

	}while(ret == eLexNoError);

	JZWRITE_DEBUG("ret is :%d",ret);
	return ret ;
}

uint32 LexBase::heartBeatForNormalWord(string& word)
{
  //normal word
  uint32 beginIndex = getLastIndex() + 1 - word.size();
  uint32 endIndex = getLastIndex();
  saveWord(word,beginIndex, endIndex);
  return eLexNoError;

}
uint32 LexBase::heartBeat(string& word)
{
  // for lex base, we don't save normal word, we just handle sepeators
  uint32 ret = consumeWord(word);
  JZWRITE_DEBUG("read word:%s",word.c_str());

  if ("" == word)
  {
    return ret;
  }
  if (true == LexUtil::isInterpunction(word[0]))
  {
    LexPatternHandler handler = mPatternTable->getPattern(word[0]);
    if (NULL == handler)
    {
      //no handler is registered,means this is a normal input
      if (true == LexUtil::isEmptyInput(word[0]))
      {
        return eLexNoError;
      }
      uint32 beginIndex = getLastIndex();
      uint32 endIndex = getLastIndex();
      saveWord(word,beginIndex, endIndex);
    }
    else
    {
      uint32 err = eLexNoError;
      err = (this->*handler)();
      if (err != eLexNoError)
      {
        writeError(err);
        JZFUNC_END_LOG();
        return err;
      }
    }
    return ret;
  }
  return heartBeatForNormalWord(word);
}


LexRecList LexBase::getRecList()
{
  return mLexRecList;
}

void LexBase::printLexRec()
{
	JZFUNC_BEGIN_LOG();
	auto it = mLexRecList.begin();
	string line = "";
	uint32 curLine = 0;
	JZWRITE_DEBUG("now print lex");
	for(; it != mLexRecList.end(); it++)
	{
		if (it->line == curLine)
		{
			line += " ";
			line += it->word;
		}
		else
		{
			printf("%s\n",line.c_str());
			line = it->word;
			curLine = it->line;
		}
	}
	printf("%s\n",line.c_str());

	JZFUNC_END_LOG();
}

uint32 LexBase::readChar(char *ret)
{
	*ret = 0;
	if (mReaderStack.empty())
	{
		return eLexReaderStackEmpty;
	}
	FileReaderRecord &record = mReaderStack.top();
	if (record.bufferSize == record.curIndex)
	{
		JZFUNC_END_LOG();
		return eLexReachFileEnd;
	}

	*ret = record.buffer[record.curIndex];

	return eLexNoError;

}

uint32 LexBase::consumeChar(char *ret)
{
	*ret = 0;
	if (mReaderStack.empty())
	{
		return eLexReaderStackEmpty;
	}
	FileReaderRecord &record = mReaderStack.top();
	if (record.bufferSize == record.curIndex)
	{
		return eLexReachFileEnd;
	}

	*ret = record.buffer[record.curIndex];
	if (true == LexUtil::isLineEnder(*ret))
	{
		record.curLineNum++;
	}
	record.curIndex++;

	return eLexNoError;
}

uint32 LexBase::getLastIndex()
{
	if (!mReaderStack.empty())
	{
		return mReaderStack.top().curIndex - 1;
	}
	JZWRITE_ERROR("try to get index when empty");
	return 0;
}

uint32 LexBase::undoConsume()
{
	if (mReaderStack.empty())
	{
		return eLexReaderStackEmpty;
	}
	FileReaderRecord &record = mReaderStack.top();
	if (record.curIndex <= 0)
	{
		return eLexAlreadyLastWord;
	}
	record.curIndex--;
	char curChar = record.buffer[record.curIndex];
	if(true == LexUtil::isLineEnder(curChar))
	{
		record.curLineNum --;	
	}
	return eLexNoError;
}

void LexBase::saveWordTo(
		const string& input,LexRecList& list,
		uint32 beginIndex,uint32 endIndex,
	   	uint32 recordType)
{
	//if this is a word, you must already consume a seperator
//refactor log, this part of logic should be implement into the child of lex base.
//	if (false == isLastStreamUseful())
//	{
//		//yeah! this stream is not useful!
//		JZWRITE_DEBUG("stream is off");
//		return;
//	}
	if (true == LexUtil::isEmptyInput(input))
	{
		//don't add empty input
		return;
	}
	LexRec rec = 
	{
		.word = input,
		.line = mReaderStack.top().curLineNum,
		.file = mReaderStack.top().fileName,
		.type = recordType,
		.fileType = mReaderStack.top().recordType,
		.endIndex = endIndex,
		.beginIndex = beginIndex,
	};
//	JZWRITE_DEBUG("input is :[%s],begin:%d,end:%d",input.c_str(),beginIndex,endIndex);
	list.push_back(rec);
}

void LexBase::saveWord(
		const string& input,
		uint32 beiginIndex,
	   	uint32 endIndex,
	   	uint32 recordType)
{
	saveWordTo(input,mLexRecList, beiginIndex, endIndex, recordType);
}

uint32 LexBase::tryToMatchWord(const string& word)
{
	JZFUNC_BEGIN_LOG();
	if ("" == word)
	{
		JZFUNC_END_LOG();
		return eLexNoError;
	}
	if (mReaderStack.empty())
	{
		JZFUNC_END_LOG();
		return eLexReaderStackEmpty;
	}
	FileReaderRecord &record = mReaderStack.top();
	if (record.bufferSize <= record.curIndex + word.size())
	{
		JZSAFE_DELETE(mReaderStack.top().buffer);
		mReaderStack.pop();
		JZFUNC_END_LOG();
		return eLexReachFileEnd;
	}

	string subStrWord = "";
	for(int i  = 0 ; i < word.size() ; i ++)
	{
		subStrWord += record.buffer[record.curIndex + i];	
	}
	if (subStrWord == word)
	{
		record.curIndex += word.size();
		JZFUNC_END_LOG();
		return eLexNoError;
	}

	JZFUNC_END_LOG();
	return eLexWordNotMatch;
	
}

uint32 LexBase::consumeCharUntilReach(const char inputEnder, string *ret, LexInput inOneLine)
{
	JZFUNC_BEGIN_LOG();
	if (NULL == ret)
	{
		return eLexUnknowError;
	}
	*ret = "";
	char nextInput;
	uint32 readRet = eLexNoError;
	while ((readRet = readChar(&nextInput)) == eLexNoError)
	{
		if (
			eLexInOneLine == inOneLine &&
			true == LexUtil::isLineEnder(nextInput)
			)
		{
			return eLexReachLineEnd;
		}
		*ret += nextInput;
		consumeChar(&nextInput);
		if (nextInput == inputEnder)
		{
			break;
		}
	}
	return readRet;
}

void LexBase::pushReaderRecord(FileReaderRecord record)
{
	mReaderStack.push(record);
}

void LexBase::pushReaderRecord(const char* buff,uint64 size,const string& fileName,uint32 recordType  )
{
	FileReaderRecord rec = 
		initFileRecord(
				buff,size, fileName,recordType);
  pushReaderRecord(rec);
  //alarm: these part of logic should be in child of lexbase
//	switch(recordType)
//	{
//		case eFileTypeMacroParam:
//			mPreprocessingMacroSet.insert(fileName);
//			break;
//		case eFileTypeFile:
//			mPreprocessedFile.insert(fileName);
//		default:
//		{
//			break;	
//		}
//	}
}

void LexBase::popReaderRecord()
{
	switch(mReaderStack.top().recordType)
	{
  //alarm: these part of logic should be in child of lexbase
//		case eFileTypeMacroParam:
//			{
//				auto toEraseIt = mPreprocessingMacroSet.find(mReaderStack.top().fileName);
//				mPreprocessingMacroSet.erase(toEraseIt);
//			}
//			break;
		default:
		{
			break;	
		}
	}
	mReaderStack.pop();
}


uint32 LexBase::consumeWord(
		string &retStr,
		LexInput skipEmptyInput,
		LexInput inOneLine)
{
	JZFUNC_BEGIN_LOG();
	uint32 ret;
	char nextChar;
	retStr = "";
	while (eLexNoError == (ret = readChar(&nextChar)))
	{
		if (
			eLexInOneLine == inOneLine &&
			true == LexUtil::isLineEnder(nextChar)
			)
		{
			JZFUNC_END_LOG();
			return eLexReachLineEnd;
		}
		if (eLexSkipEmptyInput == skipEmptyInput && 
			true == LexUtil::isEmptyInput(nextChar) &&
			LexUtil::isEmptyInput(retStr))
		{
			//skip empty input
			consumeChar(&nextChar);
			continue;
		}
		if (false == LexUtil::isInterpunction(nextChar))
		{
			//not interpunction
			consumeChar(&nextChar);
//			if (retStr.size() == 0 && true == LexUtil::isConstNumberChar(nextChar))
//			{
//				LexUtil::pointIsSeperator(false);
//			}
			retStr += nextChar;
		}
		else if(LexUtil::isEmptyInput(retStr))
		{
			consumeChar(&nextChar);
			retStr += nextChar;	
			JZFUNC_END_LOG();
			return ret;
		}
		else
		{
			break;
		}
	}
	if (true == LexUtil::isEmptyInput(retStr) && ret == eLexReachFileEnd)
	{
		//so it can pop file record 
		consumeChar(&nextChar);
	}
	JZFUNC_END_LOG();
	return ret;
}
/*********************************************************
	data init helper func 
 ********************************************************/
FileReaderRecord LexBase::initFileRecord(
		const char* buff,uint64 size,const string& fileName,
		uint32 recordType)
{
	FileReaderRecord ret = 
	{
		.buffer = buff,
		.bufferSize = size,
		.curIndex = 0,
    //actually the curLineNumber paam is not useful enough now because we erase comment and back slash at the very begining, I am thinking about refator this process but not sure if it's necessary
		.curLineNum = 1,
		.fileName = fileName,
		.recordType = recordType,
		.mStreamOffTag = 0,	
		.mFuncLikeMacroParamAnalyzing = false,
	};
	return ret;
}

uint32 LexBase::handleSingleQuotation()
{
	JZFUNC_BEGIN_LOG();

	//when you enter this func,you have already get a ',so
	uint32 beginIndex = getLastIndex();
	string toSave = "'";
	uint32 toSaveLen = 0;
	bool isConvertBackSlant = false;
	do
	{
		string retStr = "";
		uint32 retErr = eLexNoError;
		
		retErr = consumeCharUntilReach('\'',&retStr,eLexInOneLine);
		if (eLexNoError != retErr)
		{
			JZFUNC_END_LOG();
			return retErr;
		}
		JZWRITE_DEBUG("ret str is : %s",retStr.c_str());
		for(int i = 0 ; i < retStr.size() ; i++)
		{
			if (true == LexUtil::isBackSlant(retStr[i]))
			{
				isConvertBackSlant = !isConvertBackSlant;
			}
			else
			{
				isConvertBackSlant = false;	
			}
		
		}
		toSave += retStr;
		toSaveLen = toSave.size();
	}while(true == isConvertBackSlant);

//	handle const char input
//	...
	uint32 endIndex = getLastIndex();
	saveWord(toSave,beginIndex, endIndex ,  eLexRecTypeConstChar);

	JZFUNC_END_LOG();
	return eLexNoError;
}

uint32 LexBase::handleDoubleQuotation()
{
	JZFUNC_BEGIN_LOG();
	uint32 beginIndex = getLastIndex();
	//when you enter this func,you have already get a ",so init a " here
	string toSave = "\"";
	uint32 toSaveLen = 0;
	bool isConvertBackSlant = false;
	do
	{
		string retStr = "";
		uint32 retErr = eLexNoError;
		retErr = consumeCharUntilReach('"',&retStr,eLexInOneLine);
		if (eLexNoError != retErr)
		{
			JZFUNC_END_LOG();
			return retErr;
		}
		for(int i = 0 ; i < retStr.size() ; i++)
		{
			if (true == LexUtil::isBackSlant(retStr[i]))
			{
				isConvertBackSlant = !isConvertBackSlant;
			}
			else
			{
				isConvertBackSlant = false;	
			}
		
		}
		toSave += retStr;
		toSaveLen = toSave.size();
	}while(true == isConvertBackSlant);

//	handle const char input
//	...
	uint32 endIndex = getLastIndex();
	saveWord(toSave,beginIndex, endIndex, eLexRecTypeConstChar);

	JZFUNC_END_LOG();
	return eLexNoError;
	return 0;
}

#define ERROR_CASE(caseid) case caseid:{JZWRITE_DEBUG("error id %d,"#caseid,caseid);break;}
void LexBase::writeError(uint32 err)
{
	switch(err)
	{
		ERROR_CASE(eLexNoError)
		ERROR_CASE(eLexReachFileEnd)	
		ERROR_CASE(eLexReaderStackEmpty)
		ERROR_CASE(eLexSharpFollowedNothing)
		ERROR_CASE(eLexWordNotMatch)
		ERROR_CASE(eLexAlreadyLastWord)
		ERROR_CASE(eLexSharpDefineFollowedNothing)
		ERROR_CASE(eLexUnexpectedSeperator)
		ERROR_CASE(eLexValParamNotLast)
		ERROR_CASE(eLexSharpIfdefFollowedWithNothing)
		ERROR_CASE(eLexUnmatchMacro)
		ERROR_CASE(eLexUnknowMacro)
		ERROR_CASE(eLexSharpEndIfFollowWithOtherThing)
		ERROR_CASE(eLexSharpElseFollowWithOtherThing)
		ERROR_CASE(eLexWordIsNotDefined)
		ERROR_CASE(eLexParamAnalyzeOVer)	//this is not an error
		ERROR_CASE(eLexReachLineEnd)
		ERROR_CASE(eLexFuncLikeMacroParamTooLess)
		ERROR_CASE(eLexFuncLikeMacroParamTooManay)

		//unknow should be last
		ERROR_CASE(eLexUnknowError) 

		default:
		{
			break;	
		}
	}
}
