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
  uint32 declatorRet = handleDeclator(lastIndex + 1, lastIndex, curBlock);
  if(eGrmErrNoError != declatorRet)
  {
    return declatorRet;
  }

  return handleFunctionBody(lastIndex + 1 , lastIndex, curBlock);

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
    else
    {
      return nextRet;
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
  uint32 trailingRet = handleTrailingTypeSpecifier(index, lastIndex, curBlock);
  if (eGrmErrNoError == trailingRet)
  {
    return eGrmErrNoError;
  }
  
  uint32 enumRet = handleEnumSpecifier(index, lastIndex, curBlock);
  if (eGrmErrNoError == trailingRet)
  {
    return eGrmErrNoError;
  }

  uint32 classRet = handleClassSpecifier(index, lastIndex, curBlock);
  if (eGrmErrNoError == classRet)
  {
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleTrailingTypeSpecifier(int index, int& lastIndex, GrammarBlock* curBlock)
{
  uint32 simpleRet = handleSimpleTypeSpecifier(index, lastIndex, curBlock);
  if (eGrmErrNoError == simpleRet)
  {
    return eGrmErrNoError;
  }
  
//let's move it later

//  uint32 elaboratedRet = handleElaboratedTypeSpecifier(index, lastIndex, curBlock);
//  if (eGrmErrNoError == elaboratedRet)
//  {
//    return eGrmErrNoError;
//  }

//let's move it later

//  uint32 typeNameRet = handleTypeNameSpecifier(index, lastIndex, curBlock);
//  if (eGrmErrNoError == typeNameRet)
//  {
//    return eGrmErrNoError;
//  }

  uint32 cvType = eGramIsNothing;
  uint32 cvRet = getCVQualifier(index, lastIndex, cvType);
  if (eGrmErrNoError == cvRet)
  {
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleSimpleTypeSpecifier(int index, int& lastIndex, GrammarBlock* curBlock)
{
  //let's skip :: and nested name at first

  uint32 charExp = expect("char",index);
  if (eGrmErrNoError == charExp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }
  uint32 char16_tExp = expect("char16_t",index);
  if (eGrmErrNoError == char16_tExp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }
  uint32 char32_tExp = expect("char32_t",index);
  if (eGrmErrNoError == char32_tExp)
  {
    return eGrmErrNoError;
  }
  uint32 wchar_tExp = expect("wchar_t",index);
  if (eGrmErrNoError == wchar_tExp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }
  uint32 boolExp = expect("bool",index);
  if (eGrmErrNoError == boolExp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }
  uint32 shortExp = expect("short",index);
  if (eGrmErrNoError == shortExp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }
  uint32 intExp = expect("int",index);
  if (eGrmErrNoError == intExp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }
  uint32 longExp = expect("long",index);
  if (eGrmErrNoError == longExp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }
  uint32 signedExp = expect("signed",index);
  if (eGrmErrNoError == signedExp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }
  uint32 unsignedExp = expect("unsigned",index);
  if (eGrmErrNoError == unsignedExp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }
  uint32 floatExp = expect("float",index);
  if (eGrmErrNoError == floatExp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }
  uint32 doubleExp = expect("double",index);
  if (eGrmErrNoError == doubleExp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }
  uint32 voidExp = expect("void",index);
  if (eGrmErrNoError == voidExp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }
//  need a cpp 11 mark
//  uint32 autoExp = expect("auto",index);
//  if (eGrmErrNoError == autoExp)
//  {
//    lastIndex = index;
//    return eGrmErrNoError;
//  }

  //let's skip decltype-specifier at first
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleEnumSpecifier(int index, int& lastIndex, GrammarBlock* curBlock)
{
  //should be same as handle enum, but I don't sur why I am traced into this .
  return handleEnum(index, lastIndex, curBlock);
}

uint32 GrammarAnalyzer::handleDeclator(int index, int& lastIndex, GrammarBlock* curBlock)
{
  uint32 ptrRet = handlePtrDeclarator(index, lastIndex, curBlock);
  if (eGrmErrNoError == ptrRet)
  {
    return ptrRet;
  }

//this part is cpp 11 standard

//  uint32 noPtrRet = handleNonPtrDeclarator(index,lastIndex, curBlock);
//  if (eGrmErrNoError == noPtrRet)
//  {
//    uint32 parametersRet = handleParameterAndQualifiers(lastIndex + 1, lastIndex, curBlock);
//    if (eGrmErrNoError == parametersRet)
//    {
//      uint32 trailingRet = handleTrailingReturenType(lastIndex + 1, lastIndex, curBlock);
//      if (eGrmErrNoError == trailingRet)
//      {
//        return eGrmErrNoError;
//      }
//    }
//  }

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handlePtrDeclarator(int index, int& lastIndex, GrammarBlock* curBlock)
{
  uint32 noPtrRet = handleNonPtrDeclarator(index,lastIndex, curBlock);
  if (eGrmErrNoError == noPtrRet)
  {
    return eGrmErrNoError;
  }

  uint32 ptrOperatorRet = handlePtrOperator(index, lastIndex, curBlock);
  if (eGrmErrNoError == ptrOperatorRet)
  {
    return handleNonPtrDeclarator(lastIndex + 1, lastIndex, curBlock);
  }
  return eGrmErrNoError;
}


uint32 GrammarAnalyzer::handleNestNameSpecifier(int index, int& lastIndex, GrammarBlock* curBlock)
{
  uint32 typeNameRet = handleTypeName(index, lastIndex, curBlock);
  if (eGrmErrNoError == typeNameRet)
  {
    uint32 ret = expect("::", lastIndex + 1);
    if (eGrmErrNoError == ret)
    {
      lastIndex = lastIndex + 1;
      return eGrmErrNoError;
    }
  }

  uint32 namespaceRet = handleNameSpaceName(index, lastIndex, curBlock);
  if (eGrmErrNoError == namespaceRet)
  {
    uint32 ret = expect("::", lastIndex + 1);
    if (eGrmErrNoError == ret)
    {
      lastIndex = lastIndex + 1;
      return eGrmErrNoError;
    }
  }

  //cpp 11 mark
  
//  uint32 decltypeSpecifierRet = handleDecltypeSpecifier(index, lastIndex, curBlock);
//  if (eGrmErrNoError == decltypeSpecifierRet)
//  {
//    uint32 ret = expect("::", index + 1);
//    if (eGrmErrNoError == ret)
//    {
//      lastIndex = index + 1;
//      return eGrmErrNoError;
//    }
//  }

  uint32 nestedNameRet = handleNestNameSpecifier(index, lastIndex, curBlock);
  if (eGrmErrNoError == typeNameRet)
  {
    uint32 idRet = handleIdentifier(lastIndex + 1, lastIndex, curBlock);
    if (eGrmErrNoError == idRet)
    {
      uint32 ret = expect("::", lastIndex + 1);
      if (eGrmErrNoError == ret)
      {
        lastIndex = lastIndex + 1;
        return eGrmErrNoError;
      }
    }

//    ok, let skip template part at first

//    uint32 expTemplateRet = expect("template", lastIndex + 1);
//    if (eGrmErrNoError == expTemplateRet)
//    {
//      uint32 simpleTemplateIdRet = handleSimpleTemplateId(lastIndex + 2, lastIndex, curBlock);
//      if (eGrmErrNoError == simpleTemplateIdRet)
//      {
//        uint32 ret = expect("::", lastIndex + 1);
//        if (eGrmErrNoError == ret)
//        {
//          lastIndex = lastIndex + 1;
//          return eGrmErrNoError;
//        }
//      }
//    }
  }

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handlePtrOperator(int index, int& lastIndex, GrammarBlock* curBlock)
{
  uint32 starExp = expect("*", index);
  if (eGrmErrNoError == starExp)
  {
    uint32 attRet = handleAttributes(index + 1, lastIndex, curBlock);
    if(eGrmErrNoError == attRet || eGrmErrNotAttri == attRet){
      uint32 ret = eGramIsNothing;
      return getCVQualifier(lastIndex + 1,lastIndex, ret);
    }
  }

  uint32 doubleAndExp = expect("&&", index);
  if (eGrmErrNoError == doubleAndExp)
  {
    uint32 attRet = handleAttributes(index + 1, lastIndex, curBlock);
    if(eGrmErrNoError == attRet || eGrmErrNotAttri == attRet){
      return eGrmErrNoError;
    }
    
  }

  uint32 andExp = expect("&", index);
  if (eGrmErrNoError == andExp)
  {
    uint32 attRet = handleAttributes(index + 1, lastIndex, curBlock);
    if(eGrmErrNoError == attRet || eGrmErrNotAttri == attRet){
      return eGrmErrNoError;
    }
  }
  uint32 expNameSpc = expect("::", index);
  uint32 nestRet = eGrmErrUnknown;
  if (expNameSpc == eGrmErrNoError)
  {
    nestRet = handleNestNameSpecifier(index + 1, lastIndex, curBlock);
  }
  else
  {
    nestRet = handleNestNameSpecifier(index, lastIndex, curBlock);
  }
  if (eGrmErrNoError == nestRet)
  {
    uint32 iStarExp = expect("*", lastIndex + 1 );
    if (eGrmErrNoError != iStarExp)
    {
      return iStarExp;
    }
    lastIndex = lastIndex + 1;

    uint32 attRet = handleAttributes(lastIndex + 1, lastIndex, curBlock);
    if (eGrmErrNoError == attRet || eGrmErrNotAttri == attRet)
    {
      uint32 ret = eGramIsNothing;
      uint32 cvRet = getCVQualifier(lastIndex + 1, lastIndex, ret);
      return cvRet;
    }
  }

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleIdentifier(int index, int& lastIndex, GrammarBlock* curBlock)
{
  if (mRecList.size() <= index)
  {
    return eGrmErrFileEnd;
  }

  string id = mRecList[index].word;

  if (true == isLegalVarIdentify(id, curBlock))
  {
    return eGrmErrNoError;
  }
  else
  {
    return eGrmErrDoubleDefinedVar;
  }
}

uint32 GrammarAnalyzer::handleNameSpaceName(int index, int& lastIndex, GrammarBlock* curBlock)
{
  uint32 orgRet = handleOriginalNamespaceName(index, lastIndex, curBlock);
  if (eGrmErrNoError == orgRet)
  {
    return eGrmErrNoError;
  }

  uint32 namespcAlias = handleNamespaceAlias(index, lastIndex, curBlock);
  if (eGrmErrNoError == namespcAlias)
  {
    return eGrmErrNoError;
  }

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleOriginalNamespaceName(int index, int& lastIndex, GrammarBlock* curBlock)
{
  return handleIdentifier(index, lastIndex, curBlock);
}

uint32 GrammarAnalyzer::handleNamespaceAlias(int index, int& lastIndex, GrammarBlock* curBlock)
{
  return handleIdentifier(index, lastIndex, curBlock);
}

uint32 GrammarAnalyzer::handleTypedefName(int index, int& lastIndex, GrammarBlock* curBlock)
{

  return handleIdentifier(index, lastIndex, curBlock);
}

uint32 GrammarAnalyzer::handleEnumName(int index, int& lastIndex, GrammarBlock* curBlock)
{
  return handleIdentifier(index, lastIndex, curBlock);
}

uint32 GrammarAnalyzer::handleClassName(int index, int& lastIndex, GrammarBlock* curBlock)
{ 
  uint32 idRet = handleIdentifier(index, lastIndex, curBlock);
  if (idRet == eGrmErrNoError)
  {
    return eGrmErrNoError;
  }

  uint32 tmpIdRet = handleSimpleTemplateId(index, lastIndex, curBlock);
  if (eGrmErrNoError == tmpIdRet)
  {
    return tmpIdRet;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleTypeName(int index, int& lastIndex, GrammarBlock* curBlock)
{
  uint32 classNameRet = handleClassName(index, lastIndex, curBlock);
  if (eGrmErrNoError == classNameRet)
  {
    return eGrmErrNoError;
  }

  uint32 enumNameRet = handleEnumName(index, lastIndex, curBlock);
  if (eGrmErrNoError == enumNameRet)
  {
    return eGrmErrNoError;
  }

  uint32 typedefName = handleTypedefName(index, lastIndex, curBlock);
  if (eGrmErrNoError == typedefName)
  {
    return eGrmErrNoError;
  }

  uint32 simpleTmpRet = handleSimpleTemplateId(index, lastIndex, curBlock);
  if (eGrmErrNoError == simpleTmpRet)
  {
    return eGrmErrNoError;
  }

  return eGrmErrNoError;
}

