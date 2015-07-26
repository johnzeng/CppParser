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
		eLexUnmatchMacro = 10,
		eLexUnknowMacro = 11,
		eLexSharpEndIfFollowWithOtherThing = 12,
		eLexSharpElseFollowWithOtherThing = 13,


		//unknow should be last
		eLexUnknowError ,
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

	//alarm: this only useful for a char,in a file.
	//if that file is poped,this func can do nothing on it
	uint32 undoConsume();

	uint32 consumeWord(
			string &retStr,char &retSeperator,
			LexInput skipEmptyInput = eLexSkipEmptyInput,
		   	LexInput inOneLine = eLexInMulLine);

	// isSuccess is useless when word == "#else" or word == "#endif"
	uint32 pushPrecompileStreamControlWord(uint32 word, bool isSuccess = true);
	bool isLastStreamUseful();
	bool isLastMacroSuccess();
	void turnOffCompileStream(uint32 tag);
	void turnOnCompileStream();
	uint32 getCompileStream();

	uint32 isMacroSuccess(const LexRecList& logic,bool *ret);

public:
	//handler function
	uint32 handleSingleQuotation();
	uint32 handleDoubleQuotation();
	uint32 handleSharp();
	uint32 handleSlant();

	uint32 handleCommentLine();
	uint32 handleCommentBlock();

	//macro handler
	uint32 handleSharpDefine();
	uint32 handleSharpIf();
	uint32 handleSharpIfdef();
	uint32 handleSharpInclude();
	uint32 handleSharpEndIf();
	uint32 handleSharpElse();

private:
	
	stack<FileReaderRecord> mReaderStack;	//no so sure if I need this
	vector<PrecompileSelector> mPSStack;
	LexRecList mLexRecList;
	DefineManager mDefMgr;
};

namespace LexUtil {
	char* eraseLineSeperator(const char* input,uint64 *bufSize);
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
	LexPatternHandler getMacroPattern(const string& input);

private:

	void init();
	LexPatternTable ();
	map<char,LexPatternHandler> mPatternHandlerMap;
	map<string ,LexPatternHandler> mMacroPatternHandlerMap;
};
#define LexPtnTbl LexPatternTable::getInstance()

#endif /* end of include guard: LEX_H */
