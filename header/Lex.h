#ifndef LEX_H
#define LEX_H

#include <vector>
#include <string>
#include <stack>
#include "LexData.h"
#include "JZCommonDefine.h"
#include "DefineManager.h"
using namespace std;

//class define
class Lex {
public:
	enum LexReturnNum
	{
		eLexUnknowError = -1,
		eLexNoError = 0,
		eLexReachFileEnd = 1,	
		eLexReaderStackEmpty = 2,
		eLexSharpFollowedNothing = 3,
		eLexWordNotMatch = 4,
		eLexAlreadyLastWord = 5,
		eLexSharpDefineFollowedNothing = 6,
		eLexUnexpectedSeperator = 7,
		eLexValParamNotLast = 8,
		eLexSharpIfdefFollowedWithNothing = 9,
	};
	enum LexInput
	{
		eLexSkipEmptyInput,
		eLexDontSkipEmptyInput,	
		eLexInOneLine,
		eLexInMulLine,
	};
	Lex ();
	virtual ~Lex ();
	void analyzeAFile(const string& fileName);

	void printLexRec();

private:
	//helper method
	void doLex();
	void saveWord(const string& input,uint32 recordType = eLexRecTypeNormal);
	void saveWordTo(const string& input, LexRecList& list,uint32 recordType = eLexRecTypeNormal);	

	void writeError(uint32 err);


	//consumor fun
	uint32 consumeChar(char *ret);
	uint32 readChar(char* ret);		//don't move cur index ptr
	uint32 consumeCharUntilReach(
			const char inputEnder, string *ret,
		   	LexInput inOneLine = eLexInMulLine);

	//alarm: this func don't care about seperator,so if you need to check seperator,check it by yourself
	uint32 tryToMatchWord(const string& word);

	//alarm: this only useful for a word,in a file.
	//if that file is poped,this func can do nothing on it
	uint32 undoConsume();
	uint32 consumeWord(
			string &retStr,char &retSeperator,
			LexInput skipEmptyInput = eLexSkipEmptyInput,
		   	LexInput inOneLine = eLexInMulLine);

public:
	//handler function
	uint32 handleSingleQuotation();
	uint32 handleDoubleQuotation();
	uint32 handleSharp();
	uint32 handleSlant();

	uint32 handleCommentLine();
	uint32 handleCommentBlock();
	uint32 handleSharpDefine();
	uint32 handleSharpIf();
	uint32 handleSharpIfdef();
	uint32 handleSharpInclude();

private:

	stack<FileReaderRecord> mReaderStack;	//no so sure if I need this
	stack<PrecompileSelector> mPSStack;
	LexRecList mLexRecList;
	DefineManager mDefMgr;
};

namespace LexUtil {
	bool isInterpunction(const char input);
	bool isLineEnder(const char input);
	bool isEmptyInput(const char input);
	bool isBackSlant(const char input);
	bool isEmptyInput(const string& input);
	bool isEndWithBackSlant(const string& input);
	char seperatorMatcher(const char input);
	string eatLREmptyInput(const string &toBeEatan);
} /* LexUtil */

typedef uint32 (Lex::*LexPatternHandler)();

class LexPatternTable {
public:
	static LexPatternTable* getInstance();
	virtual ~LexPatternTable ();

	LexPatternHandler getPattern(const char input);

private:

	void init();
	LexPatternTable ();
	map<char,LexPatternHandler> mPatternHandlerMap;
};
#define LexPtnTbl LexPatternTable::getInstance()

#endif /* end of include guard: LEX_H */
