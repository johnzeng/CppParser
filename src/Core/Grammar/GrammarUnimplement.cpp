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

uint32 GrammarAnalyzer::handleLabeledStatement(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleSelectionStatement(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleIterationStatement(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleJumpStatement(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
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

uint32 GrammarAnalyzer::handleElaboratedTypeSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLastA = index;
  bool retA = INVOKE(ClassKey, tryLastA, tryLastA , curBlock, returner, NOT_OPT) &&
    INVOKE(Attributes, tryLastA + 1, tryLastA, curBlock, returner, IS_OPT) &&
    EXPECT(tryLastA + 1, tryLastA, "::", IS_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(NestNameSpecifier, tryLastA + 1, tryLastA, curBlock, returner, IS_OPT) &&
    INVOKE(Identifier, tryLastA  + 1, tryLastA, curBlock, returner, NOT_OPT);
  if (retA)
  {
    lastIndex = tryLastA;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}
