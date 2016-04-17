#include "MacroParamLex.h"
#include "JZLogger.h"

MacroParamLex::MacroParamLex()
{
	mPatternTable->insertPattern('(',  (LexPatternHandler)(&MacroParamLex::handleLeftBracket));
	mPatternTable->insertPattern(')',  (LexPatternHandler)(&MacroParamLex::handleRightBracket));
	mPatternTable->insertPattern(',',  (LexPatternHandler)(&MacroParamLex::handleComma));
}

MacroParamLex::~MacroParamLex()
{
}

uint32 MacroParamLex::handleRightBracket()
{
	JZFUNC_BEGIN_LOG();
	uint32 bracketBeginIndex = getLastIndex();
  JZWRITE_DEBUG("now pop a mark");

  if (0 == getBracketMarkStackSize())
  {
    JZFUNC_END_LOG();
    JZWRITE_DEBUG("get right ) when there is no left bracket");
    return eLexUnknowError;
  }
  if (1 == getBracketMarkStackSize())
  {
    pushIntoParamList();
  }
  popLeftBracket();
  if (0 == getBracketMarkStackSize())
  {
    JZFUNC_END_LOG();
    return eLexParamAnalyzeOVer;
  }

	uint32 bracketEndIndex = getLastIndex();
	saveWord(")", bracketBeginIndex, bracketEndIndex);
	JZFUNC_END_LOG();
	return eLexNoError;
}

uint32 MacroParamLex::handleLeftBracket()
{
	JZFUNC_BEGIN_LOG();

	uint32 beginIndex = getLastIndex();
  //now it is analyzing macro like func,do something
  uint32 mark = mLexRecList.size();
  pushLeftBracket(mark);

	uint32 endIndex = getLastIndex();
	saveWord("(",beginIndex,endIndex);

	JZFUNC_END_LOG();
	return eLexNoError;
}

uint32 MacroParamLex::getParamSepMark()
{
	return mParamSepStack.top();
}

uint32 MacroParamLex::handleComma()
{
	uint32 commaBeginIndex = getLastIndex();
  if (1 == getBracketMarkStackSize())
  {
    pushIntoParamList();
  }
	
	uint32 commaEndIndex = getLastIndex();
	saveWord(",",commaBeginIndex,commaEndIndex);
	return eLexNoError;
}

uint32 MacroParamLex::getBracketMarkStackSize()
{
	return mBracketMarkStack.size();
}

void MacroParamLex::pushLeftBracket(uint32 mark)
{
  if(0 == mBracketMarkStack.size())
  {
    mParamSepStack.push(mark);
  }
	mBracketMarkStack.push(mark);
}

void MacroParamLex::popLeftBracket()
{
	mBracketMarkStack.pop();
}

RealParamList MacroParamLex::getParamList()
{
  return mRealParamList;
}

void MacroParamLex::pushIntoParamList()
{
  int lastMark = getParamSepMark();
  LexRec beginRec = mLexRecList[lastMark];
  uint32 beginIndex = beginRec.beginIndex;
  uint32 endIndex = getLastIndex();
  string param = "";
  for(int i = beginIndex + 1; i < endIndex; i++)
  {
    param += mReaderStack.top().buffer[i];
  }
  mRealParamList.push_back(param);
  mParamSepStack.push(mLexRecList.size());
}
