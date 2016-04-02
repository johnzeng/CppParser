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
