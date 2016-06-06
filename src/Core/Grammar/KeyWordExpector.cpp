#include "GrammarAnalyzer.h"
#include "GrammarUtil.h"
#include "LexUtil.h"
#include "JZLogger.h"
#include "GrammarReturner.h"

uint32 GrammarAnalyzer::handleRefQualifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret)
{

  uint32 exp1 = expect("&", index);
  if (eGrmErrNoError == exp1)
  {
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eRefQualifier, "&");
      ret -> addChild(base);
    }
    lastIndex = index;
    return eGrmErrNoError;
  }
  
  uint32 exp2 = expect("&&", index);
  if (eGrmErrNoError == exp2)
  {
    lastIndex = index;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eRefQualifier, "&&");
      ret -> addChild(base);
    }
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleFunctionSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret)
{
//  ret = eGramIsNothing;
  uint32 expInline = expect("inline", index);
  if (eGrmErrNoError == expInline)
  {
    lastIndex = index;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eFunctionSpecifier, "inline");
      ret -> addChild(base);
    }
    return eGrmErrNoError;
  }
  
  uint32 expVirtual = expect("virtual", index);
  if (eGrmErrNoError == expVirtual)
  {
    lastIndex = index;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eFunctionSpecifier, "virtual");
      ret -> addChild(base);
    }
    return eGrmErrNoError;
  }

  uint32 expExplicit = expect("explicit", index);
  if (eGrmErrNoError == expExplicit)
  {
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eFunctionSpecifier, "explicit");
      ret -> addChild(base);
    }
    lastIndex = index;
    return eGrmErrNoError;
  }
  return eGrmErrNotFunctionSpecifier;
}

uint32 GrammarAnalyzer::handleStorageClassSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret)
{
  uint32 expAuto = expect("auto", index);
  if (eGrmErrNoError == expAuto)
  {
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eStorageClassSpecifier, "auto");
      ret -> addChild(base);
    }
    lastIndex = index;
    return eGrmErrNoError;
  }
  
  uint32 expRegister = expect("register", index);
  if (eGrmErrNoError == expRegister)
  {
    lastIndex = index;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eStorageClassSpecifier, "register");
      ret -> addChild(base);
    }
    return eGrmErrNoError;
  }

  uint32 expStatic = expect("static", index);
  if (eGrmErrNoError == expStatic)
  {
    lastIndex = index;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eStorageClassSpecifier, "static");
      ret -> addChild(base);
    }
    return eGrmErrNoError;
  }

  uint32 expThreadLocal = expect("thread_local", index);
  if (eGrmErrNoError == expThreadLocal)
  {
    lastIndex = index;
//    ret = eGramIsThreadLocal;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eStorageClassSpecifier, "thread_local");
      ret -> addChild(base);
    }
    return eGrmErrNoError;
  }

  uint32 expExtern = expect("extern", index);
  if (eGrmErrNoError == expExtern)
  {
    lastIndex = index;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eStorageClassSpecifier, "extern");
      ret -> addChild(base);
    }
    return eGrmErrNoError;
  }

  uint32 expMutable = expect("mutable", index);
  if (eGrmErrNoError == expMutable)
  {
    lastIndex = index;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eStorageClassSpecifier, "mutable");
      ret -> addChild(base);
    }
//    ret = eGramIsMutable;
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
//    JZWRITE_DEBUG("expected %s not match", expected.c_str());
    return eGrmErrNotExpected;
  }
  return eGrmErrNoError;
}

uint32 GrammarAnalyzer::handleCVQualifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret)
{
  uint32 expConst = expect("const", index);
  if (eGrmErrNoError == expConst)
  {
    lastIndex = index;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eCVQualifier, "const");
      ret -> addChild(base);
    }
    return eGrmErrNoError;
  }
  
  uint32 expVolatile = expect("volatile", index);
  if (eGrmErrNoError == expVolatile)
  {
    lastIndex = index;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eCVQualifier, "volatile");
      ret -> addChild(base);
    }
    return eGrmErrNoError;
  }
  return eGrmErrNotCVQualifier;
}

uint32 GrammarAnalyzer::handleUnaryOperator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret)
{
//  ret = eGramIsNothing;
  uint32 expStar = expect("*", index);
  if (eGrmErrNoError == expStar)
  {
    lastIndex = index;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eUnaryOperator, "*");
      ret -> addChild(base);
    }
    return eGrmErrNoError;
  }
  
  uint32 expAnd = expect("&", index);
  if (eGrmErrNoError == expAnd)
  {
    lastIndex = index;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eUnaryOperator, "&");
      ret -> addChild(base);
    }
//    ret = eGramIsAnd;
    return eGrmErrNoError;
  }

  uint32 expPlus = expect("+", index);
  if (eGrmErrNoError == expPlus)
  {
    lastIndex = index;
//    ret = eGramIsPlus;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eUnaryOperator, "+");
      ret -> addChild(base);
    }
    return eGrmErrNoError;
  }

  uint32 expMinus = expect("-", index);
  if (eGrmErrNoError == expMinus)
  {
    lastIndex = index;
//    ret = eGramIsMinus;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eUnaryOperator, "-");
      ret -> addChild(base);
    }
    return eGrmErrNoError;
  }

  uint32 expBone = expect("!", index);
  if (eGrmErrNoError == expBone)
  {
    lastIndex = index;
//    ret = eGramIsBone;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eUnaryOperator, "!");
      ret -> addChild(base);
    }
    return eGrmErrNoError;
  }

  uint32 expWave = expect("~", index);
  if (eGrmErrNoError == expWave)
  {
    lastIndex = index;
//    ret = eGramIsWave;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eUnaryOperator, "~");
      ret -> addChild(base);
    }
    return eGrmErrNoError;
  }
  return eGrmErrNotUnaryOperator;
  
}
 
uint32 GrammarAnalyzer::handleAccessSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret)
{
  
  uint32 expPrivate = expect("private", index);
  if (eGrmErrNoError == expPrivate)
  {
    lastIndex = index;
//    ret = eGramIsPrivate;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eAccessSpecifier, "private");
      ret -> addChild(base);
    }
    return eGrmErrNoError;
  }

  uint32 expPublic = expect("public", index);
  if (eGrmErrNoError == expPublic)
  {
    lastIndex = index;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eAccessSpecifier, "public");
      ret -> addChild(base);
    }
//    ret = eGramIsPublic;
    return eGrmErrNoError;
  }

  uint32 expProtected = expect("protected", index);
  if (eGrmErrNoError == expProtected)
  {
    lastIndex = index;
//    ret = eGramIsProtected;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eAccessSpecifier, "protected");
      ret -> addChild(base);
    }
    return eGrmErrNoError;
  }
  return eGrmErrNotAccessSpecifier;
}

uint32 GrammarAnalyzer::handlePureSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret)
{
  uint32 expEqual = expect("=", index);
  if (eGrmErrNoError == expEqual)
  {
    uint32 expZero = expect("0", index + 1);
    if(eGrmErrNoError == expZero)
    {
//      ret = eGramIsPureSpecifier;
      if (ret)
      {
        GrammarReturnerBase* base = new GrammarReturnerBase(ePureSpecifier, "pure");
        ret -> addChild(base);
      }
      lastIndex = index + 1;
      return eGrmErrNoError;
    }
  }

  return eGrmErrNotPure;

}

uint32 GrammarAnalyzer::handleClassVirtSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret)
{
  uint32 expExplicit = expect("explicit", index);
  if (eGrmErrNoError == expExplicit)
  {
    lastIndex = index;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eClassVirtSpecifier, "explicit");
      ret -> addChild(base);
    }
    return eGrmErrNoError;
  }

  uint32 expFinal = expect("final", index);
  if (eGrmErrNoError == expFinal)
  {
    lastIndex = index;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eClassVirtSpecifier, "final");
      ret -> addChild(base);
    }
//    ret = eGramIsFinal;
    return eGrmErrNoError;
  }

  return eGrmErrUnknown;
  
}

uint32 GrammarAnalyzer::handleVirtSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret)
{
  uint32 expOverride = expect("override", index);
  if (eGrmErrNoError == expOverride)
  {
    lastIndex = index;
//    ret = eGramIsOverride;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eVirtSpecifier, "override");
      ret -> addChild(base);
    }
    return eGrmErrNoError;
  }

  uint32 expFinal = expect("final", index);
  if (eGrmErrNoError == expFinal)
  {
    lastIndex = index;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eVirtSpecifier, "final");
      ret -> addChild(base);
    }
//    ret = eGramIsFinal;
    return eGrmErrNoError;
  }

  uint32 expNew = expect("new", index);
  if (eGrmErrNoError == expFinal)
  {
    lastIndex = index;
//    ret = eGramIsNew;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eVirtSpecifier, "new");
      ret -> addChild(base);
    }
    return eGrmErrNoError;
  }
  return eGrmErrNotUnaryOperator;
  
}

uint32 GrammarAnalyzer::handleAssignmentOperator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret)
{
  //refactor: change exp name and ret value
  uint32 exp1 = expect("=", index);
  if (eGrmErrNoError == exp1)
  {
    lastIndex = index;
//    ret = eGramIsNothing;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eAssignmentOperator, "=");
      ret -> addChild(base);
    }
    return eGrmErrNoError;
  }

  uint32 exp2 = expect("*=", index);
  if (eGrmErrNoError == exp2)
  {
    lastIndex = index;
//    ret = eGramIsNothing;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eAssignmentOperator, "*=");
      ret -> addChild(base);
    }
    return eGrmErrNoError;
  }

  uint32 exp3 = expect("/=", index);
  if (eGrmErrNoError == exp3)
  {
    lastIndex = index;
//    ret = eGramIsNothing;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eAssignmentOperator, "/=");
      ret -> addChild(base);
    }
    return eGrmErrNoError;
  }

  uint32 exp4 = expect("%=", index);
  if (eGrmErrNoError == exp4)
  {
    lastIndex = index;
//    ret = eGramIsNothing;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eAssignmentOperator, "%=");
      ret -> addChild(base);
    }
    return eGrmErrNoError;
  }

  uint32 exp5 = expect("+=", index);
  if (eGrmErrNoError == exp5)
  {
    lastIndex = index;
//    ret = eGramIsNothing;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eAssignmentOperator, "+=");
      ret -> addChild(base);
    }
    return eGrmErrNoError;
  }

  uint32 exp6 = expect("-=", index);
  if (eGrmErrNoError == exp6)
  {
    lastIndex = index;
//    ret = eGramIsNothing;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eAssignmentOperator, "-=");
      ret -> addChild(base);
    }
    return eGrmErrNoError;
  }

  uint32 exp7 = expect(">>=", index);
  if (eGrmErrNoError == exp7)
  {
    lastIndex = index;
//    ret = eGramIsNothing;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eAssignmentOperator, ">>=");
      ret -> addChild(base);
    }
    return eGrmErrNoError;
  }

  uint32 exp8 = expect("<<=", index);
  if (eGrmErrNoError == exp8)
  {
    lastIndex = index;
//    ret = eGramIsNothing;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eAssignmentOperator, "<<=");
      ret -> addChild(base);
    }
    return eGrmErrNoError;
  }

  uint32 exp9 = expect("%=", index);
  if (eGrmErrNoError == exp9)
  {
    lastIndex = index;
//    ret = eGramIsNothing;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eAssignmentOperator, "%=");
      ret -> addChild(base);
    }
    return eGrmErrNoError;
  }

  uint32 exp10 = expect("^=", index);
  if (eGrmErrNoError == exp10)
  {
    lastIndex = index;
//    ret = eGramIsNothing;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eAssignmentOperator, "^=");
      ret -> addChild(base);
    }
    return eGrmErrNoError;
  }

  uint32 exp11 = expect("|=", index);
  if (eGrmErrNoError == exp11)
  {
    lastIndex = index;
//    ret = eGramIsNothing;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eAssignmentOperator, "!=");
      ret -> addChild(base);
    }
    return eGrmErrNoError;
  }

  return eGrmErrNotUnaryOperator;
  
}

uint32 GrammarAnalyzer::handleOverloadableOperator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret)
{
  if (index >= mRecList.size())
  {
    return eGrmErrFileEnd;
  }
  string key = mRecList[index].word;
  bool isOverloadable = GrmUtilPtr->isOverloadableOpt(key);
  if (isOverloadable)
  {
    switch(key[0])
    {
      //n for new, d for delete
      case 'n':
      case 'd':
      {
        lastIndex = index;
        uint32 expLeft = expect("[", index + 1);
        if (eGrmErrNoError == expLeft)
        {
          uint32 expRight = expect("]", index + 2);
          if (eGrmErrNoError == expRight)
          {
            if (ret)
            {
              GrammarReturnerBase* base = new GrammarReturnerBase(eOverloadableOperator, key + "[]");
              ret -> addChild(base);
            }
            lastIndex = index + 2;
            return eGrmErrNoError;
          }
        }
        return eGrmErrNoError;
      }
      break;
      case '[':
      {
        uint32 expRight = expect("]", index + 1);
        if (eGrmErrNoError == expRight)
        {
          lastIndex = index + 1;
          if (ret)
          {
            GrammarReturnerBase* base = new GrammarReturnerBase(eOverloadableOperator, "[]");
            ret -> addChild(base);
          }
          return eGrmErrNoError;
        }
      }
      break;
      case '(':
      {
        uint32 expRight = expect(")", index + 1);
        if (eGrmErrNoError == expRight)
        {
          lastIndex = index + 1;
          if (ret)
          {
            GrammarReturnerBase* base = new GrammarReturnerBase(eOverloadableOperator, "()");
            ret -> addChild(base);
          }
          return eGrmErrNoError;
        }
      }
      break;
      default:
      {
        lastIndex = index;
        if (ret)
        {
          GrammarReturnerBase* base = new GrammarReturnerBase(eOverloadableOperator, key);
          ret -> addChild(base);
        }
        return eGrmErrNoError;
      }
    }
    
  }
  return eGrmErrNotOverloadableOperator;
}

uint32 GrammarAnalyzer::handleEnumKey(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret)
{
//  ret = eGramIsNothing;
  uint32 exp1 = expect("enum", index);
  if (eGrmErrNoError == exp1)
  {
    lastIndex = index;
    uint32 expClass = expect("class", index + 1);
    if (eGrmErrNoError == expClass)
    {
      lastIndex = index + 1;
      if (ret)
      {
        GrammarReturnerBase* base = new GrammarReturnerBase(eEnumKey, "enum class");
        ret -> addChild(base);
      }
      return eGrmErrNoError;
    }
    uint32 expStruct = expect("struct", index + 1);
    if (eGrmErrNoError == expStruct)
    {
      lastIndex = index + 1;
      if (ret)
      {
        GrammarReturnerBase* base = new GrammarReturnerBase(eEnumKey, "enum struct");
        ret -> addChild(base);
      }
      return eGrmErrNoError;
    }
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eEnumKey, "enum");
      ret -> addChild(base);
    }
    return eGrmErrNoError;
  }
  
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleClassKey(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret)
{
//  ret = eGramIsNothing;
  uint32 exp1 = expect("class", index);
  if (eGrmErrNoError == exp1)
  {
    lastIndex = index;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eClassKey, "class");
      ret -> addChild(base);
    }
    return eGrmErrNoError;
  }
  
  uint32 exp2 = expect("struct", index);
  if (eGrmErrNoError == exp2)
  {
    lastIndex = index;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eClassKey, "struct");
      ret -> addChild(base);
    }
    return eGrmErrNoError;
  }

  uint32 exp3 = expect("union", index);
  if (eGrmErrNoError == exp3)
  {
    lastIndex = index;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eClassKey, "union");
      ret -> addChild(base);
    }
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleExternLiteral(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret)
{
  if (mRecList.size() <= index)
  {
    return eGrmErrFileEnd;
  }

  string word = mRecList[index].word;

  if (0 == word.size())
  {
    return eGrmErrUnknown;
  }
  if ("\"C\"" == word)
  {
    lastIndex = index;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eExternLiteral, "\"C\"");
      ret -> addChild(base);
    }
    return eGrmErrNoError;
  }
  if ("\"C++\"" == word)
  {
    lastIndex = index;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eExternLiteral, "\"C++\"");
      ret -> addChild(base);
    }
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleStringLiteral(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret)
{
  if (mRecList.size() <= index)
  {
    return eGrmErrFileEnd;
  }

  string word = mRecList[index].word;

  if (0 == word.size())
  {
    return eGrmErrUnknown;
  }
  if ('"' == word[0])
  {
    lastIndex = index;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleLiteral(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret)
{
  if (mRecList.size() <= index)
  {
    return eGrmErrFileEnd;
  }

  string word = mRecList[index].word;

  if (0 == word.size())
  {
    return eGrmErrUnknown;
  }
  if (eGrmErrNoError == handleStringLiteral(index, lastIndex, curBlock ))
  {
    return eGrmErrNoError;
  }
  else if('\'' == word[0])
  {
    lastIndex = index;
    return eGrmErrNoError;
  }
  else if("true" == word || "false" == word)
  {
    lastIndex = index;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eLiteral, word);
      ret -> addChild(base);
    }
    return eGrmErrNoError;
  }
  else if("nullptr" == word || "NULL" == word)
  {
    lastIndex = index;
    if (ret)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eLiteral, word);
      ret -> addChild(base);
    }
    return eGrmErrNoError;
  }
  else if(false == LexUtil::isInterpunction(word[0]) && false == GrmUtilPtr->isKeyWord(word))
  {
    if (true == GrmUtilPtr->isConstIntNumber(word))
    {
      lastIndex = index;
      if (ret)
      {
        GrammarReturnerBase* base = new GrammarReturnerBase(eLiteral, word);
        ret -> addChild(base);
      }
      return eGrmErrNoError;
    }
  }
  return eGrmErrUnknown;
}
