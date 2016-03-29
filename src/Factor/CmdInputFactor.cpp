#include "CmdInputFactor.h"
#include "JZFileUtil.h"
#include <fcntl.h>
#include <unistd.h>
#include "JZLogger.h"

CmdInputFactor::CmdInputFactor()
{
	mCurIndex = 0;
}
CmdInputFactor* CmdInputFactor::getInstance()
{
	static CmdInputFactor* instance = NULL;
	if(NULL == instance)
	{
		instance = new CmdInputFactor();	
	}
	return instance;
}

void CmdInputFactor::init()
{
	registerNonOptHandler((NonOptHandlerPtr)&CmdInputFactor::nonOptHandler);
}

void CmdInputFactor::nonOptHandler(const string& args)
{
	if(-1 == access(args.c_str(), F_OK))
	{
		return ;
	}
	JZWRITE_DEBUG("get file name : %s", args.c_str());
	mToCompileFile.push_back(args);
}

string CmdInputFactor::getNextFile()
{
	string ret = "";
	if(mCurIndex < mToCompileFile.size())
	{
		ret = mToCompileFile[mCurIndex];
	}
	else
	{
		JZWRITE_DEBUG("no more to compile file");
	}
	mCurIndex++;
	return ret;
}
