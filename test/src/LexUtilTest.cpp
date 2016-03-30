#include <gtest/gtest.h>
#include "Lex.h"
#include "JZFileUtil.h"
#include <string.h>

TEST(LexUtil, eraseLineSeperator){
  //these cases are a little complex in input because they are full of special charactor.
  const char* input0 = "this is a test!\n this is a test";
  uint64 bufSize0 = strlen(input0);
  const char* eraseRet0 = LexUtil::eraseLineSeperator((const char*)input0, &bufSize0);
  ASSERT_STREQ("this is a test!\n this is a test", eraseRet0);

  const char* input1 = "this is a test!\\ \n this is a test";
  uint64 bufSize1 = strlen(input1);
  const char* eraseRet1 = LexUtil::eraseLineSeperator((const char*)input1, &bufSize1);
  ASSERT_STREQ("this is a test! this is a test", eraseRet1);

  const char* input2 = "\"this is a test!\\ \n this is a test\"";
  uint64 bufSize2 = strlen(input2);
  const char* eraseRet2 = LexUtil::eraseLineSeperator((const char*)input2, &bufSize2);
  ASSERT_STREQ("\"this is a test! this is a test\"", eraseRet2);

  const char* input3 = "\"this is a test!\\\\ \n this is a test\"";
  uint64 bufSize3 = strlen(input3);
  const char* eraseRet3 = LexUtil::eraseLineSeperator((const char*)input3, &bufSize3);
  ASSERT_STREQ("\"this is a test!\\\\ \n this is a test\"", eraseRet3);
}
