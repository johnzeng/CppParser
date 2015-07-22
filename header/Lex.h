#ifndef LEX_H
#define LEX_H

#include <vector>
#include <string>
#include <stack>
#include "JZCommonDefine.h"
using namespace std;

//pre define
class DefineManager;

//struct define
struct LexRec
{
	string word;
	int line;
	string file;
	uint32 type;
} ;

enum LexRecordType
{
	eLexRecTypeNormal,
	eLexRecTypeConstChar,
	eLexRecTypeString,
	eLexRecTypeFuncLikeMacroParam,
	eLexRecTypeFuncLikeMacroVarParam,
};

struct FileReaderRecord
{
	const char* buffer;
	int bufferSize;		//should not be change after init
	int curIndex;
	int curLineNum;
	string fileName;	//if this is a define ,file name will be key

} ;
typedef vector<LexRec> LexRecList;

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
	};
	enum LexInput
	{
		eLexSkipEmptyInput,
		eLexDontSkipEmptyInput,	
	};
	Lex ();
	virtual ~Lex ();
	void analyzeAFile(const string& fileName);

	void printLexRec();

private:
	//helper method
	void doLex();
	uint32 consumeWord(string &retStr,char &retSeperator,LexInput skipEmptyInput = eLexSkipEmptyInput);
	void saveWord(const string& input,uint32 recordType = eLexRecTypeNormal);
	void saveWordTo(const string& input, LexRecList& list,uint32 recordType = eLexRecTypeNormal);	
	void writeError(uint32 err);


	//consumor fun
	uint32 consumeChar(char *ret);
	uint32 readChar(char* ret);		//don't move cur index ptr
	uint32 consumeCharUntilReach(const char inputEnder, string *ret);
	uint32 tryToMatchWord(const string& word);
	uint32 undoConsume();

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
	uint32 handleSharpInclude();

private:
	stack<FileReaderRecord> mReaderStack;	//no so sure if I need this
	LexRecList mLexRecList;
	DefineManager* mDefMgr;
};

namespace LexUtil {
	bool isInterpunction(const char input);
	bool isLineEnder(const char input);
	bool isEmptyInput(const char input);
	bool isBackSlant(const char input);
	bool isEmptyInput(const string& input);
	bool isEndWithBackSlant(const string& input);
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
