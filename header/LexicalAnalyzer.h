#ifndef LEXICALANALYZER_H

#define LEXICALANALYZER_H

#include "JZCommonDefine.h"
#include <stack>
using namespace std;
class LexicalAnalyzer {
public:
	LexicalAnalyzer ();
	virtual ~LexicalAnalyzer ();

private:
	StringStack mOriginalWords;
	
};

#endif /* end of include guard: LEXICALANALYZER_H */
