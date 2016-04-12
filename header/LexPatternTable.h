#ifndef LEXPATTERNTABLE_H
#define LEXPATTERNTABLE_H

#include "JZCommonDefine.h"
using namespace std;
class LexBase;

typedef uint32 (LexBase::*LexPatternHandler)();

class LexPatternTable {
public:
	virtual ~LexPatternTable ();
	LexPatternTable ();

	LexPatternHandler getPattern(const char input);
	LexPatternHandler getMacroPattern(const string& input);

  void insertPattern(const char input, const LexPatternHandler);
  void insertPattern(const string input, const LexPatternHandler);

private:

	map<char,LexPatternHandler> mPatternHandlerMap;
	map<string ,LexPatternHandler> mStrPatternHandlerMap;
};


#endif /* end of include guard: LEXPATTERNTABLE_H */
