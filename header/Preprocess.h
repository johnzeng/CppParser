#ifndef PREPROCESS_H
#define PREPROCESS_H

#include "LexicalAnalyzer.h"
class Preprocess {
public:
	Preprocess ();
	virtual ~Preprocess ();
	
	void init(LexicalAnalyzer* rootLex);

	void analyze();

private:

	LexicalAnalyzer *mRootLex;
	DefineManager mDefinemanager;
	std::vector<LexicalRecord> mExpendedList;
};

#endif /* end of include guard: PREPROCESS_H */
