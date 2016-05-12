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
 
uint32 GrammarAnalyzer::getAccessSpecifier(int index, int& lastIndex, uint32 &ret)
{
  
  uint32 expPrivate = expect("private", index);
  if (eGrmErrNoError == expPrivate)
  {
    lastIndex = index;
    ret = eGramIsPrivate;
    return eGrmErrNoError;
  }

  uint32 expPublic = expect("public", index);
  if (eGrmErrNoError == expPublic)
  {
    lastIndex = index;
    ret = eGramIsPublic;
    return eGrmErrNoError;
  }

  uint32 expProtected = expect("protected", index);
  if (eGrmErrNoError == expProtected)
  {
    lastIndex = index;
    ret = eGramIsProtected;
    return eGrmErrNoError;
  }
  return eGrmErrNotAccessSpecifier;
}

uint32 GrammarAnalyzer::getPureSpecifier(int index, int& lastIndex, uint32 &ret)
{
  uint32 expEqual = expect("=", index);
  if (eGrmErrNoError == expEqual)
  {
    uint32 expZero = expect("0", index + 1);
    if(eGrmErrNoError == expZero)
    {
      ret = eGramIsPureSpecifier;
      lastIndex = index + 1;
      return eGrmErrNoError;
    }
  }

  return eGrmErrNotPure;

}

uint32 GrammarAnalyzer::getVirtSpecifier(int index, int& lastIndex, uint32 &ret)
{
  uint32 expOverride = expect("override", index);
  if (eGrmErrNoError == expOverride)
  {
    lastIndex = index;
    ret = eGramIsOverride;
    return eGrmErrNoError;
  }

  uint32 expFinal = expect("final", index);
  if (eGrmErrNoError == expFinal)
  {
    lastIndex = index;
    ret = eGramIsFinal;
    return eGrmErrNoError;
  }

  uint32 expNew = expect("new", index);
  if (eGrmErrNoError == expFinal)
  {
    lastIndex = index;
    ret = eGramIsNew;
    return eGrmErrNoError;
  }
  return eGrmErrNotUnaryOperator;
  
}

uint32 GrammarAnalyzer::getAssignmentOperator(int index, int& lastIndex, uint32 &ret)
{
  //refactor: change exp name and ret value
  uint32 exp1 = expect("=", index);
  if (eGrmErrNoError == exp1)
  {
    lastIndex = index;
    ret = eGramIsNothing;
    return eGrmErrNoError;
  }

  uint32 exp2 = expect("*=", index);
  if (eGrmErrNoError == exp2)
  {
    lastIndex = index;
    ret = eGramIsNothing;
    return eGrmErrNoError;
  }

  uint32 exp3 = expect("/=", index);
  if (eGrmErrNoError == exp3)
  {
    lastIndex = index;
    ret = eGramIsNothing;
    return eGrmErrNoError;
  }

  uint32 exp4 = expect("%=", index);
  if (eGrmErrNoError == exp4)
  {
    lastIndex = index;
    ret = eGramIsNothing;
    return eGrmErrNoError;
  }

  uint32 exp5 = expect("+=", index);
  if (eGrmErrNoError == exp5)
  {
    lastIndex = index;
    ret = eGramIsNothing;
    return eGrmErrNoError;
  }

  uint32 exp6 = expect("-=", index);
  if (eGrmErrNoError == exp6)
  {
    lastIndex = index;
    ret = eGramIsNothing;
    return eGrmErrNoError;
  }

  uint32 exp7 = expect(">>=", index);
  if (eGrmErrNoError == exp7)
  {
    lastIndex = index;
    ret = eGramIsNothing;
    return eGrmErrNoError;
  }

  uint32 exp8 = expect("<<=", index);
  if (eGrmErrNoError == exp8)
  {
    lastIndex = index;
    ret = eGramIsNothing;
    return eGrmErrNoError;
  }

  uint32 exp9 = expect("%=", index);
  if (eGrmErrNoError == exp9)
  {
    lastIndex = index;
    ret = eGramIsNothing;
    return eGrmErrNoError;
  }

  uint32 exp10 = expect("^=", index);
  if (eGrmErrNoError == exp10)
  {
    lastIndex = index;
    ret = eGramIsNothing;
    return eGrmErrNoError;
  }

  uint32 exp11 = expect("|=", index);
  if (eGrmErrNoError == exp11)
  {
    lastIndex = index;
    ret = eGramIsNothing;
    return eGrmErrNoError;
  }

  return eGrmErrNotUnaryOperator;
  
}
