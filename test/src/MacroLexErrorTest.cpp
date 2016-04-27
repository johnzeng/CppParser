#include "MacroLex.h"
#include "CmdInputFactor.h"
#include <gtest/gtest.h>
#include "JZLogger.h"

//cover case : error
TEST(MacroLex, errorTest1){
  //this is really a strong test, so don't do it for more than one file every time
  int argc = 2;
  char argv0[128] = {0},argv1[128] = {0};
  strcpy(argv0,"tester");
  strcpy(argv1,"./test/TestSet/macro_test_error_1");
  char* argv[2] = {argv0,argv1};

	JZSetLoggerLevel(JZ_LOG_TEST);

	//analyze command line input
	CmdInputFactor::getInstance()->analyze(argc, argv);

	//now begin to analyze the files input from command line
	string toCompileFile = CmdInputFactor::getInstance()->getNextFile();

  MacroLex lex;
//	JZSetLoggerLevel(JZ_LOG_DEBUG);
  lex.analyzeAFile(toCompileFile);


}

//cover case : error
TEST(MacroLex, errorTest2){
  //this is really a strong test, so don't do it for more than one file every time
  int argc = 2;
  char argv0[128] = {0},argv1[128] = {0};
  strcpy(argv0,"tester");
  strcpy(argv1,"./test/TestSet/macro_test_error_2");
  char* argv[2] = {argv0,argv1};

	JZSetLoggerLevel(JZ_LOG_TEST);

	//analyze command line input
	CmdInputFactor::getInstance()->analyze(argc, argv);

	//now begin to analyze the files input from command line
	string toCompileFile = CmdInputFactor::getInstance()->getNextFile();

  MacroLex lex;
//	JZSetLoggerLevel(JZ_LOG_DEBUG);
  lex.analyzeAFile(toCompileFile);


}

//cover case : error
TEST(MacroLex, errorTest3){
  //this is really a strong test, so don't do it for more than one file every time
  int argc = 2;
  char argv0[128] = {0},argv1[128] = {0};
  strcpy(argv0,"tester");
  strcpy(argv1,"./test/TestSet/macro_test_error_3");
  char* argv[2] = {argv0,argv1};

	JZSetLoggerLevel(JZ_LOG_TEST);

	//analyze command line input
	CmdInputFactor::getInstance()->analyze(argc, argv);

	//now begin to analyze the files input from command line
	string toCompileFile = CmdInputFactor::getInstance()->getNextFile();

  MacroLex lex;
//	JZSetLoggerLevel(JZ_LOG_DEBUG);
  lex.analyzeAFile(toCompileFile);


}

//cover case : error, also var arg without other args
TEST(MacroLex, errorTest4){
  //this is really a strong test, so don't do it for more than one file every time
  int argc = 2;
  char argv0[128] = {0},argv1[128] = {0};
  strcpy(argv0,"tester");
  strcpy(argv1,"./test/TestSet/macro_test_error_4");
  char* argv[2] = {argv0,argv1};

	JZSetLoggerLevel(JZ_LOG_TEST);

	//analyze command line input
	CmdInputFactor::getInstance()->analyze(argc, argv);

	//now begin to analyze the files input from command line
	string toCompileFile = CmdInputFactor::getInstance()->getNextFile();

  MacroLex lex;
//	JZSetLoggerLevel(JZ_LOG_DEBUG);
  uint32 analyzeRet = lex.analyzeAFile(toCompileFile);

  ASSERT_EQ(analyzeRet, LexBase::eLexFuncLikeMacroParamTooManay);

  LexRecList ret = lex.getRecList();
  
  ASSERT_EQ(ret.size(), 8);
  ASSERT_STREQ(ret[0].word.c_str(), "hello");
  ASSERT_STREQ(ret[1].word.c_str(), "(");
  ASSERT_STREQ(ret[2].word.c_str(), "a");
  ASSERT_STREQ(ret[3].word.c_str(), ",");
  ASSERT_STREQ(ret[4].word.c_str(), "b");
  ASSERT_STREQ(ret[5].word.c_str(), ",");
  ASSERT_STREQ(ret[6].word.c_str(), "c");
  ASSERT_STREQ(ret[7].word.c_str(), ")");

}

//cover case : error
TEST(MacroLex, errorTest5){
  //this is really a strong test, so don't do it for more than one file every time
  int argc = 2;
  char argv0[128] = {0},argv1[128] = {0};
  strcpy(argv0,"tester");
  strcpy(argv1,"./test/TestSet/macro_test_error_5");
  char* argv[2] = {argv0,argv1};

	JZSetLoggerLevel(JZ_LOG_TEST);

	//analyze command line input
	CmdInputFactor::getInstance()->analyze(argc, argv);

	//now begin to analyze the files input from command line
	string toCompileFile = CmdInputFactor::getInstance()->getNextFile();

  MacroLex lex;
//	JZSetLoggerLevel(JZ_LOG_DEBUG);
  uint32 ret = lex.analyzeAFile(toCompileFile);

  ASSERT_EQ(LexBase::eLexFuncLikeMacroParamTooLess, ret);

}

//cover case : error because input without )
TEST(MacroLex, errorTest6){
  //this is really a strong test, so don't do it for more than one file every time
  int argc = 2;
  char argv0[128] = {0},argv1[128] = {0};
  strcpy(argv0,"tester");
  strcpy(argv1,"./test/TestSet/macro_test_error_6");
  char* argv[2] = {argv0,argv1};

	JZSetLoggerLevel(JZ_LOG_TEST);

	//analyze command line input
	CmdInputFactor::getInstance()->analyze(argc, argv);

	//now begin to analyze the files input from command line
	string toCompileFile = CmdInputFactor::getInstance()->getNextFile();

  MacroLex lex;
//	JZSetLoggerLevel(JZ_LOG_DEBUG);
  uint32 ret = lex.analyzeAFile(toCompileFile);

}

//cover case : error because looping define macro
TEST(MacroLex, errorTest7){
  //this is really a strong test, so don't do it for more than one file every time
  int argc = 2;
  char argv0[128] = {0},argv1[128] = {0};
  strcpy(argv0,"tester");
  strcpy(argv1,"./test/TestSet/macro_test_error_7");
  char* argv[2] = {argv0,argv1};

	JZSetLoggerLevel(JZ_LOG_TEST);

	//analyze command line input
	CmdInputFactor::getInstance()->analyze(argc, argv);

	//now begin to analyze the files input from command line
	string toCompileFile = CmdInputFactor::getInstance()->getNextFile();

  MacroLex lex;
//	JZSetLoggerLevel(JZ_LOG_DEBUG);
  uint32 ret = lex.analyzeAFile(toCompileFile);

  ASSERT_EQ(ret, LexBase::eLexMacroIsAlreadyExpending);

}
