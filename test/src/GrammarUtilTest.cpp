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
  EXPECT_EQ(true, GrmUtilPtr->isFloatNumber("1234567890.000001"));

  EXPECT_EQ(true, GrmUtilPtr->isConstNumber("0x1234567890abcdefABCDEF"));
  EXPECT_EQ(true, GrmUtilPtr->isConstNumber("0123445670"));
  EXPECT_EQ(true, GrmUtilPtr->isConstNumber("1234567890"));
  EXPECT_EQ(true, GrmUtilPtr->isConstNumber("1234567890.000001"));

  EXPECT_EQ(true, GrmUtilPtr->isConstIntNumber("0x1234567890abcdefABCDEF"));
  EXPECT_EQ(true, GrmUtilPtr->isConstIntNumber("0123445670"));
  EXPECT_EQ(true, GrmUtilPtr->isConstIntNumber("1234567890"));
  EXPECT_EQ(false, GrmUtilPtr->isConstIntNumber("1234567890.000001"));
//Yes we don't think this is a legal number or double, 
//we think that '-' should be a separatro, so it should not appear in the number.
  EXPECT_EQ(false, GrmUtilPtr->isConstIntNumber("-1234567890"));
  EXPECT_EQ(false, GrmUtilPtr->isConstIntNumber("-1234567890.000001"));

  EXPECT_EQ(false, GrmUtilPtr->isConstIntNumber("abcccccddddddeeefff"));
  EXPECT_EQ(false, GrmUtilPtr->isConstIntNumber("hijklim"));
}


TEST(GrammarUtil, strTrans){
//  JZSetLoggerLevel(JZ_LOG_ALL);
  EXPECT_EQ(100, GrmUtilPtr->str2int32("100"));
  EXPECT_EQ(1, GrmUtilPtr->str2int32("1"));
  EXPECT_EQ(0, GrmUtilPtr->str2int32("0"));
  
  //should get 0 because  this is not a legal input
  //Actually I think we need to add some exception to this funciton,
  //because we get a chance that this is not a legal input
  EXPECT_EQ(0, GrmUtilPtr->str2int32("-1"));

  EXPECT_EQ(100.01, GrmUtilPtr->str2float("100.01"));
  EXPECT_EQ(1, GrmUtilPtr->str2float("1"));
  EXPECT_EQ(0, GrmUtilPtr->str2float("0"));
}
