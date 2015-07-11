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
} ;

struct FileReaderRecord
{
	const char* buffer;
	int bufferSize;		//should not be change after init
	int curIndex;
	int curLineNum;
	string fileName;

} ;
typedef vector<LexRec> LexRecList;

//class define
class Lex {
public:
	enum LexReturnNum
	{
		eLexNoError = 0,
		eLexReachFileEnd = 1,	
		eLexReaderStackEmpty = 2,
	};
	Lex ();
	virtual ~Lex ();
	void analyzeAFile(const string& fileName);

private:
	//helper method
	uint32 consumeChar(char *ret);
	void doLex();
	void saveWord(const string& input);
	void writeError(uint32 err);

private:
	stack<FileReaderRecord> mReaderStack;	//no so sure if I need this
	LexRecList mLexRecList;
	DefineManager* mDefMgr;
};

namespace LexUtil {
	bool isInterpunction(const char input);
	bool isLineEnder(const char input);
	bool isEmptyInput(const char input);
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
