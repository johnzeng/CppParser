#include "CommandLineFactorMngr.h"
#include "JZFileUtil.h"
#include "ErrorCode.h"
#include "JZLogger.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
using namespace std;
using namespace JZErrorCode;



CommandLineFactorMngr::CommandLineFactorMngr()
{
	
}
CommandLineFactorMngr* CommandLineFactorMngr::getInstance()
{
	static CommandLineFactorMngr* instance = NULL;
	if(NULL == instance)
	{
		instance = new CommandLineFactorMngr();
	}
	return instance;
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
	ifstream fileStream;
	fileStream.open(mAddonFactorDefineFile.c_str());
	if(false == fileStream.is_open())
	{
		return errOpenFileFail;
	}
	string inputLine;
	do
	{
		getline(fileStream, inputLine);
		if(true == fileStream.fail())
		{
			break;
		}

		if ("" == inputLine) {
			continue;
		}
		
		JZWRITE_DEBUG("%s",inputLine.c_str());
		if (2 < inputLine.size()) {
			if ('-' == inputLine[0] && 'D' == inputLine[1]) {
				handleBarD(inputLine);
			}
		}
	}while(true);
	return errNoError;
}

