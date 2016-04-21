#include <gtest/gtest.h>
#include "GrammarUtil.h"
#include "JZLogger.h"

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
  EXPECT_EQ(true, GrmUtilPtr->isDeci("123445670l"));
  EXPECT_EQ(true, GrmUtilPtr->isDeci("123445670ll"));
  EXPECT_EQ(true, GrmUtilPtr->isDeci("1234567890"));
  EXPECT_EQ(false, GrmUtilPtr->isDeci("1234567890.000001"));

  EXPECT_EQ(false, GrmUtilPtr->isFloatNumber("0x1234567890abcdefABCDEF"));
  EXPECT_EQ(false, GrmUtilPtr->isFloatNumber("0123445670"));
  EXPECT_EQ(false, GrmUtilPtr->isFloatNumber("123456780"));
  EXPECT_EQ(true, GrmUtilPtr->isFloatNumber("1234567890.000001"));
  EXPECT_EQ(true, GrmUtilPtr->isFloatNumber("1234567890.000001f"));
  EXPECT_EQ(true, GrmUtilPtr->isFloatNumber("1234567890.000001lf"));
  EXPECT_EQ(true, GrmUtilPtr->isFloatNumber("0.05"));

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

  EXPECT_EQ(0, GrmUtilPtr->getNum('0'));
  EXPECT_EQ(1, GrmUtilPtr->getNum('1'));
  EXPECT_EQ(2, GrmUtilPtr->getNum('2'));
  EXPECT_EQ(3, GrmUtilPtr->getNum('3'));
  EXPECT_EQ(4, GrmUtilPtr->getNum('4'));
  EXPECT_EQ(5, GrmUtilPtr->getNum('5'));
  EXPECT_EQ(9, GrmUtilPtr->getNum('9'));
  EXPECT_EQ(10, GrmUtilPtr->getNum('a'));
  EXPECT_EQ(11, GrmUtilPtr->getNum('b'));
  EXPECT_EQ(12, GrmUtilPtr->getNum('c'));
  EXPECT_EQ(13, GrmUtilPtr->getNum('d'));
  EXPECT_EQ(14, GrmUtilPtr->getNum('e'));
  EXPECT_EQ(15, GrmUtilPtr->getNum('f'));
  EXPECT_EQ(10, GrmUtilPtr->getNum('A'));
  EXPECT_EQ(11, GrmUtilPtr->getNum('B'));
  EXPECT_EQ(12, GrmUtilPtr->getNum('C'));
  EXPECT_EQ(13, GrmUtilPtr->getNum('D'));
  EXPECT_EQ(14, GrmUtilPtr->getNum('E'));
  EXPECT_EQ(15, GrmUtilPtr->getNum('F'));

}


TEST(GrammarUtil, strTrans){
//  JZSetLoggerLevel(JZ_LOG_ALL);
  EXPECT_EQ(100, GrmUtilPtr->str2int32("100"));
  EXPECT_EQ(1, GrmUtilPtr->str2int32("1"));
  EXPECT_EQ(0, GrmUtilPtr->str2int32("0"));

  //hex trans
  EXPECT_EQ(0, GrmUtilPtr->str2int32("0x0"));
  EXPECT_EQ(16, GrmUtilPtr->str2int32("0x10"));
  EXPECT_EQ(10, GrmUtilPtr->str2int32("0xa"));
  EXPECT_EQ(28, GrmUtilPtr->str2int32("0x1c"));
  
  EXPECT_EQ(0, GrmUtilPtr->str2int32("00"));
  EXPECT_EQ(8, GrmUtilPtr->str2int32("010"));
  EXPECT_EQ(7, GrmUtilPtr->str2int32("07"));
  EXPECT_EQ(28, GrmUtilPtr->str2int32("034"));
  
  //should get 0 because  this is not a legal input
  //Actually I think we need to add some exception to this funciton,
  //because we get a chance that this is not a legal input
  EXPECT_EQ(0, GrmUtilPtr->str2int32("-1"));

  EXPECT_EQ(0.5, GrmUtilPtr->str2float("0.5"));
  EXPECT_EQ(0.51f, GrmUtilPtr->str2float("0.51"));
  EXPECT_EQ(1, GrmUtilPtr->str2float("1.0"));
  EXPECT_EQ(0, GrmUtilPtr->str2float("0"));

  EXPECT_EQ(0.5, GrmUtilPtr->str2double("0.5"));
  EXPECT_EQ(0.51, GrmUtilPtr->str2double("0.51"));
  EXPECT_EQ(1, GrmUtilPtr->str2double("1.0"));
  EXPECT_EQ(0, GrmUtilPtr->str2double("0"));
}
