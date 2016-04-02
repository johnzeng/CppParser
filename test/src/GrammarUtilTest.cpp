#include <gtest/gtest.h>
#include "GrammarUtil.h"

//TEST(GrammarUtil, initWithoutError){
//  GrammarUtil::getInstance()->init();
//  EXPECT_EQ(1,1);
//}

TEST(GrammarUtil, operatorSet){
  StringList input;
  input.push_back("+");
  input.push_back("-");
  input.push_back("*");
  input.push_back("/");
  input.push_back("==");
  input.push_back("!=");
  input.push_back("<");
  input.push_back(">");
  input.push_back("<=");
  input.push_back(">=");
  input.push_back("==");
  input.push_back("!=");
  input.push_back("<<");
  input.push_back(">>");
  input.push_back("%");
  input.push_back("|");
  input.push_back("&");
  input.push_back("||");
  input.push_back("&&");
  input.push_back("^");
  input.push_back("~");
  for (int i = 0; i < input.size(); i++) 
  {
    EXPECT_EQ(true, GrmUtilPtr->isOperator(input[i]));
  }

  StringList falseInput;
  falseInput.push_back("abc");
  falseInput.push_back("+-");
  for (int i = 0; i < falseInput.size(); i++) 
  {
    EXPECT_EQ(false, GrmUtilPtr->isOperator(falseInput[i]));
  }
}

TEST(GrammarUtil, numberType){
  EXPECT_EQ(false, GrmUtilPtr->isOcto("0x1234567890abcdefABCDEF"));
  EXPECT_EQ(true, GrmUtilPtr->isOcto("0123445670"));
  EXPECT_EQ(false, GrmUtilPtr->isOcto("1234567890"));
  EXPECT_EQ(false, GrmUtilPtr->isOcto("1234567890.000001"));

  EXPECT_EQ(true, GrmUtilPtr->isHex("0x1234567890abcdefABCDEF"));
  EXPECT_EQ(false, GrmUtilPtr->isHex("0123445670"));
  EXPECT_EQ(false, GrmUtilPtr->isHex("1234567890"));
  EXPECT_EQ(false, GrmUtilPtr->isHex("1234567890.000001"));

  EXPECT_EQ(true, GrmUtilPtr->isHex("0x1234567890abcdefABCDEF"));
  EXPECT_EQ(false, GrmUtilPtr->isHex("0123445670"));
  EXPECT_EQ(false, GrmUtilPtr->isHex("1234567890"));
  EXPECT_EQ(false, GrmUtilPtr->isHex("1234567890.000001"));

  EXPECT_EQ(false, GrmUtilPtr->isDeci("0x1234567890abcdefABCDEF"));
  EXPECT_EQ(false, GrmUtilPtr->isDeci("0123445670"));
  EXPECT_EQ(true, GrmUtilPtr->isDeci("1234567890"));
  EXPECT_EQ(false, GrmUtilPtr->isDeci("1234567890.000001"));

  EXPECT_EQ(false, GrmUtilPtr->isFloatNumber("0x1234567890abcdefABCDEF"));
  EXPECT_EQ(false, GrmUtilPtr->isFloatNumber("0123445670"));
  EXPECT_EQ(false, GrmUtilPtr->isFloatNumber("123456780"));
  EXPECT_EQ(false, GrmUtilPtr->isFloatNumber("1234567890.000001"));

  EXPECT_EQ(true, GrmUtilPtr->isConstNumber("0x1234567890abcdefABCDEF"));
  EXPECT_EQ(true, GrmUtilPtr->isConstNumber("0123445670"));
  EXPECT_EQ(true, GrmUtilPtr->isConstNumber("1234567890"));
  EXPECT_EQ(false, GrmUtilPtr->isConstNumber("1234567890.000001"));
}
