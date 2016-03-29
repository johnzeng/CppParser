#include "JZLogger.h"
#include "JZMacroFunc.h"
#include "CommandLineFactorMngr.h"
#include "JZFileUtil.h"
#include <unistd.h>
#include "CmdInputFactor.h"
#include "IncludeHandler.h"
#include "DefineManager.h"
#include "ErrorCode.h"
#include "Lex.h"

void globalInitAtBegin()
{
	//init logger
	JZLoggerInit();
	JZSetLoggerLevel(JZ_LOG_ALL);
	JZSetLoggerOutPutStatue(1,1);
	JZSetLogFileName("log");

	//analyze command line addon data from .factor file
	CommandLineFactorMngr::getInstance()->searchAddonFactorFile();
	CommandLineFactorMngr::getInstance()->addonFactorAnalyser();

	//init include handler
	IncludeHandler::getInstance()->init();

	//now add common line factor to include handler
	auto headerSearchPath = CommandLineFactorMngr::getInstance()->getHeaderFileSearchPath();
	auto headerSearchFileIt = headerSearchPath.begin();
	for(; headerSearchFileIt != headerSearchPath.end(); headerSearchFileIt++)
	{
		IncludeHandler::getInstance()->addUserHeaderSearchPath(*headerSearchFileIt);	
	}
	
	//init global define manager
	DefineManager::getGlobalInstance()->init();
	auto userDefineMap = CommandLineFactorMngr::getInstance()->getDefineMap();
	//add addon define
	auto defineMapIt = userDefineMap.begin();
	for(; defineMapIt != userDefineMap.end() ; defineMapIt++ )
	{
		DefineRec rec = 
		{
			.key = defineMapIt->first,
			.isFuncLikeMacro = false,
			.isVarArgs = false,
			.defineStr = defineMapIt->second,
		};
		auto err = DefineManager::getGlobalInstance()->addDefineMap(defineMapIt->first, rec);
		if (err != DefineManager::eDefMgrNoError)
		{
			JZWRITE_ERROR("double define : %s", defineMapIt->first.c_str());
			return ;
		}
	}
}

int libEntry(int argc, char* argv[])
{
	//init
	globalInitAtBegin();

	//analyze command line input
	CmdInputFactor::getInstance()->analyze(argc, argv);

	//now begin to analyze the files input from command line
	string toCompileFile = "";
	while((toCompileFile = CmdInputFactor::getInstance()->getNextFile() ) != "")
	{
		Lex lex;
		lex.analyzeAFile(toCompileFile);
		lex.printLexRec();
	}
  return 0;
}
