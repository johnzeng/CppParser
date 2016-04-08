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


#include <signal.h>
#include <execinfo.h>
#include <stdlib.h>
 
inline void printStackTrace( FILE *out = stderr, unsigned int max_frames = 63 )
{
   fprintf(out, "stack trace:\n");
 
   // storage array for stack trace address data
   void* addrlist[max_frames+1];
 
   // retrieve current stack addresses
   uint32 addrlen = backtrace( addrlist, sizeof( addrlist ) / sizeof( void* ));
 
   if ( addrlen == 0 ) 
   {
      fprintf( out, "  \n" );
      return;
   }
 
   // create readable strings to each frame.
   char** symbollist = backtrace_symbols( addrlist, addrlen );
 
   // print the stack trace.
   for ( uint32 i = 0; i < addrlen; i++ )
      fprintf( out, "%s\n", symbollist[i]);
 
   free(symbollist);
}
void abortHandler( int signum )
{
   printStackTrace();
   exit( signum );
}
void stackTrace()
{
  signal( SIGABRT, abortHandler );
  signal( SIGSEGV, abortHandler );
  signal( SIGILL,  abortHandler );
  signal( SIGFPE,  abortHandler );
}

void globalInitAtBegin()
{
	//init logger
  stackTrace();

	JZLoggerInit();
	JZSetLoggerLevel(JZ_LOG_ALL);
	JZSetLoggerOutPutStatue(1,1);
	JZSetLogFileName("logs/app.log");

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
