#ifndef LEXBASE_H
#define LEXBASE_H

#include "LexData.h"
class LexBase{
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
		eLexWordIsNotDefined = 14,
		eLexParamAnalyzeOVer = 15,	//this is not an error
		eLexReachLineEnd = 16,
		eLexFuncLikeMacroParamTooLess = 17,
		eLexFuncLikeMacroParamTooManay = 18,
		eLexCanNotPushPrecompileStream = 19,
		eLexMacroIsAlreadyExpending = 20,
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
	virtual uint32 analyzeAFile(const string& fileName);

	void printLexRec();

  LexRecList getRecList();
protected:
	LexRecList mLexRecList;
};

#endif /* end of include guard: LEXBASE_H */
