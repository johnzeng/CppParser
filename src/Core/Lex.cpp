#include "Lex.h"
#include "DefineManager.h"
#include "JZFileUtil.h"
#include "JZLogger.h"
#include "JZMacroFunc.h"
#include "IncludeHandler.h"
#include "KeyWordDefine.h"
#include <stdlib.h>
/*********************************************************
	Lex begin here 
 ********************************************************/

Lex::Lex()
{
}

Lex::~Lex()
{
}

uint32 Lex::analyzeAFile(const string& fileName)
{
	JZFUNC_BEGIN_LOG();
	if (true == isOnceFile(fileName))
	{
		return eLexNoError;
	}
	JZWRITE_DEBUG("now analyze file : %s", fileName.c_str());
	uint64 bufSize;
	unsigned char* buff = JZGetFileData(fileName.c_str(), &bufSize);
	const char* buffWithOutBackSlant = LexUtil::eraseLineSeperator((const char*)buff,&bufSize);
	JZSAFE_DELETE(buff);

	const char* buffWithOutComment = LexUtil::eraseComment(buffWithOutBackSlant,&bufSize);
	JZSAFE_DELETE(buffWithOutBackSlant);

	pushReaderRecord(buffWithOutComment,bufSize,fileName,eFileTypeFile);
	uint32 ret = doLex();
	popReaderRecord();
	JZWRITE_DEBUG("analyze file end");
	JZFUNC_END_LOG();
	return ret;
}

uint32 Lex::doLex()
{
	JZFUNC_BEGIN_LOG();
	uint32 ret;
	string word;
	do
	{
		ret = consumeWord(word);
		JZWRITE_DEBUG("read word:%s",word.c_str());

		if ("" == word)
		{
			return ret;
		}
		if (true == LexUtil::isInterpunction(word[0]))
		{
			LexPatternHandler handler = LexPtnTbl->getPattern(word[0]);
			if (NULL == handler)
			{
				//no handler is registered,means this is a normal input
				if (true == LexUtil::isEmptyInput(word[0]))
				{
					continue;
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
			continue;
		}
		JZWRITE_DEBUG("is macro:%d,is Stream useful %d",eFileTypeFile == mReaderStack.top().recordType,isLastStreamUseful());
		//not interpunction
		if (isLastStreamUseful() &&
			"defined" == word)
		{
			//this is defined in #if or #elif
			uint32 beginIndex = getLastIndex() + 1 - word.size() ;
			uint32 endIndex = getLastIndex();
			uint32 err = handleIsDefined(word);
			if (err != eLexNoError)
			{
				writeError(err);
				JZFUNC_END_LOG();
				return err;
			}
			JZWRITE_DEBUG("save a is defined word");
			saveWord(word,beginIndex, endIndex);
		}
		else if (
				false == isFuncLikeMacroMode() &&
			   	true == isLastStreamUseful() &&
				DefineManager::eDefMgrDefined == mDefMgr.isDefined(word) &&
				false == isMacroExpending(word))
		{
			//don't expend when it is FuncLikeMacroMode
			uint32 err = handleDefinedWord(word);
			if (err != eLexNoError)
			{
				writeError(err);
				return err;
			}
		}
		else
		{
			//normal word
			uint32 beginIndex = getLastIndex() + 1 - word.size();
			uint32 endIndex = getLastIndex();
			saveWord(word,beginIndex, endIndex);
		}
		
	}while(ret == eLexNoError);

	JZWRITE_DEBUG("ret is :%d",ret);
	return ret ;
}

uint32 Lex::expendMacro(const DefineRec* def,const RealParamList& paramList, string &ret)
{
	JZFUNC_BEGIN_LOG();
	ret = "";
	if (NULL == def)
	{
		JZFUNC_END_LOG();
		return eLexUnknowError; 
	}

	JZWRITE_DEBUG("now expend macor:%s,str is :%s",def->key.c_str(),def->defineStr.c_str());
	RealParamList list;
	list.resize(def->formalParam.size(), "");
	for(int i = 0 ; i < def->formalParam.size() && i < paramList.size() ; i++)
	{
		list[i] = paramList[i];	
	}
	if (true == def->isVarArgs)
	{
		//this is var func like def
		string varStr = "";
		for(int i = def->formalParam.size() ;  i < paramList.size() ; i++)
		{
			varStr += ",";
			varStr += paramList[i];
		}
		list.back() += varStr;
		JZWRITE_DEBUG("var str is :[%s]",list.back().c_str());
	}
	string word = "";
	stack<string> symStack;
	bool singleSharp = false;
	bool doubleSharp = false;

	for(int i = 0 ; i < def->defineStr.size() ; i++)
	{
		char curChar = def->defineStr[i];
		if(true == LexUtil::isInterpunction(curChar))
		{
			if (def->paramMap.find(word) != def->paramMap.end())
			{
				//this is a param,save other thing then word
				int index = def->paramMap.find(word)->second;
				if (list.size() <= index)
				{
					JZWRITE_DEBUG("unmatch param size");
					JZFUNC_END_LOG();
					return eLexFuncLikeMacroParamTooLess; 
				}
				string paramStr = list[index];
				//normal param
				if (true == singleSharp)
				{
					ret += "\"";
					ret += paramStr;
					ret += "\"";
				}
				else if(true == doubleSharp)
				{
					if (def->isVarArgs == true && index == def->formalParam.size() - 1)
					{
						//var param
						if (true == LexUtil::isEmptyInput(paramStr))
						{
							if(',' == ret.back())
							{
								ret.pop_back();	
							}
						}
					}
					ret += paramStr;
				}
				else
				{
					ret += paramStr;	
				}
				singleSharp = false;
				doubleSharp = false;
			}
			else if(true == doubleSharp && true == LexUtil::isEmptyInput(word))
			{
				//do nothing
				JZWRITE_DEBUG("skip empty input");
			}
			else if(true == singleSharp)
			{
				if (true == LexUtil::isEmptyInput(word))
				{
					continue;
				}
				else
				{
					JZWRITE_DEBUG("# not following macor param");
					JZWRITE_DEBUG("def str is :%s",def->defineStr.c_str());
					popErrorSite();
					return eLexUnexpectedSeperator;	
				}
			}
			else
			{
				ret += word;	
				singleSharp = false;
				doubleSharp = false;
			}
			word = "";
			if ('\'' == curChar || '"' == curChar)
			{
				//read char or string
				do
				{
					ret += def->defineStr[i];
					i++;
				}while(def->defineStr.size() > i && def->defineStr[i] != curChar && def->defineStr[i - 1] != '\\');
				ret += def->defineStr[i];
			}
			else if('#' == curChar)
			{
				//assume sharp come
				if (singleSharp || doubleSharp)
				{
					JZWRITE_DEBUG("unexpect sharp!");
					return eLexUnexpectedSeperator; 
				}
				singleSharp = true;
				if (def->defineStr.size() > i+1)
				{
					if ('#' == def->defineStr[i+1])
					{
						i++;
						//this is double sharp!
						JZWRITE_DEBUG("double sharp");
						doubleSharp = true;
						singleSharp = false;
						while(ret.size() > 0 && true == LexUtil::isEmptyInput(ret.back()))
						{
							JZWRITE_DEBUG("pop empty input");
							ret.pop_back();	
						}
					}
				}
			}
			else
			{
				if (true == doubleSharp )
				{
					if (true == LexUtil::isEmptyInput(curChar))
					{
						//don't save
						continue;
					}
					else
					{
						JZWRITE_DEBUG("unexpect seperator");
						return eLexUnexpectedSeperator;
					}
				}
				ret += curChar;
			}
		}
		else
		{
			word += curChar;
		}
	}
	if (word != "")
	{
		//not end with interpunction;
		ret += word;
	}
	JZWRITE_DEBUG("ret is :[%s]",ret.c_str());
	return eLexNoError;
}

uint32 Lex::consumeWord(
		string &retStr,
		LexInput skipEmptyInput,
		LexInput inOneLine)
{
	JZFUNC_BEGIN_LOG();
	uint32 ret;
	char nextChar;
	retStr = "";
	LexUtil::pointIsSeperator(true);
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
			if (retStr.size() == 0 && true == LexUtil::isConstNumberChar(nextChar))
			{
				LexUtil::pointIsSeperator(false);
			}
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

uint32 Lex::handleDefinedWord(const string& word)
{
	JZFUNC_BEGIN_LOG();
	auto defRec = mDefMgr.findDefineMap(word);
	if (NULL == defRec)
	{
		JZFUNC_END_LOG();
		return eLexWordIsNotDefined;
	}
	if (true == isMacroExpending(word))
	{
		JZFUNC_END_LOG();
		return eLexMacroIsAlreadyExpending;
	}
	JZWRITE_DEBUG("now expending macro:%s",word.c_str());
	if (true == defRec->isFuncLikeMacro)
	{
		string matchWord;
		uint32 errRet = consumeCharUntilReach('(',&matchWord);
		if (errRet != eLexNoError)
		{
			JZFUNC_END_LOG();
			return eLexUnexpectedSeperator;
		}
		//read real param
		turnOnFuncLikeMacroMode();
		handleLeftBracket();
		uint32 ret = doLex();
		JZWRITE_DEBUG("do lex end");
		turnOffFuncLikeMacroMode();
		//untile here, param list is all get
		if (eLexNoError != ret && eLexReachFileEnd != ret && eLexParamAnalyzeOVer != ret)
		{
			popErrorSite();
			JZFUNC_END_LOG();
			return ret;
		}

		JZWRITE_DEBUG("now add param list");
#ifdef DEBUG
		JZWRITE_DEBUG("now print param list");
		for(int i = 0 ; i < mRealParamList.size(); i++)
		{
			JZWRITE_DEBUG("i is %d,word is :[%s]",i,mRealParamList[i].c_str());	
		}
#endif
		//param number check
		if (true == defRec->isVarArgs)
		{
			if (defRec->paramMap.size() - 1 > mRealParamList.size())
			{
				JZWRITE_DEBUG("var func like marco param not enough");
				return eLexFuncLikeMacroParamTooLess;
			}
		}
		else
		{
			if (defRec->paramMap.size() != mRealParamList.size())
			{
				JZWRITE_DEBUG("Func like macro param not right");
				return defRec->paramMap.size() > mRealParamList.size() ?
				eLexFuncLikeMacroParamTooLess : eLexFuncLikeMacroParamTooManay;
			}
		}
	}

	//not func like ,just expend it
	//use defineManager to expend it
	string expendStr = "";
	uint32 expendErr = expendMacro(defRec, mRealParamList, expendStr);
	if (eLexNoError != expendErr)
	{
		JZFUNC_END_LOG();
		return expendErr;
	}
	
	char* buff = (char*)malloc(expendStr.size());
	strncpy(buff, expendStr.c_str(), expendStr.size());
	pushReaderRecord(buff,expendStr.size(),word,eFileTypeMacroParam);
	mRealParamList.clear();
	uint32 ret = doLex();

	if (ret != eLexNoError && ret != eLexReachFileEnd)
	{
		//error happen 
		JZFUNC_END_LOG();
		return ret;
	}
	popReaderRecord();
	JZFUNC_END_LOG();
	return eLexNoError;	
}
void Lex::pushReaderRecord(const char* buff,uint64 size,const string& fileName,uint32 recordType  )
{
	FileReaderRecord rec = 
		initFileRecord(
				buff,size, fileName,recordType);
	mReaderStack.push(rec);
	switch(recordType)
	{
		case eFileTypeMacroParam:
			mPreprocessingMacroSet.insert(fileName);
			break;
		case eFileTypeFile:
			mPreprocessedFile.insert(fileName);
		default:
		{
			break;	
		}
	}
}

void Lex::popReaderRecord()
{
	JZSAFE_DELETE(mReaderStack.top().buffer);
	switch(mReaderStack.top().recordType)
	{
		case eFileTypeMacroParam:
			{
				auto toEraseIt = mPreprocessingMacroSet.find(mReaderStack.top().fileName);
				mPreprocessingMacroSet.erase(toEraseIt);
			}
			break;
		default:
		{
			break;	
		}
	}
	mReaderStack.pop();
}

#define ERROR_CASE(caseid) case caseid:{JZWRITE_DEBUG("error id %d,"#caseid,caseid);break;}
void Lex::writeError(uint32 err)
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

void Lex::saveWordTo(
		const string& input,LexRecList& list,
		uint32 beginIndex,uint32 endIndex,
	   	uint32 recordType)
{
	//if this is a word, you must already consume a seperator
	if (false == isLastStreamUseful())
	{
		//yeah! this stream is not useful!
		JZWRITE_DEBUG("stream is off");
		return;
	}
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

void Lex::saveWord(
		const string& input,
		uint32 beiginIndex,
	   	uint32 endIndex,
	   	uint32 recordType)
{
	saveWordTo(input,mLexRecList, beiginIndex, endIndex, recordType);
}

void Lex::printLexRec()
{
	JZFUNC_BEGIN_LOG();
	JZWRITE_DEBUG("print defnie");
	mDefMgr.printAllDefine();
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

	JZWRITE_DEBUG("Final define num:%u,lex num:%lu",mDefMgr.getDefineNum(),mLexRecList.size());
	JZFUNC_END_LOG();
}

uint32 Lex::undoConsume()
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

uint32 Lex::consumeChar(char *ret)
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

uint32 Lex::getLastIndex()
{
	if (!mReaderStack.empty())
	{
		return mReaderStack.top().curIndex - 1;
	}
	JZWRITE_ERROR("try to get index when empty");
	return 0;
}
/*********************************************************
	when handling ' and ", I simply ignore the \ problem
    I just find the next quotation,so some error will not
	happen in my compilor	
 ********************************************************/

uint32 Lex::handleSingleQuotation()
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

uint32 Lex::handleDoubleQuotation()
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

uint32 Lex::readChar(char *ret)
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


uint32 Lex::handleBar()
{
	uint32 beginIndex = getLastIndex();
	uint32 ret = eLexNoError;
	char nextChar = 0;
	ret = readChar(&nextChar);
	string toSave = "|";
	if (
		'=' == nextChar ||
		'|' == nextChar)
	{
		consumeChar(&nextChar);
		toSave += nextChar;
	}
	uint32 endIndex = getLastIndex();
	saveWord(toSave,beginIndex, endIndex);
	return ret;
}

uint32 Lex::handleLeftSharpBracket()
{
	uint32 beginIndex = getLastIndex();
	uint32 ret = eLexNoError;
	char nextChar = 0;
	ret = readChar(&nextChar);
	string toSave = "<";
	if (
		'=' == nextChar ||
		'<' == nextChar)
	{
		consumeChar(&nextChar);
		toSave += nextChar;
	}
	//if any error happen,return now
	if (ret != eLexNoError)
	{
		return ret;
	}

	if (0 == toSave.compare("<<") )
	{
		//toSave == "<<"
		char nextChar = 0;
		ret = readChar(&nextChar);
		if ('=' == nextChar)
		{
			toSave += nextChar;
		}
	}
	uint32 endIndex = getLastIndex();
	saveWord(toSave,beginIndex, endIndex);
	return ret;
}

uint32 Lex::handleComma()
{
	uint32 commaBeginIndex = getLastIndex();
	if (true == isFuncLikeMacroMode())
	{
		if (1 == getBracketMarkStackSize())
		{
			JZWRITE_DEBUG("save a Param ");
			//save it!
			uint32 beginMark = getBracketBeginMark();
			string param;
			int endIndex = mLexRecList.back().endIndex;
			int beginIndex = mLexRecList[beginMark].beginIndex;
			JZWRITE_DEBUG("begin is :%d,end is :%d", beginIndex, endIndex);
			for(int i = beginIndex ; i <= endIndex ; i++)
			{
				param += mReaderStack.top().buffer[i];	
			}
			while(mLexRecList.size() > beginMark)
			{
				mLexRecList.pop_back();
			}
			mRealParamList.push_back(param);
			return eLexNoError;
		}
	}
	
	uint32 commaEndIndex = getLastIndex();
	saveWord(",",commaBeginIndex,commaEndIndex);
	return eLexNoError;
}

uint32 Lex::handleRightSharpBracket()
{
	uint32 beginIndex = getLastIndex();
	uint32 ret = eLexNoError;
	char nextChar = 0;
	ret = readChar(&nextChar);
	string toSave = ">";
	if (
		'=' == nextChar ||
		'>' == nextChar)
	{
		consumeChar(&nextChar);
		toSave += nextChar;
	}
	//if any error happen,return now
	if (ret != eLexNoError)
	{
		return ret;
	}

	if (0 == toSave.compare(">>") )
	{
		//toSave == ">>"
		nextChar = 0;
		ret = readChar(&nextChar);
		if ('=' == nextChar)
		{
			consumeChar(&nextChar);
			toSave += nextChar;
		}
	}
	uint32 endIndex = getLastIndex();
	saveWord(toSave, beginIndex, endIndex);
	return ret;
}
uint32 Lex::getBracketMarkStackSize()
{
	return mReaderStack.top().mBracketMarkStack.size();
}

void Lex::pushLeftBracket(uint32 mark)
{
	mReaderStack.top().mBracketMarkStack.push(mark);
}

void Lex::popLeftBracket()
{
	mReaderStack.top().mBracketMarkStack.pop();
}

uint32 Lex::handleLeftBracket()
{
	uint32 beginIndex = getLastIndex();
	JZFUNC_BEGIN_LOG();
	if (true == isFuncLikeMacroMode())
	{
		//now it is analyzing macro like func,do something
		JZWRITE_DEBUG("now push a mark");
		uint32 mark = mLexRecList.size();
		pushLeftBracket(mark);
		JZWRITE_DEBUG("push over");
		if (1 == getBracketMarkStackSize())
		{
			JZWRITE_DEBUG("first bracket,don't save");
			return eLexNoError;
		}
	}
	uint32 endIndex = getLastIndex();
	saveWord("(",beginIndex,endIndex);
	JZFUNC_END_LOG();
	return eLexNoError;
}
uint32 Lex::getBracketBeginMark()
{
	return mReaderStack.top().mBracketMarkStack.top();
}

uint32 Lex::handleRightBracket()
{
	uint32 bracketBeginIndex = getLastIndex();
	JZFUNC_BEGIN_LOG();
	if (true == isFuncLikeMacroMode())
	{
		JZWRITE_DEBUG("now pop a mark");
		if (0 == getBracketMarkStackSize())
		{
			JZFUNC_END_LOG();
			return eLexUnknowError;
		}
		if (1 == getBracketMarkStackSize())
		{
			JZWRITE_DEBUG("save a Param ");
			//save it!
			uint32 beginMark = getBracketBeginMark(); 
			string param;
			int beginIndex = mLexRecList[beginMark].beginIndex;
			int endIndex = mLexRecList.back().endIndex;
			JZWRITE_DEBUG("begin is :%d,end is :%d", beginIndex, endIndex);
			for(int i = beginIndex ; i <= endIndex ; i++)
			{
				param += mReaderStack.top().buffer[i];
			}
			while(mLexRecList.size() > beginMark)
			{
				mLexRecList.pop_back();
			}
			mRealParamList.push_back(param);
		}
		popLeftBracket();
		if (0 == getBracketMarkStackSize())
		{
			JZFUNC_END_LOG();
			return eLexParamAnalyzeOVer;
		}

	}
	uint32 bracketEndIndex = getLastIndex();
	saveWord(")", bracketBeginIndex, bracketEndIndex);
	JZFUNC_END_LOG();
	return eLexNoError;
}

uint32 Lex::handlePoint()
{
	uint32 beginIndex = getLastIndex();
	uint32 ret = eLexNoError;
	char nextChar = 0;
	ret = readChar(&nextChar);
	string toSave = ".";
	if ('.' == nextChar)
	{
		nextChar = 0;
		ret = readChar(&nextChar);
		if ('.' == nextChar)
		{
			//consume two char:".."
			consumeChar(&nextChar);
			consumeChar(&nextChar);
			toSave = "...";
		}
	}else if ('*' == nextChar)
	{
		//.*
		toSave += nextChar;
	}
	uint32 endIndex = getLastIndex();
	saveWord(toSave,beginIndex,endIndex);
	return ret;
}

uint32 Lex::handleAnd()
{
	uint32 beginIndex = getLastIndex();
	uint32 ret = eLexNoError;
	char nextChar = 0;
	ret = readChar(&nextChar);
	string toSave = "&";
	if (
		'=' == nextChar ||
		'&' == nextChar)
	{
		consumeChar(&nextChar);
		toSave += nextChar;
	}
	uint32 endIndex = getLastIndex();
	saveWord(toSave,beginIndex,endIndex);
	return ret;
}

uint32 Lex::handleEqual()
{
	uint32 beginIndex = getLastIndex();
	uint32 ret = eLexNoError;
	char nextChar = 0;
	ret = readChar(&nextChar);
	string toSave = "=";
	if ('=' == nextChar)
	{
		consumeChar(&nextChar);
		toSave += nextChar;
	}
	uint32 endIndex = getLastIndex();
	saveWord(toSave,beginIndex,endIndex);
	return ret;
}

uint32 Lex::handleMinus()
{
	uint32 beginIndex = getLastIndex();
	uint32 ret = eLexNoError;
	char nextChar = 0;
	ret = readChar(&nextChar);
	string toSave = "-";
	if (
		'-' == nextChar||	//--
		'>' == nextChar||	//->
		'=' == nextChar)	//-=
	{
		consumeChar(&nextChar);
		toSave += nextChar;
	}
	if ("->" == toSave)
	{
		ret = readChar(&nextChar);
		if ('*' == nextChar) //->*
		{
			toSave += nextChar;
		}
	}
	uint32 endIndex = getLastIndex();
	saveWord(toSave,beginIndex,endIndex);
	return ret;
}

uint32 Lex::handleWave()
{
	uint32 beginIndex = getLastIndex();
	uint32 ret = eLexNoError;
	char nextChar = 0;
	ret = readChar(&nextChar);
	string toSave = "~";
	if ('=' == nextChar)
	{
		consumeChar(&nextChar);
		toSave += nextChar;
	}
	uint32 endIndex = getLastIndex();
	saveWord(toSave,beginIndex,endIndex);
//	saveWord(toSave);
	return ret;
}

uint32 Lex::handleUpponSharp()
{
	uint32 beginIndex = getLastIndex();
	uint32 ret = eLexNoError;
	char nextChar = 0;
	ret = readChar(&nextChar);
	string toSave = "^";
	if ('=' == nextChar)
	{
		consumeChar(&nextChar);
		toSave += nextChar;
	}
//	saveWord(toSave);
	uint32 endIndex = getLastIndex();
	saveWord(toSave,beginIndex,endIndex);
	return ret;
}

uint32 Lex::handleColon()
{
	uint32 beginIndex = getLastIndex();
	uint32 ret = eLexNoError;
	char nextChar = 0;
	ret = readChar(&nextChar);
	string toSave = ":";
	if (':' == nextChar)
	{
		consumeChar(&nextChar);
		toSave += nextChar;
	}
	uint32 endIndex = getLastIndex();
	saveWord(toSave,beginIndex,endIndex);
	return ret;

}

uint32 Lex::handleMod()
{
	uint32 beginIndex = getLastIndex();
	uint32 ret = eLexNoError;
	char nextChar = 0;
	ret = readChar(&nextChar);
	string toSave = "%";
	if ('=' == nextChar)
	{
		consumeChar(&nextChar);
		toSave += nextChar;
	}
//	saveWord(toSave);
	uint32 endIndex = getLastIndex();
	saveWord(toSave,beginIndex,endIndex);
	return ret;

}


uint32 Lex::handlePlus()
{
	uint32 beginIndex = getLastIndex();
	uint32 ret = eLexNoError;
	char nextChar = 0;
	ret = readChar(&nextChar);
	string toSave = "+";
	if (
		'+' == nextChar||
		'=' == nextChar)
	{
		consumeChar(&nextChar);
		toSave += nextChar;
	}
//	saveWord(toSave);
	uint32 endIndex = getLastIndex();
	saveWord(toSave,beginIndex,endIndex);
	return ret;
}

uint32 Lex::handleStart()
{
	uint32 beginIndex = getLastIndex();
	uint32 ret = eLexNoError;
	char nextChar = 0;
	ret = readChar(&nextChar);
	string toSave = "*";
	if ('=' == nextChar)
	{
		consumeChar(&nextChar);
		toSave += nextChar;
	}
//	saveWord(toSave);
	uint32 endIndex = getLastIndex();
	saveWord(toSave,beginIndex,endIndex);
	return ret;
}

uint32 Lex::handleExclamation()
{
	//handle "!"
	uint32 beginIndex = getLastIndex();
	uint32 ret = eLexNoError;
	char nextChar = 0;
	ret = readChar(&nextChar);
	string toSave = "!";
	if ('=' == nextChar)
	{
		consumeChar(&nextChar);
		toSave += nextChar;
	}
//	saveWord(toSave);
	uint32 endIndex = getLastIndex();
	saveWord(toSave,beginIndex,endIndex);
	return ret;
}

uint32 Lex::handleDivideSlant()
{
	uint32 beginIndex = getLastIndex();
	uint32 ret = eLexNoError;
	char nextChar = 0;
	ret = readChar(&nextChar);
	string toSave = "&";
	if ('=' == nextChar)
	{
		consumeChar(&nextChar);
		toSave += nextChar;
	}
//	saveWord(toSave);
	uint32 endIndex = getLastIndex();
	saveWord(toSave,beginIndex,endIndex);
	return ret;
}

uint32 Lex::handleSlant()
{
	uint32 beginIndex = getLastIndex();
	JZFUNC_BEGIN_LOG();
	char nextChar = 0;
	uint32 ret = eLexNoError;
	ret = readChar(&nextChar);
	switch(nextChar)
	{
		case '/':
		{
			ret = readChar(&nextChar);
			JZFUNC_END_LOG();
			return handleCommentLine();	
		}
		case '*':
		{
			ret = readChar(&nextChar);
			JZFUNC_END_LOG();
			return handleCommentBlock();	
		}
		default:
		{
			//maybe this means divide
			JZFUNC_END_LOG();
			return handleDivideSlant();
		}
	}

	JZFUNC_END_LOG();
	return eLexNoError;
}

uint32 Lex::handleSharpPragma()
{
	JZFUNC_BEGIN_LOG();
	uint32 ret = eLexNoError;
	string word;
	ret = consumeWord(word,eLexSkipEmptyInput,eLexInOneLine);
	//there is other more word to handle,but I just care about once
	//if more key word is to handle,I will make a func ptr map table
	
	if (word == "once")
	{
		mOnceFileSet.insert(mReaderStack.top().fileName);
	}
	ret = consumeCharUntilReach('\n',&word);
	JZFUNC_END_LOG();
	return ret ;
}

uint32 Lex::handleCommentLine()
{
	JZFUNC_BEGIN_LOG();
	char nextChar = 0;
	uint32 ret = eLexNoError;
	bool backSlantBeforeLineSeperator = false;

	while ((ret = consumeChar(&nextChar)) == eLexNoError)
	{
		if (false == LexUtil::isEmptyInput(nextChar) && true == backSlantBeforeLineSeperator)
		{
			backSlantBeforeLineSeperator = false;
			continue;
		}
		if ('\\' == nextChar)
		{
			backSlantBeforeLineSeperator = true;
			continue;
			
		}
		if (true == LexUtil::isLineEnder(nextChar))
		{
			if (true == backSlantBeforeLineSeperator)
			{

				JZWRITE_DEBUG("line ended with back slant");
				continue;
			}
			else
			{
				break;
			}
		}
	}
	JZFUNC_END_LOG();
	return ret;
}

uint32 Lex::handleCommentBlock()
{
	JZFUNC_BEGIN_LOG();
	uint32 ret = eLexNoError;
	string retStr = "";
	char retChar = 0;

	do
	{
		ret = consumeCharUntilReach('*', &retStr);
		if (eLexNoError == ret)
		{
			ret = readChar(&retChar);
		}
		else
		{
			return ret;
		}
		if (retChar == '/')
		{
			char slant;
			consumeChar(&slant);
			break;
		}
	}while(ret == eLexNoError);

	JZFUNC_END_LOG();
	return ret;
}

uint32 Lex::handleSharp()
{
	JZFUNC_BEGIN_LOG();
	uint32 beginIndex = getLastIndex();
	uint32 ret = eLexNoError;
	string word = "";

	ret = consumeWord(word, eLexSkipEmptyInput, eLexInOneLine);
	//in this situation, reach line end means error
	if (ret != eLexNoError)
	{
		if (ret == eLexReachLineEnd && false == LexUtil::isEmptyInput(word))
		{
			JZWRITE_DEBUG("simply reach line end");	
		}
		else
		{
			JZFUNC_END_LOG();
			return ret;
		}
	}
	if (false == isLastStreamUseful())
	{
		//when compile stream is off,only handle these macro
		if(true == LexUtil::ignoreMacroWhenStreamIsOff(word))	
		{
			JZFUNC_END_LOG();
			return eLexNoError;
		}
		
	}
	JZWRITE_DEBUG("handle word:%s",word.c_str());	
	LexPatternHandler handler = LexPtnTbl->getMacroPattern(word);
	if (NULL == handler)
	{
		JZWRITE_DEBUG("no handler ,maybe this is a sharp input");
		uint32 endIndex = getLastIndex();
		saveWord("#", beginIndex, endIndex);
		JZWRITE_DEBUG("unknow macor:%s",word.c_str());
		popErrorSite();
		JZFUNC_END_LOG();
		return eLexUnknowError;
	}
	else
	{
		uint32 err = eLexNoError;
		err = (this->*handler)();
		if (err != eLexNoError)
		{
			JZFUNC_END_LOG();
			return err;
		}
	}
	JZFUNC_END_LOG();
	return eLexNoError;
}

uint32 Lex::handleSharpElse()
{
	JZFUNC_BEGIN_LOG();
	string word = "";
	uint32 ret = consumeWord(
			word,eLexSkipEmptyInput,eLexInOneLine);
	if (eLexNoError != ret && ret != eLexReachLineEnd)
	{
		JZFUNC_END_LOG();
		return ret;
	}
	if (false ==  LexUtil::isEmptyInput(word))
	{
		JZWRITE_DEBUG("word is :[%s]",word.c_str());
		JZFUNC_END_LOG();
		popErrorSite();
		return eLexSharpElseFollowWithOtherThing;
	}
	JZFUNC_END_LOG();
	return pushPrecompileStreamControlWord(eLexPSELSE);

}

uint32 Lex::handleSharpEndIf()
{
	JZFUNC_BEGIN_LOG();
	string word = "";
	uint32 ret = consumeWord(
			word,eLexSkipEmptyInput,eLexInOneLine);
	if (eLexNoError != ret && ret != eLexReachLineEnd)
	{
		JZFUNC_END_LOG();
		return ret;
	}
	if (false ==  LexUtil::isEmptyInput(word))
	{
		JZFUNC_END_LOG();
		JZWRITE_DEBUG("word is :%s",word.c_str());
		return eLexSharpEndIfFollowWithOtherThing;
	}
	JZFUNC_END_LOG();
	return pushPrecompileStreamControlWord(eLexPSENDIF);

}

uint32 Lex::handleSharpWarning()
{
	string word;
	uint32 errRet;
	errRet = consumeCharUntilReach('\n',&word);
	JZWRITE_DEBUG("Warning:%s",word.c_str());
	return errRet;
}
uint32 Lex::handleSharpError()
{
	string word;
	uint32 errRet;
	errRet = consumeCharUntilReach('\n',&word);
	JZWRITE_DEBUG("Error:%s",word.c_str());
	return errRet;
	
}
uint32 Lex::handleSharpIfndef()
{
	JZFUNC_BEGIN_LOG();
	uint32 ret;
	string word;
	ret = consumeWord(word,eLexSkipEmptyInput, eLexInOneLine);
	if (eLexNoError != ret && eLexReachLineEnd != ret)
	{
		JZFUNC_END_LOG();
		return ret;
	}
	JZWRITE_DEBUG("word is :[%s]",word.c_str());
	//check if word is defined
	if (true == LexUtil::isEmptyInput(word))
	{
		JZFUNC_END_LOG();
		return eLexSharpIfdefFollowedWithNothing;
	}
	
	bool isDef = (DefineManager::eDefMgrDefined == mDefMgr.isDefined(word));
	JZWRITE_DEBUG("word:%s,is define :%d",word.c_str(), isDef);

	ret = pushPrecompileStreamControlWord(eLexPSIFNDEF, !isDef);
	
	JZFUNC_END_LOG();
	return ret ;
}

uint32 Lex::handleSharpIfdef()
{
	JZFUNC_BEGIN_LOG();
	uint32 ret;
	string word;
	ret = consumeWord(word,eLexSkipEmptyInput, eLexInOneLine);
	if (eLexNoError != ret && eLexReachLineEnd != ret)
	{
		JZFUNC_END_LOG();
		return ret;
	}
	JZWRITE_DEBUG("word is :[%s]",word.c_str());
	//check if word is defined
	if (true == LexUtil::isEmptyInput(word))
	{
		JZFUNC_END_LOG();
		return eLexSharpIfdefFollowedWithNothing;
	}
	
	bool isDef = (DefineManager::eDefMgrDefined == mDefMgr.isDefined(word));
	JZWRITE_DEBUG("word:%s,is define :%d",word.c_str(), isDef);

	ret = pushPrecompileStreamControlWord(eLexPSIFDEF, isDef);
	
	JZFUNC_END_LOG();
	return ret ;
}

void Lex::popErrorSite()
{
	string curLineChar = "";
	uint32 lastEnderIndex = 0;
	uint32 curIndex = (int)mReaderStack.top().curIndex;
	if ('\n' == mReaderStack.top().buffer[curIndex])
	{
		curIndex --;
	}
	for(int i = curIndex; i >=0 ; i --)
	{
		if ('\n' == mReaderStack.top().buffer[i])
		{
			break;
		}
		lastEnderIndex = i;
	}
	for(uint64 i = lastEnderIndex; i < mReaderStack.top().bufferSize; i++)
	{
		if (mReaderStack.top().buffer[i] == '\n')
		{
			break;
		}
		curLineChar += mReaderStack.top().buffer[i];	
	}
	JZWRITE_DEBUG("FileName:%s,curIndex:%d,curLineChar:[%s]",mReaderStack.top().fileName.c_str(),mReaderStack.top().curIndex,curLineChar.c_str());
}
uint32 Lex::handleIsDefined(string& ret)
{
	JZFUNC_BEGIN_LOG();
	string word;
	uint32 errRet;
	errRet = consumeCharUntilReach('(',&word,eLexInOneLine);
	if (errRet != eLexNoError)
	{
		JZFUNC_END_LOG();
		return errRet;
	}
	errRet = consumeWord(word, eLexSkipEmptyInput, eLexInOneLine);
	if (errRet != eLexNoError && errRet != eLexReachLineEnd)
	{
		JZFUNC_END_LOG();
		return errRet;
	}

	//check
	if (DefineManager::eDefMgrDefined == mDefMgr.isDefined(word))
	{
		ret = "1";
	}
	else
	{
		ret = "0";	
	}
	JZWRITE_DEBUG("cur file is :%s",mReaderStack.top().fileName.c_str());
	JZWRITE_DEBUG("check word:%s",word.c_str());
	//consume until another )
	errRet = consumeCharUntilReach(')',&word,eLexInOneLine);
	if ((errRet != eLexNoError && errRet != eLexReachLineEnd)|| true == LexUtil::isEmptyInput(word))
	{
		popErrorSite();
		JZFUNC_END_LOG();
		return eLexUnexpectedSeperator;
	}
	JZWRITE_DEBUG("ret word is :%s",ret.c_str());
	JZFUNC_END_LOG();
	return errRet;
}

uint32 Lex::pushPrecompileStreamControlWord(uint32 mark,bool isSuccess)
{
	JZFUNC_BEGIN_LOG();
	PrecompileSelector ps = 
	{
		.mark = mark,
		.isSuccess = isSuccess,
		.tag = getTopPSStack().size() + 1,
	};

	//set begin tag
	switch(mark)
	{
		case eLexPSIF:
		case eLexPSIFNDEF:
		case eLexPSIFDEF:
			ps.beginTag = ps.tag;
			break;
		default:
		{
			if (0 == getTopPSStack().size())
			{
				JZWRITE_DEBUG("mark is :%d",mark);
				return eLexUnmatchMacro;
			}
			ps.beginTag = getTopPSStack().back().beginTag;
		}
	};

	//check if this stream is really success
	switch(mark)
	{
		case eLexPSIF:
		case eLexPSIFNDEF:
		case eLexPSIFDEF:
			//do nothing
			break;
		case eLexPSELSE:
		case eLexPSELIF:
		{
			if (false == ps.isSuccess)
			{
				break;
			}
			for(int i = ps.beginTag - 1; i < getTopPSStack().size(); i++)
			{
				if (true == getTopPSStack()[i].isSuccess)
				{
					ps.isSuccess =false;
					break;
				}
			}
			uint32 offStream = getCompileStream();
			if (0 != offStream)
			{
				if (ps.beginTag == getTopPSStack()[offStream - 1].beginTag)
				{
					turnOnCompileStream();
				}
			}
			break;
		}
		case eLexPSENDIF:
		{
			uint32 offStream = getCompileStream();
			if (0 != offStream)
			{
				if (ps.beginTag == getTopPSStack()[offStream - 1].beginTag)
				{
					turnOnCompileStream();
				}
			}
			uint32 curMark = eLexPSENDIF;
			while (false == getTopPSStack().empty() && ps.beginTag == getTopPSStack().back().beginTag)
			{
				if (false == LexUtil::canPopCompileStream(curMark,getTopPSStack().back().mark))
				{
					JZFUNC_END_LOG();
					return eLexCanNotPushPrecompileStream;
				}
				curMark = getTopPSStack().back().mark;
				getTopPSStack().pop_back();
			}
			return eLexNoError;
		}
		default:
		{
			JZWRITE_DEBUG("unregister mark:%d", mark);	
			return eLexUnknowMacro;
		}
	}

	if (true == isLastStreamUseful() && false == ps.isSuccess)
	{
		turnOffCompileStream(ps.tag);
	}

	getTopPSStack().push_back(ps);
	JZFUNC_END_LOG();
	return eLexNoError;
}

void Lex::turnOnCompileStream()
{
	if (!mReaderStack.empty())
	{
		mReaderStack.top().mStreamOffTag = 0;
	}
}

uint32 Lex::getCompileStream()
{
	if (mReaderStack.empty())
	{
		return 0;
	}
	return mReaderStack.top().mStreamOffTag;
}

void Lex::turnOffCompileStream(uint32 tag)
{
	if (!mReaderStack.empty())
	{
		mReaderStack.top().mStreamOffTag = tag;
	}
}
vector<PrecompileSelector>& Lex::getTopPSStack()
{
	return mReaderStack.top().mPSStack;
}

bool Lex::isLastMacroSuccess()
{
	if (true == getTopPSStack().empty())
	{
		//if stack is empty ,always return true;
		return true;
	}
	return getTopPSStack().back().isSuccess;
}

bool Lex::isLastStreamUseful()
{
	return (getCompileStream() == 0) && isLastMacroSuccess();
}

uint32 Lex::handleSharpDefine()
{
	JZFUNC_BEGIN_LOG();

	/*********************************************************
		now handle define! 
	 ********************************************************/
	
	uint32 ret = eLexNoError;
	char seperator = 0;
	string key = "";

	ret = consumeWord(key);
	if (eLexNoError != ret)
	{
		return ret;
	}

	DefineRec defineRec;
	defineRec.key = key;
	ret = consumeChar(&seperator);
	if ('(' == seperator)
	{
		defineRec.isFuncLikeMacro = true;
		//read format parm
		string param;
		char seperator;
		uint32 paramRet;
		bool isParam = true;
		while(eLexNoError == (paramRet = consumeWord(param,eLexSkipEmptyInput,eLexInOneLine)))
		{
			if (param == "")
			{
				JZWRITE_DEBUG("read param should not reach empty");
				return eLexUnexpectedSeperator;
			}
			JZWRITE_DEBUG("param is :%s",param.c_str());
			if (param[0] != ')' && param[0] != ',')
			{
				if ('.' != param[0] && true == LexUtil::isInterpunction(param[0]))
				{
					JZFUNC_END_LOG();
					return eLexUnexpectedSeperator;
				}
				if (seperator == '.')
				{
					uint32 endIndex = getLastIndex() - 1;
					uint32 beginIndex = getLastIndex() - 1 - param.size();
					if (eLexNoError == tryToMatchWord(".."))
					{
						defineRec.isVarArgs = true;
						if (false == isParam)
						{
							defineRec.formalParam.back().type = eLexRecTypeFuncLikeMacroVarParam;
						}
						else
						{
							saveWordTo(C_MACRO_WORD___VA_ARGS__,defineRec.formalParam,beginIndex,endIndex,eLexRecTypeFuncLikeMacroVarParam);
						}
						isParam = false;
					}
					else
					{
						JZWRITE_DEBUG("reach here");
						return eLexUnexpectedSeperator;
					}
					continue;
				}
				if (false == isParam)
				{
					JZFUNC_END_LOG();
					popErrorSite();
					return eLexUnexpectedSeperator;
				}
				isParam = false;
				JZWRITE_DEBUG("save a param :%s",param.c_str());
				if (defineRec.formalParam.size() > 0 &&
					defineRec.formalParam.back().type == eLexRecTypeFuncLikeMacroVarParam )
				{
					//should not save more param
					JZWRITE_DEBUG("val param end with not val param:%s",param.c_str());
					return eLexValParamNotLast;
				}
				uint32 endIndex = getLastIndex() - 1;
				uint32 beginIndex = getLastIndex() - 1 - param.size();
				saveWordTo(param ,defineRec.formalParam,beginIndex,endIndex, eLexRecTypeFuncLikeMacroParam);
				
			}
			else if (param[0]== ')')
			{
				//reach end
				if (false == isParam)
				{
					break;
				}
				else if(defineRec.formalParam.size() == 0)
				{
					break;	
				}
				else
				{
					JZFUNC_END_LOG();
					return eLexUnexpectedSeperator;	
				}
			}
			else if (param[0] != ',')
			{
				JZWRITE_DEBUG("reach here,word is : %s",param.c_str());
				popErrorSite();
				return eLexUnexpectedSeperator;	
			}
			else
			{
				
				isParam = true;
			}
		}
	}
	else
	{
		defineRec.isVarArgs = false;
		defineRec.isFuncLikeMacro =false;
	}

	string defWord = "";
	uint32 retErr = eLexNoError;
	if (seperator == '\n')
	{
		defWord = "";
	}
	else
	{
		retErr = consumeCharUntilReach('\n',&defWord);
		
		if (eLexNoError != retErr)
		{
			JZFUNC_END_LOG();
			return retErr;
		}
	}


	defineRec.defineStr = defWord;
	mDefMgr.addDefineMap(key, defineRec);	
	JZFUNC_END_LOG();
	return ret;
}

uint32 Lex::checkMacro(bool *isSuccess,uint32 checkMark)
{
	JZFUNC_BEGIN_LOG();
	string logicStr = "";
	uint32 retErr = eLexNoError;
	
	retErr = consumeCharUntilReach('\n',&logicStr);
	if (eLexNoError != retErr)
	{
		JZFUNC_END_LOG();
		return retErr;
	}
	char* buff = (char*)malloc(logicStr.size());
	if (NULL == buff)
	{
		return eLexUnknowError;
	}
	memcpy(buff,logicStr.c_str(), logicStr.size());
	JZWRITE_DEBUG("analyze buff :%s",logicStr.c_str());
	pushReaderRecord(buff,logicStr.size(),"",eFileTypeMacro);

	//pop until reach last
	int lastRecPtr = mLexRecList.size();
	uint32 lexret = doLex();
	if (lexret != eLexNoError && lexret != eLexReachFileEnd)
	{
		return lexret;
	}
	popReaderRecord();

	LexRecList list;
	list.resize(mLexRecList.size() - lastRecPtr);
	JZWRITE_DEBUG("list size :%ld",list.size());

	for(int i = mLexRecList.size() - 1; i >= lastRecPtr; i--)
	{
		list[i - lastRecPtr] = mLexRecList[i];
		mLexRecList.pop_back();
	}
#ifdef DEBUG
	JZWRITE_DEBUG("top file is :%s,now print to check macro",mReaderStack.top().fileName.c_str());
	for(int i = 0 ; i < list.size() ; i++)
	{
		JZWRITE_DEBUG("word:[%s]",list[i].word.c_str());	
	}
#endif
	if (NULL == isSuccess)
	{
		//no ptr, this is a comsume input
		JZFUNC_END_LOG();
		return eLexNoError;
	}
	uint32  ret = eLexNoError;
	if(true == isLastStreamUseful() ||
			(checkMark == eLexPSELIF && getTopPSStack().back().beginTag == getCompileStream()) )
	{
		ret = isMacroSuccess(list, isSuccess);
	}
	else
	{
		*isSuccess = false;
	}
	return ret;

}
uint32 Lex::handleSharpElif()
{
	JZFUNC_BEGIN_LOG();
	bool isSuccess = false;
	uint32 ret = checkMacro(&isSuccess, eLexPSELIF);
	if (ret != eLexNoError)
	{
		JZFUNC_END_LOG();
		return ret;
	}
	JZFUNC_END_LOG();
	return pushPrecompileStreamControlWord(eLexPSELIF, isSuccess);
}

uint32 Lex::handleSharpIf()
{
	JZFUNC_BEGIN_LOG();
	bool isSuccess = false;
	uint32 ret = checkMacro(&isSuccess, eLexPSIF);
	if (ret != eLexNoError)
	{
		JZFUNC_END_LOG();
		popErrorSite();
		return ret;
	}
	ret = pushPrecompileStreamControlWord(eLexPSIF, isSuccess);
	JZFUNC_END_LOG();
	return ret;
}

uint32 Lex::handleSharpInclude()
{
	JZFUNC_BEGIN_LOG();
	uint32 ret = eLexNoError;
	string word;
	ret = this->consumeWord(word,eLexSkipEmptyInput, eLexInOneLine);
	if (ret != eLexNoError)
	{
		return ret;
	}
	if (true == LexUtil::isEmptyInput(word))
	{
		JZWRITE_DEBUG("include follow with not empty input");
		return eLexUnknowError;
	}
	if (word[0]!= '<' && word[0]!= '"')
	{
		JZWRITE_DEBUG("include follow with error seperator");
		return eLexUnexpectedSeperator;
	}

	string toMatch = "";
	ret = consumeCharUntilReach(LexUtil::seperatorMatcher(word[0]),&toMatch);
	if (ret  != eLexNoError)
	{
		JZWRITE_DEBUG("can not match the seperator in #include for:%c", word[0]);
		return ret;
	}
	string toIncludeFile = toMatch.substr(0,toMatch.size() - 1); 

//	my test show that clang compiler do not eat empty input..
//	maybe some people will name their file with empty end? I doublt it,
//	but I stick to the clang compiler, so I just skip this one here

//	toIncludeFile = LexUtil::eatLREmptyInput(toIncludeFile);
	JZWRITE_DEBUG("to include file is :[%s]", toIncludeFile.c_str());

	string fullPath = IncludeHandler::getInstance()->getFullPathForIncludeFile(toIncludeFile);

	if (fullPath != "")
	{
		uint32 analyzeRet = this->analyzeAFile(fullPath);
		if (analyzeRet != eLexNoError && analyzeRet != eLexReachFileEnd)
		{
			return analyzeRet;
		}
	}
	else
	{
		JZWRITE_DEBUG("no such file");	
	}
	JZFUNC_END_LOG();
	return eLexNoError;
}

uint32 Lex::tryToMatchWord(const string& word)
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

uint32 Lex::consumeCharUntilReach(const char inputEnder, string *ret, LexInput inOneLine)
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

bool Lex::isMacroExpending(const string& input)
{
	JZFUNC_BEGIN_LOG();
	if (mPreprocessingMacroSet.find(input) == mPreprocessingMacroSet.end())
	{
		return false;
	}
	return true;
}

bool Lex::isOnceFile(const string& input)
{
	JZFUNC_BEGIN_LOG();
	if (mOnceFileSet.find(input) == mOnceFileSet.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Lex::turnOnFuncLikeMacroMode()
{
	JZFUNC_BEGIN_LOG();
	if (mReaderStack.empty())
	{
		return;
	}
	mReaderStack.top().mFuncLikeMacroParamAnalyzing = true;
}

void Lex::turnOffFuncLikeMacroMode()
{
	JZFUNC_BEGIN_LOG();
	if (mReaderStack.empty())
	{
		return;
	}
	mReaderStack.top().mFuncLikeMacroParamAnalyzing = false;
}

bool Lex::isFuncLikeMacroMode()
{
	if (mReaderStack.empty())
	{
		return false;
	}
	bool ret = mReaderStack.top().mFuncLikeMacroParamAnalyzing;
	if (ret)
	{
		JZWRITE_DEBUG("func like mode");
	}
	return ret;
}

/*********************************************************
	Lex end here
	now begin the LexUtil   	
 ********************************************************/

bool LexUtil::isInterpunction(const char input)
{
	if (true == isLineEnder(input))
	{
		return true;
	}

	if (true == isEmptyInput(input))
	{
		return true;
	}

	if (true == isBackSlant(input))
	{
		return true;
	}

	if ('.' == input)
	{
    //double type const!!!
		return sPointIsSeperator;
	}
	switch(input)
	{
		case '|':
		case '%':
		case '^':
		case '=':
		case '+':
		case '/':
		case '*':
		case '-':
		case '~':
		case '\'':
		case '"':
		case ' ':
		case ',':
		case '!':
		case '?':
		case '&':
		case '(':
		case ')':
		case '[':
		case ']':
		case '{':
		case '}':
		case '<':
		case '>':
		case '#':
		case '\t':
		case '\n':
		case ';':
		case ':':
		{
			return true;
		}
		default:
		{
			break;	
		}
	}
	return false;
}

bool LexUtil::isLineEnder(const char input)
{
	if ('\n' == input)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool LexUtil::isEmptyInput(const char input)
{
	switch(input)
	{
		case '\t':
		case '\n':
		case ' ':
		case 0:
			return true;
		default:
		{
			return false;
		}
	}
}

bool  LexUtil::isBackSlant(const char input)
{
	if ('\\' == input)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool LexUtil::isEmptyInput(const string& input)
{
	int len = input.size();
	for (int i = 0; i < len; i++) {
		if (false == isEmptyInput(input[i]))
		{
			return false;
		}
	}
	return true;	
}

bool LexUtil::isEndWithBackSlant(const string& input)
{
	bool ret = false;
	for(int32 i = input.size() - 1 ; i >= 0 ; i--)
	{
		if (input[i] == '\\')
		{
			ret = true;
			break;
		}
		else if(false == isEmptyInput(input[i]))
		{
			break;
		}
	}
	return ret;
}

char LexUtil::seperatorMatcher(const char input)
{
	char ret = 0;
	switch(input)
	{
		case '"':
			ret = '"';
			break;
		case '\'':
			ret = '\'';
			break;
		case '[':
			ret = ']';
			break;
		case '<':
			ret = '>';
			break;
		case '(':
			ret = ')';
			break;
		case '{':
			ret = '}';
			break;
		default:
		{
			JZWRITE_DEBUG("not registed input : %c",input);	
			break;
		}
	}
	return ret;
}

char* LexUtil::eraseComment(const char* input,uint64 *bufSize)
{
	JZFUNC_BEGIN_LOG();
	char *ret = (char*)malloc((*bufSize)*sizeof(char));
	uint64 j = 0;
	memset(ret,0,(*bufSize) * sizeof(char));
	bool isString = false;
	bool isChar = false;
	bool lastIsBlant = true;
	bool isCommentLine = false;
	bool isCommentBlock = false;
	for(uint64 i = 0 ; i < (*bufSize); i++)
	{
		if (true == isString)
		{
			ret[j] = input[i];
			if (ret[j] == '"' && false == lastIsBlant)
			{
				isString = false;
			}
			if ('\\' == ret[j])
			{
				lastIsBlant = !lastIsBlant;
			}
			else
			{
				lastIsBlant = false;
			}
			j++;
		}
		else if(true == isChar)
		{
			ret[j] = input[i];
			if (ret[j] == '\'' && false == lastIsBlant)
			{
				isChar= false;
			}
			if ('\\' == ret[j])
			{
				lastIsBlant = !lastIsBlant;
			}
			else
			{
				lastIsBlant = false;
			}
			j++;
		
		}
		else if(true == isCommentLine)
		{
			if (input[i] == '\n')
			{
				isCommentLine = false;
				ret[j] = '\n';
				j++;
			}
		}
		else if(true == isCommentBlock)
		{
			if (input[i] == '/' && input[i - 1] == '*')
			{
				isCommentBlock = false;
			}
		}
		else
		{
			if ('"' == input[i])
			{
				isString = true;
			}
			else if ('\'' == input[i])
			{
				isChar = true;
			}
			else if ('/' == input[i])
			{
				if (i+1 < *bufSize)
				{
					if ('/' == input[i+1] )
					{
						isCommentLine = true;
						i++;
						continue;
					}
					else if('*' == input[i+1])
					{
						isCommentBlock = true;
						i++;
						continue;
					}
				}
			}
			ret[j] += input[i];
			j++;
		}
	}
	*bufSize = j;
	JZFUNC_END_LOG();
	return ret;
}

char* LexUtil::eraseLineSeperator(const char* input,uint64 *bufSize)
{
	JZFUNC_BEGIN_LOG();

  //Bug log: if we init a char point exactly same as the input, it may have some unset memory
  //follows the output, and it make dirty output 
	char *ret = (char*)malloc((*bufSize + 1)*sizeof(char));
	memset(ret,0,(*bufSize + 1) * sizeof(char));

	uint64 j = 0;
	bool isConverBackSlant = false;

	JZWRITE_DEBUG("buff size is :%lld",*bufSize);
	for(uint64 i = 0 ; i < (*bufSize); i++)
	{
		if (false == isBackSlant(input[i]))
		{
			ret[j++] = input[i];
			isConverBackSlant = false;
			continue;
		}
		//check next not empty input
		isConverBackSlant = !isConverBackSlant;
		if (false == isConverBackSlant)
		{
			ret[j++] = '\\';	
			continue;
		}
		int k = i + 1;
		bool endWithBackSlant = true;
		while(k < (*bufSize) && false == isLineEnder(input[k]))
		{
			if (false == isEmptyInput(input[k]))
			{
				endWithBackSlant = false;
				break;
			}
			k++;
		};
		if (true == endWithBackSlant)
		{
			i = k;
		}
		else
		{
			ret[j++] = '\\';	
		}
	}
	*bufSize = j;
	JZFUNC_END_LOG();
	return ret;
}

string LexUtil::eatLREmptyInput(const string& toBeEatan)
{
	string ret = "";
	int leftEmptyNum = 0;
	int rightEmptyNum = 0;
	for(int i = 0; i < toBeEatan.size(); i++)
	{
		if (false == isEmptyInput(toBeEatan[i]))
		{
			break;
		}
		leftEmptyNum++;
	}
	for(int i = toBeEatan.size() - 1; i >= 0; i-- )
	{
		if (false == isEmptyInput(toBeEatan[i]))
		{
			break;
		}
		rightEmptyNum++;	
	}
	for(int i  = leftEmptyNum ; i + rightEmptyNum < toBeEatan.size() ; i++)
	{
		ret += toBeEatan[i];	
	}
	return ret;
}

bool LexUtil::canPopCompileStream(uint32 curMark,uint32 toPopMark)
{
	switch(curMark)
	{
		case eLexPSENDIF:
			if (eLexPSENDIF == toPopMark)
			{
				return false;
			}
			return true;
		case eLexPSELSE:
		case eLexPSELIF:
			if (eLexPSENDIF == toPopMark || eLexPSELSE == toPopMark)
			{
				return false;
			}
			return true;
			if (eLexPSENDIF == toPopMark || eLexPSELSE == toPopMark)
			{
				return false;
			}
			return true;
		case eLexPSIF:
		case eLexPSIFDEF:
		case eLexPSIFNDEF:
			return false;
		default:
		{
			break;	
		}
	}
	return false;
}
bool LexUtil::ignoreMacroWhenStreamIsOff(const string& word)
{
	if (
		word != "if" &&
		word != "ifdef" &&
		word != "ifndef" &&
		word != "endif" &&
		word != "elif" &&
		word != "else" 
		)
	{
		return true;	
	}
	return false;
}

//Ah, I think this is stupid... it make this program unable to be multi process, maybe it will be better for each Lex to have a util, otherwise we will have this problem
void LexUtil::pointIsSeperator(bool state)
{
	sPointIsSeperator = state;
}

bool LexUtil::isConstNumberChar(const char input)
{
	if ('0' <= input && input <= '9')
	{
		return true;
	}
	return false;
}

/*********************************************************
	LexUtil End here, now begin the lex pattern table 
 ********************************************************/

LexPatternTable::LexPatternTable()
{
	init();
}

LexPatternTable::~LexPatternTable()
{
	mPatternHandlerMap.clear();
}

LexPatternTable* LexPatternTable::getInstance()
{
	static LexPatternTable* instance = NULL;
	if (NULL == instance)
	{
		instance = new LexPatternTable();
	}
	return instance;
}

void LexPatternTable::init()
{
	/*********************************************************
		init pattern map here 
	 ********************************************************/
	mPatternHandlerMap['\''] = &Lex::handleSingleQuotation;
	mPatternHandlerMap['"']  = &Lex::handleDoubleQuotation;
	mPatternHandlerMap['#']  = &Lex::handleSharp;
	mPatternHandlerMap['/']  = &Lex::handleSlant;
	mPatternHandlerMap['|']  = &Lex::handleBar;
	mPatternHandlerMap['.']  = &Lex::handlePoint;
	mPatternHandlerMap['<']  = &Lex::handleLeftSharpBracket;
	mPatternHandlerMap['>']  = &Lex::handleRightSharpBracket;
	mPatternHandlerMap['&']  = &Lex::handleAnd;
	mPatternHandlerMap['=']  = &Lex::handleEqual;
	mPatternHandlerMap['*']  = &Lex::handleStart;
	mPatternHandlerMap['!']  = &Lex::handleExclamation;
	mPatternHandlerMap['+']  = &Lex::handlePlus;
	mPatternHandlerMap['-']  = &Lex::handleMinus;
	mPatternHandlerMap['^']  = &Lex::handleUpponSharp;
	mPatternHandlerMap['~']  = &Lex::handleWave;
	mPatternHandlerMap['(']  = &Lex::handleLeftBracket;
	mPatternHandlerMap[')']  = &Lex::handleRightBracket;
	mPatternHandlerMap[',']  = &Lex::handleComma;
	mPatternHandlerMap['%']  = &Lex::handleMod;
	mPatternHandlerMap[':']  = &Lex::handleColon;

	/*********************************************************
		init marco pattern map here 
	 ********************************************************/

	mMacroPatternHandlerMap["ifdef"]   = &Lex::handleSharpIfdef;	
	mMacroPatternHandlerMap["ifndef"]   = &Lex::handleSharpIfndef;	
	mMacroPatternHandlerMap["else"]    = &Lex::handleSharpElse;	
	mMacroPatternHandlerMap["if"]      = &Lex::handleSharpIf;	
	mMacroPatternHandlerMap["endif"]   = &Lex::handleSharpEndIf;	
	mMacroPatternHandlerMap["define"]  = &Lex::handleSharpDefine;	
	mMacroPatternHandlerMap["include"] = &Lex::handleSharpInclude;	
	mMacroPatternHandlerMap["pragma"]  = &Lex::handleSharpPragma;
	mMacroPatternHandlerMap["warning"]  = &Lex::handleSharpWarning;
	mMacroPatternHandlerMap["error"]  = &Lex::handleSharpError;
	mMacroPatternHandlerMap["elif"]  = &Lex::handleSharpElif;
}

LexPatternHandler LexPatternTable::getMacroPattern(const string& input)
{
	if (mMacroPatternHandlerMap.find(input) != mMacroPatternHandlerMap.end())
	{
		return mMacroPatternHandlerMap[input];
	}
	else
	{
		return NULL;	
	}

}

LexPatternHandler LexPatternTable::getPattern(const char input)
{
	if (mPatternHandlerMap.find(input) != mPatternHandlerMap.end())
	{
		return mPatternHandlerMap[input];
	}
	else
	{
		return NULL;	
	}
}
/*********************************************************
	data init helper func 
 ********************************************************/

FileReaderRecord initFileRecord(
		const char* buff,uint64 size,const string& fileName,
		uint32 recordType)
{
	FileReaderRecord ret = 
	{
		.buffer = buff,
		.bufferSize = size,
		.curIndex = 0,
		.curLineNum = 1,
		.fileName = fileName,
		.recordType = recordType,
		.mStreamOffTag = 0,	
		.mFuncLikeMacroParamAnalyzing = false,
	};
	return ret;
}

