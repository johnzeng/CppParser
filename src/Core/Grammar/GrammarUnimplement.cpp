#include "GrammarAnalyzer.h"

uint32 GrammarAnalyzer::getLiteral(int index, int& lastIndex, uint32 &ret)
{
  //I am going to handle this function by my self, not by bnf because I already
  return eGrmErrNoError;
}

uint32 GrammarAnalyzer::handleParameterAndQualifiers(int index, int& lastIndex, GrammarBlock* curBlock)
{

  return eGrmErrNoError;
}
uint32 GrammarAnalyzer::handleTrailingReturenType(int index, int& lastIndex, GrammarBlock* curBlock)
{

  return eGrmErrNoError;
}

uint32 GrammarAnalyzer::handleClassSpecifier(int index, int& lastIndex, GrammarBlock* curBlock)
{
  //should be same as handleClass
  return eGrmErrNoError;
}

uint32 GrammarAnalyzer::handleNewExpression(int index, int& lastIndex, GrammarBlock* curBlock)
{
  return eGrmErrNoError;
}

uint32 GrammarAnalyzer::handleCtorInitializer(int index, int& lastIndex, GrammarBlock* curBlock)
{
  return eGrmErrNoError;
}

uint32 GrammarAnalyzer::handleFunctionTryBlock(int index, int& lastIndex, GrammarBlock* curBlock)
{
  return eGrmErrNoError;
}

uint32 GrammarAnalyzer::handleStatement(int index, int& lastIndex, GrammarBlock* curBlock)
{
  lastIndex = index;
  return eGrmErrNoError;
}
