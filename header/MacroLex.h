#ifndef MACROLEX_H
#define MACROLEX_H

#include <vector>
#include <string>
#include <stack>
#include "JZCommonDefine.h"
#include "LexBase.h"
#include "DefineManager.h"
#include "LexUtil.h"

class MacroLex:public LexBase {
public:
  MacroLex();
  virtual ~MacroLex();

	virtual void printLexRec();

	//handler function
	uint32 handleSharp();					//#
	uint32 handleBar();						//|
	uint32 handleAnd();						//&
	uint32 handleLeftSharpBracket();		//<
	uint32 handleRightSharpBracket();		//>
	uint32 handleExclamation();				//!
	uint32 handleEqual();					//=

	//should be part of macro param handler
  
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

	//if seperator is '(',and word is a func like macro,then seperator will change to ' '
	uint32 handleDefinedWord(const string& word);
	uint32 handleIsDefined(string& ret);
	uint32 checkMacro(bool *isSuccess,uint32 checkMark);
	uint32 expendMacroParam(const string& word);

	//if not param ,return NULL;
	const string* getRealParam(const string& word);

	uint32 expendMacro(const DefineRec* def,const RealParamList& paramList, string& ret);
	
	vector<PrecompileSelector>& getTopPSStack();
protected:
  virtual uint32 heartBeat();
	virtual uint32 heartBeatForNormalWord(string& word );

  virtual void pushReaderRecord(FileReaderRecord record);

	virtual FileReaderRecord popReaderRecord();

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

//	void turnOnFuncLikeMacroMode();
//	void turnOffFuncLikeMacroMode();
	void pushDefParam(const DefineRec *rec,const ParamSiteMap *indexMap);
	void popDefParam();
//	bool isFuncLikeMacroMode();

private:
	StringSet mOnceFileSet;
	StringSet mPreprocessingMacroSet;

	DefineManager mDefMgr;
};

#endif /* end of include guard: MACROLEX_H */
