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
	}
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

//	LexicalAnalyzer *mRootLex;
	stack<LexReaderStruct> mLexStack;
	DefineManager mDefinemanager;
	std::vector<LexicalRecord> mExpendedList;
};

#endif /* end of include guard: PREPROCESS_H */
