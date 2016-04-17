#ifndef MACROPARMLEX_H
#define MACROPARMLEX_H

#include "LexBase.h"

class MacroParamLex:public LexBase {
public:
  MacroParamLex ();
  virtual ~MacroParamLex ();

	uint32 handleLeftBracket();				//(
	uint32 handleRightBracket();			//)
	uint32 handleComma();					//,

  RealParamList getParamList();

private:
	void pushLeftBracket(uint32 mark);
	void popLeftBracket();
	uint32 getBracketMarkStackSize();
	uint32 getBracketBeginMark();

private:
	BracketMarkStack mBracketMarkStack;
	RealParamList mRealParamList;
};

#endif /* end of include guard: MACROPARMLEX_H */
