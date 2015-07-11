#include "Lex.h"
#include "JZFileUtil.h"
#include "JZLogger.h"

/*********************************************************
	Lex begin here 
 ********************************************************/



Lex::Lex()
{
}

Lex::~Lex()
{

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
	char input = 0;
	uint32 ret;
	string curWord = "";
	while (eLexNoError == (ret = consumeChar(&input)))
	{
		LexPatternHandler handler = LexPtnTbl->getPattern(input);
		if (NULL == handler)
		{
			//no handler is registered,means this is a normal input
			if (false == LexUtil::isInterpunction(input))
			{
				curWord += input;
			}
			else
			{
				saveWord(curWord);
				if (false == LexUtil::isEmptyInput(input))
				{
					curWord = "";
					curWord += input;
					saveWord(curWord);
				}
				curWord = "";
			}
		}
		else
		{
			saveWord(curWord);
			curWord = "";
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

void Lex::writeError(uint32 err)
{
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

void Lex::saveWord(const string& input)
{
	LexRec rec;
	rec.word = input;
	rec.line = mReaderStack.top().curLineNum;
	rec.file = mReaderStack.top().fileName;
	mLexRecList.push_back(rec);
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
		case '\t':
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
		case ' ':
			return true;
		default:
		{
			return false;
		}
	}
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
