#ifndef PREPROCESS_H
#define PREPROCESS_H

#include "LexicalAnalyzer.h"
#include "DefineManager.h"
#include <stack>

using namespace std;

struct LexReaderStruct
{
	LexReaderStruct()
	{
		curIndex = 0;
		lexPtr = NULL;
		expendingMacro = "";
	}
	string expendingMacro;
	int curIndex;
	LexicalAnalyzer* lexPtr;
};

class Preprocess {
public:
	Preprocess ();
	virtual ~Preprocess ();
	
	void init(LexicalAnalyzer* rootLex);

	void analyze();

private:

	//these two method may need to be abstrat as a father method
	const LexicalRecord* getNextRecord();

	//only get none empty record
	const LexicalRecord* getNextRecordAndSkipEmptyRecord();
	
	std::vector<LexicalRecord*> getLineRecordTillLineEnd();

	void pushLexReader(LexReaderStruct reader);

	void popLexReader();

private:
	//follow the private member functions
	//functions about to analyze the grama

//	handle #
	int handleSharp();

//handle #define
	int handleSharpDefine();

//handle #include
	int handleSharpInclude();

//handle line comment
	int handleLineComment();

//handle block comment
	int handleBlockComment();

//handle define
	int handleDefine(const LexicalRecord* recordPtr);

private:
	int32 getMacroParams(vector<LexRecordList>& ret);

private:
	//follow the private member variables
	
//	LexicalAnalyzer *mRootLex;
	set<string>	mExpendingMacroSet;
	stack<LexReaderStruct> mLexStack;
	DefineManager mDefinemanager;
	std::vector<LexicalRecord> mExpendedList;
};

#endif /* end of include guard: PREPROCESS_H */
