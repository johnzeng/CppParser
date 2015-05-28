#include "JZLogger.h"
#include "JZMarcoFunc.h"
#include "CommandLineFactorMngr.h"
#include "JZFileUtil.h"
#include <unistd.h>
#include "CmdInputFactor.h"
#include "LexicalAnalyzer.h"

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
		LexicalAnalyzer* analyzer = new LexicalAnalyzer();
		analyzer->setSourceCodeDir(toCompileFile);
		analyzer->doAnalyze();
		AnalyzerCollector::getInstance()->addAnalyzer(toCompileFile, analyzer)	;
	}
}
