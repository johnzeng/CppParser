#include "CommandLineFactorMngr.h"
#include "JZFileUtil.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
CommandLineFactorMngr::CommandLineFactorMngr()
{
	
}

void CommandLineFactorMngr::searchAddonFactorFile()
{
	string curPath = JZGetCurrentWorkingPath();
	string addonFilePath = JZTryToSearchFileUntilRoot(curPath.c_str(), DEFAULT_COMMON_LINE_FACTOR_FILE_NAME);
	mAddonFactorDefineFile = addonFilePath;
}


void CommandLineFactorMngr::setAddonFactorFileDirectory(string path)
{
	if(0 == access(path.c_str(), F_OK))
	{
		mAddonFactorDefineFile = path;
	}
}

uint32 CommandLineFactorMngr::addonFactorAnalyser()
{
}


