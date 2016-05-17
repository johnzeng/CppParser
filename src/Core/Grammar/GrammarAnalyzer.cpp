#include "GrammarAnalyzer.h"
#include "GrammarUtil.h"
#include "LexUtil.h"
#include "JZLogger.h"



uint32 GrammarAnalyzer::doAnalyze()
{
  int index = 0;
  int listSize = mRecList.size();
  int32 lastIndex = 0;
  return handleDeclarationSeq(0,lastIndex , &mTopBlock);
}

uint32 GrammarAnalyzer::handleCVQualifierSeq(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
//  uint32 getRet1 = eGramIsNothing;
  uint32 errRet1 = getCVQualifier(index,lastIndex,curBlock/*, getRet1*/);
  if(eGrmErrNoError != errRet1)
  {
    //it's reasonable to get nothing here
    return eGrmErrNoError;
  }
  
//  uint32 getRet2 = eGramIsNothing;
  uint32 errRet2 = getCVQualifier(index + 1,lastIndex, curBlock);
  if(eGrmErrNoError != errRet2)
  {
    //it's reasonable to get nothing here
    JZFUNC_END_LOG();
    return eGrmErrNoError;
  }

//  if(getRet1 == getRet2)
//  {
//    JZFUNC_END_LOG();
//    return eGrmErrDoubleCVQualifier;
//  }
  //should add more code to attach CV-qualifier to the following block
  JZFUNC_END_LOG();
  return eGrmErrNoError;
}

uint32 GrammarAnalyzer::handleAttributes(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
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

uint32 GrammarAnalyzer::handleFuncDefinition(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 nexIndex = index;
  int32 tryLast = lastIndex;
  uint32 attRet = handleAttributes(index, tryLast, curBlock);
  if(eGrmErrNoError == attRet)
  {
    nexIndex = tryLast + 1;
  }
  uint32 decSpecifierSeqRet = handleDeclSpecifierSeq(nexIndex, tryLast, curBlock);
  if (eGrmErrNotDecSpecifierSeq != decSpecifierSeqRet && eGrmErrNoError != decSpecifierSeqRet)
  {
    JZFUNC_END_LOG();
    return decSpecifierSeqRet;
  }
  else
  {
    nexIndex = tryLast + 1;
  }
  uint32 declatorRet = handleDeclarator(nexIndex, tryLast, curBlock);
  if(eGrmErrNoError != declatorRet)
  {
    JZFUNC_END_LOG();
    return declatorRet;
  }

  //also = delete and = default in cpp11
  uint32 bodyRet = handleFunctionBody(tryLast + 1, tryLast, curBlock);
  if (eGrmErrNoError == bodyRet)
  {
    lastIndex = tryLast;
    JZFUNC_END_LOG();
    return eGrmErrNoError;
  }
  JZFUNC_END_LOG();
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleDeclSpecifierSeq(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 specifierRet = handleDeclSpecifier(index, lastIndex, curBlock);
  if (eGrmErrNoError != specifierRet)
  {
    JZFUNC_END_LOG();
    return specifierRet;
  }
  uint32 attRet = handleAttributes(lastIndex + 1, lastIndex, curBlock);
  if(eGrmErrNotAttri == attRet)
  {
    uint32 nextRet = handleDeclSpecifierSeq(lastIndex + 1, lastIndex, curBlock);
    JZFUNC_END_LOG();
    return eGrmErrNoError;
  }
  else if(eGrmErrNoError == attRet)
  {
    JZFUNC_END_LOG();
    return eGrmErrNoError;
  }
  else
  {
    //not sure about this ret
    JZFUNC_END_LOG();
    return attRet;
  }
}

uint32 GrammarAnalyzer::handleDeclSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
//  uint32 ret = eGramIsNothing;
  uint32 storageClassSpecRet = getStorageClassSpecifier(index, lastIndex , curBlock);
  if(eGrmErrNoError == storageClassSpecRet)
  {
    JZWRITE_DEBUG("shoulde add this property into seq");
    JZFUNC_END_LOG();
    return eGrmErrNoError;
  }
  else if (eGrmErrNotStorageClassSpecifier != storageClassSpecRet)
  {
    JZFUNC_END_LOG();
    return storageClassSpecRet;
  }

  uint32 funcSpecifierRet = getFunctionSpecifier(index, lastIndex , curBlock);
  if(eGrmErrNoError == funcSpecifierRet)
  {
    JZWRITE_DEBUG("shoulde add this property into seq");
    JZFUNC_END_LOG();
    return eGrmErrNoError;
  }
  else if (eGrmErrNotFunctionSpecifier != funcSpecifierRet)
  {
    JZFUNC_END_LOG();
    return funcSpecifierRet;
  }

  uint32 expFriend = expect("friend", index);
  if (eGrmErrNoError == expFriend)
  {
    lastIndex = index;
    //ret = eGramIsFriend;
    JZFUNC_END_LOG();
    return eGrmErrNoError;
  }

  uint32 expTypeDef = expect("typedef", index);
  if (eGrmErrNoError == expTypeDef)
  {
    lastIndex = index;
    //ret = eGramIsTypedef;
    JZFUNC_END_LOG();
    return eGrmErrNoError;
  }

  uint32 expContExpr = expect("constexpr", index);
  if (eGrmErrNoError == expTypeDef)
  {
    lastIndex = index;
    //ret = eGramIsConstExpr;
    JZFUNC_END_LOG();
    return eGrmErrNoError;
  }

  uint32 typeSpeciRet = handleTypeSpecifier(index, lastIndex, curBlock);
  if (eGrmErrNoError == typeSpeciRet)
  {
    JZFUNC_END_LOG();
    return eGrmErrNoError;
  }
  JZFUNC_END_LOG();
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleTypeSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 trailingRet = handleTrailingTypeSpecifier(index, lastIndex, curBlock);
  if (eGrmErrNoError == trailingRet)
  {
    return eGrmErrNoError;
  }
  
  uint32 enumRet = handleEnumSpecifier(index, lastIndex, curBlock);
  if (eGrmErrNoError == enumRet)
  {
    JZFUNC_END_LOG();
    return eGrmErrNoError;
  }

  uint32 classRet = handleClassSpecifier(index, lastIndex, curBlock);
  if (eGrmErrNoError == classRet)
  {
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleTrailingTypeSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
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

  uint32 typeNameRet = handleTypenameSpecifier(index, lastIndex, curBlock);
  if (eGrmErrNoError == typeNameRet)
  {
    return eGrmErrNoError;
  }

//  uint32 cvType = eGramIsNothing;
  uint32 cvRet = getCVQualifier(index, lastIndex, curBlock);
  if (eGrmErrNoError == cvRet)
  {
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleSimpleTypeSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
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

uint32 GrammarAnalyzer::handleDeclarator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 ptrRet = handlePtrDeclarator(index, lastIndex, curBlock);
  if (eGrmErrNoError == ptrRet)
  {
    return ptrRet;
  }

//this part is cpp 11 standard

  uint32 noPtrRet = handleNonPtrDeclarator(index,lastIndex, curBlock);
  if (eGrmErrNoError == noPtrRet)
  {
    uint32 parametersRet = handleParameterAndQualifiers(lastIndex + 1, lastIndex, curBlock);
    if (eGrmErrNoError == parametersRet)
    {
      uint32 trailingRet = handleTrailingReturenType(lastIndex + 1, lastIndex, curBlock);
      if (eGrmErrNoError == trailingRet)
      {
        return eGrmErrNoError;
      }
    }
  }

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handlePtrDeclarator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
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


uint32 GrammarAnalyzer::handleNestNameSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
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
  
  uint32 decltypeSpecifierRet = handleDecltypeSpecifier(index, lastIndex, curBlock);
  if (eGrmErrNoError == decltypeSpecifierRet)
  {
    uint32 ret = expect("::", index + 1);
    if (eGrmErrNoError == ret)
    {
      lastIndex = index + 1;
      return eGrmErrNoError;
    }
  }
  //need to add loop breaker

//  uint32 nestedNameRet = handleNestNameSpecifier(index, lastIndex, curBlock);
//  if (eGrmErrNoError == typeNameRet)
//  {
//    uint32 idRet = handleIdentifier(lastIndex + 1, lastIndex, curBlock);
//    if (eGrmErrNoError == idRet)
//    {
//      uint32 ret = expect("::", lastIndex + 1);
//      if (eGrmErrNoError == ret)
//      {
//        lastIndex = lastIndex + 1;
//        return eGrmErrNoError;
//      }
//    }
//
//    ok, let skip template part at first
//
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
//  }

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handlePtrOperator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 starExp = expect("*", index);
  if (eGrmErrNoError == starExp)
  {
    uint32 attRet = handleAttributes(index + 1, lastIndex, curBlock);
    if(eGrmErrNoError == attRet || eGrmErrNotAttri == attRet){
//      uint32 ret = eGramIsNothing;
      return getCVQualifier(lastIndex + 1,lastIndex, curBlock);
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
//      uint32 ret = eGramIsNothing;
      uint32 cvRet = getCVQualifier(lastIndex + 1, lastIndex, curBlock);
      return cvRet;
    }
  }

  return eGrmErrUnknown;
}
uint32 GrammarAnalyzer::handleDeclaratorId(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 dotRet = expect("...", index);
  if (eGrmErrNoError == dotRet)
  {
    uint32 idExpRet = handleIdExpression(index + 1, lastIndex, curBlock);
    if (eGrmErrNoError == idExpRet)
    {
      return eGrmErrNoError;
    }
  }
  else
  {
    uint32 idExpRet = handleIdExpression(index , lastIndex, curBlock);
    if (eGrmErrNoError == idExpRet)
    {
      return eGrmErrNoError;
    }
  }

  uint32 doubleExp = expect("::", index);
  if (eGrmErrNoError == doubleExp)
  {
    uint32 nestRet = handleNestNameSpecifier(index + 1, lastIndex, curBlock);
    if (eGrmErrNoError == nestRet)
    {
      return handleClassName(index + 2, lastIndex, curBlock);
    }
    else
    {
      return handleClassName(index + 1, lastIndex , curBlock);
    }
  }
  else
  {
    uint32 nestRet = handleNestNameSpecifier(index, lastIndex , curBlock);
    if (eGrmErrNoError == nestRet)
    {
      return handleClassName(index + 1, lastIndex, curBlock);
    }
    else
    {
      return handleClassName(index, lastIndex , curBlock);
    }
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleIdentifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  JZFUNC_BEGIN_LOG();
  if (mRecList.size() <= index)
  {
    return eGrmErrFileEnd;
  }

  string id = mRecList[index].word;

  if (true == isLegalVarIdentify(id, curBlock))
  {
    lastIndex = index;
    JZWRITE_DEBUG("id is: %s" , id.c_str());
    JZFUNC_END_LOG();
    return eGrmErrNoError;
  }
  else
  {
    JZFUNC_END_LOG();
    return eGrmErrDoubleDefinedVar;
  }
}

uint32 GrammarAnalyzer::handleNameSpaceName(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
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

uint32 GrammarAnalyzer::handleOriginalNamespaceName(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  return handleIdentifier(index, lastIndex, curBlock);
}

uint32 GrammarAnalyzer::handleNamespaceAlias(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  return handleIdentifier(index, lastIndex, curBlock);
}

uint32 GrammarAnalyzer::handleTypedefName(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{

  return handleIdentifier(index, lastIndex, curBlock);
}

uint32 GrammarAnalyzer::handleEnumName(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  return handleIdentifier(index, lastIndex, curBlock);
}

uint32 GrammarAnalyzer::handleClassName(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
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

uint32 GrammarAnalyzer::handleTypeName(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
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

uint32 GrammarAnalyzer::handleIdExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 unqualifiedIdRet = handleUnqualifiedId(index, lastIndex, curBlock);
  if (eGrmErrNoError == unqualifiedIdRet)
  {
    return eGrmErrNoError;
  }
  uint32 qualifiedIdRet = handleQualifiedId(index, lastIndex, curBlock);
  if (eGrmErrNoError == qualifiedIdRet)
  {
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleUnqualifiedId(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 idRet = handleIdentifier(index, lastIndex, curBlock);
  if (idRet == eGrmErrNoError)
  {
    return eGrmErrNoError;
  }
  uint32 opFuncId = handleOperatorFunctionId(index, lastIndex, curBlock);
  if (opFuncId == eGrmErrNoError)
  {
    return eGrmErrNoError;
  }
  uint32 conversionFuncIdRet = handleConversionFunctionId(index, lastIndex, curBlock);
  if (eGrmErrNoError == conversionFuncIdRet)
  {
    return eGrmErrNoError;
  }

  //cpp 11 standard

//  uint32 literalOpId = handleLiteralFunctionId(index, lastIndex, curBlock);
//  if (eGrmErrNoError == literalOpId)
//  {
//    return eGrmErrNoError;
//  }

  uint32 tempId = handleTemplateId(index, lastIndex, curBlock);
  if (eGrmErrNoError == tempId)
  {
    return eGrmErrNoError;
  }

  uint32 waveExp = expect("~", index);
  if (eGrmErrNoError == waveExp)
  {
    uint32 classRet = handleClassName(index + 1, lastIndex, curBlock);
    if (eGrmErrNoError == classRet)
    {
      return eGrmErrNoError;
    }
    else
    {
      return handleDecltypeSpecifier(index + 1, lastIndex, curBlock);
    }
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleQualifiedId(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 doubleExp = expect("::", index);
  if (eGrmErrNoError == doubleExp)
  {
    uint32 idRet = handleIdentifier(index + 1, lastIndex, curBlock);
    if (idRet == eGrmErrNoError)
    {
      return eGrmErrNoError;
    }
    uint32 opFuncId = handleOperatorFunctionId(index + 1, lastIndex, curBlock);
    if (opFuncId == eGrmErrNoError)
    {
      return eGrmErrNoError;
    }
  //cpp 11 standard

    uint32 literalOpId = handleLiteralOperatorId(index + 1, lastIndex, curBlock);
    if (eGrmErrNoError == literalOpId)
    {
      return eGrmErrNoError;
    }

    uint32 tempId = handleTemplateId(index + 1, lastIndex, curBlock);
    if (eGrmErrNoError == tempId)
    {
      return eGrmErrNoError;
    }

    uint32 nestedRet = handleNestNameSpecifier(index + 1, lastIndex, curBlock);
    if (eGrmErrNoError == nestedRet)
    {
      uint32 expTemplate = expect("template", lastIndex + 1);
      if (eGrmErrNoError == expTemplate)
      {
        return handleUnqualifiedId(lastIndex + 2, lastIndex, curBlock);
      }
      else
      {
        return handleUnqualifiedId(lastIndex + 1, lastIndex, curBlock);
      }
        
    }
  }
  else
  {
    uint32 nestedRet = handleNestNameSpecifier(index , lastIndex, curBlock);
    if (eGrmErrNoError == nestedRet)
    {
      uint32 expTemplate = expect("template", lastIndex + 1);
      if (eGrmErrNoError == expTemplate)
      {
        return handleUnqualifiedId(lastIndex + 2, lastIndex, curBlock);
      }
      else
      {
        return handleUnqualifiedId(lastIndex + 1, lastIndex, curBlock);
      }
    }
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleDecltypeSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 keyExp = expect("decltype", index);
  if (eGrmErrNoError != keyExp)
  {
    return eGrmErrUnknown;
  }
  uint32 leftExp = expect("(", index + 1);
  if (eGrmErrNoError != leftExp)
  {
    return eGrmErrUnknown;
  }

  uint32 expRet = handleExpression(index + 2 , lastIndex, curBlock);
  if (eGrmErrNoError != expRet)
  {
    return eGrmErrUnknown;
  }

  uint32 rightExp = expect(")", lastIndex + 1);
  if (eGrmErrNoError != rightExp)
  {
    lastIndex ++;
    return eGrmErrUnknown;
  }
  return eGrmErrNoError;
}

uint32 GrammarAnalyzer::handleExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 assRet = handleAssignmentExpression(index, lastIndex, curBlock);
  if (eGrmErrNoError == assRet)
  {
    uint32 commaExp = expect(",", lastIndex + 1);
    if (eGrmErrNoError == commaExp)
    {
      return handleExpression(lastIndex + 2, lastIndex, curBlock);
    }
    return eGrmErrNoError;
  }

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleAssignmentExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 condRet = handleConditionalExpression(index, lastIndex, curBlock);
  if (eGrmErrNoError == condRet)
  {
    return eGrmErrNoError;
  }

  uint32 throwRet = handleThrowExpression(index, lastIndex, curBlock);
  if (eGrmErrNoError == throwRet)
  {
    return eGrmErrNoError;
  }
  uint32 logicRet = handleLogicOrExpression(index, lastIndex, curBlock);
  if (eGrmErrNoError == logicRet)
  {
//    uint32 ret = eGramIsNothing;
    uint32 expAssignmentRet = getAssignmentOperator(lastIndex + 1, lastIndex, curBlock);
    if (eGrmErrNoError == expAssignmentRet)
    {
      return handleInitializerClause(lastIndex + 1, lastIndex, curBlock);
    }
  }

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleConditionalExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 logicRet = handleLogicOrExpression(index, lastIndex, curBlock);
  if (eGrmErrNoError == logicRet)
  {
    uint32 expAsk = expect("?", lastIndex + 1);
    if (eGrmErrNoError == expAsk)
    {
      uint32 expExpress = handleExpression(lastIndex + 2, lastIndex, curBlock);
      if (eGrmErrNoError != expExpress)
      {
        return expExpress;
      }
      uint32 expRet = expect(":", lastIndex + 1);
      if (eGrmErrNoError != expRet)
      {
        return expRet;
      }
      uint32 assignExp = handleAssignmentExpression(lastIndex + 2, lastIndex, curBlock);
      if (eGrmErrNoError == assignExp)
      {
        return assignExp;
      }
      return eGrmErrNoError;
    }
    else
    {
      return eGrmErrNoError;
    }
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleLogicOrExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 nextRet = handleLogicAndExpression(index, lastIndex, curBlock);
  if (eGrmErrNoError == nextRet)
  {
    uint32 orRet = expect("||", lastIndex + 1);
    if (eGrmErrNoError == orRet )
    {
      return handleLogicOrExpression(lastIndex + 2, lastIndex, curBlock);
    }
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleLogicAndExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 inRet = handleInclusiveOrExpression(index, lastIndex, curBlock);
  if (eGrmErrNoError == inRet)
  {
    uint32 expAnd = expect("&&", lastIndex + 1);
    if (eGrmErrNoError == expAnd)
    {
      return handleLogicAndExpression(lastIndex + 2, lastIndex, curBlock);
    }
    else
    {
      return eGrmErrNoError;
    }
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleInclusiveOrExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 nextRet = handleExclusiveOrExpression(index, lastIndex, curBlock);
  if (eGrmErrNoError == nextRet)
  {
    uint32 orRet = expect("|", lastIndex + 1);
    if (eGrmErrNoError == orRet )
    {
      return handleInclusiveOrExpression(lastIndex + 2, lastIndex, curBlock);
    }
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleExclusiveOrExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 nextRet = handleAndExpression(index, lastIndex, curBlock);
  if (eGrmErrNoError == nextRet)
  {
    uint32 orRet = expect("^", lastIndex + 1);
    if (eGrmErrNoError == orRet )
    {
      return handleExclusiveOrExpression(lastIndex + 2, lastIndex, curBlock);
    }
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleAndExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 nextRet = handleEqualityExpression(index, lastIndex, curBlock);
  if (eGrmErrNoError == nextRet)
  {
    uint32 orRet = expect("&", lastIndex + 1);
    if (eGrmErrNoError == orRet )
    {
      return handleAndExpression(lastIndex + 2, lastIndex, curBlock);
    }
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleEqualityExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 nextRet = handleRelationalExpression(index, lastIndex, curBlock);
  if (eGrmErrNoError == nextRet)
  {
    uint32 eqRet = expect("==", lastIndex + 1);
    if (eGrmErrNoError == eqRet)
    {
      return handleEqualityExpression(lastIndex + 2, lastIndex, curBlock);
    }

    uint32 nqRet = expect("!=", lastIndex + 1);
    if (eGrmErrNoError == nqRet)
    {
      return handleEqualityExpression(lastIndex + 2, lastIndex, curBlock);
    }

    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleRelationalExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 shiftRet = handleShiftExpression(index, lastIndex, curBlock);
  if (eGrmErrNoError == shiftRet)
  {
    uint32 exp1 = expect("<", lastIndex + 1);
    if (exp1 == eGrmErrNoError)
    {
      return handleRelationalExpression(lastIndex + 2, lastIndex, curBlock);
    }

    uint32 exp2 = expect(">", lastIndex + 1);
    if (exp2 == eGrmErrNoError)
    {
      return handleRelationalExpression(lastIndex + 2, lastIndex, curBlock);
    }

    uint32 exp3 = expect("<=", lastIndex + 1);
    if (exp3 == eGrmErrNoError)
    {
      return handleRelationalExpression(lastIndex + 2, lastIndex, curBlock);
    }

    uint32 exp4 = expect(">=", lastIndex + 1);
    if (exp4 == eGrmErrNoError)
    {
      return handleRelationalExpression(lastIndex + 2, lastIndex, curBlock);
    }

    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleShiftExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 addRet = handleAdditiveExpression(index, lastIndex, curBlock);
  if (eGrmErrNoError == addRet)
  {
    uint32 eqRet = expect("<<", lastIndex + 1);
    if (eGrmErrNoError == eqRet)
    {
      return handleShiftExpression(lastIndex + 2, lastIndex, curBlock);
    }

    uint32 nqRet = expect(">>", lastIndex + 1);
    if (eGrmErrNoError == nqRet)
    {
      return handleShiftExpression(lastIndex + 2, lastIndex, curBlock);
    }

    return eGrmErrNoError;
    
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleAdditiveExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 mulRet = handleMultiplicativeExpression(index, lastIndex,curBlock);
  if (eGrmErrNoError == mulRet)
  {
    uint32 exp1 = expect("+", lastIndex + 1);
    if (exp1 == eGrmErrNoError)
    {
      return handleAdditiveExpression(lastIndex + 2, lastIndex, curBlock);
    }

    uint32 exp2 = expect("=", lastIndex + 1);
    if (exp2 == eGrmErrNoError)
    {
      return handleAdditiveExpression(lastIndex + 2, lastIndex, curBlock);
    }

    return eGrmErrNoError;
    
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleMultiplicativeExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 pmRet = handlePmExpression(index, lastIndex, curBlock);
  if (eGrmErrNoError == pmRet)
  {
    
    uint32 exp1 = expect("*", lastIndex + 1);
    if (exp1 == eGrmErrNoError)
    {
      return handleMultiplicativeExpression(lastIndex + 2, lastIndex, curBlock);
    }

    uint32 exp2 = expect("/", lastIndex + 1);
    if (exp2 == eGrmErrNoError)
    {
      return handleMultiplicativeExpression(lastIndex + 2, lastIndex, curBlock);
    }

    uint32 exp3 = expect("%", lastIndex + 1);
    if (exp3 == eGrmErrNoError)
    {
      return handleMultiplicativeExpression(lastIndex + 2, lastIndex, curBlock);
    }
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handlePmExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 castRet = handleCastExpression(index, lastIndex, curBlock);
  if (eGrmErrNoError == castRet)
  {
    uint32 exp1 = expect(".*", lastIndex + 1);
    if (exp1 == eGrmErrNoError)
    {
      return handlePmExpression(lastIndex + 2, lastIndex, curBlock);
    }

    uint32 exp2 = expect("->*", lastIndex + 1);
    if (exp2 == eGrmErrNoError)
    {
      return handlePmExpression(lastIndex + 2, lastIndex, curBlock);
    }

    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleCastExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 unArryRet = handleUnaryExpression(index, lastIndex, curBlock);
  if (eGrmErrNoError == unArryRet)
  {
    return eGrmErrNoError;
  }

  uint32 leftExp = expect("(", index);
  if (leftExp == eGrmErrNoError)
  {
    uint32 typeRet = handleTypeId(index + 1, lastIndex, curBlock);
    if (eGrmErrNoError == typeRet)
    {
      uint32 rightExp = expect(")", lastIndex + 1);
      if (rightExp == eGrmErrNoError)
      {
        return handleCastExpression(lastIndex + 2, lastIndex, curBlock);
      }
    }
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleTypeId(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 typeSpecifierRet = handleTypeSpecifierSeq(index, lastIndex, curBlock);
  if (eGrmErrNoError == typeSpecifierRet)
  {
    uint32 abstrRet = handleAbstractDeclarator(lastIndex + 1, lastIndex, curBlock);
    if (eGrmErrNoError == abstrRet || eGrmErrNotAbstractDeclarator == abstrRet)
    {
      return eGrmErrNoError;
    }
  }
  return eGrmErrNoError;
}

uint32 GrammarAnalyzer::handleTypeSpecifierSeq(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 typeSpeRet = handleTypeSpecifier(index, lastIndex, curBlock);
  if (typeSpeRet == eGrmErrNoError)
  {
    uint32 attRet = handleAttributes(lastIndex + 1, lastIndex, curBlock);
    if (eGrmErrNoError == attRet)
    {
      return eGrmErrNoError;
    }
    else if (eGrmErrNotAttri == attRet)
    {
      uint32 seqRet = handleTypeSpecifierSeq(lastIndex + 1, lastIndex, curBlock);
      if (seqRet == eGrmErrNotTypeSpecifierSeq)
      {
        return eGrmErrNoError;
      }
    }
    return attRet;
  }

  return eGrmErrNotTypeSpecifierSeq;
}

uint32 GrammarAnalyzer::handleAbstractDeclarator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 ptrRet = handlePtrAbstractDeclarator(index, lastIndex, curBlock);
  if (eGrmErrNoError == ptrRet)
  {
    return eGrmErrNoError;
  }

  uint32 noPtrRet = handleNoptrAbstractDeclarator(index, lastIndex, curBlock);
  if (eGrmErrNoError == noPtrRet)
  {
    uint32 paramterRet = handleParameterAndQualifiers(lastIndex + 1, lastIndex ,curBlock);
    if (eGrmErrNoError == paramterRet)
    {
      return handleTrailingReturenType(lastIndex + 1, lastIndex ,curBlock);
    }
  }
  else
  {
    uint32 paramterRet = handleParameterAndQualifiers(index, lastIndex ,curBlock);
    if (eGrmErrNoError == paramterRet)
    {
      return handleTrailingReturenType(lastIndex + 1, lastIndex ,curBlock);
    }
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handlePtrAbstractDeclarator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 noptrRet = handleNoptrAbstractDeclarator(index, lastIndex, curBlock);
  if (eGrmErrNoError == noptrRet)
  {
    return eGrmErrNoError;
  }

  uint32 ptrOptRet = handlePtrOperator(index, lastIndex, curBlock);
  if (eGrmErrNoError == ptrOptRet)
  {
    uint32 ptrRet = handlePtrAbstractDeclarator(lastIndex + 1, lastIndex, curBlock);
    return ptrRet;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleNoptrAbstractDeclarator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 paramRet = handleParameterAndQualifiers(index, lastIndex, curBlock);
  if (eGrmErrNoError == paramRet)
  {
    uint32 nextRet = handleNoptrAbstractDeclarator(lastIndex + 1 , lastIndex, curBlock);
    return eGrmErrNoError;
  }

  uint32 leftSeqBracket = expect("[", index);
  if (eGrmErrNoError == leftSeqBracket)
  {
    uint32 constRet = handleConstantExpression(index + 1, lastIndex, curBlock);
    if (eGrmErrNoError == constRet)
    {
      uint32 rightSeqBreacket = expect("]", lastIndex + 1);
      if (eGrmErrNoError == rightSeqBreacket)
      {
        lastIndex++;
        handleAttributes(lastIndex + 1, lastIndex, curBlock);
        uint32 nextRet = handleNoptrAbstractDeclarator(lastIndex + 1,lastIndex, curBlock);
        return eGrmErrNoError;
      }
    }
  }

  uint32 expLeftBracket = expect("(", index);
  if (eGrmErrNoError == expLeftBracket)
  {
    uint32 ptrRet = handlePtrAbstractDeclarator(index + 1, lastIndex, curBlock);
    if (eGrmErrNoError == ptrRet)
    {
      uint32 ret = expect(")", lastIndex + 1);
      if (ret == eGrmErrNoError)
      {
        lastIndex ++;
        return eGrmErrNoError;
      }
    }
  }
  return eGrmErrNoError;
}

uint32 GrammarAnalyzer::handleConstantExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  return handleConditionalExpression(index, lastIndex, curBlock);
}

uint32 GrammarAnalyzer::handleUnaryExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 postfixRet = handlePostfixExpression(index, lastIndex, curBlock);
  if (eGrmErrNoError == postfixRet)
  {
    return eGrmErrNoError;
  }

  uint32 plusExp = expect("++", index);
  if (eGrmErrNoError == plusExp)
  {
    return handleCastExpression(index + 1, lastIndex, curBlock);
  }

  uint32 minusExp = expect("--", index);
  if (eGrmErrNoError == minusExp)
  {
    return handleCastExpression(index + 1, lastIndex, curBlock);
  }

//  uint32 getRet = eGramIsNothing;
  uint32 getOptRet = getUnaryOperator(index, lastIndex, curBlock);
  if (eGrmErrNoError == getOptRet)
  {
    return handleCastExpression(index + 1, lastIndex, curBlock);
  }

  uint32 sizeofExp = expect("sizeof", index);
  if (eGrmErrNoError == sizeofExp)
  {
    uint32 expNextRet = handleUnaryExpression(index + 1, lastIndex, curBlock);
    if (eGrmErrNoError == expNextRet)
    {
      return eGrmErrNoError;
    }

    uint32 expLeft = expect("(", index + 1);
    if (eGrmErrNoError == expLeft)
    {
      uint32 typeRet = handleTypeId(index + 2, lastIndex, curBlock);
      if (eGrmErrNoError == typeRet)
      {
        uint32 expRight = expect(")", lastIndex + 1);
        if (eGrmErrNoError == expRight)
        {
          lastIndex ++;
          return eGrmErrNoError;
        }
      }
    }

    uint32 expDot = expect("...", index + 1);
    if (eGrmErrNoError == expDot)
    {
      uint32 idRet = handleIdentifier(index + 2, lastIndex, curBlock);
      if (eGrmErrNoError == idRet)
      {
        uint32 expRight = expect(")", lastIndex + 1);
        if (eGrmErrNoError == expRight)
        {
          lastIndex ++;
          return eGrmErrNoError;
        }
      }
    }
  }

  uint32 expAlignof = expect("alignof", index);
  if(eGrmErrNoError == expAlignof)
  {
    uint32 expLeft = expect("(", index + 1);
    if (eGrmErrNoError == expLeft)
    {
      uint32 typeRet = handleTypeId(index + 2, lastIndex, curBlock);
      if (eGrmErrNoError == typeRet)
      {
        uint32 expRight = expect(")", lastIndex + 1);
        if (eGrmErrNoError == expRight)
        {
          lastIndex ++;
          return eGrmErrNoError;
        }
      }
    }
  }

  uint32 noexceptRet = handleNoexceptExpression(index, lastIndex, curBlock);
  if (eGrmErrNoError == noexceptRet)
  {
    return eGrmErrNoError;
  }

  uint32 newExpRet = handleNewExpression(index, lastIndex, curBlock);
  if (eGrmErrNoError == newExpRet)
  {
    return eGrmErrNoError;
  }

  uint32 deleteRet = handleDeleteExpression(index, lastIndex, curBlock);
  if (eGrmErrNoError == deleteRet)
  {
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleNoexceptExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 keyRet = expect("noexcept",index);
  if (eGrmErrNoError == keyRet)
  {
    uint32 leftRet = expect("(", index + 1);
    if (eGrmErrNoError == leftRet)
    {
      uint32 expRet = handleExpression(index + 2, lastIndex, curBlock);
      if (eGrmErrNoError == expRet)
      {
        uint32 expRight = expect(")", lastIndex + 1);
        if (eGrmErrNoError == expRight)
        {
          lastIndex ++;
          return eGrmErrNoError;
        }
      }
    }
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handlePostfixExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 prmRet = handlePrimaryExpression(index, lastIndex, curBlock);
  if (eGrmErrNoError == prmRet)
  {
    return eGrmErrNoError;
  }

  uint32 leftSeqRet = expect("[", index);
  if (eGrmErrNoError == leftSeqRet)
  {
    uint32 expRet = handleExpression(index + 1, lastIndex, curBlock);
    if (eGrmErrNoError == expRet)
    {
      uint32 rightSeqRet = expect("]", lastIndex + 1);
      if (eGrmErrNoError == rightSeqRet)
      {
        uint32 nextPostRet = handlePostfixExpression(lastIndex + 2, lastIndex, curBlock);
        if (nextPostRet == eGrmErrNoError)
        {
          return eGrmErrNoError;
        }
//        if (nextPostRet == eGrmErrNotPostfixExpress)
//        {
//          return eGrmErrNoError
//        }

      }
    }

    uint32 bracedRet = handleBracedInitList(index + 1, lastIndex, curBlock);
    if (eGrmErrNoError == bracedRet /*|| eGrmErrNotBraceInitList*/)
    {
      uint32 rightSeqRet = expect("]", lastIndex + 1);
      if (eGrmErrNoError == rightSeqRet)
      {
        uint32 nextPostRet = handlePostfixExpression(lastIndex + 2, lastIndex, curBlock);
        if (nextPostRet == eGrmErrNoError)
        {
          return eGrmErrNoError;
        }
//        if (nextPostRet == eGrmErrNotPostfixExpress)
//        {
//          return eGrmErrNoError
//        }

      }
    }
  }

  uint32 expLeftBracket = expect("(", index, curBlock);
  if (eGrmErrNoError == expLeftBracket)
  {
    uint32 expressListRet = handleExpressionList(index + 1,lastIndex, curBlock);
    if (eGrmErrNoError == expressListRet /* || eGrmErrNotExpressList == expressListRet*/)
    {
      uint32 expRightBracket = expect(")", lastIndex + 1, curBlock);
      if (eGrmErrNoError == expRightBracket)
      {
        lastIndex ++;
        uint32 nextRet = handlePostfixExpression(lastIndex + 1, lastIndex, curBlock);
        return eGrmErrNoError;
      }
    }
  }

  uint32 simpleRet = handleSimpleTypeSpecifier(index, lastIndex, curBlock);
  if (eGrmErrNoError == simpleRet)
  {
    uint32 expLeftBracket = expect("(", lastIndex + 1, curBlock);
    if (eGrmErrNoError == expLeftBracket)
    {
      uint32 expressListRet = handleExpressionList(lastIndex + 2,lastIndex, curBlock);
      if (eGrmErrNoError == expressListRet /* || eGrmErrNotExpressList == expressListRet*/)
      {
        uint32 expRightBracket = expect(")", lastIndex + 1, curBlock);
        if (eGrmErrNoError == expRightBracket)
        {
          lastIndex ++;
          return eGrmErrNoError;
        }
      }
    }

    return handleBracedInitList(lastIndex + 1, lastIndex, curBlock);
  }

  uint32 typenameRet = handleTypenameSpecifier(index, lastIndex, curBlock);
  if (eGrmErrNoError == typenameRet)
  {
    uint32 expLeftBracket = expect("(", lastIndex + 1, curBlock);
    if (eGrmErrNoError == expLeftBracket)
    {
      uint32 expressListRet = handleExpressionList(lastIndex + 2,lastIndex, curBlock);
      if (eGrmErrNoError == expressListRet /* || eGrmErrNotExpressList == expressListRet*/)
      {
        uint32 expRightBracket = expect(")", lastIndex + 1, curBlock);
        if (eGrmErrNoError == expRightBracket)
        {
          lastIndex ++;
          return eGrmErrNoError;
        }
      }
    }
    return handleBracedInitList(lastIndex + 1, lastIndex, curBlock);
  }

  uint32 expDot = expect(".", index);
  if (eGrmErrNoError == expDot)
  {
    uint32 expTep = expect("template", index + 1);
    if (eGrmErrNoError == expTep)
    {
      uint32 idExpRet = handleIdExpression(index + 2, lastIndex, curBlock);
      if (idExpRet == eGrmErrNoError)
      {
        uint32 nextRet = handlePostfixExpression(lastIndex + 1, lastIndex, curBlock);
        return eGrmErrNoError;
      }
    }
    else
    {
      uint32 idExpRet = handleIdExpression(index + 1, lastIndex, curBlock);
      if (idExpRet == eGrmErrNoError)
      {
        uint32 nextRet = handlePostfixExpression(lastIndex + 1, lastIndex, curBlock);
        return eGrmErrNoError;
      }
    }

    return handlePseudoDestructorName(index + 1, lastIndex, curBlock);
  }

  uint32 expArray = expect("->", index);
  if (eGrmErrNoError == expArray)
  {
    uint32 expTep = expect("template", index + 1);
    if (eGrmErrNoError == expTep)
    {
      uint32 idExpRet = handleIdExpression(index + 2, lastIndex, curBlock);
      if (idExpRet == eGrmErrNoError)
      {
        uint32 nextRet = handlePostfixExpression(lastIndex + 1, lastIndex, curBlock);
        return eGrmErrNoError;
      }
    }
    else
    {
      uint32 idExpRet = handleIdExpression(index + 1, lastIndex, curBlock);
      if (idExpRet == eGrmErrNoError)
      {
        uint32 nextRet = handlePostfixExpression(lastIndex + 1, lastIndex, curBlock);
        return eGrmErrNoError;
      }
      
    }
    return handlePseudoDestructorName(index + 1, lastIndex, curBlock);
  }

  uint32 plusExp = expect("++", index);
  if (eGrmErrNoError == plusExp)
  {
    handlePostfixExpression(index + 1, lastIndex, curBlock);
    return eGrmErrNoError;
  }

  uint32 minuseExp = expect("--", index);
  if (eGrmErrNoError == minuseExp)
  {
    handlePostfixExpression(index + 1, lastIndex, curBlock);
    return eGrmErrNoError;
  }

  uint32 expDynamic = expect("dynamic_cast",index);
  uint32 expStatic = expect("static_cast",index);
  uint32 expRein = expect("reinterpret_cast",index);
  uint32 expConstCast = expect("const_cast",index);
  if (expDynamic == eGrmErrNoError || expStatic == eGrmErrNoError || eGrmErrNoError == expRein || eGrmErrNoError == expConstCast)
  {
    uint32 leftSharp = expect("<", index + 1);
    if (eGrmErrNoError == leftSharp)
    {
      uint32 typeIdRet = handleTypeId(index + 2, lastIndex , curBlock);
      if (eGrmErrNoError == typeIdRet)
      {
        uint32 expRightSharp = expect(">", lastIndex + 1);
        if (eGrmErrNoError == expRightSharp)
        {
          uint32 expLeftBracket = expect("(", lastIndex + 2);
          if (eGrmErrNoError == expLeftBracket)
          {
            uint32 innerExpressRet = handleExpression(lastIndex + 3, lastIndex, curBlock);
            if (eGrmErrNoError == innerExpressRet)
            {
              uint32 rightBracketRet = expect(")", lastIndex + 1);
              if (eGrmErrNoError == rightBracketRet)
              {
                return eGrmErrNoError;
              }
            }
          }
        }
      }
    }
  }

  uint32 typeidRet = expect("typeid", index);
  if (eGrmErrNoError == typeidRet)
  {
    uint32 innerLeftBraceRet = expect("(", index +1);
    if (eGrmErrNoError == innerLeftBraceRet)
    {
      uint32 expressionRet = handleExpression(index + 2, lastIndex, curBlock);
      if (eGrmErrNoError == expressionRet)
      {
        uint32 rightRet = expect(")", lastIndex + 1);
        if (eGrmErrNoError == rightRet)
        {
          lastIndex ++;
          return eGrmErrNoError;
        }
      }

      uint32 typeIdRet = handleTypeId(index + 1, lastIndex, curBlock);
      if (eGrmErrNoError == typeIdRet)
      {
        uint32 rightRet = expect(")", lastIndex + 1);
        if (eGrmErrNoError == rightRet)
        {
          lastIndex ++;
          return eGrmErrNoError;
        }
        
      }
    }
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handlePseudoDestructorName(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 expWave = expect("~", index);
  if (eGrmErrNoError == expWave)
  {
    return handleDecltypeSpecifier(index + 1, lastIndex, curBlock);
  }

  uint32 expDouble = expect("::", index);
  uint32 offset = (eGrmErrNoError == expDouble) ? 1: 0;
  uint32 nextIndex = index +  offset;
  uint32 nestRet = handleNestNameSpecifier(nextIndex, lastIndex, curBlock);
  if (eGrmErrNoError == nestRet)
  {
    nextIndex = lastIndex + 1;
    uint32 tmpRet = expect("template", lastIndex + 1);
    if (eGrmErrNoError == tmpRet)
    {
      uint32 smpRet = handleSimpleTemplateId(lastIndex + 2, lastIndex, curBlock);
      if (eGrmErrNoError == smpRet)
      {
        uint32 expNexDouble = expect("::", lastIndex + 1);
        if (eGrmErrNoError == expNexDouble)
        {
          uint32 expNextWave = expect("~", lastIndex + 2);
          return handleTypeName(lastIndex + 3, lastIndex, curBlock);
        }
      }
    }
  }
  uint32 expWaveNext = expect("~", nextIndex);
  if (eGrmErrNoError == expWaveNext)
  {
    return handleTypeName(nextIndex + 1, lastIndex, curBlock);
  }
  else
  {
    uint32 typeRet = handleTypeName(nextIndex, lastIndex, curBlock);
    if (eGrmErrNoError == typeRet)
    {
      uint32 expNexDouble = expect("::", lastIndex + 1);
      if (expNexDouble == eGrmErrNoError)
      {
        uint32 expWaveRet = expect("~", lastIndex + 2);
        if (eGrmErrNoError == expWaveRet)
        {
          return handleTypeName(lastIndex + 3, lastIndex, curBlock);
        }
      }
    }
  }

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleTypenameSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 expRet = expect("typename", index);
  if (eGrmErrNoError != expRet)
  {
    return eGrmErrNotTypenameSpecifier;
  }

  uint32 offset = expect("::", index + 1) == eGrmErrNoError ? 1:0;
  uint32 nestedRet = handleNestNameSpecifier(index + 1 + offset, lastIndex, curBlock);
  if (eGrmErrNoError != nestedRet)
  {
    return eGrmErrNotTypenameSpecifier;
  }
  uint32 tmpRet = expect("template", lastIndex + 1);
  if (eGrmErrNoError == tmpRet)
  {
    return handleSimpleTemplateId(lastIndex + 2, lastIndex, curBlock);
  }
  else
  {
    uint32 simpRet = handleSimpleTemplateId(lastIndex + 1, lastIndex,  curBlock);
    if (eGrmErrNoError == simpRet)
    {
      return eGrmErrNoError;
    }

    uint32 idRet = handleIdentifier(lastIndex + 1, lastIndex, curBlock);
    if (eGrmErrNoError == idRet)
    {
      return eGrmErrNoError;
    }
  }
  
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleBracedInitList(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 expLeft = expect("{", index);
  if (eGrmErrNoError == expLeft)
  {
    uint32 initList = handleInitializerList(index + 1, lastIndex, curBlock);
    if (eGrmErrNoError == initList)
    {
      uint32 commaRet = expect(",", lastIndex + 1);
      if (eGrmErrNoError == commaRet)
      {
        lastIndex ++;
      }
    }
  }
  uint32 expRight = expect("}", lastIndex + 1);
  if (eGrmErrNoError)
  {
    lastIndex ++;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleExpressionList(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  return handleInitializerList(index, lastIndex ,curBlock);
}

uint32 GrammarAnalyzer::handleInitializerList(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 clauseRet = handleInitializerClause(index, lastIndex, curBlock);
  if (eGrmErrNoError == clauseRet)
  {
    uint32 tripleDot = expect("...", lastIndex + 1);
    if (eGrmErrNoError == tripleDot)
    {
      lastIndex++;
    }
    else
    {
      uint32 CommaRet = expect(",", lastIndex + 1);
      if (eGrmErrNoError == CommaRet)
      {
        handleInitializerList(lastIndex + 2, lastIndex, curBlock);
        return eGrmErrNoError;
      }
    }
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handlePrimaryExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 thisExp = expect("this", index);
  if (eGrmErrNoError == thisExp)
  {
    lastIndex = index;
    return thisExp;
  }
//  uint32 litRet = eGramIsNothing;
  uint32 literalExp = getLiteral(index, lastIndex, curBlock);
  if (eGrmErrNoError == literalExp)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }

  uint32 idExpRet = handleIdExpression(index, lastIndex, curBlock);
  if (eGrmErrNoError == idExpRet)
  {
    return eGrmErrNoError;
  }

  uint32 leftExp = expect("(", index);
  if (eGrmErrNoError == leftExp)
  {
    uint32 expRet = handleExpression(index + 1, lastIndex, curBlock);
    if (expRet == eGrmErrNoError)
    {
      uint32 rightExp = expect(")", lastIndex + 1);
      if (eGrmErrNoError == rightExp)
      {
        lastIndex ++;
        return eGrmErrNoError;
      }
    }
  }

//  mark for lambda

//  uint32 lambdaExp = handleLambdaExpression(index, lastIndex, curBlock);
//  if (eGrmErrNoError == lambdaExp)
//  {
//    return eGrmErrNoError;
//  }

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleDeleteExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 offset = eGrmErrNoError == expect("::", index) ? 1:0;
  uint32 deleteExp = expect("delete", index + offset);
  if (eGrmErrNoError == deleteExp)
  {
    uint32 expLeft = expect("[", index + 1 + offset);
    if (eGrmErrNoError == expLeft)
    {
      uint32 expRight = expect("]", index + 2 + offset);
      if (eGrmErrNoError == expRight)
      {
        return handleCastExpression(index + 3 + offset, lastIndex ,curBlock);
      }
    }

    return handleCastExpression(index + 1 + offset, lastIndex ,curBlock);
  }
  
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleInitializerClause(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 assignmentRet = handleAssignmentExpression(index, lastIndex, curBlock);
  if (eGrmErrNoError == assignmentRet)
  {
    return eGrmErrNoError;
  }
  uint32 bracedRet = handleBracedInitList(index, lastIndex, curBlock);
  if (eGrmErrNoError == bracedRet)
  {
    return eGrmErrNoError;
  }
  return eGrmErrNoError;
}

uint32 GrammarAnalyzer::handleThrowExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 expThrow = expect("throw", index);
  if (eGrmErrNoError == expThrow)
  {
    lastIndex = index;
    handleAssignmentExpression(index + 1, lastIndex, curBlock);
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleConversionFunctionId(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 operatorExp = expect("operator", index);
  if (eGrmErrNoError == operatorExp)
  {
    return handleConversionTypeId(index + 1, lastIndex, curBlock);
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleConversionTypeId(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 typeSpRet = handleTypeSpecifierSeq(index, lastIndex, curBlock);
  if (eGrmErrNoError == typeSpRet)
  {
    handleConversionDeclarator(lastIndex + 1, lastIndex, curBlock);
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleConversionDeclarator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 ptrRet = handlePtrOperator(index, lastIndex, curBlock);
  if (eGrmErrNoError == ptrRet)
  {
    uint32 nextRet = handleConversionDeclarator(lastIndex + 1, lastIndex, curBlock);
    return eGrmErrNoError;
  }
  return eGrmErrNoError;
}

uint32 GrammarAnalyzer::handleLiteralOperatorId(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 expOpe = expect("operator", index);
  if (eGrmErrNoError == expOpe)
  {
    uint32 expLi = expect("\"\"", index + 1);
    if (eGrmErrNoError == expLi)
    {
      return handleIdentifier(index + 2, lastIndex, curBlock);
    }
  }
  
  return eGrmErrNoError;
}

uint32 GrammarAnalyzer::handleOperatorFunctionId(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 expOperator = expect("operator", index);
  if (eGrmErrNoError == expOperator)
  {
//    uint32 ret = eGramIsNothing;
    uint32 opRet = getOverloadableOperator(index + 1, lastIndex, curBlock);
    if (eGrmErrNoError == opRet)
    {
      uint32 leftExp = expect("<", lastIndex + 1);
      if (eGrmErrNoError == leftExp)
      {
        uint32 templateRet = handleTemplateArgumentList(lastIndex + 2, lastIndex, curBlock);
        if (eGrmErrNoError == templateRet)
        {
          uint32 expRight = expect(">", lastIndex + 1);
          if (eGrmErrNoError == expRight)
          {
            lastIndex ++;
            return eGrmErrNoError;
          }
        }
      }
      else
      {
        return eGrmErrNoError;
      }
    }
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleSimpleTemplateId(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 tmpName = handleTemplateName(index, lastIndex, curBlock);
  if (eGrmErrNoError == tmpName)
  {
    uint32 expLeft = expect("<", lastIndex + 1);
    if (eGrmErrNoError == expLeft)
    {
      lastIndex ++;
      uint32 argListRet = handleTemplateArgumentList(lastIndex + 2, lastIndex, curBlock);
      uint32 expRight = expect(">", lastIndex + 1);
      if (eGrmErrNoError == expRight)
      {
        return expRight;
      }
    }
  }
  
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleTemplateId(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 nexTemp = handleSimpleTemplateId(index, lastIndex, curBlock);
  if (eGrmErrNoError == nexTemp)
  {
    return eGrmErrNoError;
  }

  uint32 operatorRet = handleOperatorFunctionId(index, lastIndex, curBlock);
  if (eGrmErrNoError == operatorRet)
  {
    uint32 expLeft = expect("<", lastIndex + 1);
    if (eGrmErrNoError == expLeft)
    {
      lastIndex ++;
      uint32 argRet = handleTemplateArgumentList(lastIndex, lastIndex, curBlock);
      uint32 expRight = expect(">", lastIndex + 1);
      if (eGrmErrNoError == expRight)
      {
        return eGrmErrNoError;
      }
    }
  }

  uint32 literalOpRet = handleLiteralOperatorId(index, lastIndex, curBlock);
  if (eGrmErrNoError == literalOpRet)
  {
    uint32 expLeft = expect("<", lastIndex + 1);
    if (eGrmErrNoError == expLeft)
    {
      lastIndex ++;
      uint32 argRet = handleTemplateArgumentList(lastIndex, lastIndex, curBlock);
      uint32 expRight = expect(">", lastIndex + 1);
      if (eGrmErrNoError == expRight)
      {
        return eGrmErrNoError;
      }
    }
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleTemplateName(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  return handleIdentifier(index, lastIndex, curBlock);
}

uint32 GrammarAnalyzer::handleTemplateArgumentList(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 tmpArg = handleTemplateArgument(index, lastIndex, curBlock);
  if (eGrmErrNoError == tmpArg)
  {
    uint32 expDot = expect("...", lastIndex + 1);
    if (eGrmErrNoError == expDot)
    {
      return eGrmErrNoError;
    }

    uint32 expComma = expect(",", lastIndex + 1);
    if (eGrmErrNoError == expComma)
    {
      return handleTemplateArgumentList(lastIndex + 1, lastIndex, curBlock);
    }
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleTemplateArgument(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 constExpRet = handleConstantExpression(index, lastIndex, curBlock);
  if (eGrmErrNoError == constExpRet)
  {
    return eGrmErrNoError;
  }

  uint32 expTypeId = handleTypeId(index, lastIndex, curBlock);
  if (eGrmErrNoError == expTypeId)
  {
    return eGrmErrNoError;
  }

  uint32 expIdExp = handleIdExpression(index, lastIndex, curBlock);
  if (eGrmErrNoError == expIdExp)
  {
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleFunctionBody(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 tryRet = handleFunctionTryBlock(index, lastIndex, curBlock);
  if (eGrmErrNoError == tryRet)
  {
    JZFUNC_END_LOG();
    return eGrmErrNoError;
  }

  uint32 ctorInitializerRet = handleCtorInitializer(index, lastIndex, curBlock);
  if (eGrmErrNoError == ctorInitializerRet)
  {
    JZFUNC_END_LOG();
    return handleCompoundStatement(lastIndex + 1, lastIndex, curBlock);
  }
  else
  {
    JZFUNC_END_LOG();
    return handleCompoundStatement(index, lastIndex, curBlock);
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleCompoundStatement(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 expLeft = expect("{", index);
  if (eGrmErrNoError == expLeft)
  {
    uint32 statRet = handleStatementSeq(index + 1,lastIndex, curBlock);
    if (eGrmErrNoError == statRet)
    {
      uint32 expRight = expect("}", lastIndex + 1);
      if (eGrmErrNoError == expRight)
      {
        return eGrmErrNoError;
      }
    }
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleNonPtrDeclarator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 idRet = handleDeclaratorId(index, lastIndex, curBlock);
  if (eGrmErrNoError == idRet)
  {
    handleAttributes(lastIndex + 1, lastIndex, curBlock);
    return eGrmErrNoError;
  }

  uint32 paramRet = handleParameterAndQualifiers(index, lastIndex, curBlock);
  if (eGrmErrNoError == paramRet)
  {
    handleNonPtrDeclarator(lastIndex + 1, lastIndex, curBlock);
    return eGrmErrNoError;
  }

  uint32 expLeftSeqRet = expect("[", index);
  if (eGrmErrNoError == expLeftSeqRet)
  {
    handleConstantExpression(index +  1, lastIndex, curBlock);
    uint32 expRightSeqRet = expect("]", lastIndex + 1);
    if (eGrmErrNoError == expRightSeqRet)
    {
      handleAttributes(lastIndex + 1, lastIndex, curBlock);
      handleNonPtrDeclarator(lastIndex + 1, lastIndex, curBlock);
      return eGrmErrNoError;
    }
  }

  uint32 expLeftBracket = expect("(", index);
  if (eGrmErrNoError == expLeftBracket)
  {
    uint32 ptrDec = handlePtrDeclarator(index + 1, lastIndex, curBlock);
    if (eGrmErrNoError == ptrDec)
    {
      uint32 expRight = expect(")", lastIndex + 1);
      if (eGrmErrNoError == expRight)
      {
        lastIndex ++;
        return eGrmErrNoError;
      }
    }
  }
  return eGrmErrNoError;
}

uint32 GrammarAnalyzer::handleStatementSeq(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 statRet = handleStatement(index, lastIndex, curBlock);
  if (eGrmErrNoError == statRet)
  {
    uint32 nextRet = handleStatementSeq(lastIndex + 1, lastIndex, curBlock);
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleParameterAndQualifiers(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 expLeft = expect("(", index);
  if (eGrmErrNoError == expLeft)
  {
    uint32 parameClauseRet = handleParameterDeclarationList(index + 1, lastIndex, curBlock);
    if (eGrmErrNoError == parameClauseRet)
    {
      uint32 expRight = expect(")", lastIndex + 1);
      if (eGrmErrNoError == expRight)
      {
        lastIndex ++;
        handleAttributes(lastIndex + 1, lastIndex, curBlock);
        handleCVQualifierSeq(lastIndex + 1, lastIndex, curBlock);
//        uint32 ret = eGramIsNothing;
        getRefQualifier(lastIndex + 1, lastIndex, curBlock);
        handleExceptionSpeciafier(lastIndex + 1, lastIndex, curBlock);
        return eGrmErrNoError;
      }
    }
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleParameterDeclarationClause(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  lastIndex = index;
  uint32 listRet = handleParameterDeclarationList(index, lastIndex, curBlock);
  if (eGrmErrNoError == listRet)
  {
    uint32 expComma = expect(",", lastIndex + 1);
    if (eGrmErrNoError == expComma)
    {
      uint32 expDot = expect("...", lastIndex + 2);
      if (eGrmErrNoError == expDot)
      {
        lastIndex += 2;
        return eGrmErrNoError;
      }
    }
    else
    {
      uint32 expDot = expect("...", lastIndex + 1);
      if (eGrmErrNoError == expDot)
      {
        lastIndex += 1;
      }
      return eGrmErrNoError;
    }
  }
  return eGrmErrNoError;
}

uint32 GrammarAnalyzer::handleParameterDeclarationList(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 declarationRet = handleParameterDeclaration(index, lastIndex, curBlock);
  if (eGrmErrNoError == declarationRet)
  {
    uint32 expComma = expect(",", lastIndex + 1);
    if (eGrmErrNoError == expComma)
    {
      return handleParameterDeclarationList(lastIndex + 1, lastIndex, curBlock);
    }

    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleParameterDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 handleAttRet = handleAttributes(index , lastIndex, curBlock);
  uint32 declSpecifierRet = eGrmErrUnknown;
  if (eGrmErrNoError == handleAttRet)
  {
    declSpecifierRet = handleDeclSpecifierSeq(lastIndex + 1, lastIndex, curBlock);
  }
  else
  {
    declSpecifierRet = handleDeclSpecifierSeq(index, lastIndex, curBlock);
  }

  if (eGrmErrNoError == declSpecifierRet)
  {
    uint32 declaRet = handleDeclarator(lastIndex + 1, lastIndex, curBlock);
    if (eGrmErrNoError == declaRet)
    {
      uint32 expEqual = expect("=", lastIndex + 1);
      if (eGrmErrNoError == expEqual)
      {
        return handleInitializerClause(lastIndex + 2, lastIndex, curBlock);
      }
      return eGrmErrNoError;
    }

    uint32 abstractRet = handleAbstractDeclarator(lastIndex + 1, lastIndex, curBlock);
    if (eGrmErrNoError == abstractRet)
    {
      uint32 expEqual = expect("=", lastIndex + 1);
      if (eGrmErrNoError == expEqual)
      {
        return handleInitializerClause(lastIndex + 2, lastIndex, curBlock);
      }
      return eGrmErrNoError;
    }
    return eGrmErrNoError;
  }

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleStatement(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  lastIndex = index;
  return eGrmErrNoError;
  //I will move this on at first
  uint32 handleLabelRet = handleLabeledStatement(index, lastIndex, curBlock);
  if (eGrmErrNoError == handleLabelRet)
  {
    return eGrmErrNoError;
  }

  uint32 declarationStateRet = handleDeclarationStatement(index, lastIndex, curBlock);
  if (eGrmErrNoError == declarationStateRet)
  {
    return eGrmErrNoError;
  }

  uint32 attRet = handleAttributes(index, lastIndex, curBlock);
  uint32 toIndex = index;
  if (eGrmErrNoError == attRet)
  {
    toIndex = lastIndex + 1;
  }

  uint32 expStat = handleExpressionStatement(toIndex, lastIndex, curBlock);
  if (eGrmErrNoError == expStat)
  {
    return eGrmErrNoError;
  }

  uint32 compoundStat = handleCompoundStatement(toIndex, lastIndex, curBlock);
  if (eGrmErrNoError == compoundStat)
  {
    return eGrmErrNoError;
  }

  uint32 selectionStat = handleSelectionStatement(toIndex, lastIndex, curBlock);
  if (eGrmErrNoError == expStat)
  {
    return eGrmErrNoError;
  }
  uint32 iterationRet = handleIterationStatement(toIndex, lastIndex, curBlock);
  if (eGrmErrNoError == iterationRet)
  {
    return eGrmErrNoError;
  }
  uint32 jumpStat = handleJumpStatement(toIndex, lastIndex, curBlock);
  if (eGrmErrNoError == jumpStat)
  {
    return eGrmErrNoError;
  }
  uint32 tryState = handleTryBlock(toIndex, lastIndex, curBlock);
  if (eGrmErrNoError == tryState)
  {
    return eGrmErrNoError;
  }

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleExpressionStatement(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 expRet = handleExpression(index, lastIndex, curBlock);
  if (eGrmErrNoError == expRet)
  {
    return expect(";", lastIndex + 1);
  }
  else
  {
    return expect(";", index );
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleEnumSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  JZFUNC_BEGIN_LOG();
  uint32 handleHead = handleEnumHead(index, lastIndex, curBlock);
  if (eGrmErrNoError == handleHead)
  {
    uint32 expLeft = expect("{", lastIndex + 1);
    if (eGrmErrNoError == expLeft)
    {
      uint32 listRet = handleEnumeratorList(lastIndex + 2, lastIndex, curBlock);
      if (eGrmErrNoError == listRet)
      {
        uint32 offset = expect(",", lastIndex + 1) == eGrmErrNoError ? 1:0;
        uint32 expRight = expect("}", lastIndex + 1 + offset);
        if (eGrmErrNoError == expRight)
        {
          lastIndex += offset;
          lastIndex++;
          JZFUNC_END_LOG();
          return eGrmErrNoError;
        }
      }
    }
  }

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleEnumHead(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
//  uint32 keyRet = eGramIsNothing;
  uint32 getRet = getEnumKey(index, lastIndex, curBlock);
  if (eGrmErrNoError == getRet)
  {
    handleAttributes(lastIndex + 1, lastIndex, curBlock);
    uint32 nestedNameRet = handleNestNameSpecifier(lastIndex + 1, lastIndex, curBlock);
    if (eGrmErrNoError == nestedNameRet)
    {
      uint32 idRet = handleIdentifier(lastIndex + 1, lastIndex, curBlock);
      if (eGrmErrNoError == idRet)
      {
        handleEnumBase(lastIndex + 1, lastIndex, curBlock);
          JZFUNC_END_LOG();
        return eGrmErrNoError;
      }
    }
    else
    {
      handleIdentifier(lastIndex + 1, lastIndex, curBlock);
      handleEnumBase(lastIndex + 1, lastIndex, curBlock);
      JZFUNC_END_LOG();
      return eGrmErrNoError;
    }
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleEnumBase(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 expOpt = expect(":", index);
  if (eGrmErrNoError == expOpt)
  {
    return handleTypeSpecifierSeq(index + 1, lastIndex, curBlock);
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleEnumeratorList(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  JZFUNC_BEGIN_LOG();
  uint32 defRet = handleEnumeratorDefinition(index, lastIndex, curBlock);
  if (eGrmErrNoError == defRet)
  {
    uint32 expComma = expect(",", lastIndex + 1);
    if (eGrmErrNoError == expComma)
    {
      handleEnumeratorList(lastIndex + 2, lastIndex, curBlock);
    }
    JZFUNC_END_LOG();
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleEnumeratorDefinition(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
//  uint32 expEnumerator = handleEnumerator(index, lastIndex, curBlock);
//  if (eGrmErrNoError == expEnumerator)
//  {
//    uint32 eqExp = expect("=", lastIndex + 1);
//    if (eGrmErrNoError == eqExp)
//    {
//      return handleConstantExpression(lastIndex + 2, lastIndex, curBlock);
//    }
//    return eGrmErrNoError;
//  }
//  return eGrmErrUnknown;
//  
//
//  Let's zuosi
//
  GrammarReturnerBase base;
  int32 trylast = index;

  bool ret = INVOKE(Enumerator, index, trylast, curBlock, &base, false) ;
  if (ret)
  {
    lastIndex = trylast;
    bool continueRet = EXPECT(trylast + 1, trylast, "=", false, false) &&
    INVOKE(ConstantExpression, trylast + 1, trylast, curBlock, &base,false) ;
    if (continueRet)
    {
      lastIndex = trylast;
    }
  }

  if (ret)
  {
    JZFUNC_END_LOG();
    return eGrmErrNoError;
  }
  else
  {
    JZFUNC_END_LOG();
    return eGrmErrUnknown;
  }
}

uint32 GrammarAnalyzer::handleEnumerator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  return handleIdentifier(index, lastIndex, curBlock);
}

uint32 GrammarAnalyzer::handleClassSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 classHead = handleClassHead(index, lastIndex, curBlock);
  if (eGrmErrNoError == classHead)
  {
    uint32 expRet = expect("{", lastIndex + 1, curBlock);
    if (eGrmErrNoError == expRet)
    {
      uint32 memberRet =handleMemberSpecification(lastIndex + 1, lastIndex, curBlock);
      uint32 expRight = expect("}", lastIndex + 1, curBlock);
      if (eGrmErrNoError == expRight)
      {
        lastIndex ++;
        return eGrmErrNoError;
      }
    }
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleClassHead(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
//  uint32 keyType = eGramIsNothing;
  uint32 keyRet = getClassKey(index, lastIndex, curBlock);
  if (eGrmErrNoError == keyRet)
  {
    handleAttributes(lastIndex + 1, lastIndex, curBlock);
    uint32 headnameRet = handleClassHeadName(lastIndex + 1, lastIndex, curBlock);
    if (eGrmErrNoError == headnameRet)
    {
      handleClassVirtSpecifierSeq(lastIndex + 1, lastIndex, curBlock);
    }
    handleBaseClause(lastIndex + 1, lastIndex, curBlock);
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleClassHeadName(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 nestedNameRet = handleNestNameSpecifier(index, lastIndex, curBlock);
  if (eGrmErrUnknown == nestedNameRet)
  {
    return handleClassName(lastIndex + 1, lastIndex, curBlock);
  }
  else
  {
    return handleClassName(index, lastIndex, curBlock);
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleClassVirtSpecifierSeq(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
//  uint32 ret = eGramIsNothing;
  uint32 getRet = getVirtSpecifier(index, lastIndex, curBlock);
  if (eGramIsNothing == getRet)
  {
    handleClassVirtSpecifierSeq(lastIndex + 1, lastIndex, curBlock);
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleBaseClause(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 exp1 = expect(":", index);
  if (eGrmErrNoError == exp1)
  {
    return handleBaseSpecifierList(index + 1, lastIndex, curBlock);
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleBaseSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 attRet = handleAttributes(index, lastIndex, curBlock);
  if (eGrmErrNoError == attRet )
  {
    uint32 baseRet = handleBaseTypeSpecifier(lastIndex + 1, lastIndex, curBlock);
    if (eGrmErrNoError == baseRet)
    {
      return eGrmErrNoError;
    }

//    uint32 ret = eGramIsNothing;
    uint32 expVir = expect("virtual", lastIndex + 1);
    if (eGrmErrNoError == expVir)
    {
      getAccessSpecifier(lastIndex + 2, lastIndex, curBlock);
      return handleBaseTypeSpecifier(lastIndex + 1, lastIndex, curBlock);
    }

    uint32 accessRet = getAccessSpecifier(lastIndex + 1, lastIndex, curBlock);
    if (eGrmErrNoError == accessRet)
    {
      uint32 offset = expect("virtual", lastIndex + 1) ? 1:0;
      return handleBaseTypeSpecifier(lastIndex + 1 + offset, lastIndex, curBlock);
    }
  }
  return eGrmErrUnknown;
}
uint32 GrammarAnalyzer::handleBaseSpecifierList(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 specRet = handleBaseSpecifier(index, lastIndex, curBlock);
  if (eGrmErrNoError == specRet)
  {
    uint32 expComma = expect(",", lastIndex + 1);
    if (eGrmErrNoError == expComma)
    {
      return handleBaseSpecifierList(lastIndex + 2, lastIndex, curBlock);
    }

    uint32 expDot = expect("...", lastIndex + 1);
    if (eGrmErrNoError == expDot)
    {
      lastIndex ++;
    }
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleBaseTypeSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  return handleClassOrDecltype(index, lastIndex, curBlock);
}

uint32 GrammarAnalyzer::handleClassOrDecltype(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 decRet = handleDecltypeSpecifier(index, lastIndex, curBlock);
  if (eGrmErrNoError == decRet)
  {
    return eGrmErrNoError;
  }

  uint32 offset = expect("::", index) == eGrmErrNoError ? 1:0;
  uint32 nestedNameRet = handleNestNameSpecifier(index + 1 + offset, lastIndex, curBlock);
  if (eGrmErrNoError == nestedNameRet)
  {
    return handleClassName(lastIndex + 1, lastIndex, curBlock);
  }
  else
  {
    return handleClassName(index + offset + 1, lastIndex, curBlock);
  }
  
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleDeclarationSeq(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 declaraRet = handleDeclaration(index, lastIndex, curBlock);
  if (eGrmErrNoError == declaraRet)
  {
    if (lastIndex + 1 == mRecList.size())
    {
      return eGrmErrNoError;
    }
    return handleDeclarationSeq(lastIndex + 1, lastIndex,  curBlock);
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 blockRet = handleBlockDeclaration(index, lastIndex, curBlock);
  if (eGrmErrNoError == blockRet)
  {
    return eGrmErrNoError;
  }

  uint32 functionRet = handleFuncDefinition(index, lastIndex, curBlock);
  if (eGrmErrNoError == functionRet)
  {
    return eGrmErrNoError;
  }
  uint32 templateRet = handleTemplateDecaration(index, lastIndex, curBlock);
  if (eGrmErrNoError == templateRet)
  {
    return eGrmErrNoError;
  }

  uint32 explicitInsRet = handleExplicitInstantiation(index, lastIndex, curBlock);
  if (eGrmErrNoError == explicitInsRet)
  {
    return eGrmErrNoError;
  }

  uint32 explicitSpecRet = handleExplicitSpecification(index, lastIndex, curBlock);
  if (eGrmErrNoError == explicitSpecRet)
  {
    return eGrmErrNoError;
  }

  uint32 linkageRet = handleLinkageSpecification(index, lastIndex, curBlock);
  if (eGrmErrNoError == linkageRet)
  {
    return eGrmErrNoError;
  }

  uint32 namespaceRet = handleNamespaceDefinition(index, lastIndex, curBlock);
  if (eGrmErrNoError == namespaceRet)
  {
    return eGrmErrNoError;
  }

  uint32 emptyRet = handleEmptyDeclaration(index, lastIndex, curBlock);
  if (eGrmErrNoError == emptyRet)
  {
    return eGrmErrNoError;
  }

  uint32 attRet = handleAttibuteDeclaration(index, lastIndex, curBlock);
  if (eGrmErrNoError == attRet)
  {
    return eGrmErrNoError;
  }

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleEmptyDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 expRet = expect(";", index);
  if (eGrmErrNoError == expRet)
  {
    lastIndex = index;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleBlockDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 simpleRet = handleSimpleDeclaration(index, lastIndex, curBlock);
  if (eGrmErrNoError == simpleRet)
  {
    return eGrmErrNoError;
  }

  uint32 asmRet = handleAsmDeclaration(index, lastIndex, curBlock);
  if (eGrmErrNoError == asmRet)
  {
    return eGrmErrNoError;
  }

  uint32 namespaceRet = handleNamespaceAliasDefinition(index, lastIndex, curBlock);
  if (eGrmErrNoError == namespaceRet)
  {
    return eGrmErrNoError;
  }

  uint32 usingDeclRet = handleUsingDeclaration(index, lastIndex, curBlock);
  if (eGrmErrNoError == usingDeclRet)
  {
    return eGrmErrNoError;
  }

  uint32 usingDireRet = handleUsingDirective(index, lastIndex, curBlock);
  if (eGrmErrNoError == usingDireRet)
  {
    return eGrmErrNoError;
  }

  uint32 static_assertRet = handleStatic_assertDeclaration(index, lastIndex, curBlock);
  if (eGrmErrNoError == static_assertRet)
  {
    return eGrmErrNoError;
  }

  uint32 aliasDeclRet = handleAliasDeclaration(index, lastIndex, curBlock);
  if (eGrmErrNoError == aliasDeclRet)
  {
    return eGrmErrNoError;
  }

  uint32 opaqueRet = handleOpaqueEnumDeclaration(index, lastIndex, curBlock);
  if (eGrmErrNoError == opaqueRet)
  {
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleSimpleDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int tryLastA = lastIndex;
  int tryLastB = lastIndex;
  int tryLastC = lastIndex;
  int tryIndex = index;
  uint32 attRet = handleAttributes(index, tryLastA, curBlock);
  if (eGrmErrNoError == attRet)
  {
    tryIndex = tryLastA + 1;
    lastIndex = tryLastA;
  }

  uint32 declSeqRet = handleDeclSpecifierSeq(tryIndex, tryLastB, curBlock);
  if (eGrmErrNoError == declSeqRet)
  {
    tryIndex = tryLastB + 1;
    lastIndex = tryLastB;
  }

  uint32 initRet = handleInitDeclaratorList(tryIndex,tryLastC, curBlock);
  if (eGrmErrNoError == initRet)
  {
    lastIndex = tryLastC;
    tryIndex = tryLastC + 1;
  }

  uint32 expEnd = expect(";", tryIndex);
  if (eGrmErrNoError == expEnd)
  {
    lastIndex = tryIndex;
    JZFUNC_END_LOG();
    return eGrmErrNoError;
  }
    
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleInitDeclaratorList(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int tryLastA = lastIndex;
  bool retA = INVOKE(InitDeclarator, index, tryLastA, curBlock, returner, false);
  if (retA)
  {
    lastIndex = tryLastA;
    int tryLastB = tryLastA;
    bool retB = 
      EXPECT(tryLastB + 1, tryLastB, ",", false, false) &&
      INVOKE(InitDeclaratorList, tryLastB + 1, tryLastB, curBlock, returner, false);
    if (retB)
    {
      lastIndex = tryLastB;
    }
    return eGrmErrNoError;
  }

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleInitDeclarator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = lastIndex;
  bool ret = 
    INVOKE(Declarator, index, tryLast, curBlock, returner, false) &&
    INVOKE(Initializer, tryLast + 1, tryLast, curBlock, returner, true);
  if (ret)
  {
    lastIndex = tryLast;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleInitializer(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLastA = lastIndex;
  bool retA = 
    INVOKE(BraceOrEqualInitializer, index, tryLastA, curBlock, returner, false);
  if (retA)
  {
    lastIndex = tryLastA;
    return eGrmErrNoError;
  }

  int32 tryLastB = lastIndex;
  bool retB = 
    EXPECT(index, tryLastB, "(", false, false) &&
    INVOKE(ExpressionList, tryLastB + 1, tryLastB, curBlock, returner, false) &&
    EXPECT(tryLastB + 1, tryLastB, ")", false, false) ;
  if (retB)
  {
    lastIndex = tryLastB;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleBraceOrEqualInitializer(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLastA = lastIndex;
  bool retA = INVOKE(BracedInitList, index, tryLastA, curBlock, returner, false);
  if (retA)
  {
    lastIndex = tryLastA;
    return eGrmErrNoError;
  }

  int32 tryLastB = lastIndex;
  bool retB = 
    EXPECT(index, tryLastB, "=", false, false)&&
    INVOKE(InitializerClause, tryLastB + 1, tryLastB, curBlock, returner, false);
  if (retB)
  {
    lastIndex = tryLastB;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

