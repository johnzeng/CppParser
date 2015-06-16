#include "JZLogger.h"
#include "JZMarcoFunc.h"
#include "CommandLineFactorMngr.h"
#include "JZFileUtil.h"
#include <unistd.h>
#include "CmdInputFactor.h"
#include "LexicalAnalyzer.h"
#include "IncludeHandler.h"
#include "DefineManager.h"
#include "ErrorCode.h"

int main(int argc, char* argv[])
{

	//init logger
	JZLoggerInit();
	JZSetLoggerLevel(JZ_LOG_ALL);
	JZSetLoggerOutPutStatue(1,0);

	//analyze command line input
	CmdInputFactor::getInstance()->analyze(argc, argv);

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

	//init define manager
	DefineManager::getGlobalInstance()->init();
	auto userDefineMap = CommandLineFactorMngr::getInstance()->getDefineMap();
	//add addon define
	auto defineMapIt = userDefineMap.begin();
	for(; defineMapIt != userDefineMap.end() ; defineMapIt++ )
	{
		int32 err = DefineManager::getGlobalInstance()->addDefineMap(defineMapIt->first, defineMapIt->second);
		if (err != JZErrorCode::errNoError)
		{
			JZWRITE_ERROR("double define : %s", defineMapIt->first.c_str());
			return err;
		}
	}

	//now begin to analyze the files input from command line
	string toCompileFile = "";
	while((toCompileFile = CmdInputFactor::getInstance()->getNextFile() ) != "")
	{
		//do lexical analyze at first
		LexicalAnalyzer* analyzer = new LexicalAnalyzer();
		analyzer->setSourceCodeDir(toCompileFile);
		analyzer->doAnalyze();
		//add it into collector, so you will not do it again
		AnalyzerCollector::getInstance()->addAnalyzer(toCompileFile, analyzer)	;
	}
}
