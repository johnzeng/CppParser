#include "LexPatternTable.h"
#include "LexBase.h"

LexPatternTable::LexPatternTable()
{
}

LexPatternTable::~LexPatternTable()
{
	mPatternHandlerMap.clear();
  mStrPatternHandlerMap.clear();
}

void LexPatternTable::insertPattern(const char input, const LexPatternHandler handler)
{
  mPatternHandlerMap[input] = handler;
}

void LexPatternTable::insertPattern(const string input, const LexPatternHandler handler)
{
  mStrPatternHandlerMap[input] = handler;
}

LexPatternHandler LexPatternTable::getMacroPattern(const string& input)
{
	if (mStrPatternHandlerMap.find(input) != mStrPatternHandlerMap.end())
	{
		return mStrPatternHandlerMap[input];
	}
	else
	{
		return NULL;	
	}

}

LexPatternHandler LexPatternTable::getPattern(const char input)
{
	if (mPatternHandlerMap.find(input) != mPatternHandlerMap.end())
	{
		return mPatternHandlerMap[input];
	}
	else
	{
		return NULL;	
	}
}


