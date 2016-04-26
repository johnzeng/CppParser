#include "MacroLex.h"
#include "MacroParamLex.h"
#include "DefineManager.h"
#include "JZMacroFunc.h"
#include "LexPatternTable.h"
#include "IncludeHandler.h"
#include "KeyWordDefine.h"
#include <stdlib.h>
#include "LexUtil.h"
#include "string.h"

MacroLex::MacroLex()
{
	/*********************************************************
		init pattern map here 
	 ********************************************************/
	mPatternTable->insertPattern('|',  (LexPatternHandler)(&MacroLex::handleBar));
	mPatternTable->insertPattern('#',  (LexPatternHandler)(&MacroLex::handleSharp));
	mPatternTable->insertPattern('<',  (LexPatternHandler)(&MacroLex::handleLeftSharpBracket));
	mPatternTable->insertPattern('>',  (LexPatternHandler)(&MacroLex::handleRightSharpBracket));
	mPatternTable->insertPattern('&',  (LexPatternHandler)(&MacroLex::handleAnd));
	mPatternTable->insertPattern('=',  (LexPatternHandler)(&MacroLex::handleEqual));
	mPatternTable->insertPattern('!',  (LexPatternHandler)(&MacroLex::handleExclamation));
	/*********************************************************
		init marco pattern map here 
	 ********************************************************/

	mPatternTable->insertPattern("ifdef",   (LexPatternHandler)(&MacroLex::handleSharpIfdef));	
	mPatternTable->insertPattern("ifndef",   (LexPatternHandler)(&MacroLex::handleSharpIfndef));	
	mPatternTable->insertPattern("else",    (LexPatternHandler)(&MacroLex::handleSharpElse));	
	mPatternTable->insertPattern("if",      (LexPatternHandler)(&MacroLex::handleSharpIf));	
	mPatternTable->insertPattern("endif",   (LexPatternHandler)(&MacroLex::handleSharpEndIf));	
	mPatternTable->insertPattern("define",  (LexPatternHandler)(&MacroLex::handleSharpDefine));	
	mPatternTable->insertPattern("include", (LexPatternHandler)(&MacroLex::handleSharpInclude));	
	mPatternTable->insertPattern("pragma",  (LexPatternHandler)(&MacroLex::handleSharpPragma));
	mPatternTable->insertPattern("warning",  (LexPatternHandler)(&MacroLex::handleSharpWarning));
	mPatternTable->insertPattern("error",  (LexPatternHandler)(&MacroLex::handleSharpError));
	mPatternTable->insertPattern("elif",  (LexPatternHandler)(&MacroLex::handleSharpElif));
}

MacroLex::~MacroLex()
{
}

uint32 MacroLex::handleIsDefined(string& ret)
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

uint32 MacroLex::pushPrecompileStreamControlWord(uint32 mark,bool isSuccess)
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

void MacroLex::turnOnCompileStream()
{
	if (!mReaderStack.empty())
	{
		mReaderStack.top().mStreamOffTag = 0;
	}
}

uint32 MacroLex::getCompileStream()
{
	if (mReaderStack.empty())
	{
		return 0;
	}
	return mReaderStack.top().mStreamOffTag;
}

void MacroLex::turnOffCompileStream(uint32 tag)
{
	if (!mReaderStack.empty())
	{
		mReaderStack.top().mStreamOffTag = tag;
	}
}
vector<PrecompileSelector>& MacroLex::getTopPSStack()
{
	return mReaderStack.top().mPSStack;
}

uint32 MacroLex::handleSharpPragma()
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

uint32 MacroLex::handleSharp()
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
	LexPatternHandler handler = mPatternTable->getMacroPattern(word);
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

uint32 MacroLex::handleSharpElse()
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

uint32 MacroLex::handleSharpEndIf()
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

uint32 MacroLex::handleSharpWarning()
{
	string word;
	uint32 errRet;
	errRet = consumeCharUntilReach('\n',&word);
	JZWRITE_DEBUG("Warning:%s",word.c_str());
	return errRet;
}
uint32 MacroLex::handleSharpError()
{
	string word;
	uint32 errRet;
	errRet = consumeCharUntilReach('\n',&word);
	JZWRITE_DEBUG("Error:%s",word.c_str());
	return errRet;
	
}
uint32 MacroLex::handleSharpIfndef()
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

uint32 MacroLex::handleSharpIfdef()
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


bool MacroLex::isLastMacroSuccess()
{
	if (true == getTopPSStack().empty())
	{
		//if stack is empty ,always return true;
		return true;
	}
	return getTopPSStack().back().isSuccess;
}

bool MacroLex::isLastStreamUseful()
{
	return (getCompileStream() == 0) && isLastMacroSuccess();
}

uint32 MacroLex::handleSharpDefine()
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
				if (param[0] == '.')
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

uint32 MacroLex::checkMacro(bool *isSuccess,uint32 checkMark)
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
	pushReaderRecordByParams(buff,logicStr.size(),"",eFileTypeMacro);

	//pop until reach last
	int lastRecPtr = mLexRecList.size();
	uint32 lexret = doLex();
	if (lexret != eLexNoError && lexret != eLexReachFileEnd)
	{
		return lexret;
	}
  JZSAFE_DELETE(buff)
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
uint32 MacroLex::handleSharpElif()
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

uint32 MacroLex::handleSharpIf()
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

uint32 MacroLex::handleSharpInclude()
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
    if (true == isOnceFile(fullPath))
    {
      return eLexNoError;
    }

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

bool MacroLex::isMacroExpending(const string& input)
{
	JZFUNC_BEGIN_LOG();
	if (mPreprocessingMacroSet.find(input) == mPreprocessingMacroSet.end())
	{
		return false;
	}
	return true;
}

bool MacroLex::isOnceFile(const string& input)
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

uint32 MacroLex::heartBeatForNormalWord(string& word)
{
	JZFUNC_BEGIN_LOG();
	uint32 ret = eLexNoError;

  JZWRITE_DEBUG("is macro:%d,is Stream useful %d",eFileTypeFile == mReaderStack.top().recordType,isLastStreamUseful());
  if(false == isLastStreamUseful())
  {
    return eLexNoError;
  }
  //not interpunction
  if ( "defined" == word && mReaderStack.top().recordType == eFileTypeMacro)
  {
    //this is defined in #if or #elif, we should add more test on this. I think I create a bug here
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
    ret = LexBase::heartBeatForNormalWord(word);
  }
	JZWRITE_DEBUG("ret is :%d",ret);
	return ret ;
}

uint32 MacroLex::expendMacro(const DefineRec* def,const RealParamList& paramList, string &ret)
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
								ret.erase(ret.size() - 1 );	
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
							ret.erase(ret.size() - 1);	
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


uint32 MacroLex::handleDefinedWord(const string& word)
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
  RealParamList paramList;
	if (true == defRec->isFuncLikeMacro)
	{
    MacroParamLex parmLexer;

    FileReaderRecord curFileRecord = popReaderRecord();
    parmLexer.pushReaderRecord(curFileRecord);
    uint32 parmLexerRet = parmLexer.doLex();
    FileReaderRecord afterLexRecord = parmLexer.popReaderRecord();
    pushReaderRecord(afterLexRecord);
    if (eLexNoError != parmLexerRet && eLexReachFileEnd != parmLexerRet && eLexParamAnalyzeOVer != parmLexerRet)
    {
      //this should be error;
      JZWRITE_DEBUG("return here")
      return parmLexerRet;
    }
    paramList = parmLexer.getParamList();
		//param number check
		if (defRec->isVarArgs)
		{
			if (defRec->paramMap.size() - 1 > paramList.size())
			{
				JZWRITE_DEBUG("var func like marco param not enough");
				return eLexFuncLikeMacroParamTooLess;
			}
		}
		else
		{
			if (defRec->paramMap.size() != paramList.size())
			{
				JZWRITE_DEBUG("Func like macro param not right and defRec->isVarArgs is: %d", defRec->isVarArgs);
				return defRec->paramMap.size() > paramList.size() ?
				eLexFuncLikeMacroParamTooLess : eLexFuncLikeMacroParamTooManay;
			}
		}
	}

	//not func like ,just expend it
	//use defineManager to expend it
	string expendStr = "";
	uint32 expendErr = expendMacro(defRec, paramList, expendStr);
	if (eLexNoError != expendErr)
	{
		JZFUNC_END_LOG();
		return expendErr;
	}
	
	char* buff = (char*)malloc(expendStr.size());
	strncpy(buff, expendStr.c_str(), expendStr.size());
	pushReaderRecordByParams(buff,expendStr.size(),word,eFileTypeMacroParam);

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

uint32 MacroLex::handleLeftSharpBracket()
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
      consumeChar(&nextChar);
			toSave += nextChar;
		}
	}
	uint32 endIndex = getLastIndex();
	saveWord(toSave,beginIndex, endIndex);
	return ret;
}

uint32 MacroLex::handleRightSharpBracket()
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

uint32 MacroLex::handleAnd()
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

uint32 MacroLex::handleEqual()
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

uint32 MacroLex::handleExclamation()
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

uint32 MacroLex::handleBar()
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

FileReaderRecord MacroLex::popReaderRecord()
{
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
  return LexBase::popReaderRecord();
}

void MacroLex::pushReaderRecord(FileReaderRecord record)
{
	switch(record.recordType)
	{
		case eFileTypeMacroParam:
			mPreprocessingMacroSet.insert(record.fileName);
			break;
		case eFileTypeFile:
			mPreprocessedFile.insert(record.fileName);
		default:
		{
			break;	
		}
	}
  LexBase::pushReaderRecord(record);
}

uint32 MacroLex::heartBeat()
{
  if(false == isLastStreamUseful())
  {
    char nextChar = 0;
    uint32 consumeRet = eLexNoError;
    while(eLexNoError == ( consumeRet = consumeChar(&nextChar)))
    {
      if(nextChar == '#')
      {
        uint32 ret = handleSharp();
        if(eLexNoError == ret)
        {
          return ret;
        }
        //else continue
      }
    }
    if(eLexNoError != consumeRet)
    {
      JZWRITE_ERROR("not end with correct macro");
    }
    return consumeRet;
  }
  else
  {
    return LexBase::heartBeat();
  }
}

void MacroLex::printLexRec()
{
  LexBase::printLexRec();
  mDefMgr.printAllDefine();
}
