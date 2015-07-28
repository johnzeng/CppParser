#ifndef GRAMAUTIL_H
#define GRAMAUTIL_H

#include "JZCommonDefine.h"
using namespace std;

class GrammarUtil {
public:
	GrammarUtil ();
	virtual ~GrammarUtil ();

	void init();
	static GrammarUtil* getInstance();

	bool isOperator(const string& input);
	bool isPreprocessOperator(const string& input);

private:
	StringSet mOperatorSet;
	StringSet mPreprocessOperatorSet;
	StringSet mKeyWordSet;
};
#endif /* end of include guard: GRAMAUTIL_H */
