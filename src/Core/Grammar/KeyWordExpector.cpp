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

uint32 GrammarAnalyzer::getOverloadableOperator(int index, int& lastIndex, uint32 &ret)
{
// this func haven't handle the the ret value yet
  ret = eGramIsNothing;
  uint32 newExp = expect("new", index);
  if (eGrmErrNoError == newExp)
  {
    lastIndex = index;
    ret = eGramIsNew;
    uint32 expLeft = expect("[", index + 1);
    if (eGrmErrNoError == expLeft)
    {
      uint32 expRight = expect("]", index + 2);
      if (eGrmErrNoError == expRight)
      {
        lastIndex = index + 2;
        return eGrmErrNoError;
      }
    }
    return eGrmErrNoError;
  }

  uint32 deleteExp = expect("delete", index);
  if (eGrmErrNoError == deleteExp)
  {
    lastIndex = index;
    ret = eGramIsNew;
    uint32 expLeft = expect("[", index + 1);
    if (eGrmErrNoError == expLeft)
    {
      uint32 expRight = expect("]", index + 2);
      if (eGrmErrNoError == expRight)
      {
        lastIndex = index + 2;
        return eGrmErrNoError;
      }
    }
    return eGrmErrNoError;
  }

  uint32 op1Exp = expect("+", index);
  if (eGrmErrNoError == op1Exp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }

  uint32 op2Exp = expect("-", index);
  if (eGrmErrNoError == op2Exp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }

  uint32 op3Exp = expect("*", index);
  if (eGrmErrNoError == op3Exp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }

  uint32 op4Exp = expect("/", index);
  if (eGrmErrNoError == op4Exp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }

  uint32 op5Exp = expect("%", index);
  if (eGrmErrNoError == op5Exp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }

  uint32 op6Exp = expect("^", index);
  if (eGrmErrNoError == op6Exp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }

  uint32 op7Exp = expect("&", index);
  if (eGrmErrNoError == op7Exp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }

  uint32 op8Exp = expect("|", index);
  if (eGrmErrNoError == op8Exp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }

  uint32 op9Exp = expect("~", index);
  if (eGrmErrNoError == op9Exp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }

  uint32 op10Exp = expect("!", index);
  if (eGrmErrNoError == op10Exp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }

  uint32 op11Exp = expect("=", index);
  if (eGrmErrNoError == op11Exp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }

  uint32 op12Exp = expect("<", index);
  if (eGrmErrNoError == op12Exp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }

  uint32 op13Exp = expect(">", index);
  if (eGrmErrNoError == op13Exp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }

  uint32 op14Exp = expect("+=", index);
  if (eGrmErrNoError == op14Exp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }

  uint32 op15Exp = expect("-=", index);
  if (eGrmErrNoError == op15Exp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }

  uint32 op16Exp = expect("*=", index);
  if (eGrmErrNoError == op16Exp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }

  uint32 op17Exp = expect("/=", index);
  if (eGrmErrNoError == op17Exp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }

  uint32 op18Exp = expect("%=", index);
  if (eGrmErrNoError == op18Exp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }

  uint32 op19Exp = expect("^=", index);
  if (eGrmErrNoError == op19Exp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }

  uint32 op20Exp = expect("&=", index);
  if (eGrmErrNoError == op20Exp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }

  uint32 op21Exp = expect("|=", index);
  if (eGrmErrNoError == op21Exp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }

  uint32 op22Exp = expect("<<", index);
  if (eGrmErrNoError == op22Exp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }

  uint32 op23Exp = expect(">>", index);
  if (eGrmErrNoError == op23Exp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }

  uint32 op24Exp = expect(">>=", index);
  if (eGrmErrNoError == op24Exp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }

  uint32 op25Exp = expect("<<=", index);
  if (eGrmErrNoError == op25Exp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }

  uint32 op26Exp = expect("==", index);
  if (eGrmErrNoError == op26Exp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }

  uint32 op27Exp = expect("!=", index);
  if (eGrmErrNoError == op27Exp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }

  uint32 op28Exp = expect("<=", index);
  if (eGrmErrNoError == op28Exp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }

  uint32 op29Exp = expect(">=", index);
  if (eGrmErrNoError == op29Exp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }

  uint32 op30Exp = expect("&&", index);
  if (eGrmErrNoError == op30Exp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }

  uint32 op31Exp = expect("||", index);
  if (eGrmErrNoError == op31Exp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }

  uint32 op32Exp = expect("++", index);
  if (eGrmErrNoError == op32Exp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }

  uint32 op33Exp = expect("--", index);
  if (eGrmErrNoError == op33Exp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }

  uint32 op34Exp = expect(",", index);
  if (eGrmErrNoError == op34Exp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }

  uint32 op35Exp = expect("->*", index);
  if (eGrmErrNoError == op35Exp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }

  uint32 op36Exp = expect("->", index);
  if (eGrmErrNoError == op36Exp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }

  uint32 op37Exp = expect("(", index);
  if (eGrmErrNoError == op37Exp)
  {
    uint32 expRight = expect(")", index + 1);
    if (eGrmErrNoError == expRight)
    {
      lastIndex = index + 1;
      return eGrmErrNoError;
    }
  }

  uint32 op38Exp = expect("[", index);
  if (eGrmErrNoError == op38Exp)
  {
    uint32 expRight = expect("]", index + 1);
    if (eGrmErrNoError == expRight)
    {
      lastIndex = index + 1;
      return eGrmErrNoError;
    }
    return eGrmErrNoError;
  }
  return eGrmErrNotOverloadableOperator;
}
