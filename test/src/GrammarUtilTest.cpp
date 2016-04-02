#include <gtest/gtest.h>
#include "GrammarUtil.h"

//TEST(GrammarUtil, initWithoutError){
//  GrammarUtil::getInstance()->init();
//  EXPECT_EQ(1,1);
//}

TEST(GrammarUtil, operatorSet){
  string input1 = "+";
  EXPECT_EQ(true, GrmUtilPtr->isOperator(input1));

  string input2 = "-";
  EXPECT_EQ(true, GrmUtilPtr->isOperator(input2));

  string input3 = "*";
  EXPECT_EQ(true, GrmUtilPtr->isOperator(input3));

  string input4 = "/";
  EXPECT_EQ(true, GrmUtilPtr->isOperator(input4));

  string input5 = "==";
  EXPECT_EQ(true, GrmUtilPtr->isOperator(input5));

  string input6 = "!=";
  EXPECT_EQ(true, GrmUtilPtr->isOperator(input6));

  string input7 = "+";
  EXPECT_EQ(true, GrmUtilPtr->isOperator(input7));

  string input8 = "+";
  EXPECT_EQ(true, GrmUtilPtr->isOperator(input8));
}
