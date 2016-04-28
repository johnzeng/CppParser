#include "Lex.h"
#include "CmdInputFactor.h"
#include <gtest/gtest.h>
#include "JZLogger.h"

//cover: strange operator
TEST(Lex, strangeOpr)
{
  int argc = 2;
  char argv0[128] = {0},argv1[128] = {0};
  strcpy(argv0,"tester");
  strcpy(argv1,"./test/TestSet/lex_test_1");
  char* argv[2] = {argv0,argv1};


	//analyze command line input
	CmdInputFactor::getInstance()->analyze(argc, argv);

	//now begin to analyze the files input from command line
	string toCompileFile = CmdInputFactor::getInstance()->getNextFile();

  Lex lex;

  lex.analyzeAFile(toCompileFile);
//	JZSetLoggerLevel(JZ_LOG_DEBUG);
  lex.printLexRec();
//	JZSetLoggerLevel(JZ_LOG_TEST);

  LexRecList recList = lex.getRecList();

  ASSERT_EQ(49, recList.size());
  ASSERT_STREQ("int", recList[0].word.c_str());
  ASSERT_EQ(recList[0].line , 13);
  ASSERT_STREQ("pr", recList[1].word.c_str());
  ASSERT_STREQ("(", recList[2].word.c_str());
  ASSERT_STREQ("a", recList[3].word.c_str());
  ASSERT_STREQ(",", recList[4].word.c_str());
  ASSERT_STREQ("...", recList[5].word.c_str());
  ASSERT_STREQ(")", recList[6].word.c_str());
  ASSERT_STREQ("a", recList[7].word.c_str());
  ASSERT_STREQ(".*", recList[8].word.c_str());
  ASSERT_STREQ("var", recList[9].word.c_str());
  ASSERT_STREQ("a", recList[10].word.c_str());
  ASSERT_EQ(recList[10].line , 19);
  ASSERT_STREQ("->*", recList[11].word.c_str());
  ASSERT_STREQ("var", recList[12].word.c_str());
  ASSERT_STREQ("a", recList[13].word.c_str());
  ASSERT_STREQ("++", recList[14].word.c_str());
  ASSERT_STREQ("b", recList[15].word.c_str());
  ASSERT_STREQ("a", recList[16].word.c_str());
  ASSERT_STREQ("--", recList[17].word.c_str());
  ASSERT_STREQ("b", recList[18].word.c_str());
  ASSERT_STREQ("a", recList[19].word.c_str());
  ASSERT_STREQ("/=", recList[20].word.c_str());
  ASSERT_STREQ("1", recList[21].word.c_str());
  ASSERT_STREQ("b", recList[22].word.c_str());
  ASSERT_STREQ("+=", recList[23].word.c_str());
  ASSERT_STREQ("1", recList[24].word.c_str());
  ASSERT_STREQ("a", recList[25].word.c_str());
  ASSERT_STREQ("^=", recList[26].word.c_str());
  ASSERT_STREQ("1", recList[27].word.c_str());
  ASSERT_STREQ("hello", recList[28].word.c_str());
  ASSERT_STREQ("::", recList[29].word.c_str());
  ASSERT_STREQ("a", recList[30].word.c_str());

  ASSERT_STREQ("b", recList[31].word.c_str());
  ASSERT_STREQ("%=", recList[32].word.c_str());
  ASSERT_STREQ("3", recList[33].word.c_str());

  ASSERT_STREQ("c", recList[34].word.c_str());
  ASSERT_STREQ("*=", recList[35].word.c_str());
  ASSERT_STREQ("3", recList[36].word.c_str());

  ASSERT_STREQ("d", recList[37].word.c_str());
  ASSERT_STREQ("<<=", recList[38].word.c_str());
  ASSERT_STREQ("4", recList[39].word.c_str());

  ASSERT_STREQ("e", recList[40].word.c_str());
  ASSERT_STREQ(">>=", recList[41].word.c_str());
  ASSERT_STREQ("d", recList[42].word.c_str());

  ASSERT_STREQ("f", recList[43].word.c_str());
  ASSERT_STREQ("&=", recList[44].word.c_str());
  ASSERT_STREQ("f", recList[45].word.c_str());

  ASSERT_STREQ("g", recList[46].word.c_str());
  ASSERT_STREQ("|=", recList[47].word.c_str());
  ASSERT_STREQ("0", recList[48].word.c_str());
}
