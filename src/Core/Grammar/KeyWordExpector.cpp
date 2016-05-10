#include "GrammarAnalyzer.h"
#include "GrammarUtil.h"
#include "LexUtil.h"
#include "JZLogger.h"

uint32 GrammarAnalyzer::getFunctionSpecifier(int index, int& lastIndex, uint32 &ret)
{
  ret = eGramIsNothing;
  uint32 expInline = expect("inline", index);
  if (eGrmErrNoError == expInline)
  {
    lastIndex = index;
    ret = eGramIsInline;
    return eGrmErrNoError;
  }
  
  uint32 expVirtual = expect("virtual", index);
  if (eGrmErrNoError == expVirtual)
  {
    lastIndex = index;
    ret = eGramIsVirtual;
    return eGrmErrNoError;
  }

  uint32 expExplicit = expect("explicit", index);
  if (eGrmErrNoError == expExplicit)
  {
    lastIndex = index;
    ret = eGramIsExplicit;
    return eGrmErrNoError;
  }
  return eGrmErrNotFunctionSpecifier;
}

uint32 GrammarAnalyzer::getStorageClassSpecifier(int index, int& lastIndex, uint32 &ret)
{
  ret = eGramIsNothing;
  //removed in cpp 11,so need more checking here
  uint32 expAuto = expect("auto", index);
  if (eGrmErrNoError == expAuto)
  {
    lastIndex = index;
    ret = eGramIsAuto;
    return eGrmErrNoError;
  }
  
  uint32 expRegister = expect("register", index);
  if (eGrmErrNoError == expRegister)
  {
    lastIndex = index;
    ret = eGramIsRegister;
    return eGrmErrNoError;
  }

  uint32 expStatic = expect("static", index);
  if (eGrmErrNoError == expRegister)
  {
    lastIndex = index;
    ret = eGramIsStatic;
    return eGrmErrNoError;
  }

  uint32 expThreadLocal = expect("thread_local", index);
  if (eGrmErrNoError == expRegister)
  {
    lastIndex = index;
    ret = eGramIsThreadLocal;
    return eGrmErrNoError;
  }

  uint32 expExtern = expect("extern", index);
  if (eGrmErrNoError == expExtern)
  {
    lastIndex = index;
    ret = eGramIsExtern;
    return eGrmErrNoError;
  }

  uint32 expMutable = expect("mutable", index);
  if (eGrmErrNoError == expRegister)
  {
    lastIndex = index;
    ret = eGramIsMutable;
    return eGrmErrNoError;
  }

  return eGrmErrNotStorageClassSpecifier;
  
}

uint32 GrammarAnalyzer::expect(const string& expected, int index, bool oneLine)
{
  if(mRecList.size() <= index)
  {
    return eGrmErrFileEnd;
  }
  //need to check the id
  if (mRecList[index].word != expected)
  {
    JZWRITE_DEBUG("expected %s not match", expected.c_str());
    return eGrmErrNotExpected;
  }
  return eGrmErrNoError;
}

uint32 GrammarAnalyzer::getCVQualifier(int index, int& lastIndex, uint32 &ret)
{
  ret = eGramIsNothing;
  uint32 expConst = expect("const", index);
  if (eGrmErrNoError == expConst)
  {
    lastIndex = index;
    ret = eGramIsConst;
    return eGrmErrNoError;
  }
  
  uint32 expVolatile = expect("volatile", index);
  if (eGrmErrNoError == expVolatile)
  {
    lastIndex = index;
    ret = eGramIsVolatile;
    return eGrmErrNoError;
  }
  return eGrmErrNotCVQualifier;
}

uint32 GrammarAnalyzer::getUnaryOperator(int index, int& lastIndex, uint32& ret)
{
  ret = eGramIsNothing;
  uint32 expStar = expect("*", index);
  if (eGrmErrNoError == expStar)
  {
    lastIndex = index;
    ret = eGramIsStar;
    return eGrmErrNoError;
  }
  
  uint32 expAnd = expect("&", index);
  if (eGrmErrNoError == expAnd)
  {
    lastIndex = index;
    ret = eGramIsAnd;
    return eGrmErrNoError;
  }

  uint32 expPlus = expect("+", index);
  if (eGrmErrNoError == expPlus)
  {
    lastIndex = index;
    ret = eGramIsPlus;
    return eGrmErrNoError;
  }

  uint32 expMinus = expect("-", index);
  if (eGrmErrNoError == expMinus)
  {
    lastIndex = index;
    ret = eGramIsMinus;
    return eGrmErrNoError;
  }

  uint32 expBone = expect("!", index);
  if (eGrmErrNoError == expBone)
  {
    lastIndex = index;
    ret = eGramIsBone;
    return eGrmErrNoError;
  }

  uint32 expWave = expect("~", index);
  if (eGrmErrNoError == expWave)
  {
    lastIndex = index;
    ret = eGramIsWave;
    return eGrmErrNoError;
  }
  return eGrmErrNotUnaryOperator;
  
}

