#include "LexBase.h"
#include "JZLogger.h"

LexBase::LexBase()
{
}
LexBase::~LexBase()
{
}

uint32 LexBase::analyzeAFile(const string& fileName)
{
  return 0;
}

LexRecList LexBase::getRecList()
{
  return mLexRecList;
}

void LexBase::printLexRec()
{
	JZFUNC_BEGIN_LOG();
	auto it = mLexRecList.begin();
	string line = "";
	uint32 curLine = 0;
	JZWRITE_DEBUG("now print lex");
	for(; it != mLexRecList.end(); it++)
	{
		if (it->line == curLine)
		{
			line += " ";
			line += it->word;
		}
		else
		{
			printf("%s\n",line.c_str());
			line = it->word;
			curLine = it->line;
		}
	}
	printf("%s\n",line.c_str());

	JZFUNC_END_LOG();
}
