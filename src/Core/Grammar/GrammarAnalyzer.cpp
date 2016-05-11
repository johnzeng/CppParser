#include "GrammarAnalyzer.h"
#include "GrammarUtil.h"
#include "LexUtil.h"
#include "JZLogger.h"

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
  int nextEnumIndex = index;
  uint32 ret = handleEnum(index, nextEnumIndex, curBlock) ;
  if (ret == eGrmErrNoError)
  {
    lastIndex = nextEnumIndex;
    JZFUNC_END_LOG();
    return ret;
  }

  int32 nextFuncIndex = index;
  uint32 funcRet = handleFuncDefinition(index, nextFuncIndex, curBlock);
  if (funcRet == eGrmErrNoError)
  {
    lastIndex = nextFuncIndex;
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
  //should handle enum class and enum struct here
  lastIndex = index;

  uint32 attRet = handleAttributes(index + 1, lastIndex, curBlock);
  if(attRet != eGrmErrNoError && attRet != eGrmErrNotAttri)
  {
    return attRet;
  }

  JZFUNC_END_LOG();
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

  //should hanlde enum 
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

uint32 GrammarAnalyzer::handleFuncDefinition(int index, int& lastIndex, GrammarBlock* curBlock)
{
  uint32 attRet = handleAttributes(index, lastIndex, curBlock);
  if(eGrmErrNotAttri != attRet && eGrmErrNoError != attRet)
  {
    return attRet;
  }
  uint32 decSpecifierSeqRet = handleDeclSpecifierSeq(lastIndex + 1, lastIndex, curBlock);
  if (eGrmErrNotDecSpecifierSeq != decSpecifierSeqRet && eGrmErrNoError != decSpecifierSeqRet)
  {
    return decSpecifierSeqRet;
  }
  return eGrmErrNoError;
}

uint32 GrammarAnalyzer::handleDeclSpecifierSeq(int index, int& lastIndex, GrammarBlock* curBlock)
{
  uint32 specifierRet = handleDeclSpecifier(index, lastIndex, curBlock);
  if (eGrmErrNoError != specifierRet)
  {
    return specifierRet;
  }
  uint32 attRet = handleAttributes(lastIndex + 1, lastIndex, curBlock);
  if(eGrmErrNotAttri == attRet)
  {
    uint32 nextRet = handleDeclSpecifierSeq(lastIndex + 1, lastIndex, curBlock);
    if (eGrmErrNotDecSpecifierSeq == nextRet)
    {
      return eGrmErrNoError;
    }
  }
  else if(eGrmErrNoError == attRet)
  {
    return eGrmErrNoError;
  }
  else
  {
    //not sure about this ret
    JZFUNC_END_LOG();
    return attRet;
  }
}

uint32 GrammarAnalyzer::handleDeclSpecifier(int index, int& lastIndex, GrammarBlock* curBlock)
{
  uint32 ret = eGramIsNothing;
  uint32 storageClassSpecRet = getStorageClassSpecifier(index, lastIndex , ret);
  if(eGrmErrNoError == storageClassSpecRet)
  {
    JZWRITE_DEBUG("shoulde add this property into seq");
    return eGrmErrNoError;
  }
  else if (eGrmErrNotStorageClassSpecifier != storageClassSpecRet)
  {
    return storageClassSpecRet;
  }

  uint32 funcSpecifierRet = getFunctionSpecifier(index, lastIndex , ret);
  if(eGrmErrNoError == funcSpecifierRet)
  {
    JZWRITE_DEBUG("shoulde add this property into seq");
    return eGrmErrNoError;
  }
  else if (eGrmErrNotFunctionSpecifier != funcSpecifierRet)
  {
    return funcSpecifierRet;
  }

  uint32 expFriend = expect("friend", index);
  if (eGrmErrNoError == expFriend)
  {
    lastIndex = index;
    //ret = eGramIsFriend;
    return eGrmErrNoError;
  }

  uint32 expTypeDef = expect("typedef", index);
  if (eGrmErrNoError == expTypeDef)
  {
    lastIndex = index;
    //ret = eGramIsTypedef;
    return eGrmErrNoError;
  }

  uint32 expContExpr = expect("constexpr", index);
  if (eGrmErrNoError == expTypeDef)
  {
    lastIndex = index;
    //ret = eGramIsConstExpr;
    return eGrmErrNoError;
  }

  uint32 typeSpeciRet = handleTypeSpecifier(index, lastIndex, curBlock);
  if (eGrmErrNoError == typeSpeciRet)
  {
    JZFUNC_END_LOG();
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleTypeSpecifier(int index, int& lastIndex, GrammarBlock* curBlock)
{
  //Not implement yet
  return eGrmErrNoError;
}
