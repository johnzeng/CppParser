#ifndef LEX_H
#define LEX_H

#include <vector>
#include <string>
#include <stack>
#include "JZCommonDefine.h"
#include "DefineManager.h"
#include "LexUtil.h"
#include "LexBase.h"

using namespace std;

//class define
class Lex:public LexBase {

public:
	virtual uint32 analyzeAFile(const string& fileName);
  Lex();
private:

	uint32 doLex();

	void writeError(uint32 err);

	// isSuccess is useless when word == "#else" or word == "#endif"
	uint32 pushPrecompileStreamControlWord(uint32 word, bool isSuccess = true);
	bool isLastStreamUseful();
	bool isLastMacroSuccess();
	void turnOffCompileStream(uint32 tag);
	void turnOnCompileStream();
	uint32 getCompileStream();

	uint32 isMacroSuccess(const LexRecList& logic,bool *ret);
	bool isMacroExpending(const string& input);
	bool isOnceFile(const string& input);

	void turnOnFuncLikeMacroMode();
	void turnOffFuncLikeMacroMode();
	void pushDefParam(const DefineRec *rec,const ParamSiteMap *indexMap);
	void popDefParam();
	bool isFuncLikeMacroMode();
	uint32 getBracketMarkStackSize();
	void pushLeftBracket(uint32 mark);
	void popLeftBracket();
	uint32 getBracketBeginMark();

	void popErrorSite();
public:
	//handler function
	uint32 handleSharp();					//#
	uint32 handleSlant();					// /
	uint32 handleBar();						//|
	uint32 handleLeftSharpBracket();		//<
	uint32 handleRightSharpBracket();		//>
	uint32 handlePoint();					//.
	uint32 handleAnd();						//&
	uint32 handleEqual();					//=
	uint32 handleStart();					//*
	uint32 handleExclamation();				//!
	uint32 handlePlus();					//+
	uint32 handleMinus();					//-
	uint32 handleUpponSharp();				//^
	uint32 handleWave();					//~
	uint32 handleLeftBracket();				//(
	uint32 handleRightBracket();			//)
	uint32 handleComma();					//,
	uint32 handleMod();						//%
	uint32 handleColon();					//:

	//macro handler
	uint32 handleSharpDefine();
	uint32 handleSharpIf();
	uint32 handleSharpElif();
	uint32 handleSharpIfdef();
	uint32 handleSharpIfndef();
	uint32 handleSharpInclude();
	uint32 handleSharpEndIf();
	uint32 handleSharpElse();
	uint32 handleSharpPragma();
	uint32 handleSharpWarning();
	uint32 handleSharpError();

private:
	//if seperator is '(',and word is a func like macro,then seperator will change to ' '
	uint32 handleDefinedWord(const string& word);
	uint32 handleIsDefined(string& ret);
	uint32 checkMacro(bool *isSuccess,uint32 checkMark);
	uint32 expendMacroParam(const string& word);

	//if not param ,return NULL;
	const string* getRealParam(const string& word);

	uint32 expendMacro(const DefineRec* def,const RealParamList& paramList, string& ret);
	
	vector<PrecompileSelector>& getTopPSStack();
private:
	RealParamList mRealParamList;
	StringSet mOnceFileSet;
	StringSet mPreprocessingMacroSet;

	DefineManager mDefMgr;
};


#endif /* end of include guard: LEX_H */
