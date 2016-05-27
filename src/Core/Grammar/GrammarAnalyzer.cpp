#include "GrammarAnalyzer.h"
#include "GrammarUtil.h"
#include "LexUtil.h"
#include "JZLogger.h"
#include "GrammarReturner.h"


uint32 GrammarAnalyzer::doAnalyze()
{
  int index = 0;
  int listSize = mRecList.size();
  int32 lastIndex = 0;
  GrammarReturnerBase *base = new GrammarReturnerBase(eGrmTop, "");
  bool ret = INVOKE(DeclarationSeq, 0, lastIndex, &mTopBlock, base, NOT_OPT);
  //should do some analyze
  base->printAllChild(0);
  delete base;
  if (ret)
  {
    return lastIndex == mRecList.size();
  }
  return eGrmErrUnknown;

}

uint32 GrammarAnalyzer::handleCVQualifierSeq(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index - 1;
  bool inLoop = false;
  GrammarReturnerBase* base = new GrammarReturnerBase(eCVQualifierSeq,"");
  while (INVOKE(CVQualifier, tryLast + 1, tryLast, curBlock, base, NOT_OPT))
  {
    inLoop = true;
  }
  if (inLoop)
  {
    if (returner)
    {
      returner -> addChild(base);
    }
    else
    {
      delete base;
    }
    lastIndex = tryLast;
    return eGrmErrNoError;
  }
  delete base;
  
  JZFUNC_END_LOG();
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleAttributes(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  //I don't care about attributes, so not going to add returner here
  uint32 expDoubleSquareBrackets1 = expect("[", index);
  uint32 expDoubleSquareBrackets2 = expect("[", index + 1);
  if(eGrmErrNoError == expDoubleSquareBrackets1 && eGrmErrNoError == expDoubleSquareBrackets2)
  {
    uint32 i = 1;
    uint32 expNextDoubleBrackets1 = expect("]", index + i);
    uint32 expNextDoubleBrackets2 = expect("]", index + i + 1);
    if(eGrmErrNoError == expNextDoubleBrackets1 && eGrmErrNoError == expNextDoubleBrackets2)
    {
      return eGrmErrUnexpDblSqlBracket;
    }

    do
    {
      if(expNextDoubleBrackets1 == eGrmErrFileEnd || expNextDoubleBrackets2 == eGrmErrFileEnd)
      {
        return eGrmErrExpectNextDblSqBracket;
      }
      i++;
      expNextDoubleBrackets1 = expect("]", index + i);
      expNextDoubleBrackets2 = expect("]", index + i + 1);
    }
    while(expNextDoubleBrackets1 != eGrmErrNoError || expNextDoubleBrackets2 != eGrmErrNoError);
    lastIndex = index + i + 1;

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

uint32 GrammarAnalyzer::handleFunctionDefinition(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  GrammarReturnerBase* base = new GrammarReturnerBase(eFunctionDefinition, "");
  bool ret = INVOKE(Attributes, index, tryLast, curBlock, base, IS_OPT) &&
    INVOKE(DeclSpecifierSeq, tryLast + 1, tryLast, curBlock, base, IS_OPT) &&
    INVOKE(Declarator, tryLast + 1, tryLast, curBlock, base, NOT_OPT);
  if (ret)
  {
    int32 tryLast01 = tryLast;
    if (EXPECT(tryLast01 + 1, tryLast01, "=", NOT_OPT, NOT_IN_ONE_LINE) &&
        EXPECT(tryLast01 + 1, tryLast01, "default", NOT_OPT, NOT_IN_ONE_LINE))
    {
      if (returner)
      {
        returner -> addChild(base);
      }
      else
      {
        delete base;
      }
      lastIndex = tryLast01;
      return eGrmErrNoError;
    }

    int32 tryLast02 = tryLast;
    if (EXPECT(tryLast02 + 1, tryLast02, "=", NOT_OPT, NOT_IN_ONE_LINE) &&
        EXPECT(tryLast02 + 1, tryLast02, "delete", NOT_OPT, NOT_IN_ONE_LINE))
    {
      if (returner)
      {
        returner -> addChild(base);
      }
      else
      {
        delete base;
      }
      lastIndex = tryLast02;
      return eGrmErrNoError;
    }

    int32 tryLast03 = tryLast;
    if (INVOKE(FunctionBody, tryLast03 + 1, tryLast03, curBlock, base, NOT_OPT))
    {
      if (returner)
      {
        returner -> addChild(base);
      }
      else
      {
        delete base;
      }
      lastIndex = tryLast03;
      return eGrmErrNoError;
      
    }
  }
  delete base;
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleDeclSpecifierSeq(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  bool ret = INVOKE(DeclSpecifier, index, tryLast, curBlock, returner, NOT_OPT);
  if (ret)
  {
    int32 tryLastA = tryLast;
    bool retA = INVOKE(Attributes, tryLastA + 1, tryLastA, curBlock, returner, NOT_OPT);
    if (retA)
    {
      lastIndex = tryLastA;
      return eGrmErrNoError;
    }

    int32 tryLastB = tryLast;
    bool retB = INVOKE(DeclSpecifierSeq, tryLastB + 1, tryLastB, curBlock, returner, NOT_OPT);
    if (retB)
    {
      lastIndex = tryLastB;
      return eGrmErrNoError;
    }
    lastIndex = tryLast;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleDeclSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
//  uint32 ret = eGramIsNothing;
  uint32 storageClassSpecRet = handleStorageClassSpecifier(index, lastIndex , curBlock);
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

  uint32 funcSpecifierRet = handleFunctionSpecifier(index, lastIndex , curBlock);
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

  uint32 trailingRet = handleTrailingTypeSpecifier(index, lastIndex, curBlock);
  if (eGrmErrNoError == trailingRet)
  {
    return eGrmErrNoError;
  }
  
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleTrailingTypeSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  GrammarReturnerBase *base001 = new GrammarReturnerBase(eTrailingTypeSpecifier, "");
  int32 trylast001 = index;
  if (INVOKE(SimpleTypeSpecifier, index, trylast001, curBlock, base001, NOT_OPT))
  {
    lastIndex = trylast001;
    if (returner)
    {
      returner->addChild(base001);
    }
    else{
      delete base001;
    }
    return eGrmErrNoError;
  }
  delete base001;
  base001 = NULL;

  GrammarReturnerBase *base002 = new GrammarReturnerBase(eTrailingTypeSpecifier, "");
  int32 trylast002 = index;
  if (INVOKE(ElaboratedTypeSpecifier, index, trylast002, curBlock, base002, NOT_OPT))
  {
    lastIndex = trylast002;
    if (returner)
    {
      returner->addChild(base002);
    }
    else{
      delete base002;
    }
    return eGrmErrNoError;
  }
  delete base002;
  base002 = NULL;

  GrammarReturnerBase *base003 = new GrammarReturnerBase(eTrailingTypeSpecifier, "");
  int32 trylast003 = index;
  if (INVOKE(ElaboratedTypeSpecifier, index, trylast003, curBlock, base003, NOT_OPT))
  {
    lastIndex = trylast003;
    if (returner)
    {
      returner->addChild(base003);
    }
    else{
      delete base003;
    }
    return eGrmErrNoError;
  }
  delete base003;
  base003 = NULL;

  GrammarReturnerBase *base004 = new GrammarReturnerBase(eTrailingTypeSpecifier, "");
  int32 trylast004 = index;
  if (INVOKE(TypenameSpecifier, index, trylast004, curBlock, base004, NOT_OPT))
  {
    lastIndex = trylast004;
    if (returner)
    {
      returner->addChild(base004);
    }
    else{
      delete base004;
    }
    return eGrmErrNoError;
  }
  delete base004;
  base004 = NULL;

  GrammarReturnerBase *base005 = new GrammarReturnerBase(eTrailingTypeSpecifier, "");
  int32 trylast005 = index;
  if (INVOKE(CVQualifier, index, trylast005, curBlock, base005, NOT_OPT))
  {
    lastIndex = trylast005;
    if (returner)
    {
      returner->addChild(base005);
    }
    else{
      delete base005;
    }
    return eGrmErrNoError;
  }
  delete base005;
  base005 = NULL;

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleSimpleTypeSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast001 = index;
  GrammarReturnerBase *base001 = new GrammarReturnerBase(eSimpleTypeSpecifier,"char");
  if (EXPECT(index, tryLast001, "char", NOT_OPT, NOT_IN_ONE_LINE))
  {
    lastIndex = tryLast001;
    if (returner)
    {
      returner->addChild(base001);
    }
    return eGrmErrNoError;
  }
  else
  {
    delete base001;
    base001 = NULL;
  }

  int32 tryLast002 = index;
  GrammarReturnerBase *base002 = new GrammarReturnerBase(eSimpleTypeSpecifier,"char16_t");
  if (EXPECT(index, tryLast002, "char16_t", NOT_OPT, NOT_IN_ONE_LINE))
  {
    lastIndex = tryLast002;
    if (returner)
    {
      returner->addChild(base002);
    }
    return eGrmErrNoError;
  }
  else
  {
    delete base002;
    base002 = NULL;
  }

  int32 tryLast003 = index;
  GrammarReturnerBase *base003 = new GrammarReturnerBase(eSimpleTypeSpecifier,"char32_t");
  if (EXPECT(index, tryLast003, "char32_t", NOT_OPT, NOT_IN_ONE_LINE))
  {
    lastIndex = tryLast003;
    if (returner)
    {
      returner->addChild(base003);
    }
    return eGrmErrNoError;
  }
  else
  {
    delete base003;
    base003 = NULL;
  }

  int32 tryLast004 = index;
  GrammarReturnerBase *base004 = new GrammarReturnerBase(eSimpleTypeSpecifier,"wchar_t");
  if (EXPECT(index, tryLast004, "wchar_t", NOT_OPT, NOT_IN_ONE_LINE))
  {
    lastIndex = tryLast004;
    if (returner)
    {
      returner->addChild(base004);
    }
    return eGrmErrNoError;
  }
  else
  {
    delete base004;
    base004 = NULL;
  }

  int32 tryLast005 = index;
  GrammarReturnerBase *base005 = new GrammarReturnerBase(eSimpleTypeSpecifier,"bool");
  if (EXPECT(index, tryLast005, "bool", NOT_OPT, NOT_IN_ONE_LINE))
  {
    lastIndex = tryLast005;
    if (returner)
    {
      returner->addChild(base005);
    }
    return eGrmErrNoError;
  }
  else
  {
    delete base005;
    base005 = NULL;
  }

  int32 tryLast006 = index;
  GrammarReturnerBase *base006 = new GrammarReturnerBase(eSimpleTypeSpecifier,"short");
  if (EXPECT(index, tryLast006, "short", NOT_OPT, NOT_IN_ONE_LINE))
  {
    lastIndex = tryLast006;
    if (returner)
    {
      returner->addChild(base006);
    }
    return eGrmErrNoError;
  }
  else
  {
    delete base006;
    base006 = NULL;
  }

  int32 tryLast007 = index;
  GrammarReturnerBase *base007 = new GrammarReturnerBase(eSimpleTypeSpecifier,"int");
  if (EXPECT(index, tryLast007, "int", NOT_OPT, NOT_IN_ONE_LINE))
  {
    lastIndex = tryLast007;
    if (returner)
    {
      returner->addChild(base007);
    }
    return eGrmErrNoError;
  }
  else
  {
    delete base007;
    base007 = NULL;
  }

  int32 tryLast008 = index;
  GrammarReturnerBase *base008 = new GrammarReturnerBase(eSimpleTypeSpecifier,"long");
  if (EXPECT(index, tryLast008, "long", NOT_OPT, NOT_IN_ONE_LINE))
  {
    lastIndex = tryLast008;
    if (returner)
    {
      returner->addChild(base008);
    }
    return eGrmErrNoError;
  }
  else
  {
    delete base008;
    base008 = NULL;
  }

  int32 tryLast009 = index;
  GrammarReturnerBase *base009 = new GrammarReturnerBase(eSimpleTypeSpecifier,"signed");
  if (EXPECT(index, tryLast009, "signed", NOT_OPT, NOT_IN_ONE_LINE))
  {
    lastIndex = tryLast009;
    if (returner)
    {
      returner->addChild(base009);
    }
    return eGrmErrNoError;
  }
  else
  {
    delete base009;
    base009 = NULL;
  }

  int32 tryLast010 = index;
  GrammarReturnerBase *base010 = new GrammarReturnerBase(eSimpleTypeSpecifier,"unsigned");
  if (EXPECT(index, tryLast010, "unsigned", NOT_OPT, NOT_IN_ONE_LINE))
  {
    lastIndex = tryLast010;
    if (returner)
    {
      returner->addChild(base010);
    }
    return eGrmErrNoError;
  }
  else
  {
    delete base010;
    base010 = NULL;
  }

  int32 tryLast011 = index;
  GrammarReturnerBase *base011 = new GrammarReturnerBase(eSimpleTypeSpecifier,"float");
  if (EXPECT(index, tryLast011, "float", NOT_OPT, NOT_IN_ONE_LINE))
  {
    lastIndex = tryLast011;
    if (returner)
    {
      returner->addChild(base011);
    }
    return eGrmErrNoError;
  }
  else
  {
    delete base011;
    base011 = NULL;
  }

  int32 tryLast012 = index;
  GrammarReturnerBase *base012 = new GrammarReturnerBase(eSimpleTypeSpecifier,"double");
  if (EXPECT(index, tryLast012, "double", NOT_OPT, NOT_IN_ONE_LINE))
  {
    lastIndex = tryLast012;
    if (returner)
    {
      returner->addChild(base012);
    }
    return eGrmErrNoError;
  }
  else
  {
    delete base012;
    base012 = NULL;
  }

  int32 tryLast013 = index;
  GrammarReturnerBase *base013 = new GrammarReturnerBase(eSimpleTypeSpecifier,"void");
  if (EXPECT(index, tryLast013, "void", NOT_OPT, NOT_IN_ONE_LINE))
  {
    lastIndex = tryLast013;
    if (returner)
    {
      returner->addChild(base013);
    }
    return eGrmErrNoError;
  }
  else
  {
    delete base013;
    base013 = NULL;
  }

  int32 tryLast014 = index;
  GrammarReturnerBase *base014 = new GrammarReturnerBase(eSimpleTypeSpecifier,"auto");
  if (EXPECT(index, tryLast014, "auto", NOT_OPT, NOT_IN_ONE_LINE))
  {
    lastIndex = tryLast014;
    if (returner)
    {
      returner->addChild(base014);
    }
    return eGrmErrNoError;
  }
  else
  {
    delete base014;
    base014 = NULL;
  }

  int32 tryLast015 = index;
  GrammarReturnerBase *base015 = new GrammarReturnerBase(eSimpleTypeSpecifier,"");
  if (INVOKE(DeclSpecifier, index, tryLast015, curBlock, base015, NOT_OPT))
  {
    lastIndex = tryLast015;
    if (returner)
    {
      returner->addChild(base015);
    }
    return eGrmErrNoError;
  }
  else
  {
    delete base015;
    base015 = NULL;
  }

  int32 tryLast016 = index;
  GrammarReturnerBase *base016 = new GrammarReturnerBase(eSimpleTypeSpecifier,"");
  bool ret016 = EXPECT(index, tryLast016, "::", IS_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(NestNameSpecifier, tryLast016 + 1, tryLast016, curBlock, base016, IS_OPT) &&
    INVOKE(TypeName, tryLast016 + 1, tryLast016, curBlock, base016, NOT_OPT);
  if ( ret016 )
  {
    int i = 0;
    GrammarReturnerBase* child = NULL;
    while(NULL != ( child = base016->getChild(i) ))
    {
      if (child->getType() == eTypeName)
      {
        const string& key = child->getKey();
        if (curBlock->getDataDef(key))
        {
          lastIndex = tryLast016;
          if (returner)
          {
            returner->addChild(base016);
          }
          else
          {
            delete base016;
          }
          return eGrmErrNoError;
        }
      }
      i++;
    }
  }
  delete base016;
  base016 = NULL;

  int32 tryLast017 = index;
  GrammarReturnerBase *base017 = new GrammarReturnerBase(eSimpleTypeSpecifier,"");
  bool ret017 = EXPECT(index, tryLast017, "::", IS_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(NestNameSpecifier, tryLast017 + 1, tryLast017, curBlock, base017, NOT_OPT) &&
    EXPECT(tryLast017 + 1, tryLast017, "template", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(SimpleTemplateId, tryLast017 + 1, tryLast017, curBlock, base017, NOT_OPT);
  if ( ret017 )
  {
    lastIndex = tryLast017;
    if (returner)
    {
      returner->addChild(base017);
    }
    return eGrmErrNoError;
  }
  else
  {
    delete base017;
    base017 = NULL;
  }


  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleDeclarator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  JZFUNC_BEGIN_LOG();
  int32 trylast001 = index;
  GrammarReturnerBase *base001 = new GrammarReturnerBase(eDeclarator, "");
  bool ret001 = INVOKE(PtrDeclarator, index, trylast001, curBlock, base001, NOT_OPT);
  if (ret001)
  {
    if (returner)
    {
      returner->addChild(base001);
    }
    else
    {
      delete base001;
    }
    lastIndex = trylast001;
    return eGrmErrNoError;
  }
  else
  {
    delete base001;
    base001 = NULL;
  }

//this part is cpp 11 standard
  int32 trylast002 = index;
  GrammarReturnerBase *base002 = new GrammarReturnerBase(eDeclarator, "");
  bool ret002 = INVOKE(NonPtrDeclarator, index, trylast002, curBlock, base002, NOT_OPT) &&
    INVOKE(ParametersAndQualifiers, trylast002 + 1, trylast002, curBlock, base002, NOT_OPT) &&
    INVOKE(TrailingReturnType, trylast002 + 1, trylast002, curBlock, base002, NOT_OPT);
  if (ret002)
  {
    lastIndex = trylast002;
    if (returner)
    {
      returner->addChild(base002);
    }
    return eGrmErrNoError;
  }
  else
  {
    delete base002;
    base002 = NULL;
  }

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleNonPtrDeclarator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  JZFUNC_BEGIN_LOG();
  int32 tryB = index - 1;
  bool loopIn = false;
  while(INVOKE(NonPtrDeclarator, tryB + 1, tryB, curBlock, returner, NOT_OPT))
  {
    JZWRITE_DEBUG("cur index: %d", tryB);
    loopIn = true;
  }
  if (loopIn)
  {

    int32 tryB1 = tryB;
    bool retB1 = INVOKE(ParametersAndQualifiers, tryB1 + 1, tryB1, curBlock, returner, NOT_OPT);
    if (retB1)
    {
      lastIndex = tryB1;
      JZFUNC_END_LOG();
      return eGrmErrNoError;
    }

    int32 tryB2 = tryB;

    bool retB2 = EXPECT(tryB2 + 1, tryB2, "[", NOT_OPT, NOT_IN_ONE_LINE) &&
      INVOKE(ConstantExpression, tryB2 + 1, tryB2, curBlock, returner, IS_OPT) &&
      EXPECT(tryB2 + 1, tryB2, "]", NOT_OPT, NOT_IN_ONE_LINE) &&
      INVOKE(Attributes, tryB2 + 1, tryB2, curBlock, returner, IS_OPT);
    if (retB2)
    {
      lastIndex = tryB2;
      JZFUNC_END_LOG();
      return eGrmErrNoError;
    }

    lastIndex = tryB;
    return eGrmErrNoError;
  }

  int32 tryA = index;
  //if we match declarator here, we don't go futher to parameters
  bool retA = INVOKE(DeclaratorId, index, tryA, curBlock, returner, NOT_OPT) &&
    INVOKE(Attributes, tryA + 1, tryA, curBlock, returner, IS_OPT);
  if (retA)
  {
    lastIndex = tryA;
    JZFUNC_END_LOG();
    return eGrmErrNoError;
  }

  int32 tryD = index;
  bool retD = EXPECT(index, tryD, "(", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(PtrDeclarator, tryD + 1, tryD, curBlock, returner, NOT_OPT) &&
    EXPECT(tryD + 1, tryD, ")", NOT_OPT, NOT_IN_ONE_LINE);
  if (retD)
  {
    lastIndex = tryD;
    JZFUNC_END_LOG();
    return eGrmErrNoError;
  }

  JZFUNC_END_LOG();
  return eGrmErrUnknown;
}


uint32 GrammarAnalyzer::handlePtrDeclarator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  JZFUNC_BEGIN_LOG();
  GrammarReturnerBase *base001 = new GrammarReturnerBase(ePtrDeclarator, "");
  int32 tryLast001 = index;
  if (INVOKE(NonPtrDeclarator, index, tryLast001, curBlock, base001, NOT_OPT))
  {
    if (returner)
    {
      returner -> addChild(base001);
    }
    else
    {
      delete base001;
    }
    lastIndex = tryLast001;
    return eGrmErrNoError;
  }
  delete base001;

  GrammarReturnerBase *base002 = new GrammarReturnerBase(ePtrDeclarator, "");
  int32 tryLast002 = index;
  if (INVOKE(PtrOperator, index, tryLast002, curBlock, base002, NOT_OPT) &&
      INVOKE(PtrDeclarator, tryLast002 + 1, tryLast002, curBlock, base002, NOT_OPT))
  {
    if (returner)
    {
      returner -> addChild(base002);
    }
    else
    {
      delete base002;
    }
    lastIndex = tryLast002;
    return eGrmErrNoError;
  }
  delete base002;

  return eGrmErrUnknown;
}


uint32 GrammarAnalyzer::handleNestNameSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryA = index - 1;
  bool inLoop = false;
  GrammarReturnerBase* base = new GrammarReturnerBase(eNestNameSpecifier, "");
  while(INVOKE(NestNameSpecifier, tryA + 1, tryA, curBlock, base, NOT_OPT))
  {
    inLoop = true;
  }
  if (inLoop)
  {
    lastIndex = tryA;
    int32 tryA1 = tryA;
    GrammarReturnerBase *base1 = new GrammarReturnerBase(eNestNameSpecifier,"");
    bool retA1 = INVOKE(Identifier, tryA1 + 1, tryA1, curBlock, base1, NOT_OPT) &&
      EXPECT(tryA1 + 1, tryA1, "::", NOT_OPT, NOT_IN_ONE_LINE);
    if (retA1)
    {
      if (returner)
      {
        base->mergeChild(base1);
        returner->addChild(base);
      }
      delete base1;
      lastIndex = tryA1;
      return eGrmErrNoError;
    }
    delete base1;

    int32 tryA2 = tryA;
    GrammarReturnerBase *base2 = new GrammarReturnerBase(eNestNameSpecifier, "");
    bool retA2 = EXPECT(tryA2 + 1, tryA2, "template", IS_OPT, NOT_IN_ONE_LINE) &&
      INVOKE(SimpleTemplateId, tryA2 + 1, tryA2, curBlock, base2, NOT_OPT) &&
      EXPECT(tryA2 + 1, tryA1, "::", NOT_OPT, NOT_IN_ONE_LINE);
    if (retA2)
    {
      if (returner)
      {
        base -> mergeChild(base2);
        returner -> addChild(base2);
      }
      delete base2;
      lastIndex = tryA2;
      return eGrmErrNoError;
    }
    delete base2;
    if (returner)
    {
      returner -> addChild(base);
    }
    else
    {
      delete base;
    }
    return eGrmErrNoError;
  }
  delete base;

  int32 tryB = index;
  GrammarReturnerBase *baseB = new GrammarReturnerBase(eNestNameSpecifier, "");
  bool retB = INVOKE(TypeName, tryB, tryB, curBlock, baseB, NOT_OPT) &&
      EXPECT(tryB + 1, tryB, "::", NOT_OPT, NOT_IN_ONE_LINE);

  if (retB)
  {
    lastIndex = tryB;
    if (returner)
    {
      returner->addChild(baseB);
    }
    else
    {
      delete baseB;
    }
    return eGrmErrNoError;
  }
  delete baseB;

  int32 tryC = index;
  GrammarReturnerBase *baseC = new GrammarReturnerBase(eNestNameSpecifier, "");
  bool retC = INVOKE(NamespaceName, tryC, tryC, curBlock, baseC, NOT_OPT) &&
      EXPECT(tryC + 1, tryC, "::", NOT_OPT, NOT_IN_ONE_LINE);
  if (retC)
  {
    lastIndex = tryC;
    if (returner)
    {
      returner -> addChild(baseC);
    }
    else
    {
      delete baseC;
    }
    return eGrmErrNoError;
  }
  delete baseC;

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handlePtrOperator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int tryLastA = index;
  GrammarReturnerBase *baseA = new GrammarReturnerBase(ePtrOperator, "");
  bool retA = EXPECT(index, tryLastA, "*", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Attributes, tryLastA + 1, tryLastA, curBlock, baseA, IS_OPT) &&
    INVOKE(CVQualifier, tryLastA + 1, tryLastA, curBlock, baseA, IS_OPT);
  if (retA)
  {
    lastIndex = tryLastA;
    if (returner)
    {
      returner -> addChild(baseA);
    }
    else
    {
      delete baseA;
    }
    return eGrmErrNoError;
  }
  delete baseA;

  int tryLastB = index;
  GrammarReturnerBase *baseB = new GrammarReturnerBase(ePtrOperator, "");
  bool retB = EXPECT(index, tryLastB, "&&", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Attributes, tryLastB + 1, tryLastB, curBlock, baseB, IS_OPT);
  if (retB)
  {
    if (returner)
    {
      returner -> addChild(baseB);
    }
    else
    {
      delete baseB;
    }
    lastIndex = tryLastB;
    return eGrmErrNoError;
  }
  delete baseB;

  GrammarReturnerBase *baseC = new GrammarReturnerBase(ePtrOperator, "");
  int tryLastC = index;
  bool retC = EXPECT(index, tryLastC, "&", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Attributes, tryLastC + 1, tryLastC, curBlock, baseC, IS_OPT);
  if (retC)
  {
    if (returner)
    {
      returner-> addChild(baseC);
    }
    else
    {
      delete baseC;
    }
    lastIndex = tryLastC;
    return eGrmErrNoError;
  }
  delete baseC;

  int tryLastD = index;
  GrammarReturnerBase *baseD = new GrammarReturnerBase(ePtrOperator, "");
  bool retD = EXPECT(index, tryLastD, "::", IS_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(NestNameSpecifier, tryLastD + 1, tryLastC, curBlock, baseD, NOT_OPT) &&
    EXPECT(tryLastD + 1, tryLastD, "*", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Attributes, tryLastC + 1, tryLastC, curBlock, baseD, IS_OPT) &&
    INVOKE(CVQualifier, tryLastC + 1, tryLastC, curBlock, baseD, IS_OPT);
  if (retD)
  {
    lastIndex = tryLastD;
    if (returner)
    {
      returner -> addChild(baseD);
    }
    else
    {
      delete baseD;
    }
    return eGrmErrNoError;
  }
  delete baseD;

  return eGrmErrUnknown;
}
uint32 GrammarAnalyzer::handleDeclaratorId(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast001 = index;
  GrammarReturnerBase *base001 = new GrammarReturnerBase(eDeclaratorId, "");
  bool ret001 = EXPECT(index, tryLast001, "...", IS_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(IdExpression, tryLast001 + 1, tryLast001, curBlock, base001, NOT_OPT);
  if (ret001)
  {
    if (returner)
    {
      returner->addChild(base001);
    }
    else
    {
      delete base001;
    }
    lastIndex = tryLast001;
    return eGrmErrNoError;
  }
  delete base001;

  int32 tryLast002 = index;
  GrammarReturnerBase *base002 = new GrammarReturnerBase(eDeclaratorId, "");
  bool ret002 = EXPECT(index, tryLast002, "::", IS_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(NestNameSpecifier, tryLast002 + 1, tryLast002, curBlock, base002, IS_OPT);
    INVOKE(ClassName, tryLast002 + 1, tryLast002, curBlock, base002, NOT_OPT);
  if (ret002)
  {
    if (returner)
    {
      returner->addChild(base002);
    }
    else
    {
      delete base002;
    }
    lastIndex = tryLast002;
    return eGrmErrNoError;
  }
  delete base002;
  
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleIdentifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
//  JZFUNC_BEGIN_LOG();
  if (mRecList.size() <= index)
  {
    return eGrmErrFileEnd;
  }

  string id = mRecList[index].word;

  if (true == isLegalVarIdentify(id, curBlock))
  {
    lastIndex = index;
    JZWRITE_DEBUG("id is: %s" , id.c_str());
//    JZFUNC_END_LOG();
    return eGrmErrNoError;
  }
  else
  {
//    JZFUNC_END_LOG();
    return eGrmErrDoubleDefinedVar;
  }
}

uint32 GrammarAnalyzer::handleNamespaceName(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
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
  int32 tryLast001 = index;
  GrammarReturnerBase * base001 = new GrammarReturnerBase(eClassName, "");
  if (INVOKE(Identifier, index, tryLast001, curBlock, base001, NOT_OPT))
  {
    if (returner)
    {
      returner->addChild(base001);
    }
    else
    {
      delete base001;
    }
    lastIndex = tryLast001;
    return eGrmErrNoError;
  }
  delete base001;

  int32 tryLast002 = index;
  GrammarReturnerBase * base002 = new GrammarReturnerBase(eClassName, "");
  if (INVOKE(SimpleTemplateId, index, tryLast002, curBlock, base002, NOT_OPT))
  {
    if (returner)
    {
      returner->addChild(base002);
    }
    else
    {
      delete base002;
    }
    lastIndex = tryLast002;
    return eGrmErrNoError;
  }
  delete base002;

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleTypeName(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  GrammarReturnerBase *base001 = new GrammarReturnerBase(eTypeName, "");
  int32 tryLast001 = index;
  bool ret001 = INVOKE(ClassName, index, tryLast001, curBlock,base001, NOT_OPT);
  if (ret001)
  {
    if (returner)
    {
      returner->addChild(base001);
    }
    else
    {
      delete base001;
    }
    lastIndex = tryLast001;
    return eGrmErrNoError;
  }
  else
  {
    delete base001;
    base001 = NULL;
  }

  GrammarReturnerBase *base002 = new GrammarReturnerBase(eTypeName, "");
  int32 tryLast002 = index;
  bool ret002 = INVOKE(EnumName, index, tryLast002, curBlock,base002, NOT_OPT);
  if (ret002)
  {
    if (returner)
    {
      returner->addChild(base002);
    }
    else
    {
      delete base002;
    }
    lastIndex = tryLast002;
    return eGrmErrNoError;
  }
  else
  {
    delete base002;
    base002 = NULL;
  }

  GrammarReturnerBase *base003 = new GrammarReturnerBase(eTypeName, "");
  int32 tryLast003 = index;
  bool ret003 = INVOKE(TypedefName, index, tryLast003, curBlock,base003, NOT_OPT);
  if (ret003)
  {
    if (returner)
    {
      returner->addChild(base003);
    }
    else
    {
      delete base003;
    }
    lastIndex = tryLast003;
    return eGrmErrNoError;
  }
  else
  {
    delete base003;
    base003 = NULL;
  }

  GrammarReturnerBase *base004 = new GrammarReturnerBase(eTypeName, "");
  int32 tryLast004 = index;
  bool ret004 = INVOKE(SimpleTemplateId, index, tryLast004, curBlock,base004, NOT_OPT);
  if (ret004)
  {
    if (returner)
    {
      returner->addChild(base004);
    }
    else
    {
      delete base004;
    }
    lastIndex = tryLast004;
    return eGrmErrNoError;
  }
  else
  {
    delete base004;
    base004 = NULL;
  }

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleIdExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  GrammarReturnerBase *base001 = new GrammarReturnerBase(eIdExpression,"");
  int32 tryLast001 = index;
  if (INVOKE(UnqualifiedId, index, tryLast001, curBlock, base001, NOT_OPT ))
  {
    if (returner)
    {
      returner -> addChild(base001);
    }
    else
    {
      delete base001;
    }
    lastIndex = tryLast001;
    return eGrmErrNoError;
  }
  delete base001;

  GrammarReturnerBase *base002 = new GrammarReturnerBase(eIdExpression,"");
  int32 tryLast002 = index;
  if (INVOKE(QualifiedId, index, tryLast002, curBlock, base002, NOT_OPT ))
  {
    if (returner)
    {
      returner -> addChild(base002);
    }
    else
    {
      delete base002;
    }
    lastIndex = tryLast002;
    return eGrmErrNoError;
  }
  delete base002;

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleUnqualifiedId(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
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

  uint32 literalOpId = handleLiteralOperatorId(index, lastIndex, curBlock);
  if (eGrmErrNoError == literalOpId)
  {
    return eGrmErrNoError;
  }

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

  uint32 idRet = handleIdentifier(index, lastIndex, curBlock);
  if (idRet == eGrmErrNoError)
  {
    return eGrmErrNoError;
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
  uint32 throwRet = handleThrowExpression(index, lastIndex, curBlock);
  if (eGrmErrNoError == throwRet)
  {
    return eGrmErrNoError;
  }
  uint32 logicRet = handleLogicOrExpression(index, lastIndex, curBlock);
  if (eGrmErrNoError == logicRet)
  {
//    uint32 ret = eGramIsNothing;
    uint32 expAssignmentRet = handleAssignmentOperator(lastIndex + 1, lastIndex, curBlock);
    if (eGrmErrNoError == expAssignmentRet)
    {
      return handleInitializerClause(lastIndex + 1, lastIndex, curBlock);
    }
  }
  
  uint32 condRet = handleConditionalExpression(index, lastIndex, curBlock);
  if (eGrmErrNoError == condRet)
  {
    return eGrmErrNoError;
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
  int32 tryLast001 = index;
  bool ret001 = INVOKE(CastExpression, index, tryLast001, curBlock, returner, NOT_OPT);
  if (ret001)
  {
    lastIndex = tryLast001;
  }
  else
  {
    return eGrmErrUnknown;
  }

  int32 tryLast002 = tryLast001;
  int32 tryLast003 = tryLast001;
  while(
      (EXPECT(tryLast002 + 1, tryLast002, ".*", NOT_OPT, NOT_IN_ONE_LINE) && INVOKE(CastExpression, tryLast002 + 1, tryLast002, curBlock, returner, NOT_OPT)) ||
      (EXPECT(tryLast003 + 1, tryLast003, "->*", NOT_OPT, NOT_IN_ONE_LINE) && INVOKE(CastExpression, tryLast003 + 1, tryLast003, curBlock, returner, NOT_OPT))
      )
  {
    if (tryLast003 > tryLast002)
    {
      tryLast002 = tryLast003;
    }
    else if (tryLast002 >  tryLast003)
    {
      tryLast003 = tryLast002;
    }
    lastIndex = tryLast002;
  }
  return eGrmErrNoError;

}

uint32 GrammarAnalyzer::handleCastExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast001 = index;
  bool ret001 = INVOKE(UnaryExpression, index, tryLast001, curBlock, returner, NOT_OPT) ;
  if (ret001)
  {
    lastIndex = tryLast001;
    return eGrmErrNoError;
  }

  int32 tryLast002 = index;
  bool ret002 = EXPECT(index, tryLast002, "(", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(TypeId, tryLast002 + 1, tryLast002, curBlock, returner, NOT_OPT)&&
    EXPECT(tryLast002 + 1, tryLast002, ")", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(CastExpression, tryLast002 + 1, tryLast002, curBlock, returner, NOT_OPT) ;
  if (ret002)
  {
    lastIndex = tryLast002;
    return eGrmErrNoError;
  }

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleTypeId(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  GrammarReturnerBase * base = new GrammarReturnerBase(eTypeId, "");
  bool ret = INVOKE(TypeSpecifierSeq, index, tryLast, curBlock, base, NOT_OPT) &&
    INVOKE(AbstractDeclarator, tryLast + 1, tryLast, curBlock, base, IS_OPT);
  if (ret)
  {
    if (returner)
    {
      returner->addChild(base);
    }
    else
    {
      delete base;
    }
    lastIndex = tryLast;
    return eGrmErrNoError;
  }
  else
  {
    delete base;
  }
  
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleTypeSpecifierSeq(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index - 1;
  GrammarReturnerBase *base = new GrammarReturnerBase(eTypeSpecifierSeq, "");
  bool inLoop = false;
  while (INVOKE(TypeSpecifier, tryLast + 1, tryLast, curBlock, base, NOT_OPT))
  {
    inLoop = true;
  }
  if (inLoop)
  {
    if (returner)
    {
      returner->addChild(base);
    }
    else
    {
      delete base;
    }
    lastIndex = tryLast;
    return eGrmErrNoError;
  }
  else
  {
    delete base;
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
    uint32 paramterRet = handleParametersAndQualifiers(lastIndex + 1, lastIndex ,curBlock);
    if (eGrmErrNoError == paramterRet)
    {
      return handleTrailingReturnType(lastIndex + 1, lastIndex ,curBlock);
    }
  }
  else
  {
    uint32 paramterRet = handleParametersAndQualifiers(index, lastIndex ,curBlock);
    if (eGrmErrNoError == paramterRet)
    {
      return handleTrailingReturnType(lastIndex + 1, lastIndex ,curBlock);
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
  uint32 paramRet = handleParametersAndQualifiers(index, lastIndex, curBlock);
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
  int32 tryLast001 = index;
  bool ret001 = EXPECT(index, tryLast001, "sizeof", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLast001 + 1, tryLast001, "...", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLast001 + 1, tryLast001, "(", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Identifier, tryLast001 + 1, tryLast001, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLast001 + 1, tryLast001, ")", NOT_OPT, NOT_IN_ONE_LINE);
  if (ret001)
  {
    lastIndex = tryLast001;
    return eGrmErrNoError;
  }

  int32 tryLast002 = index;
  bool ret002 = EXPECT(index, tryLast002, "sizeof", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(UnaryExpression, tryLast002 + 1, tryLast002, curBlock, returner, NOT_OPT);
  if (ret002)
  {
    lastIndex = tryLast002;
    return eGrmErrNoError;
  }

  int32 tryLast003 = index;
  bool ret003 = EXPECT(index, tryLast003, "sizeof", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLast003 + 1, tryLast003, "(", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(TypeId, tryLast003 + 1, tryLast003, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLast003 + 1, tryLast003, ")", NOT_OPT, NOT_IN_ONE_LINE);
  if (ret003)
  {
    lastIndex = tryLast003;
    return eGrmErrNoError;
  }

  int32 tryLast004 = index;
  bool ret004 = EXPECT(index, tryLast004, "alignof", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLast004 + 1, tryLast004, "(", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(TypeId, tryLast004 + 1, tryLast004, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLast004 + 1, tryLast004, ")", NOT_OPT, NOT_IN_ONE_LINE);
  if (ret004)
  {
    lastIndex = tryLast004;
    return eGrmErrNoError;
  }

  int32 tryLast005 = index;
  bool ret005 = EXPECT(index, tryLast005, "++", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(CastExpression, tryLast005 + 1, tryLast005, curBlock, returner, NOT_OPT);
  if (ret005)
  {
    lastIndex = tryLast005;
    return eGrmErrNoError;
  }

  int32 tryLast006 = index;
  bool ret006 = EXPECT(index, tryLast006, "--", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(CastExpression, tryLast006 + 1, tryLast006, curBlock, returner, NOT_OPT);
  if (ret006)
  {
    lastIndex = tryLast006;
    return eGrmErrNoError;
  }

  int32 tryLast007 = index;
  bool ret007 = INVOKE(NewExpression, tryLast007 , tryLast007, curBlock, returner, NOT_OPT);
  if (ret007)
  {
    lastIndex = tryLast007;
    return eGrmErrNoError;
  }

  int32 tryLast008 = index;
  bool ret008 = INVOKE(DeleteExpression, tryLast008 , tryLast008, curBlock, returner, NOT_OPT);
  if (ret008)
  {
    lastIndex = tryLast008;
    return eGrmErrNoError;
  }

  int32 tryLast009 = index;
  bool ret009 = INVOKE(NoexceptExpression, tryLast009 , tryLast009, curBlock, returner, NOT_OPT);
  if (ret009)
  {
    lastIndex = tryLast009;
    return eGrmErrNoError;
  }

  int32 tryLast0010 = index;
  bool ret0010 = INVOKE(PostfixExpression, tryLast0010 , tryLast0010, curBlock, returner, NOT_OPT);
  if (ret0010)
  {
    lastIndex = tryLast0010;
    return eGrmErrNoError;
  }

  int32 tryLast0011 = index;
  bool ret0011 = INVOKE(UnaryOperator, index, tryLast0011, curBlock, returner, NOT_OPT) &&
    INVOKE(CastExpression, tryLast0011 + 1, tryLast0011, curBlock, returner, NOT_OPT);
  if (ret0011)
  {
    lastIndex = tryLast0011;
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
  int32 tryLastLoop = index - 1;
  bool inLoop = false;
  while (INVOKE(PostfixExpression, tryLastLoop + 1, tryLastLoop, curBlock, returner, NOT_OPT))
  {
    inLoop = true;
  }
  if (inLoop)
  {

    bool inDo = false;
    do{
      inDo = false;
      int32 tryLastLoop001 = tryLastLoop;
      bool retLoop001 = EXPECT(tryLastLoop001 + 1, tryLastLoop001, "[", NOT_OPT, NOT_IN_ONE_LINE) &&
        INVOKE(Expression, tryLastLoop001 + 1, tryLastLoop001, curBlock, returner, NOT_OPT) &&
        EXPECT(tryLastLoop001 + 1, tryLastLoop001, "]", NOT_OPT, NOT_IN_ONE_LINE);
      if (retLoop001)
      {
        lastIndex = tryLastLoop001;
        tryLastLoop = tryLastLoop001;
        inDo = true;
        continue;
      }

      int32 tryLastLoop002 = tryLastLoop;
      bool retLoop002 = EXPECT(tryLastLoop002 + 1, tryLastLoop002, "[", NOT_OPT, NOT_IN_ONE_LINE) &&
        INVOKE(BracedInitList, tryLastLoop002 + 1, tryLastLoop002, curBlock, returner, IS_OPT) &&
        EXPECT(tryLastLoop002 + 1, tryLastLoop002, "]", NOT_OPT, NOT_IN_ONE_LINE);
      if (retLoop002)
      {
        lastIndex = tryLastLoop002;
        tryLastLoop = tryLastLoop002;
        inDo = true;
        continue;
      }

      int32 tryLastLoop003 = tryLastLoop;
      bool retLoop003 = EXPECT(tryLastLoop003 + 1, tryLastLoop003, "(", NOT_OPT, NOT_IN_ONE_LINE) &&
        INVOKE(ExpressionList, tryLastLoop003 + 1, tryLastLoop003, curBlock, returner, IS_OPT) &&
        EXPECT(tryLastLoop003 + 1, tryLastLoop003, ")", NOT_OPT, NOT_IN_ONE_LINE);
      if (retLoop003)
      {
        lastIndex = tryLastLoop003;
        tryLastLoop = tryLastLoop003;
        inDo = true;
        continue;
      }

      int32 tryLastLoop004 = tryLastLoop;
      bool retLoop004 = EXPECT(tryLastLoop004 + 1, tryLastLoop004, "++", NOT_OPT, NOT_IN_ONE_LINE);
      if (retLoop004)
      {
        lastIndex = tryLastLoop004;
        tryLastLoop = tryLastLoop004;
        inDo = true;
        continue;
      }
      
      int32 tryLastLoop005 = tryLastLoop;
      bool retLoop005 = EXPECT(tryLastLoop005 + 1, tryLastLoop005, "--", NOT_OPT, NOT_IN_ONE_LINE);
      if (retLoop005)
      {
        lastIndex = tryLastLoop005;
        tryLastLoop = tryLastLoop005;
        inDo = true;
        continue;
      }

      int32 tryLastLoop006 = tryLastLoop;
      bool retLoop006 = EXPECT(tryLastLoop006 + 1, tryLastLoop006, ".", NOT_OPT, NOT_IN_ONE_LINE) &&
        INVOKE(PseudoDestructorName, tryLastLoop006 + 1, tryLastLoop006, curBlock, returner, NOT_OPT);
      if (retLoop006)
      {
        lastIndex = tryLastLoop006;
        tryLastLoop = tryLastLoop006;
        inDo = true;
        continue;
      }

      int32 tryLastLoop007 = tryLastLoop;
      bool retLoop007 = EXPECT(tryLastLoop007 + 1, tryLastLoop007, "->", NOT_OPT, NOT_IN_ONE_LINE) &&
        INVOKE(PseudoDestructorName, tryLastLoop007 + 1, tryLastLoop007, curBlock, returner, NOT_OPT);
      if (retLoop007)
      {
        lastIndex = tryLastLoop007;
        tryLastLoop = tryLastLoop007;
        inDo = true;
        continue;
      }

      int32 tryLastLoop008 = tryLastLoop;
      bool retLoop008 = EXPECT(tryLastLoop008 + 1, tryLastLoop008, "->", NOT_OPT, NOT_IN_ONE_LINE) &&
        EXPECT(tryLastLoop008 + 1, tryLastLoop008, "template", IS_OPT, NOT_IN_ONE_LINE) &&
        INVOKE(IdExpression, tryLastLoop008 + 1, tryLastLoop008, curBlock, returner, NOT_OPT);
      if (retLoop008)
      {
        lastIndex = tryLastLoop008;
        tryLastLoop = tryLastLoop008;
        inDo = true;
        continue;
      }

      int32 tryLastLoop009 = tryLastLoop;
      bool retLoop009 = EXPECT(tryLastLoop009 + 1, tryLastLoop009, ".", NOT_OPT, NOT_IN_ONE_LINE) &&
        EXPECT(tryLastLoop009 + 1, tryLastLoop009, "template", IS_OPT, NOT_IN_ONE_LINE) &&
        INVOKE(IdExpression, tryLastLoop009 + 1, tryLastLoop009, curBlock, returner, NOT_OPT);
      if (retLoop009)
      {
        lastIndex = tryLastLoop009;
        tryLastLoop = tryLastLoop009;
        inDo = true;
        continue;
      }

    }while(inDo);

    lastIndex = tryLastLoop;

    return eGrmErrNoError;
  }
  int32 tryLastA = index;
  bool retA = EXPECT(index, tryLastA, "dynamic_cast", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLastA + 1, tryLastA, "<", NOT_OPT, NOT_OPT) &&
    INVOKE(TypeId, tryLastA + 1, tryLastA, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLastA + 1, tryLastA, ">", NOT_OPT, NOT_OPT) &&
    EXPECT(tryLastA + 1, tryLastA, "(", NOT_OPT, NOT_OPT) &&
    INVOKE(Expression, tryLastA + 1, tryLastA, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLastA + 1, tryLastA, ")", NOT_OPT, NOT_OPT);
  if (retA)
  {
    lastIndex = tryLastA;
    return eGrmErrNoError;
  }

  int32 tryLastB = index;
  bool retB = EXPECT(index, tryLastB, "static_cast", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLastB + 1, tryLastB, "<", NOT_OPT, NOT_OPT) &&
    INVOKE(TypeId, tryLastB + 1, tryLastB, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLastB + 1, tryLastB, ">", NOT_OPT, NOT_OPT) &&
    EXPECT(tryLastB + 1, tryLastB, "(", NOT_OPT, NOT_OPT) &&
    INVOKE(Expression, tryLastB + 1, tryLastB, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLastB + 1, tryLastB, ")", NOT_OPT, NOT_OPT);
  if (retB)
  {
    lastIndex = tryLastB;
    return eGrmErrNoError;
  }

  int32 tryLastC = index;
  bool retC = EXPECT(index, tryLastC, "reinterpret_cast", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLastC + 1, tryLastC, "<", NOT_OPT, NOT_OPT) &&
    INVOKE(TypeId, tryLastC + 1, tryLastC, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLastC + 1, tryLastC, ">", NOT_OPT, NOT_OPT) &&
    EXPECT(tryLastC + 1, tryLastC, "(", NOT_OPT, NOT_OPT) &&
    INVOKE(Expression, tryLastC + 1, tryLastC, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLastC + 1, tryLastC, ")", NOT_OPT, NOT_OPT);
  if (retC)
  {
    lastIndex = tryLastC;
    return eGrmErrNoError;
  }

  int32 tryLastD = index;
  bool retD = EXPECT(index, tryLastD, "const_cast", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLastD + 1, tryLastD, "<", NOT_OPT, NOT_OPT) &&
    INVOKE(TypeId, tryLastD + 1, tryLastD, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLastD + 1, tryLastD, ">", NOT_OPT, NOT_OPT) &&
    EXPECT(tryLastD + 1, tryLastD, "(", NOT_OPT, NOT_OPT) &&
    INVOKE(Expression, tryLastD + 1, tryLastD, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLastD + 1, tryLastD, ")", NOT_OPT, NOT_OPT);
  if (retD)
  {
    lastIndex = tryLastD;
    return eGrmErrNoError;
  }

  int32 tryLastE = index;
  bool retE = EXPECT(index, tryLastE, "typeid", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLastE + 1, tryLastE, "(", NOT_OPT, NOT_OPT) &&
    INVOKE(Expression, tryLastE + 1, tryLastE, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLastE + 1, tryLastE, ")", NOT_OPT, NOT_OPT);
  if (retE)
  {
    lastIndex = tryLastE;
    return eGrmErrNoError;
  }

  int32 tryLastF = index;
  bool retF = EXPECT(index, tryLastF, "typeid", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLastF + 1, tryLastF, "(", NOT_OPT, NOT_OPT) &&
    INVOKE(TypeId, tryLastF + 1, tryLastF, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLastF + 1, tryLastF, ")", NOT_OPT, NOT_OPT);
  if (retF)
  {
    lastIndex = tryLastF;
    return eGrmErrNoError;
  }

  int32 tryLast001 = index;
  bool ret001 = INVOKE(PrimaryExpression, index, tryLast001, curBlock, returner, NOT_OPT );
  if (ret001)
  {
    lastIndex = tryLast001;
    return eGrmErrNoError;
  }
  
  int32 tryLast002 = index;
  bool ret002 = INVOKE(SimpleTypeSpecifier, index, tryLast002, curBlock, returner, NOT_OPT ) &&
    EXPECT(tryLast002 + 1,tryLast002, "(", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(ExpressionList, tryLast002 + 1, tryLast002, curBlock, returner, IS_OPT) &&
    EXPECT(tryLast002 + 1,tryLast002, ")", NOT_OPT, NOT_IN_ONE_LINE); 
  if (ret002)
  {
    lastIndex = tryLast002;
    return eGrmErrNoError;
  }

  int32 tryLast003 = index;
  bool ret003 = INVOKE(TypenameSpecifier, index, tryLast003, curBlock, returner, NOT_OPT ) &&
    EXPECT(tryLast003 + 1,tryLast003, "(", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(ExpressionList, tryLast003 + 1, tryLast003, curBlock, returner, IS_OPT) &&
    EXPECT(tryLast003 + 1,tryLast003, ")", NOT_OPT, NOT_IN_ONE_LINE); 
  if (ret003)
  {
    lastIndex = tryLast003;
    return eGrmErrNoError;
  }

  int32 tryLast004 = index;
  bool ret004 = INVOKE(SimpleTypeSpecifier, index, tryLast004, curBlock, returner, NOT_OPT ) &&
    INVOKE(BracedInitList, tryLast004 + 1, tryLast004, curBlock, returner, NOT_OPT); 
  if (ret004)
  {
    lastIndex = tryLast004;
    return eGrmErrNoError;
  }

  int32 tryLast005 = index;
  bool ret005 = INVOKE(TypenameSpecifier, index, tryLast005, curBlock, returner, NOT_OPT ) &&
    INVOKE(BracedInitList, tryLast005 + 1, tryLast005, curBlock, returner, NOT_OPT) ;
  if (ret005)
  {
    lastIndex = tryLast005;
    return eGrmErrNoError;
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
  uint32 literalExp = handleLiteral(index, lastIndex, curBlock);
  if (eGrmErrNoError == literalExp)
  {
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

  uint32 lambdaExp = handleLambdaExpression(index, lastIndex, curBlock);
  if (eGrmErrNoError == lambdaExp)
  {
    return eGrmErrNoError;
  }

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

uint32 GrammarAnalyzer::handleOperatorFunctionId(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 expOperator = expect("operator", index);
  if (eGrmErrNoError == expOperator)
  {
//    uint32 ret = eGramIsNothing;
    uint32 opRet = handleOverloadableOperator(index + 1, lastIndex, curBlock);
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
  int32 tryLast = index;
  GrammarReturnerBase *base = new GrammarReturnerBase(eSimpleTemplateId, "");
  bool ret = INVOKE(TemplateName, index, tryLast, curBlock, base, NOT_OPT) &&
    EXPECT(tryLast + 1, tryLast, "<", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(TemplateArgumentList, tryLast + 1, tryLast, curBlock, base, IS_OPT) &&
    EXPECT(tryLast + 1, tryLast, ">", NOT_OPT, NOT_IN_ONE_LINE);
  if (ret)
  {
    if (returner)
    {
      returner -> addChild(base);
    }
    else
    {
      delete base;
    }
    lastIndex = tryLast;
    return eGrmErrNoError;
  }
  delete base;
  
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
  int32 tryLast = index;
  GrammarReturnerBase *base = new GrammarReturnerBase(eTemplateName, "");
  if (INVOKE(Identifier, index, tryLast, curBlock, base, NOT_OPT))
  {
    if (returner)
    {
      returner-> addChild(base);
    }
    else
    {
      delete base;
    }
    lastIndex = tryLast;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
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
  GrammarReturnerBase * base001 = new GrammarReturnerBase(eFunctionBody, "");
  int32 tryLast001 = index;
  bool ret001 =  INVOKE(FunctionTryBlock, index, tryLast001, curBlock, base001, NOT_OPT);
  if (ret001)
  {
    if (returner)
    {
      returner -> addChild(base001);
    }
    else
    {
      delete base001;
    }
    lastIndex = tryLast001;
    return eGrmErrNoError;
  }
  delete base001;

  GrammarReturnerBase * base002 = new GrammarReturnerBase(eFunctionBody, "");
  int32 tryLast002 = index;
  bool ret002 =  INVOKE(CtorInitializer, index, tryLast002, curBlock, base002, IS_OPT) &&
    INVOKE(CompoundStatement, tryLast002 +  1, tryLast002, curBlock, base002, NOT_OPT);
  if (ret002)
  {
    if (returner)
    {
      returner -> addChild(base002);
    }
    else
    {
      delete base002;
    }
    lastIndex = tryLast002;
    return eGrmErrNoError;
  }
  delete base002;

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleCompoundStatement(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  GrammarReturnerBase* base = new GrammarReturnerBase(eCompoundStatement, "");
  bool ret = EXPECT(index, tryLast, "{", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(StatementSeq, tryLast + 1, tryLast, curBlock, base, IS_OPT) &&
    EXPECT(tryLast + 1, tryLast, "}",  NOT_OPT, NOT_IN_ONE_LINE);
  if (ret)
  {
    if (returner)
    {
      returner -> addChild(base);
    }
    else
    {
      delete base;
    }
    lastIndex = tryLast;
    return eGrmErrNoError;
  }
  delete base;
  return eGrmErrUnknown;
}
uint32 GrammarAnalyzer::handleStatementSeq(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  bool inLoop = false;
  int32 tryLast = index - 1;
  while(INVOKE(Statement, tryLast + 1, tryLast, curBlock, returner, NOT_OPT))
  {
    inLoop = true;
  }
  if (inLoop)
  {
    lastIndex = tryLast;
    return eGrmErrNoError;
  }
  
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleParametersAndQualifiers(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  uint32 expLeft = expect("(", index);
  if (eGrmErrNoError == expLeft)
  {
    uint32 expRight = expect(")", index + 1);
    if (eGrmErrNoError == expRight)
    {
      lastIndex = index + 1;
      handleAttributes(lastIndex + 1, lastIndex, curBlock);
      handleCVQualifierSeq(lastIndex + 1, lastIndex, curBlock);
//        uint32 ret = eGramIsNothing;
      handleRefQualifier(lastIndex + 1, lastIndex, curBlock);
      handleExceptionSpecification(lastIndex + 1, lastIndex, curBlock);
      return eGrmErrNoError;
    }
    uint32 parameClauseRet = handleParameterDeclarationClause(index + 1, lastIndex, curBlock);
    if (eGrmErrNoError == parameClauseRet)
    {
      uint32 expRight = expect(")", lastIndex + 1);
      if (eGrmErrNoError == expRight)
      {
        lastIndex ++;
        handleAttributes(lastIndex + 1, lastIndex, curBlock);
        handleCVQualifierSeq(lastIndex + 1, lastIndex, curBlock);
//        uint32 ret = eGramIsNothing;
        handleRefQualifier(lastIndex + 1, lastIndex, curBlock);
        handleExceptionSpecification(lastIndex + 1, lastIndex, curBlock);
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
      return handleParameterDeclarationList(lastIndex + 2, lastIndex, curBlock);
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
  int32 tryLast001 = index;
  bool ret001 = INVOKE(Attributes, index, tryLast001, curBlock, returner, IS_OPT) &&
    INVOKE(JumpStatement, tryLast001 + 1, tryLast001, curBlock, returner, NOT_OPT);
  if (ret001)
  {
    lastIndex = tryLast001;
    return eGrmErrNoError;
  }

  int32 tryLast003 = index;
  bool ret003 = INVOKE(Attributes, index, tryLast003, curBlock, returner, IS_OPT) &&
    INVOKE(CompoundStatement, tryLast003 + 1, tryLast003, curBlock, returner, NOT_OPT);
  if (ret003)
  {
    lastIndex = tryLast003;
    return eGrmErrNoError;
  }

  int32 tryLast004 = index;
  bool ret004 = INVOKE(Attributes, index, tryLast004, curBlock, returner, IS_OPT) &&
    INVOKE(SelectionStatement, tryLast004 + 1, tryLast004, curBlock, returner, NOT_OPT);
  if (ret004)
  {
    lastIndex = tryLast004;
    return eGrmErrNoError;
  }

  int32 tryLast005 = index;
  bool ret005 = INVOKE(Attributes, index, tryLast005, curBlock, returner, IS_OPT) &&
    INVOKE(IterationStatement, tryLast005 + 1, tryLast005, curBlock, returner, NOT_OPT);
  if (ret005)
  {
    lastIndex = tryLast005;
    return eGrmErrNoError;
  }

  int32 tryLast006 = index;
  bool ret006 = INVOKE(Attributes, index, tryLast006, curBlock, returner, IS_OPT) &&
    INVOKE(TryBlock, tryLast006 + 1, tryLast006, curBlock, returner, NOT_OPT);
  if (ret006)
  {
    lastIndex = tryLast006;
    return eGrmErrNoError;
  }

  int32 tryLast002 = index;
  bool ret002 = INVOKE(Attributes, index, tryLast002, curBlock, returner, IS_OPT) &&
    INVOKE(ExpressionStatement, tryLast002 + 1, tryLast002, curBlock, returner, NOT_OPT);
  if (ret002)
  {
    lastIndex = tryLast002;
    return eGrmErrNoError;
  }

  int32 tryLast007 = index;
  bool ret007 = INVOKE(LabeledStatement, tryLast007 , tryLast007, curBlock, returner, NOT_OPT);
  if (ret007)
  {
    lastIndex = tryLast007;
    return eGrmErrNoError;
  }

  int32 tryLast008 = index;
  bool ret008 = INVOKE(DeclarationStatement, tryLast008 , tryLast008, curBlock, returner, NOT_OPT);
  if (ret008)
  {
    lastIndex = tryLast008;
    return eGrmErrNoError;
  }

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleExpressionStatement(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast001 = index;
  bool ret001 = INVOKE(Expression, index, tryLast001, curBlock, returner, IS_OPT) &&
    EXPECT(tryLast001 + 1, tryLast001, ";", NOT_OPT, NOT_IN_ONE_LINE);
  if (ret001)
  {
    lastIndex = tryLast001;
    return eGrmErrNoError;
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
  uint32 handleRet = handleEnumKey(index, lastIndex, curBlock);
  if (eGrmErrNoError == handleRet)
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
  GrammarReturnerBase *base = new GrammarReturnerBase(eEnumeratorDefinition,"");
  int32 trylast = index;

  bool ret = INVOKE(Enumerator, index, trylast, curBlock, base, false) ;
  if (ret)
  {
    lastIndex = trylast;
    bool continueRet = EXPECT(trylast + 1, trylast, "=", false, false) &&
    INVOKE(ConstantExpression, trylast + 1, trylast, curBlock, base,false) ;
    if (continueRet)
    {
      lastIndex = trylast;
    }
  }

  if (ret)
  {
    if (returner)
    {
      returner->addChild(base);
    }
    JZFUNC_END_LOG();
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
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
      uint32 memberRet =handleMemberSpecification(lastIndex + 2, lastIndex, curBlock);
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
  uint32 keyRet = handleClassKey(index, lastIndex, curBlock);
  if (eGrmErrNoError == keyRet)
  {
    handleAttributes(lastIndex + 1, lastIndex, curBlock);
    uint32 headnameRet = handleClassHeadName(lastIndex + 1, lastIndex, curBlock);
    if (eGrmErrNoError == headnameRet)
    {
      handleClassVirtSpecifierSeq(lastIndex + 1, lastIndex, curBlock);
    }
    handleBaseClause(lastIndex + 1, lastIndex, curBlock);
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleClassHeadName(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 trylast = index;
  GrammarReturnerBase * base = new GrammarReturnerBase(eClassHeadName, "");
  bool ret = INVOKE(NestNameSpecifier, index, trylast, curBlock, base, IS_OPT) &&
    INVOKE(ClassName, trylast + 1, trylast, curBlock, base, NOT_OPT);
  if (ret)
  {
    if (returner)
    {
      returner -> addChild(base);
    }
    else
    {
      delete base;
    }
    lastIndex = trylast;
    return eGrmErrNoError;
  }
  delete base;
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleClassVirtSpecifierSeq(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
//  uint32 ret = eGramIsNothing;
  uint32 handleRet = handleVirtSpecifier(index, lastIndex, curBlock);
  if (eGramIsNothing == handleRet)
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
      handleAccessSpecifier(lastIndex + 2, lastIndex, curBlock);
      return handleBaseTypeSpecifier(lastIndex + 1, lastIndex, curBlock);
    }

    uint32 accessRet = handleAccessSpecifier(lastIndex + 1, lastIndex, curBlock);
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
  bool inLoop = false ;
  int32 tryLast = index - 1;
  GrammarReturnerBase *base = new GrammarReturnerBase(eDeclarationSeq, "");
  while (INVOKE(Declaration, tryLast + 1, tryLast, curBlock, base, NOT_OPT))
  {
    inLoop = true;
  }
  if (inLoop)
  {
    lastIndex = tryLast;
    if (returner)
    {
      returner->addChild(base);
    }
    return eGrmErrNoError;
  }
  else
  {
    delete base;
    base = NULL;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  GrammarReturnerBase *base001 = new GrammarReturnerBase(eDeclaration, "");
  int32 tryLast001 = index;
  bool ret001 = INVOKE(BlockDeclaration, index, tryLast001, curBlock, base001, NOT_OPT );
  if (ret001)
  {
    lastIndex = tryLast001;
    if (returner)
    {
      returner->addChild(base001);
    }
    return eGrmErrNoError;
  }
  else
  {
    delete base001;
    base001 = NULL;
  }

  GrammarReturnerBase *base002 = new GrammarReturnerBase(eDeclaration, "");
  int32 tryLast002 = index;
  bool ret002 = INVOKE(FunctionDefinition, index, tryLast002, curBlock, base002, NOT_OPT );
  if (ret002)
  {
    lastIndex = tryLast002;
    if (returner)
    {
      returner->addChild(base002);
    }
    return eGrmErrNoError;
  }
  else
  {
    delete base002;
    base002 = NULL;
  }

  GrammarReturnerBase *base003 = new GrammarReturnerBase(eDeclaration, "");
  int32 tryLast003 = index;
  bool ret003 = INVOKE(TemplateDeclaration, index, tryLast003, curBlock, base003, NOT_OPT );
  if (ret003)
  {
    lastIndex = tryLast003;
    if (returner)
    {
      returner->addChild(base003);
    }
    return eGrmErrNoError;
  }
  else
  {
    delete base003;
    base003 = NULL;
  }

  GrammarReturnerBase *base004 = new GrammarReturnerBase(eDeclaration, "");
  int32 tryLast004 = index;
  bool ret004 = INVOKE(ExplicitInstantiation, index, tryLast004, curBlock, base004, NOT_OPT );
  if (ret004)
  {
    lastIndex = tryLast004;
    if (returner)
    {
      returner->addChild(base004);
    }
    return eGrmErrNoError;
  }
  else
  {
    delete base004;
    base004 = NULL;
  }

  GrammarReturnerBase *base005 = new GrammarReturnerBase(eDeclaration, "");
  int32 tryLast005 = index;
  bool ret005 = INVOKE(ExplicitSpecification, index, tryLast005, curBlock, base005, NOT_OPT );
  if (ret005)
  {
    lastIndex = tryLast005;
    if (returner)
    {
      returner->addChild(base005);
    }
    return eGrmErrNoError;
  }
  else
  {
    delete base005;
    base005 = NULL;
  }

  GrammarReturnerBase *base006 = new GrammarReturnerBase(eDeclaration, "");
  int32 tryLast006 = index;
  bool ret006 = INVOKE(LinkageSpecification, index, tryLast006, curBlock, base006, NOT_OPT );
  if (ret006)
  {
    lastIndex = tryLast006;
    if (returner)
    {
      returner->addChild(base006);
    }
    return eGrmErrNoError;
  }
  else
  {
    delete base006;
    base006 = NULL;
  }

  GrammarReturnerBase *base007 = new GrammarReturnerBase(eDeclaration, "");
  int32 tryLast007 = index;
  bool ret007 = INVOKE(NamespaceDefinition, index, tryLast007, curBlock, base007, NOT_OPT );
  if (ret007)
  {
    lastIndex = tryLast007;
    if (returner)
    {
      returner->addChild(base007);
    }
    return eGrmErrNoError;
  }
  else
  {
    delete base007;
    base007 = NULL;
  }

  GrammarReturnerBase *base008 = new GrammarReturnerBase(eDeclaration, "");
  int32 tryLast008 = index;
  bool ret008 = INVOKE(EmptyDeclaration, index, tryLast008, curBlock, base008, NOT_OPT );
  if (ret008)
  {
    lastIndex = tryLast008;
    if (returner)
    {
      returner->addChild(base008);
    }
    return eGrmErrNoError;
  }
  else
  {
    delete base008;
    base008 = NULL;
  }

  GrammarReturnerBase *base009 = new GrammarReturnerBase(eDeclaration, "");
  int32 tryLast009 = index;
  bool ret009 = INVOKE(AttibuteDeclaration, index, tryLast009, curBlock, base009, NOT_OPT );
  if (ret009)
  {
    lastIndex = tryLast009;
    if (returner)
    {
      returner->addChild(base009);
    }
    return eGrmErrNoError;
  }
  else
  {
    delete base009;
    base009 = NULL;
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
  int32 tryLast002 = index;
  GrammarReturnerBase* base002 = new GrammarReturnerBase(eBlockDeclaration, "");
  if (INVOKE(AsmDeclaration, index, tryLast002, curBlock, base002, NOT_OPT))
  {
    if (returner)
    {
      returner->addChild(base002);
    }
    else
    {
      delete base002;
    }
    lastIndex = tryLast002;
    return eGrmErrNoError;
  }
  delete base002;
  base002 = NULL;

  int32 tryLast003 = index;
  GrammarReturnerBase* base003 = new GrammarReturnerBase(eBlockDeclaration, "");
  if (INVOKE(NamespaceAliasDefinition, index, tryLast003, curBlock, base003, NOT_OPT))
  {
    if (returner)
    {
      returner->addChild(base003);
    }
    else
    {
      delete base003;
    }
    lastIndex = tryLast003;
    return eGrmErrNoError;
  }
  delete base003;
  base003 = NULL;

  int32 tryLast004 = index;
  GrammarReturnerBase* base004 = new GrammarReturnerBase(eBlockDeclaration, "");
  if (INVOKE(UsingDeclaration, index, tryLast004, curBlock, base004, NOT_OPT))
  {
    if (returner)
    {
      returner->addChild(base004);
    }
    else
    {
      delete base004;
    }
    lastIndex = tryLast004;
    return eGrmErrNoError;
  }
  delete base004;
  base004 = NULL;

  int32 tryLast005 = index;
  GrammarReturnerBase* base005 = new GrammarReturnerBase(eBlockDeclaration, "");
  if (INVOKE(UsingDirective, index, tryLast005, curBlock, base005, NOT_OPT))
  {
    if (returner)
    {
      returner->addChild(base005);
    }
    else
    {
      delete base005;
    }
    lastIndex = tryLast005;
    return eGrmErrNoError;
  }
  delete base005;
  base005 = NULL;
  
  int32 tryLast006 = index;
  GrammarReturnerBase* base006 = new GrammarReturnerBase(eBlockDeclaration, "");
  if (INVOKE(Static_assertDeclaration, index, tryLast006, curBlock, base006, NOT_OPT))
  {
    if (returner)
    {
      returner->addChild(base006);
    }
    else
    {
      delete base006;
    }
    lastIndex = tryLast006;
    return eGrmErrNoError;
  }
  delete base006;
  base006 = NULL;
 
  int32 tryLast007 = index;
  GrammarReturnerBase* base007 = new GrammarReturnerBase(eBlockDeclaration, "");
  if (INVOKE(AliasDeclaration, index, tryLast007, curBlock, base007, NOT_OPT))
  {
    if (returner)
    {
      returner->addChild(base007);
    }
    else
    {
      delete base007;
    }
    lastIndex = tryLast007;
    return eGrmErrNoError;
  }
  delete base007;
  base007 = NULL;
 

  int32 tryLast001 = index;
  GrammarReturnerBase* base001 = new GrammarReturnerBase(eBlockDeclaration, "");
  if (INVOKE(SimpleDeclaration, index, tryLast001, curBlock, base001, NOT_OPT))
  {
    if (returner)
    {
      returner->addChild(base001);
    }
    else
    {
      delete base001;
    }
    lastIndex = tryLast001;
    return eGrmErrNoError;
  }
  delete base001;
  base001 = NULL;
 
  int32 tryLast008 = index;
  GrammarReturnerBase* base008 = new GrammarReturnerBase(eBlockDeclaration, "");
  if (INVOKE(OpaqueEnumDeclaration, index, tryLast008, curBlock, base008, NOT_OPT))
  {
    if (returner)
    {
      returner->addChild(base008);
    }
    else
    {
      delete base008;
    }
    lastIndex = tryLast008;
    return eGrmErrNoError;
  }
  delete base008;
  base008 = NULL;
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleSimpleDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast001 = index;
  GrammarReturnerBase* base001 = new GrammarReturnerBase(eSimpleDeclaration, "");
  bool ret001 = INVOKE(Attributes, index, tryLast001, curBlock, base001, IS_OPT) &&
    INVOKE(DeclSpecifierSeq, tryLast001 + 1, tryLast001, curBlock, base001, IS_OPT) &&
    INVOKE(InitDeclaratorList, tryLast001 + 1, tryLast001, curBlock, base001, IS_OPT) &&
    EXPECT(tryLast001 + 1, tryLast001, ";", NOT_OPT, NOT_IN_ONE_LINE);
  if (ret001)
  {
    if (returner)
    {
      returner->addChild(base001);
    }
    else
    {
      delete base001;
    }
    lastIndex = tryLast001;
    return eGrmErrNoError;
  }
  delete base001;
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleInitDeclaratorList(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int tryLastA = index;
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
      JZWRITE_DEBUG("get true for , initDeclaratorList, tryLastB %d", tryLastB );
      lastIndex = tryLastB;
    }
    JZWRITE_DEBUG("get true for , initDeclaratorList, tryIndex %d", tryLastA );
    return eGrmErrNoError;
  }

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleInitDeclarator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  bool ret = 
    INVOKE(Declarator, index, tryLast, curBlock, returner, false) &&
    INVOKE(Initializer, tryLast + 1, tryLast, curBlock, returner, true);
  if (ret)
  {
    lastIndex = tryLast;
    JZFUNC_END_LOG();
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

uint32 GrammarAnalyzer::handleOpaqueEnumDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  bool ret = INVOKE(EnumKey, tryLast, tryLast, curBlock, returner, NOT_OPT) &&
    INVOKE(Attributes, tryLast + 1, tryLast, curBlock, returner, IS_OPT) &&
    INVOKE(Identifier, tryLast + 1, tryLast, curBlock, returner, NOT_OPT) &&
    INVOKE(EnumBase, tryLast + 1, tryLast, curBlock, returner, IS_OPT);
  if (ret)
  {
    lastIndex = tryLast;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}


uint32 GrammarAnalyzer::handleNamespaceAliasDefinition(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  bool ret = EXPECT(tryLast, tryLast, "namespace", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Identifier, tryLast + 1, tryLast, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLast + 1, tryLast, "=", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(QualifiedNamespaceSpecifier, tryLast + 1, tryLast, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLast + 1, tryLast, ";", NOT_OPT, NOT_IN_ONE_LINE);
  if (ret)
  {
    lastIndex = tryLast;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleQualifiedNamespaceSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  bool ret = EXPECT(index, tryLast, "::", IS_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(NestNameSpecifier, tryLast + 1, tryLast, curBlock, returner, IS_OPT) &&
    INVOKE(NamespaceName, tryLast + 1, tryLast, curBlock, returner, NOT_OPT);
  if (ret)
  {
    lastIndex = tryLast;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}


uint32 GrammarAnalyzer::handleNamespaceDefinition(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryA = index;
  bool retA = INVOKE(NamedNamespaceDefinition, index, tryA, curBlock, returner, NOT_OPT);
  if (retA)
  {
    lastIndex = tryA;
    return eGrmErrNoError;
  }

  int32 tryB = index;
  bool retB = INVOKE(UnnamedNamespaceDefinition, index, tryB, curBlock, returner, NOT_OPT);
  if (retB)
  {
    lastIndex = tryB;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleNamedNamespaceDefinition(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryA = index;
  bool retA = INVOKE(OriginalNamespaceDefinition, index, tryA, curBlock, returner, NOT_OPT);
  if (retA)
  {
    lastIndex = tryA;
    return eGrmErrNoError;
  }

  int32 tryB = index;
  bool retB = INVOKE(ExtensionNamespaceDefinition, index, tryB, curBlock, returner, NOT_OPT);
  if (retB)
  {
    lastIndex = tryB;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleOriginalNamespaceDefinition(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  bool ret = EXPECT(tryLast, tryLast, "inline", IS_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLast + 1, tryLast, "namespace", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Identifier, tryLast + 1, tryLast, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLast + 1, tryLast, "{", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(NamespaceBody, tryLast + 1, tryLast, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLast + 1, tryLast, "}", NOT_OPT, NOT_IN_ONE_LINE);
  if (ret)
  {
    lastIndex = tryLast;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleOriginalNamespaceName(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  bool ret = INVOKE(Identifier, index, tryLast, curBlock, returner, NOT_OPT);
  if (ret)
  {
    lastIndex = tryLast;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleNamespaceBody(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  bool ret = INVOKE(DeclarationSeq, index, tryLast , curBlock, returner, IS_OPT);
  if (ret)
  {
    lastIndex = tryLast;
  }
  return eGrmErrNoError;
 
}

uint32 GrammarAnalyzer::handleExtensionNamespaceDefinition(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  
  int32 tryLast = index;
  bool ret = EXPECT(tryLast, tryLast, "inline", IS_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLast + 1, tryLast, "namespace", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(OriginalNamespaceName, tryLast + 1, tryLast, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLast + 1, tryLast, "{", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(NamespaceBody, tryLast + 1, tryLast, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLast + 1, tryLast, "}", NOT_OPT, NOT_IN_ONE_LINE);
  if (ret)
  {
    lastIndex = tryLast;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleUnnamedNamespaceDefinition(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  bool ret = EXPECT(tryLast, tryLast, "inline", IS_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLast + 1, tryLast, "namespace", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLast + 1, tryLast, "{", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(NamespaceBody, tryLast + 1, tryLast, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLast + 1, tryLast, "}", NOT_OPT, NOT_IN_ONE_LINE);
  if (ret)
  {
    lastIndex = tryLast;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
   
}

uint32 GrammarAnalyzer::handleAsmDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  bool retA = EXPECT(tryLast, tryLast, "asm", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLast + 1, tryLast, "(", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(StringLiteral, tryLast + 1, tryLast, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLast + 1, tryLast, ")", NOT_OPT, NOT_IN_ONE_LINE);
    EXPECT(tryLast + 1, tryLast, ";", NOT_OPT, NOT_IN_ONE_LINE);
  if (retA)
  {
    lastIndex = tryLast;
    return eGrmErrNoError;
  }
    
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleUsingDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLastA = index;
  bool retA = EXPECT(tryLastA, tryLastA, "using", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLastA + 1, tryLastA, "typename", IS_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLastA + 1, tryLastA, "::", IS_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(NestNameSpecifier, tryLastA + 1, tryLastA, curBlock, returner, NOT_OPT) &&
    INVOKE(UnqualifiedId, tryLastA + 1, tryLastA, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLastA + 1, tryLastA, ";", NOT_OPT, NOT_IN_ONE_LINE);
  if (retA)
  {
    lastIndex = tryLastA;
    return eGrmErrNoError;
  }

  int32 tryLastB = index;
  bool retB = EXPECT(tryLastB, tryLastB, "using", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLastB + 1, tryLastB, "::", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(UnqualifiedId, tryLastB + 1, tryLastB, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLastB + 1, tryLastB, ";", NOT_OPT, NOT_IN_ONE_LINE);
  if (retB)
  {
    lastIndex = tryLastB;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleLinkageSpecification(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  bool ret = EXPECT(tryLast, tryLast, "extern", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(ExternLiteral, tryLast + 1, tryLast, curBlock, returner, NOT_OPT);
  if (ret)
  {
    int32 tryLastA = tryLast;
    bool retA = EXPECT(tryLastA + 1, tryLastA, "{", NOT_OPT, NOT_IN_ONE_LINE) &&
      INVOKE(DeclarationSeq, tryLastA + 1, tryLastA, curBlock, returner, IS_OPT) &&
      EXPECT(tryLastA + 1, tryLastA, "}", NOT_OPT, NOT_IN_ONE_LINE);
    if (retA)
    {
      lastIndex = tryLastA;
      return eGrmErrNoError;
    }

    int32 tryLastB = tryLast;
    bool retB = INVOKE(Declaration, tryLastA + 1, tryLastA, curBlock, returner, NOT_OPT) ;
    if (retB)
    {
      lastIndex = tryLastB;
      return eGrmErrNoError;
    }

  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleCtorInitializer(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  bool ret = EXPECT(tryLast, tryLast, ":", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(MemInitializerList, tryLast + 1, tryLast, curBlock, returner, NOT_OPT);
  if (ret)
  {
    lastIndex = tryLast;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleAttibuteDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index; 
  bool ret = INVOKE(Attributes, tryLast, tryLast, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLast + 1, tryLast, ";", NOT_OPT, NOT_IN_ONE_LINE);
  if (ret)
  {
    lastIndex = tryLast;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleMemInitializerId(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLastA = index;
  bool retA = INVOKE(ClassOrDecltype, index, tryLastA, curBlock, returner, NOT_OPT);
  if (retA)
  {
    lastIndex = tryLastA;
    return eGrmErrNoError;
  }

  int32 tryLastB = index;
  bool retB = INVOKE(Identifier, index, tryLastB, curBlock, returner, NOT_OPT);
  if (retB)
  {
    lastIndex = tryLastB;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleMemInitializerList(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  bool retA = INVOKE(MemInitializer, index, tryLast, curBlock, returner, NOT_OPT);
  if (retA)
  {
    int32 tryLastA = tryLast;
    bool retA1 = EXPECT(tryLastA + 1, tryLastA, ",", NOT_OPT, NOT_IN_ONE_LINE) &&
      INVOKE(MemInitializerList, tryLastA + 1, tryLastA, curBlock, returner, NOT_OPT);

    EXPECT(tryLastA + 1, tryLastA, "...", IS_OPT, NOT_IN_ONE_LINE);

    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleMemInitializer(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  bool ret = INVOKE(MemInitializerId, index, tryLast, curBlock, returner, NOT_OPT);
  if (ret)
  {
    int32 tryLastA = tryLast;
    bool retA = EXPECT(tryLastA + 1, tryLastA, "(", NOT_OPT, NOT_IN_ONE_LINE) &&
      INVOKE(ExpressionList,tryLastA + 1, tryLastA, curBlock, returner, IS_OPT) &&
      EXPECT(tryLastA + 1, tryLastA, ")", NOT_OPT, NOT_IN_ONE_LINE);
    if (retA)
    {
      lastIndex = tryLastA;
      return eGrmErrNoError;
    }

    int32 tryLastB = tryLast;
    bool retB =  INVOKE(BracedInitList,tryLastB + 1, tryLastB, curBlock, returner, NOT_OPT);
    if (retB)
    {
      lastIndex = tryLastB;
      return eGrmErrNoError;
    }
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleMemberSpecification(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLastA = index;
  bool retA = INVOKE(MemberDeclaration, index, tryLastA, curBlock, returner, NOT_OPT) &&
    INVOKE(MemberSpecification, tryLastA + 1,tryLastA, curBlock, returner, IS_OPT);
  if (retA)
  {
    lastIndex = tryLastA;
    return eGrmErrNoError;
  }

  int32 tryLastB = index;
  bool retB = INVOKE(AccessSpecifier, index, tryLastB, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLastB + 1, tryLastB, ":", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(MemberSpecification, tryLastB + 1,tryLastB, curBlock, returner, IS_OPT);
  if (retB)
  {
    lastIndex = tryLastB;
    return eGrmErrNoError;
  }

  return eGrmErrUnknown;
}


uint32 GrammarAnalyzer::handleMemberDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLastA = index;
  bool retA = INVOKE(Attributes, index, tryLastA, curBlock, returner, IS_OPT) &&
    INVOKE(DeclSpecifierSeq, tryLastA + 1, tryLastA, curBlock, returner, IS_OPT) &&
    INVOKE(MemberDeclaratorList, tryLastA + 1, tryLastA, curBlock, returner, IS_OPT) &&
    EXPECT(tryLastA + 1, tryLastA, ";", NOT_OPT, NOT_IN_ONE_LINE);
  if (retA)
  {
    lastIndex = tryLastA;
    return eGrmErrNoError;
  }

  int32 tryLastB = index;
  bool retB = INVOKE(FunctionDefinition, tryLastB, tryLastB, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLastA + 1, tryLastA, ";", IS_OPT, NOT_IN_ONE_LINE);
  if (retB)
  {
    lastIndex = tryLastB;
    return eGrmErrNoError;
  }

  int32 tryLastC = index;
  bool retC = INVOKE(UsingDeclaration, tryLastC, tryLastC, curBlock, returner, NOT_OPT);
  if (retC)
  {
    lastIndex = tryLastC;
    return eGrmErrNoError;
  }

  int32 tryLastD = index;
  bool retD = INVOKE(Static_assertDeclaration, tryLastD, tryLastD, curBlock, returner, NOT_OPT);
  if (retD)
  {
    lastIndex = tryLastD;
    return eGrmErrNoError;
  }

  int32 tryLastE = index;
  bool retE = INVOKE(TemplateDeclaration, tryLastE, tryLastE, curBlock, returner, NOT_OPT);
  if (retE)
  {
    lastIndex = tryLastE;
    return eGrmErrNoError;
  }

  int32 tryLastF = index;
  bool retF = INVOKE(AliasDeclaration, tryLastF, tryLastF, curBlock, returner, NOT_OPT);
  if (retF)
  {
    lastIndex = tryLastF;
    return eGrmErrNoError;
  }

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleMemberDeclarator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLastA = index;
  bool retA = INVOKE(Declarator, index, tryLastA, curBlock, returner, NOT_OPT) &&
    INVOKE(VirtSpecifierSeq, tryLastA + 1,tryLastA, curBlock, returner, IS_OPT);
  if (retA)
  {
    lastIndex = tryLastA;
    int32 tryLastA1 = tryLastA;
    bool retA1 = INVOKE(PureSpecifier, tryLastA1 + 1, tryLastA1, curBlock, returner, NOT_OPT);
    if (retA1)
    {
      lastIndex = tryLastA1;
      return eGrmErrNoError;
    }

    int32 tryLastA2 = tryLastA;
    bool retA2 = INVOKE(BraceOrEqualInitializer, tryLastA2 + 1, tryLastA2, curBlock, returner, NOT_OPT);
    if (retA2)
    {
      lastIndex = tryLastA2;
      return eGrmErrNoError;
    }
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleMemberDeclaratorList(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  bool ret = INVOKE(MemberDeclarator, index, tryLast, curBlock, returner, NOT_OPT);
  if (ret)
  {
    lastIndex = tryLast;
    int32 tryLastA = tryLast;
    bool retA = EXPECT(tryLast + 1, tryLastA, ",", NOT_OPT, NOT_IN_ONE_LINE) &&
      INVOKE(MemberDeclaratorList, tryLastA + 1, tryLastA, curBlock, returner, NOT_OPT);
    if (retA)
    {
      lastIndex = tryLastA;
    }

    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleVirtSpecifierSeq(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  bool ret = INVOKE(VirtSpecifier, index, tryLast, curBlock, returner, NOT_OPT) &&
    INVOKE(VirtSpecifierSeq, tryLast + 1, tryLast, curBlock, returner, IS_OPT);
  if (ret)
  {
    lastIndex = tryLast;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleLabeledStatement(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLastB = index;
  bool retB = INVOKE(Attributes, index, tryLastB, curBlock, returner, IS_OPT) &&
    EXPECT(tryLastB + 1, tryLastB, "case", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(ConstantExpression, tryLastB + 1, tryLastB, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLastB + 1, tryLastB, ":", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Statement, tryLastB + 1, tryLastB, curBlock, returner, NOT_OPT);
  if (retB)
  {
    lastIndex = tryLastB;
    return eGrmErrNoError;
  }

  int tryLastC = index;
  bool retC = INVOKE(Attributes, index, tryLastC, curBlock, returner, IS_OPT) &&
    EXPECT(tryLastC + 1, tryLastC, "default", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Statement, tryLastC + 1, tryLastC, curBlock, returner, NOT_OPT);
  if (retC)
  {
    lastIndex = tryLastC;
    return eGrmErrNoError;
  }

  int32 tryLastA = index;
  bool retA = INVOKE(Attributes, index, tryLastA, curBlock, returner, IS_OPT) &&
    INVOKE(Identifier, tryLastA + 1, tryLastA, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLastA + 1, tryLastA, ":", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Statement, tryLastA + 1, tryLastA, curBlock, returner, NOT_OPT);
  if (retA)
  {
    lastIndex = tryLastA;
    return eGrmErrNoError;
  }
    
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

uint32 GrammarAnalyzer::handleJumpStatement(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLastA = index;
  bool retA = EXPECT(index, tryLastA, "break", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLastA + 1, tryLastA, ";", NOT_OPT, NOT_IN_ONE_LINE);
  if (retA)
  {
    lastIndex = tryLastA;
    return eGrmErrNoError;
  }

  int32 tryLastB = index;
  bool retB = EXPECT(index, tryLastB, "continue", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLastB + 1, tryLastB, ";", NOT_OPT, NOT_IN_ONE_LINE);
  if (retB)
  {
    lastIndex = tryLastB;
    return eGrmErrNoError;
  }

  int32 tryLastC = index;
  bool retC = EXPECT(index, tryLastC, "return", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Expression, tryLastC + 1, tryLastC, curBlock, returner, IS_OPT) &&
    EXPECT(tryLastC + 1, tryLastC, ";", NOT_OPT, NOT_IN_ONE_LINE);
  if (retC)
  {
    lastIndex = tryLastC;
    return eGrmErrNoError;
  }

  int32 tryLastD = index;
  bool retD = EXPECT(index, tryLastD, "return", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(BracedInitList, tryLastD + 1, tryLastD, curBlock, returner, IS_OPT) &&
    EXPECT(tryLastD + 1, tryLastD, ";", NOT_OPT, NOT_IN_ONE_LINE);
  if (retD)
  {
    lastIndex = tryLastD;
    return eGrmErrNoError;
  }

  int32 tryLastE = index;
  bool retE = EXPECT(index, tryLastE, "goto", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Identifier, tryLastE + 1, tryLastD, curBlock, returner, IS_OPT) &&
    EXPECT(tryLastE + 1, tryLastE, ";", NOT_OPT, NOT_IN_ONE_LINE);
  if (retE)
  {
    lastIndex = tryLastE;
    return eGrmErrNoError;
  }

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleSelectionStatement(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLastA = index;
  bool retA = EXPECT(index, tryLastA, "if", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLastA + 1, tryLastA, "(", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Condition, tryLastA + 1, tryLastA, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLastA + 1, tryLastA, ")", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Statement, tryLastA + 1, tryLastA, curBlock, returner, NOT_OPT);

  if (retA)
  {
    int32 tryLastA1 = tryLastA;
    bool retA1 = EXPECT(tryLastA1 + 1, tryLastA1, "else", NOT_OPT, NOT_IN_ONE_LINE) &&
      INVOKE(Statement, tryLastA1 + 1, tryLastA1, curBlock, returner, NOT_OPT);

    if (retA1)
    {
      lastIndex = tryLastA1;
      return eGrmErrNoError;
    }

    lastIndex = tryLastA;
    return eGrmErrNoError;
  }

  int32 tryLastB = index;
  bool retB = EXPECT(index, tryLastB, "switch", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLastB + 1, tryLastB, "(", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Condition, tryLastB + 1, tryLastB, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLastB + 1, tryLastB, ")", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Statement, tryLastB + 1, tryLastB, curBlock, returner, NOT_OPT);
  if (retB)
  {
    lastIndex = tryLastB;
    return eGrmErrNoError;
  }

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleCondition(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLastA = index;
  bool retA = INVOKE(Expression, index, tryLastA, curBlock, returner, NOT_OPT);
  if (retA)
  {
    lastIndex = tryLastA;
    return eGrmErrNoError;
  }

  int32 tryLastB = index;
  bool retB = INVOKE(Attributes, index, tryLastB, curBlock, returner, IS_OPT) &&
    INVOKE(DeclSpecifierSeq, tryLastB + 1, tryLastB, curBlock, returner, NOT_OPT) &&
    INVOKE(Declarator, tryLastB + 1, tryLastB, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLastB + 1, tryLastB, "=", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(InitializerClause, tryLastB + 1, tryLastB, curBlock, returner, NOT_OPT);
  if (retB)
  {
    lastIndex = tryLastB;
    return eGrmErrNoError;
  }

  int32 tryLastC = index;
  bool retC = INVOKE(Attributes, index, tryLastC, curBlock, returner, IS_OPT) &&
    INVOKE(DeclSpecifierSeq, tryLastC + 1, tryLastC, curBlock, returner, NOT_OPT) &&
    INVOKE(Declarator, tryLastC + 1, tryLastC, curBlock, returner, NOT_OPT) &&
    INVOKE(BracedInitList, tryLastC + 1, tryLastC, curBlock, returner, NOT_OPT);
  if (retC)
  {
    lastIndex = tryLastC;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleIterationStatement(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLastA = index;
  bool retA = EXPECT(index, tryLastA, "while", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLastA + 1, tryLastA, "(", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Condition, tryLastA + 1, tryLastA, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLastA + 1, tryLastA, ")", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Statement, tryLastA + 1, tryLastA, curBlock, returner, NOT_OPT);
  if (retA)
  {
    lastIndex = tryLastA;
    return eGrmErrNoError;
  }

  int32 tryLastB = index;
  bool retB = EXPECT(index, tryLastB, "do", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Statement, tryLastB + 1, tryLastB, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLastB + 1, tryLastB, "while", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLastB + 1, tryLastB, "(", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Condition, tryLastB + 1, tryLastB, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLastB + 1, tryLastB, ")", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLastB + 1, tryLastB, ";", NOT_OPT, NOT_IN_ONE_LINE);
  if (retB)
  {
    lastIndex = tryLastB;
    return eGrmErrNoError;
  }

  int32 tryLastC = index;
  bool retC = EXPECT(index, tryLastC, "for", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLastC + 1, tryLastC, "(", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(ForInitStatement, tryLastC + 1, tryLastC, curBlock, returner, NOT_OPT) &&
    INVOKE(Condition, tryLastC + 1, tryLastC, curBlock, returner, IS_OPT) &&
    EXPECT(tryLastC + 1, tryLastC, ";", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Expression, tryLastC + 1, tryLastC, curBlock, returner, IS_OPT) &&
    EXPECT(tryLastC + 1, tryLastC, ")", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Statement, tryLastC + 1, tryLastC, curBlock, returner, NOT_OPT);
  if (retC)
  {
    lastIndex = tryLastC;
    return eGrmErrNoError;
  }

  int32 tryLastD = index;
  bool retD = EXPECT(index, tryLastD, "for", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLastD + 1, tryLastD, "(", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(ForRangeDeclaration, tryLastD + 1, tryLastD, curBlock, returner, NOT_OPT) &&
    INVOKE(ForRangeInitializer, tryLastD + 1, tryLastD, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLastD + 1, tryLastD, ")", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Statement, tryLastD + 1, tryLastD, curBlock, returner, NOT_OPT);
  if (retD)
  {
    lastIndex = tryLastD;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleForInitStatement(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLastA = index;
  bool retA = INVOKE(ExpressionStatement, index, tryLastA, curBlock, returner, NOT_OPT);
  if (retA)
  {
    lastIndex = tryLastA;
    return eGrmErrNoError;
  }

  int32 tryLastB = index;
  bool retB = INVOKE(SimpleDeclaration, index, tryLastB, curBlock, returner, NOT_OPT);
  if (retB)
  {
    lastIndex = tryLastB;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleForRangeDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLastA = index;
  bool retA = INVOKE(Attributes, index, tryLastA, curBlock, returner, IS_OPT) &&
    INVOKE(TypeSpecifierSeq, tryLastA + 1, tryLastA, curBlock, returner, NOT_OPT) &&
    INVOKE(Declarator, tryLastA + 1, tryLastA, curBlock, returner, NOT_OPT);
  if (retA)
  {
    lastIndex = tryLastA;
    return eGrmErrNoError;
  }

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleForRangeInitializer(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLastA = index;
  bool retA = INVOKE(Expression, index, tryLastA, curBlock, returner, NOT_OPT) &&
    INVOKE(BracedInitList, tryLastA + 1, tryLastA, curBlock, returner, NOT_OPT);
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleNewPlacement(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLastA = index;
  bool retA = EXPECT(index, tryLastA, "(", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(ExpressionList, tryLastA + 1, tryLastA, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLastA + 1, tryLastA, ")", NOT_OPT, NOT_IN_ONE_LINE);
  if (retA)
  {
    lastIndex = tryLastA;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleNewExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLastA = index;
  bool retA = EXPECT(index, tryLastA, "::", IS_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLastA + 1, tryLastA, "new", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(NewPlacement, tryLastA + 1, tryLastA, curBlock, returner, IS_OPT);
  if (retA)
  {
    int32 tryLastB = tryLastA;
    bool retB = INVOKE(NewTypeId, tryLastB + 1, tryLastB, curBlock, returner, NOT_OPT) &&
      INVOKE(NewInitializer, tryLastB + 1, tryLastB, curBlock, returner, IS_OPT);
    if (retB)
    {
      lastIndex = tryLastB;
      return eGrmErrNoError;
    }

    int32 tryLastC = tryLastA;
    bool retC = EXPECT(tryLastC + 1, tryLastC, "(", NOT_OPT, NOT_IN_ONE_LINE) &&
      INVOKE(TypeId, tryLastC + 1, tryLastC, curBlock, returner, NOT_OPT) &&
      EXPECT(tryLastC + 1, tryLastC, ")", NOT_OPT, NOT_IN_ONE_LINE) &&
      INVOKE(NewInitializer, tryLastC + 1, tryLastC, curBlock, returner, IS_OPT);
    if (retC)
    {
      lastIndex = tryLastC;
      return eGrmErrNoError;
    }
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleNewTypeId(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLastA = index;
  bool retA = INVOKE(TypeSpecifierSeq, index, tryLastA, curBlock, returner, NOT_OPT) &&
    INVOKE(NewDeclarator, tryLastA + 1, tryLastA, curBlock, returner, IS_OPT);
  if (retA)
  {
    lastIndex = tryLastA;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleNewDeclarator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLastA = index;
  bool retA =INVOKE(PtrOperator, index, tryLastA, curBlock, returner, NOT_OPT) &&
    INVOKE(NewDeclarator, tryLastA + 1, tryLastA, curBlock, returner, IS_OPT) ;
  if (retA)
  {
    lastIndex = tryLastA;
    return eGrmErrNoError;
  }

  int32 tryLastB = index;
  bool retB =INVOKE(NoptrNewDeclarator, index, tryLastB, curBlock, returner, NOT_OPT);
  if (retB)
  {
    lastIndex = tryLastB;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleNewInitializer(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLastB = index;
  bool retB = EXPECT(tryLastB, tryLastB, "(", NOT_OPT, NOT_IN_ONE_LINE) && 
    INVOKE(ExpressionList, tryLastB + 1, tryLastB, curBlock, returner, IS_OPT);
  if (retB)
  {
    lastIndex = tryLastB;
    return eGrmErrNoError;
  }

  int32 tryLastA = index;
  bool retA = INVOKE(BracedInitList, index, tryLastA, curBlock, returner, NOT_OPT);
  if (retA)
  {
    lastIndex = tryLastA;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleNoptrNewDeclarator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLastB = index;
  bool retB = EXPECT(tryLastB, tryLastB, "[", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Expression, tryLastB + 1, tryLastB, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLastB + 1, tryLastB, "]", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Attributes, tryLastB + 1, tryLastB, curBlock, returner, NOT_OPT);
  if (retB)
  {
    lastIndex = tryLastB;
    return eGrmErrNoError;
  }

  int32 tryLastA = index - 1;
  bool inLoop = false;
  while(INVOKE(NoptrNewDeclarator, tryLastA + 1, tryLastA, curBlock, returner, NOT_OPT))
  {
    inLoop = true;
  }
  if (inLoop)
  {
    int32 tryLastB = tryLastA;
    bool retB = EXPECT(tryLastB + 1, tryLastB, "[", NOT_OPT, NOT_IN_ONE_LINE) &&
      INVOKE(ConstantExpression, tryLastB + 1, tryLastB, curBlock, returner, NOT_OPT) &&
      EXPECT(tryLastB + 1, tryLastB, "]", NOT_OPT, NOT_IN_ONE_LINE) &&
      INVOKE(Attributes, tryLastB + 1, tryLastB, curBlock, returner, NOT_OPT);
    if (retB)
    {
      lastIndex = tryLastB;
      return eGrmErrNoError;
    }
    lastIndex = tryLastA;
    return eGrmErrNoError;
  }

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleTemplateDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  bool ret = EXPECT(index, tryLast, "template", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLast + 1, tryLast, "<", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(TemplateParameterList, tryLast + 1, tryLast, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLast + 1, tryLast, ">", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Declaration, tryLast + 1, tryLast, curBlock, returner, NOT_OPT);
  if (ret)
  {
    lastIndex = tryLast;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}


uint32 GrammarAnalyzer::handleDeclarationStatement(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  return handleBlockDeclaration(index, lastIndex, curBlock, returner);
}

uint32 GrammarAnalyzer::handleTemplateParameterList(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  bool ret = INVOKE(TemplateParameter, index, tryLast, curBlock, returner, NOT_OPT);
  if (ret)
  {
    int32 tryLastA = tryLast;
    bool retA = EXPECT(tryLastA + 1, tryLastA, ",", NOT_OPT, NOT_IN_ONE_LINE) &&
      INVOKE(TemplateParameterList, tryLastA + 1, tryLastA, curBlock, returner, NOT_OPT);
    if (retA)
    {
      lastIndex = tryLastA;
      return eGrmErrNoError;
    }
    lastIndex = tryLast;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleTemplateParameter(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLastA = index;
  bool retA = INVOKE(TypeParameter, index, tryLastA, curBlock, returner, NOT_OPT);
  if (retA)
  {
    lastIndex = tryLastA;
    return eGrmErrNoError;
  }

  int32 tryLastB = index;
  bool retB = INVOKE(ParameterDeclaration, index, tryLastB, curBlock, returner, NOT_OPT);
  if (retB)
  {
    lastIndex = tryLastB;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleTypeParameter(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLastA = index;
  bool retA = EXPECT(index, tryLastA, "class", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLastA + 1, tryLastA, "...", IS_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Identifier, tryLastA + 1, tryLastA, curBlock, returner, IS_OPT);
  if (retA)
  {
    lastIndex = tryLastA;
    return eGrmErrNoError;
  }

  int32 tryLastB = index;
  bool retB = EXPECT(index, tryLastB, "class", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Identifier, tryLastB + 1, tryLastB, curBlock, returner, IS_OPT) &&
    EXPECT(tryLastB + 1, tryLastB, "=", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(TypeId, tryLastB + 1, tryLastB, curBlock, returner, IS_OPT) ;
  if (retB)
  {
    lastIndex = tryLastB;
    return eGrmErrNoError;
  }

  int32 tryLastC = index;
  bool retC = EXPECT(index, tryLastC, "typename", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLastC + 1, tryLastC, "...", IS_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Identifier, tryLastC + 1, tryLastC, curBlock, returner, IS_OPT);
  if (retC)
  {
    lastIndex = tryLastC;
    return eGrmErrNoError;
  }

  int32 tryLastD = index;
  bool retD = EXPECT(index, tryLastD, "typename", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Identifier, tryLastD + 1, tryLastD, curBlock, returner, IS_OPT) &&
    EXPECT(tryLastD + 1, tryLastD, "=", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(TypeId, tryLastD + 1, tryLastD, curBlock, returner, IS_OPT) ;
  if (retD)
  {
    lastIndex = tryLastD;
    return eGrmErrNoError;
  }


  int32 tryLastE = index;
  bool retE = EXPECT(index, tryLastE, "template", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLastE + 1, tryLastE, "<", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(TemplateParameterList, tryLastE + 1, tryLastE, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLastE + 1, tryLastE, ">", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLastE + 1, tryLastE, "class", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLastE + 1, tryLastE, "...", IS_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Identifier, tryLastE + 1, tryLastE, curBlock, returner, IS_OPT);
  if (retE)
  {
    lastIndex = tryLastE;
    return eGrmErrNoError;
  }

  int32 tryLastF = index;
  bool retF = EXPECT(index, tryLastF, "template", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLastF + 1, tryLastF, "<", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(TemplateParameterList, tryLastF + 1, tryLastF, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLastF + 1, tryLastF, ">", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLastF + 1, tryLastF, "class", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Identifier, tryLastF + 1, tryLastF, curBlock, returner, IS_OPT) &&
    EXPECT(tryLastF + 1, tryLastF, "=", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(TypeId, tryLastF + 1, tryLastF, curBlock, returner, IS_OPT) ;
  if (retF)
  {
    lastIndex = tryLastF;
    return eGrmErrNoError;
  }

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleExplicitSpecification(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  bool ret = EXPECT(index, tryLast, "template", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLast + 1, tryLast, "<", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLast + 1, tryLast, ">", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Declaration, tryLast + 1, tryLast, curBlock, returner, NOT_OPT);
  if (ret)
  {
    lastIndex = tryLast;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleExplicitInstantiation(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  bool ret = EXPECT(index, tryLast, "extern", IS_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLast + 1, tryLast, "template", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Declaration, tryLast + 1, tryLast, curBlock, returner, NOT_OPT);
  if (ret)
  {
    lastIndex = tryLast;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleAliasDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  bool ret = EXPECT(index, tryLast, "using", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Identifier, tryLast + 1, tryLast, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLast + 1, tryLast, "=", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(TypeId, tryLast + 1, tryLast, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLast + 1, tryLast, ";", NOT_OPT, NOT_IN_ONE_LINE);
  if (ret)
  {
    lastIndex = tryLast;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleStatic_assertDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  bool ret = EXPECT(index, tryLast, "static_assert", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLast + 1, tryLast, "(", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(ConstantExpression, tryLast + 1, tryLast, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLast + 1, tryLast, ",", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(StringLiteral, tryLast + 1, tryLast, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLast + 1, tryLast, ")", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLast + 1, tryLast, ";", NOT_OPT, NOT_IN_ONE_LINE);
  if (ret)
  {
    lastIndex = tryLast;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleUsingDirective(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  bool ret = INVOKE(Attributes, index, tryLast, curBlock, returner, IS_OPT) &&
    EXPECT(tryLast + 1, tryLast, "using", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLast + 1, tryLast, "namespace", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLast + 1, tryLast, "::", IS_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(NestNameSpecifier, tryLast + 1, tryLast, curBlock, returner, IS_OPT) &&
    INVOKE(NamespaceName, tryLast + 1, tryLast, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLast + 1, tryLast, ";", NOT_OPT, NOT_IN_ONE_LINE);
  if (ret)
  {
    lastIndex = tryLast;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleTryBlock(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  bool ret = EXPECT(index, tryLast, "try", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(CompoundStatement, tryLast + 1, tryLast, curBlock, returner, NOT_OPT) &&
    INVOKE(HandlerSeq, tryLast + 1, tryLast, curBlock, returner, NOT_OPT);
  if (ret)
  {
    lastIndex = tryLast;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleHandler(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  bool ret = EXPECT(index, tryLast, "catch", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLast + 1, tryLast, "(", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(ExceptionDeclaration, tryLast + 1, tryLast, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLast + 1, tryLast, ")", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(CompoundStatement, tryLast + 1, tryLast, curBlock, returner, NOT_OPT) ;
  if (ret)
  {
    lastIndex = tryLast;
    return eGrmErrNoError;
  }

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleHandlerSeq(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index - 1;
  bool inLoop = false;
  while(INVOKE(Handler, tryLast + 1, tryLast, curBlock, returner, NOT_OPT))
  {
    inLoop = true;
  };
  if (inLoop)
  {
    lastIndex = tryLast;
    return eGrmErrNoError;
  }

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleFunctionTryBlock(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  bool ret = EXPECT(index, tryLast, "try", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(CtorInitializer, tryLast + 1, tryLast, curBlock, returner, IS_OPT) &&
    INVOKE(CompoundStatement, tryLast + 1, tryLast, curBlock, returner, NOT_OPT) &&
    INVOKE(HandlerSeq, tryLast + 1, tryLast, curBlock, returner, NOT_OPT);
  if (ret)
  {
    lastIndex = tryLast;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleExceptionDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  bool ret = INVOKE(Attributes, index, tryLast, curBlock, returner, IS_OPT) &&
    INVOKE(TypeSpecifierSeq, tryLast + 1, tryLast, curBlock, returner, NOT_OPT);
  if (ret)
  {
    int32 tryLastA = tryLast;
    bool retA = INVOKE(Declarator, tryLastA + 1, tryLastA, curBlock, returner, NOT_OPT) ;
    if (retA)
    {
      lastIndex = tryLastA;
      return eGrmErrNoError;
    }

    int32 tryLastB = tryLast;
    bool retB = INVOKE(AbstractDeclarator, tryLastB + 1, tryLastB, curBlock, returner, NOT_OPT);
    if (retB)
    {
      lastIndex = tryLastB;
      return eGrmErrNoError;
    }
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleTrailingReturnType(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  bool ret = EXPECT(index, tryLast, "->", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(TrailingTypeSpecifierSeq, tryLast + 1, tryLast, curBlock, returner, NOT_OPT) &&
    INVOKE(AbstractDeclarator, tryLast + 1, tryLast, curBlock, returner,  IS_OPT);
  if (ret)
  {
    lastIndex = tryLast;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleTrailingTypeSpecifierSeq(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index - 1;
  bool inLoop = false;
  while (INVOKE(TrailingTypeSpecifier, tryLast + 1, tryLast, curBlock, returner, NOT_OPT))
  {
    inLoop = true;
  }
  if (inLoop)
  {
    lastIndex = tryLast;
    int32 tryLastA = tryLast;
    bool retA = INVOKE(Attributes, tryLastA + 1, tryLastA, curBlock, returner, IS_OPT);
    if (retA)
    {
      lastIndex = tryLastA;
    }
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleExceptionSpecification(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLastA = index;
  bool retA = INVOKE(DynamicExceptionSpecification, index, tryLastA, curBlock, returner, NOT_OPT);
  if (retA)
  {
    lastIndex = tryLastA;
    return eGrmErrNoError;
  }

  int32 tryLastB = index;
  bool retB = INVOKE(NoexceptSpecification, index, tryLastB, curBlock, returner, NOT_OPT);
  if (retB)
  {
    lastIndex = tryLastB;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleDynamicExceptionSpecification(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  bool ret = EXPECT(index, tryLast, "throw", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLast + 1, tryLast, "(", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(TypeIdList, tryLast + 1, tryLast, curBlock, returner, IS_OPT) &&
    EXPECT(tryLast + 1, tryLast, ")", NOT_OPT, NOT_IN_ONE_LINE);
  if (ret)
  {
    lastIndex = tryLast;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleNoexceptSpecification(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  bool ret = EXPECT(tryLast, tryLast, "noexcept", NOT_OPT, NOT_IN_ONE_LINE);
  if (ret)
  {
    lastIndex = tryLast;
    int32 tryLastA = tryLast;
    bool retA = EXPECT(tryLastA + 1, tryLastA, "(", NOT_OPT, NOT_IN_ONE_LINE) &&
      INVOKE(ConstantExpression, tryLastA + 1, tryLastA, curBlock, returner, NOT_OPT) &&
      EXPECT(tryLastA + 1, tryLastA, ")", NOT_OPT, NOT_IN_ONE_LINE);
    if (retA)
    {
      lastIndex = tryLastA;
      return eGrmErrNoError;
    }
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleTypeIdList(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLastA = index;
  bool retA = INVOKE(TypeId, index, tryLastA, curBlock, returner, NOT_OPT);
  if (retA)
  {
    lastIndex = tryLastA;
    bool retA1 = EXPECT(tryLastA + 1, tryLastA, "...", NOT_OPT, NOT_IN_ONE_LINE);
    if (retA1)
    {
      lastIndex = tryLastA;
      return eGrmErrNoError;
    }

    int32 tryLastB = tryLastA - 1;
    bool inLoop = false;
    while (EXPECT(tryLastB + 1, tryLastB, ",", NOT_OPT, NOT_IN_ONE_LINE) && 
        INVOKE(TypeId, tryLastB + 1, tryLastB, curBlock, returner, NOT_OPT))
    {
      inLoop = true;
    }
    if (inLoop)
    {
      EXPECT(tryLastB + 1, tryLastB, "...", IS_OPT, NOT_IN_ONE_LINE);
      lastIndex = tryLastB;
    }
    return eGrmErrNoError;
  }

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleCapture(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLastA = index;
  bool retA = EXPECT(index, tryLastA, "this", NOT_OPT, NOT_IN_ONE_LINE);
  if (retA)
  {
    lastIndex = tryLastA;
    return eGrmErrNoError;
  }

  int32 tryLastB = index;
  bool retB = EXPECT(index, tryLastB, "&", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Identifier, tryLastB + 1, tryLastB, curBlock, returner, NOT_OPT);
  if (retB)
  {
    lastIndex = tryLastB;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleCaptureDefault(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  bool ret = EXPECT(index, tryLast, "&", NOT_OPT, NOT_IN_ONE_LINE) ||
    EXPECT(index, tryLast, "=", NOT_OPT, NOT_IN_ONE_LINE);
  if (ret)
  {
    lastIndex = index + 1;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleCaptureList(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  bool ret = INVOKE(Capture, index, tryLast, curBlock, returner, NOT_OPT);
  if (ret)
  {
    lastIndex = tryLast;
    bool retA = EXPECT(tryLast + 1, tryLast, "...", NOT_OPT, NOT_IN_ONE_LINE);
    if (retA)
    {
      lastIndex = tryLast;
      return eGrmErrNoError;
    }
    
    bool inLoop = false;
    int32 tryLastB = tryLast - 1;
    while(EXPECT(tryLastB + 1, tryLastB, ",", NOT_OPT, NOT_IN_ONE_LINE) &&
        INVOKE(Capture, tryLastB + 1, tryLastB, curBlock, returner, NOT_OPT))
    {
      inLoop = true;
    }
    if (inLoop)
    {
      lastIndex = tryLastB;
      bool retB = EXPECT(tryLastB + 1, tryLastB, "...", NOT_OPT, NOT_IN_ONE_LINE);
      if (retB)
      {
        lastIndex = tryLastB;
        return eGrmErrNoError;
      }
    }
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleLambdaDeclarator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  bool ret = EXPECT(index, tryLast, "(", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(ParameterDeclarationClause, tryLast + 1, tryLast, curBlock, returner, NOT_OPT) &&
    EXPECT(tryLast + 1, tryLast, ")", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLast + 1, tryLast, "mutable", IS_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(ExceptionSpecification , tryLast + 1, tryLast, curBlock, returner, IS_OPT) &&
    INVOKE(Attributes, tryLast + 1, tryLast, curBlock, returner, IS_OPT) &&
    INVOKE(TrailingReturnType, tryLast + 1, tryLast, curBlock, returner, IS_OPT);
  if (ret)
  {
    lastIndex = tryLast;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleLambdaCapture(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLastA = index;
  bool retA = INVOKE(CaptureDefault, index, tryLastA, curBlock, returner, NOT_OPT);
  if (retA)
  {
    lastIndex = tryLastA;
    int32 tryLastA1 = tryLastA;
    bool retA1 = EXPECT(tryLastA1 + 1, tryLastA1, ",", NOT_OPT, NOT_IN_ONE_LINE) &&
      INVOKE(CaptureList, tryLastA1 + 1, tryLastA1, curBlock, returner, NOT_OPT);
    if (retA1)
    {
      lastIndex = tryLastA1;
      return eGrmErrNoError;
    }
    return eGrmErrNoError;
  }

  int32 tryLastB = index;
  bool retB = INVOKE(CaptureList, index, tryLastB, curBlock, returner, NOT_OPT);
  if (retB)
  {
    lastIndex = tryLastB;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleLambdaIntroducer(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  bool ret = EXPECT(index, tryLast, "[", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(LambdaCapture, tryLast + 1, tryLast, curBlock, returner, IS_OPT) &&
    EXPECT(tryLast + 1, tryLast, "]", NOT_OPT, NOT_IN_ONE_LINE);
  if (ret)
  {
    lastIndex = tryLast;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleLambdaExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  bool ret = INVOKE(LambdaIntroducer, index, tryLast, curBlock, returner, NOT_OPT) &&
    INVOKE(LambdaDeclarator, tryLast + 1, tryLast, curBlock, returner, IS_OPT) &&
    INVOKE(CompoundStatement, tryLast + 1, tryLast, curBlock, returner, NOT_OPT);
  if (ret)
  {
    lastIndex = tryLast;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleLiteralOperatorId(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  GrammarReturnerBase *base = new GrammarReturnerBase(eLiteralOperatorId,"");
  bool ret = EXPECT(index, tryLast , "operator", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLast + 1, tryLast , "\"\"", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Identifier, tryLast + 1, tryLast, curBlock, returner, NOT_OPT);
  if (ret)
  {
    lastIndex = tryLast;
    if (returner)
    {
      returner -> addChild(base);
    }
    else
    {
      delete base;
    }
    return eGrmErrNoError;
  }
  delete base;
  return eGrmErrUnknown;
}
