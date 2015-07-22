#include "Lex.h"
#include "DefineManager.h"
#include "JZFileUtil.h"
#include "JZLogger.h"
#include "JZMacroFunc.h"
/*********************************************************
	Lex begin here 
 ********************************************************/



Lex::Lex()
{
	mDefMgr = new DefineManager();
}

Lex::~Lex()
{
	JZSAFE_DELETE(mDefMgr);
}

void Lex::analyzeAFile(const string& fileName)
{
	uint64 bufSize;
	unsigned char* buff = JZGetFileData(fileName.c_str(), &bufSize);

	FileReaderRecord fileRecord;
	fileRecord.buffer = (const char*)buff;
	fileRecord.bufferSize = bufSize;
	fileRecord.curIndex = 0;
	fileRecord.fileName = fileName;
	fileRecord.curLineNum = 1;

	mReaderStack.push(fileRecord);
	doLex();
}

void Lex::doLex()
{
	uint32 ret;
	char seperator;
	string word;
	while(eLexNoError == (consumeWord(word, seperator)))
	{
		if ("" != word)
		{
			saveWord(word);
		}
		LexPatternHandler handler = LexPtnTbl->getPattern(seperator);
		if (NULL == handler)
		{
			//no handler is registered,means this is a normal input
			if (true == LexUtil::isEmptyInput(seperator))
			{
				continue;
			}
			string toSaveSeperator = "";
			toSaveSeperator += seperator;
			saveWord(toSaveSeperator);
		}
		else
		{
			uint32 err = eLexNoError;
			err = (this->*handler)();
			if (err != eLexNoError)
			{
				writeError(err);
				return;
			}
		}
	}
}

uint32 Lex::consumeWord(string &retStr,char &retSeperator,LexInput skipEmptyInput)
{
	char input = 0;
	uint32 ret;
	string curWord = "";
	while (eLexNoError == (ret = consumeChar(&input)))
	{
		if (false == LexUtil::isInterpunction(input))
		{
			curWord += input;
		}
		else
		{
			if (eLexSkipEmptyInput == skipEmptyInput && 
				true == LexUtil::isEmptyInput(input) &&
			   	"" == curWord)
			{
				continue;
			}
			retStr = curWord;
			retSeperator = input;
			return eLexNoError;
		}
			
	}
	return ret;
}

void Lex::writeError(uint32 err)
{
	JZWRITE_DEBUG("err id : %d",err);
	switch(err)
	{
		case eLexReachFileEnd:
		{
			JZWRITE_DEBUG("reach file end");
			break;	
		}
		case eLexReaderStackEmpty:
		{
			JZWRITE_DEBUG("stack end");
			break;	
		}
		default:
		{
			break;	
		}
	}
}

void Lex::saveWordTo(const string& input,LexRecList& list, uint32 recordType)
{
	if (true == LexUtil::isEmptyInput(input))
	{
		//don't add empty input
		return;
	}
	LexRec rec;
	rec.word = input;
	rec.line = mReaderStack.top().curLineNum;
	rec.file = mReaderStack.top().fileName;
	rec.type = recordType;
	list.push_back(rec);
}

void Lex::saveWord(const string& input, uint32 recordType)
{
	saveWordTo(input,mLexRecList, recordType);
}

void Lex::printLexRec()
{
	auto it = mLexRecList.begin();
	for(; it != mLexRecList.end(); it++)
	{
		JZWRITE_DEBUG("line:%d,word:%s",it->line, it->word.c_str());
	}
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
		JZSAFE_DELETE(mReaderStack.top().buffer);
		mReaderStack.pop();
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

/*********************************************************
	when handling ' and ", I simply ignore the \ problem
    I just find the next quotation,so some error will not
	happen in my compilor	
 ********************************************************/

uint32 Lex::handleSingleQuotation()
{
	JZFUNC_BEGIN_LOG();

	//when you enter this func,you have already get a ',so
	string toSave = "'";
	uint32 toSaveLen = 0;
	do
	{
		string retStr = "";
		uint32 retErr = eLexNoError;
		
		retErr = consumeCharUntilReach('\'',&retStr);
		if (eLexNoError != retErr)
		{
			JZFUNC_END_LOG();
			return retErr;
		}
		JZWRITE_DEBUG("ret str is : %s",retStr.c_str());
		toSave += retStr;
		toSaveLen = toSave.size();
	}while(toSaveLen > 2 && true == LexUtil::isBackSlant(toSave[toSaveLen - 2]));

//	handle const char input
//	...
	saveWord(toSave, eLexRecTypeConstChar);

	JZFUNC_END_LOG();
	return eLexNoError;
}

uint32 Lex::handleDoubleQuotation()
{
	JZFUNC_BEGIN_LOG();

	//when you enter this func,you have already get a ",so init a " here
	string toSave = "\"";
	uint32 toSaveLen = 0;
	do
	{
		string retStr = "";
		uint32 retErr = eLexNoError;
		retErr = consumeCharUntilReach('"',&retStr);
		if (eLexNoError != retErr)
		{
			JZFUNC_END_LOG();
			return retErr;
		}
		toSave += retStr;
		toSaveLen = toSave.size();
	}while(toSaveLen > 2 && true == LexUtil::isBackSlant(toSave[toSaveLen - 2]));

//	handle const char input
//	...
	saveWord(toSave, eLexRecTypeConstChar);

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
		return eLexReachFileEnd;
	}

	*ret = record.buffer[record.curIndex];

	return eLexNoError;

}
uint32 Lex::handleSlant()
{
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
			string toSave = "/";
			saveWord(toSave);
			break;	
		}
	}

	JZFUNC_END_LOG();
	return eLexNoError;
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
	//hard proble now begin!
	
	//read empty input
	uint32 ret = eLexNoError;
	char nextChar = 0;
	while ((ret = consumeChar(&nextChar)) == eLexNoError)
	{
		if (false == LexUtil::isEmptyInput(nextChar))
		{
			break;
		}
		if (true == LexUtil::isLineEnder(nextChar))
		{
			return eLexSharpFollowedNothing;
		}

		//else continue;
	}
	
	switch(nextChar)
	{
		case 'd':
		{
			//define ,but d is read,so efine
			if (eLexNoError == tryToMatchWord("efine"))
			{
				return handleSharpDefine();
			}
			break;
		}
		case 'i':
		{
			if (eLexNoError == tryToMatchWord("f"))
			{
				//#if
				return handleSharpIf();
			}
			else if(eLexNoError == tryToMatchWord("nclude"))
			{
				//#include
				return handleSharpInclude();	
			}
			break;	
		}
		default:
		{
			break;	
		}
	}
	return eLexNoError;
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

	ret = consumeWord(key, seperator);
	if (eLexNoError != ret)
	{
		return ret;
	}

	DefineRec defineRec;
	defineRec.key = key;
	if ('(' == seperator)
	{
		defineRec.isFuncLikeMacro = true;
		//read format parm
		string param;
		char seperator;
		uint32 paramRet;
		while(eLexNoError == (paramRet = consumeWord(param,seperator)))
		{
			if (param != "" && (seperator == ')' || seperator == ','))
			{
				JZWRITE_DEBUG("save a param :%s",param.c_str());
				if (defineRec.formalParam.size() > 0 &&
				    defineRec.formalParam.back().type == eLexRecTypeFuncLikeMacroVarParam )
				{
					//should not save more param
					JZWRITE_DEBUG("val param end with not val param:%s",param.c_str());
					return eLexValParamNotLast;
				}
				saveWordTo(param ,defineRec.formalParam, eLexRecTypeFuncLikeMacroParam);
			}
			if (seperator == '.')
			{
				if (eLexNoError == tryToMatchWord(".."))
				{
					defineRec.isVarArgs = true;
					if ("" != param)
					{
						saveWordTo(param,defineRec.formalParam,eLexRecTypeFuncLikeMacroVarParam);
					}
				}
				else
				{
					JZWRITE_DEBUG("reach here");
					return eLexUnexpectedSeperator;
				}
				continue;
			}
			if (seperator == ')')
			{
				//reach end
				break;
			}
			if (seperator != ',')
			{
				JZWRITE_DEBUG("reach here,seperator is : %c",seperator);
				return eLexUnexpectedSeperator;			
			}
		}
	}
	else
	{
		defineRec.isVarArgs = false;
		defineRec.isFuncLikeMacro =false;
	}

	string defWord = "";
	do
	{
		string retStr = "";
		uint32 retErr = eLexNoError;
		
		retErr = consumeCharUntilReach('\n',&retStr);
		if (eLexNoError != retErr)
		{
			JZFUNC_END_LOG();
			return retErr;
		}
		defWord += retStr;
	}while(true == LexUtil::isEndWithBackSlant(defWord));

	if (defWord.back() == '\n')
	{
		defWord = defWord.substr(0,defWord.size() - 1);
	}

	defineRec.defineStr = defWord;
	mDefMgr->addDefineMap(key, defineRec);	
	JZFUNC_END_LOG();
	return ret;
}

uint32 Lex::handleSharpIf()
{
	return eLexNoError;
}

uint32 Lex::handleSharpInclude()
{
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

uint32 Lex::consumeCharUntilReach(const char inputEnder, string *ret)
{
	if (NULL == ret)
	{
		return eLexUnknowError;
	}
	*ret = "";
	char nextInput;
	uint32 readRet = eLexNoError;
	while ((readRet = consumeChar(&nextInput)) == eLexNoError)
	{
		*ret += nextInput;
		if (nextInput == inputEnder)
		{
			break;
		}
	}
	return readRet;
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
	switch(input)
	{
		case '|':
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
		case '.':
		case '!':
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
	mPatternHandlerMap['"'] = &Lex::handleDoubleQuotation;
	mPatternHandlerMap['#'] = &Lex::handleSharp;
	mPatternHandlerMap['/'] = &Lex::handleSlant;
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
