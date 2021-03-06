#include <gtest/gtest.h>
#include "Lex.h"
#include "CmdInputFactor.h"
#include "GrammarAnalyzer.h"
#include "JZLogger.h"

TEST(GrammarAnalyzer, EnumBasic0)
{
  int argc = 2;
  char argv0[128] = {0},argv1[128] = {0};
  strcpy(argv0,"tester");
  strcpy(argv1,"./test/GrammarSample/enum_sample_0");
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

  
//  GrammarBlock* topBlock = grammar.getTopBlock();
  
//  ASSERT_EQ(true, topBlock != NULL);

//  EnumDefine* enumDefTest = dynamic_cast<EnumDefine*>( topBlock->getDataDef("test") );
//  ASSERT_EQ(true, enumDefTest != NULL);
//
//  ASSERT_EQ(true, enumDefTest->fieldExist("hello"));
//  ASSERT_EQ(true, enumDefTest->fieldExist("hi"));
//  ASSERT_EQ(true, enumDefTest->fieldExist("nono"));


//  EnumDefine* enumDefTest1 = dynamic_cast<EnumDefine*>( topBlock->getDataDef("test1") );
//  ASSERT_EQ(true, enumDefTest1 != NULL);

//  ASSERT_EQ(true, enumDefTest1->fieldExist("no"));
//  ASSERT_EQ(true, enumDefTest1->fieldExist("yes"));
}

TEST(GrammarAnalyzer, EnumBasic1)
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

//  JZSetLoggerLevel(JZ_LOG_DEBUG|JZ_LOG_TRACE);
  uint32 ret = grammar.doAnalyze();

  ASSERT_EQ(eGrmErrNoError, ret);

  JZSetLoggerLevel(JZ_LOG_TEST);
  
//  GrammarBlock* topBlock = grammar.getTopBlock();
  
//  ASSERT_EQ(true, topBlock != NULL);

//  EnumDefine* enumDefTest = dynamic_cast<EnumDefine*>( topBlock->getDataDef("test") );
//  ASSERT_EQ(true, enumDefTest != NULL);
//
//  ASSERT_EQ(true, enumDefTest->fieldExist("hello"));
//  ASSERT_EQ(true, enumDefTest->fieldExist("hi"));
//  ASSERT_EQ(true, enumDefTest->fieldExist("nono"));


//  EnumDefine* enumDefTest1 = dynamic_cast<EnumDefine*>( topBlock->getDataDef("test1") );
//  ASSERT_EQ(true, enumDefTest1 != NULL);

//  ASSERT_EQ(true, enumDefTest1->fieldExist("no"));
//  ASSERT_EQ(true, enumDefTest1->fieldExist("yes"));
}

//test with attributes
TEST(GrammarAnalyzer, EnumAttributes)
{
  int argc = 2;
  char argv0[128] = {0},argv1[128] = {0};
  strcpy(argv0,"tester");
  strcpy(argv1,"./test/GrammarSample/enum_sample_2");
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
  ASSERT_EQ(eGrmErrNoError, ret);
  JZSetLoggerLevel(JZ_LOG_TEST);
  
//  GrammarBlock* topBlock = grammar.getTopBlock();
  
//  ASSERT_EQ(true, topBlock != NULL);

//  EnumDefine* enumDefTest = dynamic_cast<EnumDefine*>( topBlock->getDataDef("test") );
//  ASSERT_EQ(true, enumDefTest != NULL);

//  ASSERT_EQ(true, enumDefTest->fieldExist("hello"));
}

//test with only name defined
TEST(GrammarAnalyzer, EnumName)
{
  int argc = 2;
  char argv0[128] = {0},argv1[128] = {0};
  strcpy(argv0,"tester");
  strcpy(argv1,"./test/GrammarSample/enum_sample_3");
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
  ASSERT_EQ(eGrmErrNoError, ret);
  JZSetLoggerLevel(JZ_LOG_TEST);
  
//  GrammarBlock* topBlock = grammar.getTopBlock();
  
//  ASSERT_EQ(true, topBlock != NULL);

//  EnumDefine* enumDefTest = dynamic_cast<EnumDefine*>( topBlock->getDataDef("test") );
//  ASSERT_EQ(true, enumDefTest != NULL);

//  ASSERT_EQ(true, enumDefTest->fieldExist("hello"));
}
