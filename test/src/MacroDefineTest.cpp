#include "MacroLex.h"
#include "CmdInputFactor.h"
#include <gtest/gtest.h>
#include "JZLogger.h"

//cover case: simple define, basic input
TEST(MacroLex, macroTest1){
  //this is really a strong test, so don't do it for more than one file every time
  int argc = 2;
  char argv0[128] = {0},argv1[128] = {0};
  strcpy(argv0,"tester");
  strcpy(argv1,"./test/TestSet/macro_test_1");
  char* argv[2] = {argv0,argv1};

	JZSetLoggerLevel(JZ_LOG_TEST);

	//analyze command line input
	CmdInputFactor::getInstance()->analyze(argc, argv);

	//now begin to analyze the files input from command line
	string toCompileFile = CmdInputFactor::getInstance()->getNextFile();

  MacroLex lex;
  lex.analyzeAFile(toCompileFile);
  LexRecList recList = lex.getRecList();

	JZSetLoggerLevel(JZ_LOG_DEBUG);
  lex.printLexRec();

	JZSetLoggerLevel(JZ_LOG_TEST);

  ASSERT_EQ(10, recList.size());
  ASSERT_STREQ("main", recList[0].word.c_str());
  ASSERT_EQ(4, recList[0].line);
  ASSERT_STREQ("(", recList[1].word.c_str());
  ASSERT_STREQ("1", recList[2].word.c_str());
  ASSERT_EQ(4, recList[2].line);
  ASSERT_STREQ(")", recList[3].word.c_str());
  ASSERT_STREQ(";", recList[4].word.c_str());
  ASSERT_STREQ("hello", recList[5].word.c_str());
  ASSERT_STREQ("(", recList[6].word.c_str());
  ASSERT_STREQ("1", recList[7].word.c_str());
  ASSERT_STREQ(")", recList[8].word.c_str());
  ASSERT_STREQ(";", recList[9].word.c_str());

}

//cover case : function macro
TEST(MacroLex, macroTest2){
  //this is really a strong test, so don't do it for more than one file every time
  int argc = 2;
  char argv0[128] = {0},argv1[128] = {0};
  strcpy(argv0,"tester");
  strcpy(argv1,"./test/TestSet/macro_test_2");
  char* argv[2] = {argv0,argv1};

	JZSetLoggerLevel(JZ_LOG_TEST);

	//analyze command line input
	CmdInputFactor::getInstance()->analyze(argc, argv);

	//now begin to analyze the files input from command line
	string toCompileFile = CmdInputFactor::getInstance()->getNextFile();

  MacroLex lex;
//	JZSetLoggerLevel(JZ_LOG_DEBUG);
  lex.analyzeAFile(toCompileFile);
  LexRecList recList = lex.getRecList();
//	JZSetLoggerLevel(JZ_LOG_TEST);

  ASSERT_EQ(15, recList.size());
  ASSERT_STREQ("main", recList[0].word.c_str());
  ASSERT_STREQ("(", recList[1].word.c_str());
  ASSERT_STREQ("1", recList[2].word.c_str());
  ASSERT_STREQ(",", recList[3].word.c_str());
  ASSERT_STREQ("(", recList[4].word.c_str());
  ASSERT_STREQ("3", recList[5].word.c_str());
  ASSERT_STREQ("+", recList[6].word.c_str());
  ASSERT_STREQ("1", recList[7].word.c_str());
  ASSERT_STREQ(")", recList[8].word.c_str());
  ASSERT_STREQ(",", recList[9].word.c_str());
  ASSERT_STREQ("(", recList[10].word.c_str());
  ASSERT_STREQ("abce", recList[11].word.c_str());
  ASSERT_STREQ(")", recList[12].word.c_str());
  ASSERT_STREQ(")", recList[13].word.c_str());
  ASSERT_STREQ(";", recList[14].word.c_str());
}

//cover case : var function macro
TEST(MacroLex, macroTest3){
  //this is really a strong test, so don't do it for more than one file every time
  int argc = 2;
  char argv0[128] = {0},argv1[128] = {0};
  strcpy(argv0,"tester");
  strcpy(argv1,"./test/TestSet/macro_test_3");
  char* argv[2] = {argv0,argv1};

	JZSetLoggerLevel(JZ_LOG_TEST);

	//analyze command line input
	CmdInputFactor::getInstance()->analyze(argc, argv);

	//now begin to analyze the files input from command line
	string toCompileFile = CmdInputFactor::getInstance()->getNextFile();

  MacroLex lex;
  lex.analyzeAFile(toCompileFile);
  LexRecList recList = lex.getRecList();

  ASSERT_EQ(17, recList.size());
  ASSERT_STREQ("(", recList[0].word.c_str());
  ASSERT_STREQ("1", recList[1].word.c_str());
  ASSERT_STREQ("(", recList[2].word.c_str());
  ASSERT_STREQ("2", recList[3].word.c_str());
  ASSERT_STREQ(",", recList[4].word.c_str());
  ASSERT_STREQ("3", recList[5].word.c_str());
  ASSERT_STREQ(",", recList[6].word.c_str());
  ASSERT_STREQ("\"123\"", recList[7].word.c_str());
  ASSERT_STREQ(")", recList[8].word.c_str());
  ASSERT_STREQ("0", recList[9].word.c_str());
  ASSERT_STREQ(")", recList[10].word.c_str());

  ASSERT_STREQ("(", recList[11].word.c_str());
  ASSERT_STREQ("1", recList[12].word.c_str());
  ASSERT_STREQ("(", recList[13].word.c_str());
  ASSERT_STREQ(")", recList[14].word.c_str());
  ASSERT_STREQ("0", recList[15].word.c_str());
  ASSERT_STREQ(")", recList[16].word.c_str());

}

//cover case : # and ## in  macro function
TEST(MacroLex, macroTest4){
  //this is really a strong test, so don't do it for more than one file every time
  int argc = 2;
  char argv0[128] = {0},argv1[128] = {0};
  strcpy(argv0,"tester");
  strcpy(argv1,"./test/TestSet/macro_test_4");
  char* argv[2] = {argv0,argv1};

	JZSetLoggerLevel(JZ_LOG_TEST);

	//analyze command line input
	CmdInputFactor::getInstance()->analyze(argc, argv);

	//now begin to analyze the files input from command line
	string toCompileFile = CmdInputFactor::getInstance()->getNextFile();

  MacroLex lex;
//	JZSetLoggerLevel(JZ_LOG_DEBUG);
  lex.analyzeAFile(toCompileFile);
  LexRecList recList = lex.getRecList();
//	JZSetLoggerLevel(JZ_LOG_TEST);

  ASSERT_EQ(5, recList.size());
  ASSERT_STREQ("\"hello\"", recList[0].word.c_str());
  ASSERT_STREQ("123123", recList[1].word.c_str());
  ASSERT_STREQ("<<", recList[2].word.c_str());
  ASSERT_STREQ("123", recList[3].word.c_str());
  ASSERT_STREQ("1123", recList[4].word.c_str());
}

//cover case : # and ## in  macro function
TEST(MacroLex, macroTest5){
  //this is really a strong test, so don't do it for more than one file every time
  int argc = 2;
  char argv0[128] = {0},argv1[128] = {0};
  strcpy(argv0,"tester");
  strcpy(argv1,"./test/TestSet/macro_test_5");
  char* argv[2] = {argv0,argv1};

	JZSetLoggerLevel(JZ_LOG_TEST);

	//analyze command line input
	CmdInputFactor::getInstance()->analyze(argc, argv);

	//now begin to analyze the files input from command line
	string toCompileFile = CmdInputFactor::getInstance()->getNextFile();

  MacroLex lex;
//	JZSetLoggerLevel(JZ_LOG_DEBUG);
  lex.analyzeAFile(toCompileFile);
  LexRecList recList = lex.getRecList();
//	JZSetLoggerLevel(JZ_LOG_TEST);

  ASSERT_EQ(2, recList.size());
  ASSERT_STREQ("\"1\"", recList[0].word.c_str());
  ASSERT_STREQ("123", recList[1].word.c_str());
}

//cover case : ## in var args macro function
TEST(MacroLex, macroTest6){
  //this is really a strong test, so don't do it for more than one file every time
  int argc = 2;
  char argv0[128] = {0},argv1[128] = {0};
  strcpy(argv0,"tester");
  strcpy(argv1,"./test/TestSet/macro_test_6");
  char* argv[2] = {argv0,argv1};

	JZSetLoggerLevel(JZ_LOG_TEST);

	//analyze command line input
	CmdInputFactor::getInstance()->analyze(argc, argv);

	//now begin to analyze the files input from command line
	string toCompileFile = CmdInputFactor::getInstance()->getNextFile();

  MacroLex lex;
//	JZSetLoggerLevel(JZ_LOG_DEBUG);
  lex.analyzeAFile(toCompileFile);
  LexRecList recList = lex.getRecList();
//	JZSetLoggerLevel(JZ_LOG_TEST);

  ASSERT_EQ(10, recList.size());
  ASSERT_STREQ("printf", recList[0].word.c_str());
  ASSERT_STREQ("(", recList[1].word.c_str());
  ASSERT_STREQ("1", recList[2].word.c_str());
  ASSERT_STREQ(",", recList[3].word.c_str());
  ASSERT_STREQ("\"1\"", recList[4].word.c_str());
  ASSERT_STREQ(")", recList[5].word.c_str());

  ASSERT_STREQ("printf", recList[6].word.c_str());
  ASSERT_STREQ("(", recList[7].word.c_str());
  ASSERT_STREQ("1", recList[8].word.c_str());
  ASSERT_STREQ(")", recList[9].word.c_str());
}

//cover case : named var args macro function
TEST(MacroLex, macroTest7){
  //this is really a strong test, so don't do it for more than one file every time
  int argc = 2;
  char argv0[128] = {0},argv1[128] = {0};
  strcpy(argv0,"tester");
  strcpy(argv1,"./test/TestSet/macro_test_7");
  char* argv[2] = {argv0,argv1};

	JZSetLoggerLevel(JZ_LOG_TEST);

	//analyze command line input
	CmdInputFactor::getInstance()->analyze(argc, argv);

	//now begin to analyze the files input from command line
	string toCompileFile = CmdInputFactor::getInstance()->getNextFile();

  MacroLex lex;
//	JZSetLoggerLevel(JZ_LOG_DEBUG);
  lex.analyzeAFile(toCompileFile);
  LexRecList recList = lex.getRecList();
//	JZSetLoggerLevel(JZ_LOG_TEST);

  ASSERT_EQ(10, recList.size());
  ASSERT_STREQ("printf", recList[0].word.c_str());
  ASSERT_STREQ("(", recList[1].word.c_str());
  ASSERT_STREQ("1", recList[2].word.c_str());
  ASSERT_STREQ(",", recList[3].word.c_str());
  ASSERT_STREQ("\"1\"", recList[4].word.c_str());
  ASSERT_STREQ(")", recList[5].word.c_str());

  ASSERT_STREQ("printf", recList[6].word.c_str());
  ASSERT_STREQ("(", recList[7].word.c_str());
  ASSERT_STREQ("1", recList[8].word.c_str());
  ASSERT_STREQ(")", recList[9].word.c_str());
}

//cover case : one parameter, use manay times
TEST(MacroLex, macroTest8){
  //this is really a strong test, so don't do it for more than one file every time
  int argc = 2;
  char argv0[128] = {0},argv1[128] = {0};
  strcpy(argv0,"tester");
  strcpy(argv1,"./test/TestSet/macro_test_8");
  char* argv[2] = {argv0,argv1};

	JZSetLoggerLevel(JZ_LOG_TEST);

	//analyze command line input
	CmdInputFactor::getInstance()->analyze(argc, argv);

	//now begin to analyze the files input from command line
	string toCompileFile = CmdInputFactor::getInstance()->getNextFile();

  MacroLex lex;
//	JZSetLoggerLevel(JZ_LOG_DEBUG);
  lex.analyzeAFile(toCompileFile);
  LexRecList recList = lex.getRecList();
//	JZSetLoggerLevel(JZ_LOG_TEST);

  ASSERT_EQ(12, recList.size());
  ASSERT_STREQ("print", recList[0].word.c_str());
  ASSERT_STREQ("(", recList[1].word.c_str());
  ASSERT_STREQ("1", recList[2].word.c_str());
  ASSERT_STREQ(",", recList[3].word.c_str());
  ASSERT_STREQ("2", recList[4].word.c_str());
  ASSERT_STREQ(",", recList[5].word.c_str());
  ASSERT_STREQ("1", recList[6].word.c_str());
  ASSERT_STREQ(",", recList[7].word.c_str());
  ASSERT_STREQ("2", recList[8].word.c_str());
  ASSERT_STREQ(",", recList[9].word.c_str());
  ASSERT_STREQ("12", recList[10].word.c_str());
  ASSERT_STREQ(")", recList[11].word.c_str());

}

//cover case : undef
TEST(MacroLex, macroTest9){
  //this is really a strong test, so don't do it for more than one file every time
  int argc = 2;
  char argv0[128] = {0},argv1[128] = {0};
  strcpy(argv0,"tester");
  strcpy(argv1,"./test/TestSet/macro_test_9");
  char* argv[2] = {argv0,argv1};

	JZSetLoggerLevel(JZ_LOG_TEST);

	//analyze command line input
	CmdInputFactor::getInstance()->analyze(argc, argv);

	//now begin to analyze the files input from command line
	string toCompileFile = CmdInputFactor::getInstance()->getNextFile();

  MacroLex lex;
//	JZSetLoggerLevel(JZ_LOG_DEBUG);
  lex.analyzeAFile(toCompileFile);
  LexRecList recList = lex.getRecList();
//	JZSetLoggerLevel(JZ_LOG_TEST);

  ASSERT_EQ(1, recList.size());
  ASSERT_STREQ("hello", recList[0].word.c_str());
}
