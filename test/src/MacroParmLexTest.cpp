#include "MacroParamLex.h"
#include <gtest/gtest.h>
#include "CmdInputFactor.h"
#include "JZLogger.h"

TEST(MacroParamLexTest, simpleTest)
{
  int argc = 2;
  char argv0[128] = {0},argv1[128] = {0};
  strcpy(argv0,"tester");
  strcpy(argv1,"./test/TestSet/simple_macro_lex_1");
  char* argv[2] = {argv0,argv1};

	//analyze command line input
	CmdInputFactor::getInstance()->analyze(argc, argv);

	//now begin to analyze the files input from command line
	string toCompileFile = CmdInputFactor::getInstance()->getNextFile();

  MacroParamLex lex;
  lex.analyzeAFile(toCompileFile);
  RealParamList paramList = lex.getParamList();

  ASSERT_EQ(4, paramList.size());
  ASSERT_STREQ("1", paramList[0].c_str());
  ASSERT_STREQ("\"2\"", paramList[1].c_str());
  ASSERT_STREQ("\"332\"", paramList[2].c_str());
  ASSERT_STREQ("  def", paramList[3].c_str());

}

TEST(MacroParamLexTest, loopBracket)
{
  int argc = 2;
  char argv0[128] = {0},argv1[128] = {0};
  strcpy(argv0,"tester");
  strcpy(argv1,"./test/TestSet/simple_macro_lex_2");
  char* argv[2] = {argv0,argv1};

	//analyze command line input
	CmdInputFactor::getInstance()->analyze(argc, argv);

	//now begin to analyze the files input from command line
	string toCompileFile = CmdInputFactor::getInstance()->getNextFile();

  MacroParamLex lex;
  lex.analyzeAFile(toCompileFile);
  RealParamList paramList = lex.getParamList();

  ASSERT_EQ(2, paramList.size());
  ASSERT_STREQ("(abc)", paramList[0].c_str());
  ASSERT_STREQ("(1,2,3)", paramList[1].c_str());

}

TEST(MacroParamLexTest, emptyParam)
{
  int argc = 2;
  char argv0[128] = {0},argv1[128] = {0};
  strcpy(argv0,"tester");
  strcpy(argv1,"./test/TestSet/simple_macro_lex_3");
  char* argv[2] = {argv0,argv1};

	//analyze command line input
	CmdInputFactor::getInstance()->analyze(argc, argv);

	//now begin to analyze the files input from command line
	string toCompileFile = CmdInputFactor::getInstance()->getNextFile();

  MacroParamLex lex;
  lex.analyzeAFile(toCompileFile);
  RealParamList paramList = lex.getParamList();

  ASSERT_EQ(0, paramList.size());

}
