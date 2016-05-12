#include "MacroLex.h"
#include "CmdInputFactor.h"
#include <gtest/gtest.h>
#include "IncludeHandler.h"
#include "JZLogger.h"

//cover case: include
TEST(MacroLex, macroInclude1){
  int argc = 2;
  char argv0[128] = {0},argv1[128] = {0};
  strcpy(argv0,"tester");
  strcpy(argv1,"./test/TestSet/macro_include_test_1");
  char* argv[2] = {argv0,argv1};

	JZSetLoggerLevel(JZ_LOG_TEST);

	//analyze command line input
	CmdInputFactor::getInstance()->analyze(argc, argv);

	//now begin to analyze the files input from command line
	string toCompileFile = CmdInputFactor::getInstance()->getNextFile();

  MacroLex lex;

  lex.analyzeAFile(toCompileFile);

  LexRecList recList = lex.getRecList();
//  ASSERT_GT(recList.size(), 10);
}

//cover case: include and once
TEST(MacroLex, macroInclude2){
  int argc = 2;
  char argv0[128] = {0},argv1[128] = {0};
  strcpy(argv0,"tester");
  strcpy(argv1,"./test/TestSet/macro_include_test_2");
  char* argv[2] = {argv0,argv1};

	JZSetLoggerLevel(JZ_LOG_TEST);

	//analyze command line input
	CmdInputFactor::getInstance()->analyze(argc, argv);

	//now begin to analyze the files input from command line
	string toCompileFile = CmdInputFactor::getInstance()->getNextFile();

  MacroLex lex;

  IncludeHandler::getInstance()->addUserHeaderSearchPath("./test/TestSet");	

  lex.analyzeAFile(toCompileFile);

  LexRecList recList = lex.getRecList();
  ASSERT_EQ(recList.size(), 3);
  ASSERT_STREQ("int", recList[0].word.c_str());
  ASSERT_STREQ("hello", recList[1].word.c_str());
  ASSERT_STREQ(";", recList[2].word.c_str());
}

//cover test: can not match a word
TEST(MacroLex, unmatch_word_test){
  //this is really a strong test, so don't do it for more than one file every time
  int argc = 2;
  char argv0[128] = {0},argv1[128] = {0};
  strcpy(argv0,"tester");
  strcpy(argv1,"./test/TestSet/unmatch_word_test");
  char* argv[2] = {argv0,argv1};

	//analyze command line input
	CmdInputFactor::getInstance()->analyze(argc, argv);

	//now begin to analyze the files input from command line
	string toCompileFile = CmdInputFactor::getInstance()->getNextFile();

  MacroLex lex;
  uint32 ret = lex.analyzeAFile(toCompileFile);
  ASSERT_NE(LexBase::eLexReachLineEnd, ret);

}
