#include <gtest/gtest.h>
#include <ErrorCode.h>
#include "JZLogger.h"
#include "JZMacroFunc.h"
#include "CmdInputFactor.h"

TEST(CmdInputFactorTest, GetInputFile){
  CmdInputFactor* item = new CmdInputFactor();
  int argc = 3;
  char* argv[3] = {"tester","./test/TestSet/all_in_one","./test/TestSet/testcpp"};

  JZWRITE_TEST("the argv 0 is: %s",argv[0]);
  JZWRITE_TEST("the argv 1 is: %s",argv[1]);
  JZWRITE_TEST("the argv 2 is: %s",argv[2]);

  item->analyze(argc, argv);

  string toCompileFile = item->getNextFile();
  JZWRITE_TEST("first file is: %s", toCompileFile.c_str())
  ASSERT_STREQ("./test/TestSet/all_in_one", toCompileFile.c_str());

  toCompileFile = item->getNextFile();
  JZWRITE_TEST("second file is: %s", toCompileFile.c_str())
  ASSERT_STREQ("./test/TestSet/testcpp", toCompileFile.c_str());

  JZSAFE_DELETE(item);
}

