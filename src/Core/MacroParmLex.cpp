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
    return eLexUnknowError;
  }
  if (1 == getBracketMarkStackSize())
  {
    //I think I get the stupid think here now. The logic is ok , but we should not get the parameter from the begin mark,
    //we should get parameter from a start mark
    //we will need to do it in this way
    //1, when the left bracket is pushed, init the left bracket, and init a paramter mark
    //2, meet a comma ,update the parameter mark
    //3, meet a right bracket, end the input.
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
	uint32 beginIndex = getLastIndex();
	JZFUNC_BEGIN_LOG();
  //now it is analyzing macro like func,do something
  JZWRITE_DEBUG("now push a mark");
  uint32 mark = mLexRecList.size();
  pushLeftBracket(mark);
  JZWRITE_DEBUG("push over");
//		if (1 == getBracketMarkStackSize())
//		{
//			JZWRITE_DEBUG("first bracket,don't save");
//			return eLexNoError;
//		}
	uint32 endIndex = getLastIndex();
	saveWord("(",beginIndex,endIndex);
	JZFUNC_END_LOG();
	return eLexNoError;
}
uint32 MacroParamLex::getBracketBeginMark()
{
	return mBracketMarkStack.top();
}

uint32 MacroParamLex::handleComma()
{
	uint32 commaBeginIndex = getLastIndex();
  if (1 == getBracketMarkStackSize())
  {
    //save it!
    //the logic is right but the parameter get logic is stupid,
    //pls follow the comment I made in handleRightBracket, to get the right logic about handling marco parameter
    return eLexNoError;
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
