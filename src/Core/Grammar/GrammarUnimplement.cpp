#include "GrammarAnalyzer.h"

uint32 GrammarAnalyzer::handleTrailingReturenType(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleNewExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleFunctionTryBlock(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleExceptionSpeciafier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleIterationStatement(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLastA = index;
  bool retA = EXPECT(index, tryLastA, "while", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLastA + 1, tryLastA, "(", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Condition, tryLastA + 1, tryLastA, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLastA + 1, tryLastA, ")", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Statement, tryLastA + 1, tryLastA, curBlock, returner, NOT_OPT);
  if (retA)
  {
    lastIndex = tryLastA;
    return eGrmErrNoError;
  }

  int32 tryLastB = index;
  bool retB = EXPECT(index, tryLastB, "do", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Statement, tryLastB + 1, tryLastB, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLastB + 1, tryLastB, "while", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLastB + 1, tryLastB, "(", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Condition, tryLastB + 1, tryLastB, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLastB + 1, tryLastB, ")", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLastB + 1, tryLastB, ";", NOT_OPT, NOT_IN_ONE_LINE);
  if (retB)
  {
    lastIndex = tryLastB;
    return eGrmErrNoError;
  }

  int32 tryLastC = index;
  bool retC = EXPECT(index, tryLastC, "for", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLastC + 1, tryLastC, "(", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(ForInitStatement, tryLastC + 1, tryLastC, curBlock, returner, NOT_OPT) &&
    INVOKE(Condition, tryLastC + 1, tryLastC, curBlock, returner, IS_OPT) &&
    EXPECT(tryLastC + 1, tryLastC, ";", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Expression, tryLastC + 1, tryLastC, curBlock, returner, IS_OPT) &&
    EXPECT(tryLastC + 1, tryLastC, ")", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Statement, tryLastC + 1, tryLastC, curBlock, returner, NOT_OPT);
  if (retC)
  {
    lastIndex = tryLastC;
    return eGrmErrNoError;
  }

  int32 tryLastD = index;
  bool retD = EXPECT(index, tryLastD, "for", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLastD + 1, tryLastD, "(", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(ForRangeDeclaration, tryLastD + 1, tryLastD, curBlock, returner, NOT_OPT) &&
    INVOKE(ForRangeInitializer, tryLastD + 1, tryLastD, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLastD + 1, tryLastD, ")", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Statement, tryLastD + 1, tryLastD, curBlock, returner, NOT_OPT);
  if (retD)
  {
    lastIndex = tryLastD;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleTryBlock(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleDeclarationStatement(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleUsingDirective(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleStatic_assertDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleAliasDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleTemplateDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  
  return eGrmErrUnknown;
}


uint32 GrammarAnalyzer::handleExplicitSpecification(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleExplicitInstantiation(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  return eGrmErrUnknown;
}


uint32 GrammarAnalyzer::handleForRangeInitializer(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleForInitStatement(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleForRangeDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  return eGrmErrUnknown;
}
