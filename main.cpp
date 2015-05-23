#include "JZLogger.h"
#include "JZMarcoFunc.h"
#include "CommandLineFactorMngr.h"
#include "JZFileUtil.h"
#include <unistd.h>
#include "CmdInputFactor.h"
#include "CommentHandler.h"

int main(int argc, char* argv[])
{
	JZLoggerInit();
	JZSetLoggerLevel(JZ_LOG_ALL);
	JZSetLoggerOutPutStatue(1,0);

	CmdInputFactor::getInstance()->analyze(argc, argv);

	CommandLineFactorMngr::getInstance()->searchAddonFactorFile();
	CommandLineFactorMngr::getInstance()->addonFactorAnalyser();
	
	string toCompileFile = "";
	while((toCompileFile = CmdInputFactor::getInstance()->getNextFile() ) != "")
	{
		char* fileData = (char*)JZGetFileData(toCompileFile.c_str());
		JZIF_NULL_RETURN(fileData, 1);
		CommentHandler::getInstance()->eraseComment(fileData);
		delete fileData;
	}
}
