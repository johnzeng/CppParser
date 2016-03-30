#include <gtest/gtest.h>
#include "Lex.h"
#include "JZFileUtil.h"
#include <string.h>

TEST(LexUtil, eraseLineSeperator){
  const char* input1 = "123456\\ \n 12345";
  uint64 bufSize1 = strlen(input1);
  const char* eraseRet1 = LexUtil::eraseLineSeperator((const char*)input1, &bufSize1);
  ASSERT_STREQ("123456 12345", eraseRet1);

  const char* input2 = "\"123456\\ \n 12345\"";
  uint64 bufSize2 = strlen(input2);
  const char* eraseRet2 = LexUtil::eraseLineSeperator((const char*)input2, &bufSize2);
  ASSERT_STREQ("\"123456 12345\"", eraseRet2);
}
