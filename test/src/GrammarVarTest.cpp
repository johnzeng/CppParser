#include <gtest/gtest.h>
#include "Lex.h"
#include "CmdInputFactor.h"
#include "GrammarAnalyzer.h"
#include "JZLogger.h"

//basic function declaration
TEST(GrammarAnalyzer, VarBasic0)
{
  int argc = 2;
  char argv0[128] = {0},argv1[128] = {0};
  strcpy(argv0,"tester");
  strcpy(argv1,"./test/GrammarSample/var_sample_0");
  char* argv[2] = {argv0,argv1};

	//analyze command line input
	CmdInputFactor::getInstance()->analyze(argc, argv);

	//now begin to analyze the files input from command line
	string toCompileFile = CmdInputFactor::getInstance()->getNextFile();

  Lex lex;

  lex.analyzeAFile(toCompileFile);

  LexRecList recList = lex.getRecList();

  GrammarAnalyzer grammar = GrammarAnalyzer(recList);

//  JZSetLoggerLevel(JZ_LOG_DEBUG|JZ_LOG_TRACE);
  uint32 ret = grammar.doAnalyze();
  JZSetLoggerLevel(JZ_LOG_TEST);

  ASSERT_EQ(eGrmErrNoError, ret);

}

