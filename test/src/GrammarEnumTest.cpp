#include <gtest/gtest.h>
#include "Lex.h"
#include "CmdInputFactor.h"
#include "GrammarAnalyzer.h"
#include "JZLogger.h"

TEST(GrammarAnalyzer, EnumBasic)
{
  int argc = 2;
  char argv0[128] = {0},argv1[128] = {0};
  strcpy(argv0,"tester");
  strcpy(argv1,"./test/GrammarSample/enum_sample_1");
  char* argv[2] = {argv0,argv1};

	//analyze command line input
	CmdInputFactor::getInstance()->analyze(argc, argv);

	//now begin to analyze the files input from command line
	string toCompileFile = CmdInputFactor::getInstance()->getNextFile();

  Lex lex;

  lex.analyzeAFile(toCompileFile);

  LexRecList recList = lex.getRecList();

  GrammarAnalyzer grammar = GrammarAnalyzer(recList);

  JZSetLoggerLevel(JZ_LOG_DEBUG|JZ_LOG_TRACE);
  uint32 ret = grammar.doAnalyze();
  JZSetLoggerLevel(JZ_LOG_TEST);
  
  GrammarBlock* topBlock = grammar.getTopBlock();
  
  if(NULL == topBlock)
  {
    ASSERT_EQ(0,1);
  }

  DataTypeDefine* enumDef = topBlock->getDataDef("test");
  if(NULL == enumDef)
  {
    ASSERT_EQ(0,1);
  }

  ASSERT_EQ(1,1);
}
