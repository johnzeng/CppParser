#include "GrammarAnalyzer.h"
#include "GrammarUtil.h"
#include "LexUtil.h"
#include "JZLogger.h"

GrammarAnalyzer::GrammarAnalyzer(LexRecList list):
  mRecList(list),
  mTopBlock(GrammarBlock::createTopNode())
{
}

GrammarAnalyzer::~GrammarAnalyzer()
{
}

uint32 GrammarAnalyzer::doAnalyze()
{
  int index = 0;
  int listSize = mRecList.size();
  while (index < listSize)
  {
    int32 nextIndex = index;
    uint32 ret = eGrmErrNoError;
    if(eGrmErrNoError != (ret = blockHeartBeat(index, nextIndex, &mTopBlock) ))
    {
      return ret;
    }
    index = nextIndex + 1;
  }
  return eGrmErrNoError;
}

uint32 GrammarAnalyzer::blockHeartBeat(int32 index, int32& lastIndex, GrammarBlock* curBlock)
{
  JZFUNC_BEGIN_LOG();
  int nextIndex = index;
  uint32 ret = handleEnum(index, nextIndex, curBlock) /* || handleClass*/;
  if (ret == eGrmErrNoError)
  {
    lastIndex = nextIndex;
    JZFUNC_END_LOG();
    return ret;
  }
  JZFUNC_END_LOG();
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleEnum(int32 index, int32& lastIndex, GrammarBlock* curBlock)
{
  JZFUNC_BEGIN_LOG();
  if(mRecList.size() <= index)
  {
    JZFUNC_END_LOG();
    return eGrmErrFileEnd;
  }
  if (mRecList[index].word != "enum")
  {
    JZWRITE_DEBUG("get key word %s", mRecList[index].word.c_str());
    JZFUNC_END_LOG();
    return eGrmErrNotEnum;
  }
  lastIndex = index;
  JZFUNC_END_LOG();

  uint32 attRet = handleAttributes(index + 1, lastIndex, curBlock);
  if(attRet != eGrmErrNoError && attRet != eGrmErrNotAttri)
  {
    return attRet;
  }
  return handleEnumId(lastIndex + 1, lastIndex, curBlock);
}

uint32 GrammarAnalyzer::handleEnumId(int index, int& lastIndex, GrammarBlock* curBlock)
{
  if(mRecList.size() <= index)
  {
    return eGrmErrFileEnd;
  }
  //need to check the id
  if (false == isLegalVarIdentify(mRecList[index].word, curBlock))
  {
    return eGrmErrDoubleDefinedDataType;
  }
  lastIndex = index;
  string id = mRecList[index].word;
  EnumDefine *curEnum = new EnumDefine(id);

  uint32 ret = expect(";",index + 1);

  if (ret == eGrmErrNoError)
  {
    curEnum->setFather(curBlock);
    lastIndex = index;
    JZFUNC_END_LOG();
    return ret;
  }

  GrammarBlock* body = new GrammarBlock();
  curEnum->addBody(body);

  ret = handleLeftBrace(index + 1, lastIndex, body);
  if (ret == eGrmErrNoError)
  {
    curBlock->addDataTypeDefine(curEnum);
    JZFUNC_END_LOG();
    return ret;
  }

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleLeftBrace(int index, int& lastIndex, GrammarBlock* curBlock)
{
  if(mRecList.size() <= index)
  {
    return eGrmErrFileEnd;
  }
  //need to check the id
  if (mRecList[index].word != "{")
  {
    return eGrmErrNotLeftBrace;
  }

  auto father = curBlock->getFather();
  uint32 fatherNodeType = father == NULL ? eGrammarBlockTop : father->getType();

  lastIndex = index;
  uint32 ret = eGrmErrNoError;
  switch(fatherNodeType)
  {
    case eGrammarNodeTopNode:
      ret = blockHeartBeat(index + 1, lastIndex, curBlock);
      break;
    case eDataTypeEnum:
      ret = handleEnumFieldName(index + 1, lastIndex, curBlock);
      break;
    default:
    {
      break;  
    }
  }
  return ret;
}

uint32 GrammarAnalyzer::handleEnumFieldName(int index, int& lastIndex, GrammarBlock* curBlock)
{
  if(mRecList.size() <= index)
  {
    return eGrmErrFileEnd;
  }
  lastIndex = index;

  if(expect("}", index) == eGrmErrNoError)
  {
    if(expect(";", index + 1) == eGrmErrNoError)
    {
      lastIndex++;
      return eGrmErrNoError;
    }
    else
    {
      JZWRITE_ERROR("missing ; at the end of enum define");
      return eGrmErrMissingSemicolon;
    }
  }

  if(expect(",", index) == eGrmErrNoError)
  {
    if(index - 1 < 0){
      JZFUNC_END_LOG();
      return eGrmErrUnknown;
    }
    if (mRecList[index - 1].word == ",")
    {
      return eGrmErrUnexpectedCommon;
    }
    return handleEnumFieldName(index + 1, lastIndex, curBlock);
  }

  if(true == isLegalVarIdentify(mRecList[index].word,curBlock))
  {
    string fieldName = mRecList[index].word;
    EnumDefine *define = dynamic_cast<EnumDefine*>(curBlock->getFather());
    if(NULL == define)
    {
      return eGrmErrUnknown;
    }
    if(expect("=", index + 1) == eGrmErrNoError)
    {
      uint32 value = 0;
      DataTypeDefine typeRet;
      uint32 getValeRet = handleStatement(index + 2, lastIndex, curBlock, typeRet);
      if(getValeRet != eGrmErrNoError)
      {
        //should check value type here;
        return getValeRet;
      }
      uint32 addRet = define->addField(fieldName,value);
      if(addRet != eGrmErrNoError)
      {
        return addRet;
      }
      return handleEnumFieldName(lastIndex + 1, lastIndex, curBlock);
    }
    else
    {
      uint32 addRet = define->addField(fieldName);
      if(addRet != eGrmErrNoError)
      {
        return addRet;
      }
      lastIndex = index + 1;
      return handleEnumFieldName(index + 2, lastIndex, curBlock);
    }
  }

  return eGrmErrNoError;
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

bool GrammarAnalyzer::isLegalVarIdentify(const string& id, GrammarBlock* curBlock)
{
  const VarDefine* def = curBlock->getVarDefInBlock(id);
  if(NULL != def)
  {
    return false;
  }

  bool isKey = GrmUtilPtr->isKeyWord(id);
  if(true == isKey)
  {
    return false;
  }

  for(int i = 0 ; i < id.size() ; i++)
  {
    if(i == 0)
    {
      if (true == LexUtil::isConstNumberChar(id[i]))
      {
        //should not start with number
        return false;
      }
    }
    char curChar = id[i];
    if (false == LexUtil::isIdentifyChar(curChar))
    {
      return false;
    }
  }
  return true;
}

uint32 GrammarAnalyzer::handleStatement(int index, int& lastIndex, GrammarBlock* curBlock, DataTypeDefine& retType)
{

  //begin with (
  uint32 expLeftRet = expect("(",index);
  if(eGrmErrNoError == expLeftRet)
  {
    uint32 stRet = handleStatement(index + 1, lastIndex, curBlock, retType);
    if(eGrmErrNoError == stRet)
    {
      uint32 expRightRet = expect(")", lastIndex + 1);
      if(eGrmErrNoError == expRightRet)
      {
        lastIndex = lastIndex + 1;
        return eGrmErrNoError;
      }
      else{
        return expRightRet;
      }
    }
    else
    {
      return stRet;
    }
  }

  lastIndex = index;
  return eGrmErrNoError;
}

GrammarBlock* GrammarAnalyzer::getTopBlock()
{
  return &mTopBlock;
}

//uint32 GrammarAnalyzer::expectAConstInt(int index, int& lastIndex, GrammarBlock* curBlock)
//{
//  
//}

uint32 GrammarAnalyzer::handleCVQualifierSeq(int index, int& lastIndex, GrammarBlock* curBlock)
{
  uint32 getRet1 = eGramIsNothing;
  uint32 errRet1 = getCVQualifier(index,lastIndex, getRet1);
  if(eGrmErrNoError != errRet1)
  {
    //it's reasonable to get nothing here
    return eGrmErrNoError;
  }
  
  uint32 getRet2 = eGramIsNothing;
  uint32 errRet2 = getCVQualifier(index + 1,lastIndex, getRet2);
  if(eGrmErrNoError != errRet2)
  {
    //it's reasonable to get nothing here
    return eGrmErrNoError;
  }

  if(getRet1 == getRet2)
  {
    return eGrmErrDoubleCVQualifier;
  }
  //should add more code to attach CV-qualifier to the following block
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

uint32 GrammarAnalyzer::handleAttributes(int index, int& lastIndex, GrammarBlock* curBlock)
{
  uint32 expDoubleSquareBrackets = expect("[[", index);
  if(eGrmErrNoError == expDoubleSquareBrackets)
  {
    uint32 i = 1;
    uint32 expNextDoubleBrackets = expect("]]", index + i);
    if(eGrmErrNoError == expNextDoubleBrackets)
    {
      return eGrmErrUnexpDblSqlBracket;
    }

    do
    {
      if(expNextDoubleBrackets == eGrmErrFileEnd)
      {
        return eGrmErrExpectNextDblSqBracket;
      }
      i++;
      expNextDoubleBrackets = expect("]]", index + i);
    }
    while(expNextDoubleBrackets != eGrmErrNoError);
    lastIndex = index + i;

    uint32 nextAttRet = handleAttributes(lastIndex + 1, lastIndex, curBlock);

    if(eGrmErrNotAttri == nextAttRet || eGrmErrNoError == nextAttRet)
    {
      return eGrmErrNoError;
    }
    else
    {
      return nextAttRet;
    }
  }

  uint32 expAlignas = expect("alignas" ,index);
  if(eGrmErrNoError == expAlignas)
  {
    uint32 expLeftBracket = expect("(", index + 1);
    uint32 i = 2;
    uint32 expRightBracket = expect(")", index + i);
    if(expRightBracket == eGrmErrNoError)
    {
      return eGrmErrUnexpRightBracket;
    }

    do
    {
      if(expRightBracket == eGrmErrFileEnd)
      {
        return eGrmErrExpectNextDblSqBracket;
      }
      i++;
      expRightBracket = expect(")", index + i);
    }
    while (eGrmErrNoError != expRightBracket);

    lastIndex = index + i;

    uint32 nextAttRet = handleAttributes(lastIndex + 1, lastIndex, curBlock);

    if(eGrmErrNotAttri == nextAttRet || eGrmErrNoError == nextAttRet)
    {
      return eGrmErrNoError;
    }
    else
    {
      return nextAttRet;
    }
  }

  return eGrmErrNotAttri;

}
