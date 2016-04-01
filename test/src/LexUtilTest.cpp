#include <gtest/gtest.h>
#include "Lex.h"
#include "JZFileUtil.h"
#include <string.h>

TEST(LexUtil, eraseLineSeperator){
  //these cases are a little complex in input because they are full of special charactor.
  
  //don't remove \n if it doesn't have "\"    
  const char* input0 = "this is a test!\n this is a test";
  uint64 bufSize0 = strlen(input0);
  const char* eraseRet0 = LexUtil::eraseLineSeperator((const char*)input0, &bufSize0);
  ASSERT_STREQ("this is a test!\n this is a test", eraseRet0);

  //remove \ \n if it has a \ at the end of line
  const char* input1 = "this is a test!\\ \n this is a test";
  uint64 bufSize1 = strlen(input1);
  const char* eraseRet1 = LexUtil::eraseLineSeperator((const char*)input1, &bufSize1);
  ASSERT_STREQ("this is a test! this is a test", eraseRet1);

  //remove \ \n if it has a \ at the end of line event if it's inside the ""
  const char* input2 = "\"this is a test!\\ \n this is a test\"";
  uint64 bufSize2 = strlen(input2);
  const char* eraseRet2 = LexUtil::eraseLineSeperator((const char*)input2, &bufSize2);
  ASSERT_STREQ("\"this is a test! this is a test\"", eraseRet2);

  //don't remove \ \n if it is escaped by a \ before the last one
  const char* input3 = "\"this is a test!\\\\ \n this is a test\"";
  uint64 bufSize3 = strlen(input3);
  const char* eraseRet3 = LexUtil::eraseLineSeperator((const char*)input3, &bufSize3);
  ASSERT_STREQ("\"this is a test!\\\\ \n this is a test\"", eraseRet3);

  //remove \ \n even if it is inside a comment
  const char* input4 = "//this is a test!\\ \n this is a test";
  uint64 bufSize4 = strlen(input4);
  const char* eraseRet4 = LexUtil::eraseLineSeperator((const char*)input4, &bufSize4);
  ASSERT_STREQ("//this is a test! this is a test", eraseRet4);
}

TEST(LexUtil, eraseComment){
  //erase the comment until the end of line
  const char* input0 = "this is a test!\n this is a test// this is a comment";
  uint64 bufSize0 = strlen(input0);
  const char* eraseRet0 = LexUtil::eraseComment((const char*)input0, &bufSize0);
  ASSERT_STREQ("this is a test!\n this is a test", eraseRet0);

  const char* input1 = "this is a test!\n this is a test/* this is a comment*/";
  uint64 bufSize1 = strlen(input1);
  const char* eraseRet1 = LexUtil::eraseComment((const char*)input1, &bufSize1);
  ASSERT_STREQ("this is a test!\n this is a test", eraseRet1);

  const char* input2 = "this is a test!\n this is a test// this is a comment\nthis is a new line";
  uint64 bufSize2 = strlen(input2);
  const char* eraseRet2 = LexUtil::eraseComment((const char*)input2, &bufSize2);
  ASSERT_STREQ("this is a test!\n this is a test\nthis is a new line", eraseRet2);

  const char* input3 = "this is a test!\n this /*this is a comment*/is a test";
  uint64 bufSize3 = strlen(input3);
  const char* eraseRet3 = LexUtil::eraseComment((const char*)input3, &bufSize3);
  ASSERT_STREQ("this is a test!\n this is a test", eraseRet3);

  const char* input4 = "this is a test!\n this is a test\n\"//don't erase this comment\"";
  uint64 bufSize4 = strlen(input4);
  const char* eraseRet4 = LexUtil::eraseComment((const char*)input4, &bufSize4);
  ASSERT_STREQ("this is a test!\n this is a test\n\"//don't erase this comment\"", eraseRet4);
}

TEST(LexUtil, smallTestCase){
  EXPECT_EQ(true, LexUtil::isLineEnder('\n'));
  EXPECT_EQ(false, LexUtil::isLineEnder('\t'));

  EXPECT_EQ(true, LexUtil::isEmptyInput('\t'));
  EXPECT_EQ(true, LexUtil::isEmptyInput('\n'));
  EXPECT_EQ(true, LexUtil::isEmptyInput(' '));
  EXPECT_EQ(false, LexUtil::isEmptyInput('.'));
  
  EXPECT_EQ(true, LexUtil::isBackSlant('\\'));
  EXPECT_EQ(false, LexUtil::isBackSlant('/'));

  EXPECT_EQ(true, LexUtil::isEmptyInput("\t    \n   "));
  EXPECT_EQ(false, LexUtil::isEmptyInput("    a   \n \t"));

  EXPECT_EQ(true, LexUtil::isEndWithBackSlant("\\      \n"));
  EXPECT_EQ(true, LexUtil::isEndWithBackSlant("\\\t\n"));
  EXPECT_EQ(true, LexUtil::isEndWithBackSlant("\\\t \n"));
  EXPECT_EQ(false, LexUtil::isEndWithBackSlant("\\n      \n"));

  EXPECT_EQ('\'', LexUtil::seperatorMatcher('\''));
  EXPECT_EQ('\"', LexUtil::seperatorMatcher('\"'));
  EXPECT_EQ('}', LexUtil::seperatorMatcher('{'));
  EXPECT_EQ(']', LexUtil::seperatorMatcher('['));
  EXPECT_EQ(')', LexUtil::seperatorMatcher('('));

  EXPECT_EQ(true, LexUtil::isConstNumberChar('0'));
  EXPECT_EQ(true, LexUtil::isConstNumberChar('1'));
  EXPECT_EQ(true, LexUtil::isConstNumberChar('2'));
  EXPECT_EQ(true, LexUtil::isConstNumberChar('3'));
  EXPECT_EQ(true, LexUtil::isConstNumberChar('4'));
  EXPECT_EQ(true, LexUtil::isConstNumberChar('5'));
  EXPECT_EQ(true, LexUtil::isConstNumberChar('6'));
  EXPECT_EQ(true, LexUtil::isConstNumberChar('7'));
  EXPECT_EQ(true, LexUtil::isConstNumberChar('8'));
  EXPECT_EQ(true, LexUtil::isConstNumberChar('9'));
  EXPECT_EQ(false, LexUtil::isConstNumberChar('.'));
  EXPECT_EQ(false, LexUtil::isConstNumberChar('-'));
  EXPECT_EQ(false, LexUtil::isConstNumberChar('+'));
  EXPECT_EQ(false, LexUtil::isConstNumberChar('='));
  
  ASSERT_STREQ("abce", LexUtil::eatLREmptyInput("  abce   ").c_str());
  ASSERT_STREQ("", LexUtil::eatLREmptyInput("  \t\n ").c_str());
  ASSERT_STREQ("abce", LexUtil::eatLREmptyInput("abce   ").c_str());
  ASSERT_STREQ("abce", LexUtil::eatLREmptyInput("  abce").c_str());

  EXPECT_EQ(false, LexUtil::ignoreMacroWhenStreamIsOff("if"));
  EXPECT_EQ(false, LexUtil::ignoreMacroWhenStreamIsOff("ifdef"));
  EXPECT_EQ(false, LexUtil::ignoreMacroWhenStreamIsOff("ifndef"));
  EXPECT_EQ(false, LexUtil::ignoreMacroWhenStreamIsOff("endif"));
  EXPECT_EQ(false, LexUtil::ignoreMacroWhenStreamIsOff("else"));
  EXPECT_EQ(true, LexUtil::ignoreMacroWhenStreamIsOff("define"));
  EXPECT_EQ(true, LexUtil::ignoreMacroWhenStreamIsOff("param"));
  EXPECT_EQ(true, LexUtil::ignoreMacroWhenStreamIsOff("include"));
  EXPECT_EQ(true, LexUtil::ignoreMacroWhenStreamIsOff("once"));
  EXPECT_EQ(true, LexUtil::ignoreMacroWhenStreamIsOff("warning"));
  EXPECT_EQ(true, LexUtil::ignoreMacroWhenStreamIsOff("error"));
}
