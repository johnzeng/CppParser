#ifndef LEXBASE_H
#define LEXBASE_H

#include "LexData.h"
#include "LexPatternTable.h"

class LexBase{
public:
  // I am thinking about reorder these codes, Some of them should be legal return
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
		eLexWordIsNotDefined = 14,
		eLexParamAnalyzeOVer = 15,	//this is not an error
		eLexReachLineEnd = 16,
		eLexFuncLikeMacroParamTooLess = 17,
		eLexFuncLikeMacroParamTooManay = 18,
		eLexCanNotPushPrecompileStream = 19,
		eLexMacroIsAlreadyExpending = 20,

    eLexNormalWord = 21,
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
	LexBase ();
	virtual ~LexBase ();

	void printLexRec();

	uint32 analyzeAFile(const string& fileName);

  LexRecList getRecList();
public:
  // some basic handler
	uint32 handleSingleQuotation();  		//"
	uint32 handleDoubleQuotation();			//'
  uint32 doLex();

  virtual void pushReaderRecord(FileReaderRecord record);

	virtual void pushReaderRecordByParams(const char* buff,uint64 size,const string& fileName,uint32 recordType);

	FileReaderRecord popReaderRecord();

protected:

	virtual uint32 heartBeat(string& word );

	virtual uint32 heartBeatForNormalWord(string& word );

	void writeError(uint32 err);

	uint32 readChar(char* ret);		//don't move cur index ptr

	//consumor fun
	uint32 consumeChar(char *ret);
	uint32 consumeCharUntilReach(
			const char inputEnder, string *ret,
		   	LexInput inOneLine = eLexInMulLine);

	//alarm: this func don't care about seperator,so if you need to check seperator,check it by yourself
	uint32 tryToMatchWord(const string& word);

	//alarm: this only useful for a char,in a file.
	//if that file is poped,this func can do nothing on it
	uint32 undoConsume();

	uint32 consumeWord(
			string &retStr,
			LexInput skipEmptyInput = eLexSkipEmptyInput,
		   	LexInput inOneLine = eLexInMulLine);


	uint32 getLastIndex();

	void saveWord(const string& input,uint32 beginIndex, uint32 endIndex,uint32 recordType = eLexRecTypeNormal);
	void saveWordTo(const string& input, LexRecList& list,uint32 beginIndex, uint32 endIndex, uint32 recordType = eLexRecTypeNormal);	

  FileReaderRecord initFileRecord(
		const char* buff,uint64 size,const string& fileName,
		uint32 recordType);

	void popErrorSite();
protected:
  LexPatternTable* mPatternTable;
	StringSet mPreprocessedFile;
	LexRecList mLexRecList;
	stack<FileReaderRecord> mReaderStack;	
};

#endif /* end of include guard: LEXBASE_H */
