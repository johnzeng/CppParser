#include <gtest/gtest.h>
#include "Lex.h"
#include "CmdInputFactor.h"
#include "GrammarAnalyzer.h"
#include "JZLogger.h"
#include "GrammarReturner.h"

//basic function declaration
TEST(GrammarAnalyzer, FunctionBasic0)
{
  int argc = 2;
  char argv0[128] = {0},argv1[128] = {0};
  strcpy(argv0,"tester");
  strcpy(argv1,"./test/GrammarSample/function_sample_0");
  char* argv[2] = {argv0,argv1};

	//analyze command line input
	CmdInputFactor::getInstance()->analyze(argc, argv);

	//now begin to analyze the files input from command line
	string toCompileFile = CmdInputFactor::getInstance()->getNextFile();

  Lex lex;

  lex.analyzeAFile(toCompileFile);

  LexRecList recList = lex.getRecList();

  GrammarAnalyzer grammar = GrammarAnalyzer(recList);


  uint32 ret = grammar.doAnalyze();
  grammar.getReturner()->printAllChild(0);
  JZSetLoggerLevel(JZ_LOG_TEST);

  ASSERT_EQ(eGrmErrNoError, ret);

  
}

//basic function ptr declaration
TEST(GrammarAnalyzer, FunctionBasic1)
{
  int argc = 2;
  char argv0[128] = {0},argv1[128] = {0};
  strcpy(argv0,"tester");
  strcpy(argv1,"./test/GrammarSample/function_sample_1");
  char* argv[2] = {argv0,argv1};

	//analyze command line input
	CmdInputFactor::getInstance()->analyze(argc, argv);

	//now begin to analyze the files input from command line
	string toCompileFile = CmdInputFactor::getInstance()->getNextFile();

  Lex lex;

  lex.analyzeAFile(toCompileFile);

  LexRecList recList = lex.getRecList();

  GrammarAnalyzer grammar = GrammarAnalyzer(recList);

//  JZSetLoggerLevel(JZ_LOG_DEBUG);
  uint32 ret = grammar.doAnalyze();

  JZSetLoggerLevel(JZ_LOG_TEST);
  ASSERT_EQ(eGrmErrNoError, ret);

  
}

//basic function defination
TEST(GrammarAnalyzer, FunctionBasic2)
{
  int argc = 2;
  char argv0[128] = {0},argv1[128] = {0};
  strcpy(argv0,"tester");
  strcpy(argv1,"./test/GrammarSample/function_sample_2");
  char* argv[2] = {argv0,argv1};

	//analyze command line input
	CmdInputFactor::getInstance()->analyze(argc, argv);

	//now begin to analyze the files input from command line
	string toCompileFile = CmdInputFactor::getInstance()->getNextFile();

  Lex lex;

  lex.analyzeAFile(toCompileFile);

  LexRecList recList = lex.getRecList();

  GrammarAnalyzer grammar = GrammarAnalyzer(recList);

//  JZSetLoggerLevel(JZ_LOG_DEBUG);
  uint32 ret = grammar.doAnalyze();

  JZSetLoggerLevel(JZ_LOG_TEST);
  ASSERT_EQ(eGrmErrNoError, ret);

  
}
