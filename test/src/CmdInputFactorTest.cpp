#include <gtest/gtest.h>
#include <ErrorCode.h>
#include "JZLogger.h"
#include "JZMacroFunc.h"
#include "CmdInputFactor.h"

TEST(CmdInputFactorTest, GetInputFile){
  CmdInputFactor* item = new CmdInputFactor();
  int argc = 3;
  char argv0[7] = {0},argv1[26] = {0},argv2[23] = {0};
  strcpy(argv0,"tester");
  strcpy(argv1,"./test/TestSet/all_in_one");
  strcpy(argv2,"./test/TestSet/testcpp");
  char* argv[3] = {argv0,argv1,argv2};

  item->analyze(argc, argv);

  string toCompileFile = item->getNextFile();
//  JZWRITE_TEST("first file is: %s", toCompileFile.c_str())
  ASSERT_STREQ("./test/TestSet/all_in_one", toCompileFile.c_str());

  toCompileFile = item->getNextFile();
//  JZWRITE_TEST("second file is: %s", toCompileFile.c_str())
  ASSERT_STREQ("./test/TestSet/testcpp", toCompileFile.c_str());

  JZSAFE_DELETE(item);
}

