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

  int32 tryLastB = index;
  bool retB = INVOKE(ClassKey, tryLastB, tryLastB, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLastB + 1, tryLastB, "::", IS_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(NestNameSpecifier, tryLastB + 1, tryLastB, curBlock, returner, IS_OPT) &&
    EXPECT(tryLastB + 1, tryLastB, "template", IS_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(SimpleTemplateId, tryLastB + 1, tryLastB, curBlock, returner, IS_OPT) ;
  if (retB)
  {
    lastIndex = tryLastB;
    return eGrmErrNoError;
  }

  int32 trylastC = index;
  bool retC = EXPECT(trylastC, trylastC, "enum", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(trylastC + 1, trylastC, "::", IS_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(NestNameSpecifier, trylastC + 1, trylastC, curBlock, returner, IS_OPT) &&
    INVOKE(Identifier, trylastC + 1, trylastC, curBlock, returner, NOT_OPT) ;
  if (retC)
  {
    lastIndex = trylastC;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}
