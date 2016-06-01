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
  int32 tryLast = index - 1;
  GrammarReturnerBase * base = new GrammarReturnerBase(eDeclSpecifierSeq, "");
  bool inLoop = false;
  while(INVOKE(DeclSpecifier, tryLast + 1, tryLast, curBlock, base, NOT_OPT))
  {
    inLoop = true;
  }
  if (inLoop)
  {
    lastIndex = tryLast;

    int32 tryLastA = tryLast;
    bool retA = INVOKE(Attributes, tryLastA + 1, tryLastA, curBlock, base, NOT_OPT);
    if (retA)
    {
      lastIndex = tryLastA;
    }
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

uint32 GrammarAnalyzer::handleDeclSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast005 = index;
  bool ret005 = EXPECT(index, tryLast005, "typedef", NOT_OPT, NOT_IN_ONE_LINE );
  if (ret005)
  {
    if(returner)
    {
      GrammarReturnerBase *base005 = new GrammarReturnerBase(eDeclSpecifier, "typedef");
      returner -> addChild(base005);
    }
    lastIndex = tryLast005;
    return eGrmErrNoError;
  }

  int32 tryLast006 = index;
  bool ret006 = EXPECT(index, tryLast006, "constexpr", NOT_OPT, NOT_IN_ONE_LINE );
  if (ret006)
  {
    if(returner)
    {
      GrammarReturnerBase *base006 = new GrammarReturnerBase(eDeclSpecifier, "constexpr");
      returner -> addChild(base006);
    }
    lastIndex = tryLast006;
    return eGrmErrNoError;
  }

  int32 tryLast004 = index;
  bool ret004 = EXPECT(index, tryLast004, "friend", NOT_OPT, NOT_IN_ONE_LINE );
  if (ret004)
  {
    if(returner)
    {
      GrammarReturnerBase *base004 = new GrammarReturnerBase(eDeclSpecifier, "friend");
      returner -> addChild(base004);
    }
    lastIndex = tryLast004;
    return eGrmErrNoError;
  }

  int32 tryLast001 = index;
  GrammarReturnerBase *base001 = new GrammarReturnerBase(eDeclSpecifier, "");
  bool ret001 = INVOKE(StorageClassSpecifier, index, tryLast001, curBlock, base001, NOT_OPT);
  if (ret001)
  {
    if(returner)
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

  int32 tryLast002 = index;
  GrammarReturnerBase *base002 = new GrammarReturnerBase(eDeclSpecifier, "");
  bool ret002 = INVOKE(FunctionSpecifier, index, tryLast002, curBlock, base002, NOT_OPT);
  if (ret002)
  {
    if(returner)
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

  int32 tryLast003 = index;
  GrammarReturnerBase *base003 = new GrammarReturnerBase(eDeclSpecifier, "");
  bool ret003 = INVOKE(TypeSpecifier, index, tryLast003, curBlock, base003, NOT_OPT);
  if (ret003)
  {
    if(returner)
    {
      returner -> addChild(base003);
    }
    else
    {
      delete base003;
    }
    lastIndex = tryLast003;
    return eGrmErrNoError;
  }
  delete base003;

  JZFUNC_END_LOG();
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleTypeSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast001 = index;
  GrammarReturnerBase * base001 = new GrammarReturnerBase(eTypeSpecifier, "");
  if (INVOKE(EnumSpecifier, index, tryLast001, curBlock, base001, NOT_OPT))
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

  int32 tryLast002 = index;
  GrammarReturnerBase * base002 = new GrammarReturnerBase(eTypeSpecifier, "");
  if (INVOKE(ClassSpecifier, index, tryLast002, curBlock, base002, NOT_OPT))
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

  int32 tryLast003 = index;
  GrammarReturnerBase * base003 = new GrammarReturnerBase(eTypeSpecifier, "");
  if (INVOKE(TrailingTypeSpecifier, index, tryLast003, curBlock, base003, NOT_OPT))
  {
    if (returner)
    {
      returner -> addChild(base003);
    }
    else
    {
      delete base003;
    }
    lastIndex = tryLast003;
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
  GrammarReturnerBase * baseB = new GrammarReturnerBase(eNonPtrDeclarator, "");
  while(INVOKE(NonPtrDeclarator, tryB + 1, tryB, curBlock, baseB, NOT_OPT))
  {
    JZWRITE_DEBUG("cur index: %d", tryB);
    loopIn = true;
  }
  if (loopIn)
  {

    int32 tryB1 = tryB;
    GrammarReturnerBase * baseB1 = new GrammarReturnerBase(eNonPtrDeclarator, "");
    bool retB1 = INVOKE(ParametersAndQualifiers, tryB1 + 1, tryB1, curBlock, baseB1, NOT_OPT);
    if (retB1)
    {
      lastIndex = tryB1;
      if (returner)
      {
        baseB -> mergeChild(baseB1);
        returner -> addChild(baseB1);
      }
      else
      {
        delete baseB1;
        delete baseB;
      }
      JZFUNC_END_LOG();
      return eGrmErrNoError;
    }
    delete baseB1;

    int32 tryB2 = tryB;

    GrammarReturnerBase * baseB2 = new GrammarReturnerBase(eNonPtrDeclarator, "");
    bool retB2 = EXPECT(tryB2 + 1, tryB2, "[", NOT_OPT, NOT_IN_ONE_LINE) &&
      INVOKE(ConstantExpression, tryB2 + 1, tryB2, curBlock, baseB2, IS_OPT) &&
      EXPECT(tryB2 + 1, tryB2, "]", NOT_OPT, NOT_IN_ONE_LINE) &&
      INVOKE(Attributes, tryB2 + 1, tryB2, curBlock, baseB2, IS_OPT);
    if (retB2)
    {
      lastIndex = tryB2;
      if (returner)
      {
        baseB -> mergeChild(baseB2);
        returner -> addChild(baseB);
      }
      else
      {
        delete baseB;
        delete baseB2;
      }
      JZFUNC_END_LOG();
      return eGrmErrNoError;
    }
    delete baseB2;

    if (returner)
    {
      returner -> addChild(baseB);
    }
    else
    {
      delete baseB;
    }

    lastIndex = tryB;
    return eGrmErrNoError;
  }
  delete baseB;

  int32 tryA = index;
  GrammarReturnerBase * baseA = new GrammarReturnerBase(eNonPtrDeclarator, "");
  //if we match declarator here, we don't go futher to parameters
  bool retA = INVOKE(DeclaratorId, index, tryA, curBlock, baseA, NOT_OPT) &&
    INVOKE(Attributes, tryA + 1, tryA, curBlock, baseA, IS_OPT);
  if (retA)
  {
    lastIndex = tryA;
    if (returner)
    {
      returner -> addChild(baseA);
    }
    else
    {
      delete baseA;
    }
    JZFUNC_END_LOG();
    return eGrmErrNoError;
  }
  delete baseA;

  int32 tryD = index;
  GrammarReturnerBase * baseD = new GrammarReturnerBase(eNonPtrDeclarator, "");
  bool retD = EXPECT(index, tryD, "(", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(PtrDeclarator, tryD + 1, tryD, curBlock, baseD, NOT_OPT) &&
    EXPECT(tryD + 1, tryD, ")", NOT_OPT, NOT_IN_ONE_LINE);
  if (retD)
  {
    lastIndex = tryD;
    if (returner)
    {
      returner -> addChild(baseD);
    }
    else
    {
      delete baseD;
    }
    JZFUNC_END_LOG();
    return eGrmErrNoError;
  }
  delete baseD;

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
    INVOKE(NestNameSpecifier, tryLast002 + 1, tryLast002, curBlock, base002, IS_OPT) &&
    INVOKE(ClassName, tryLast002 + 1, tryLast002, curBlock, base002, NOT_OPT) ;
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
    if (returner)
    {
      GrammarReturnerBase* base = new GrammarReturnerBase(eIdentifier, id);
      returner -> addChild(base);
    }
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
  GrammarReturnerBase *base01 = new GrammarReturnerBase(eNamespaceName, "");
  int32 tryLast01 = index;
  if (INVOKE(OriginalNamespaceName, index, tryLast01, curBlock, base01, NOT_OPT))
  {
    if (returner)
    {
      returner -> addChild(base01);
    }
    else
    {
      delete base01;
    }
    lastIndex = tryLast01;
    return eGrmErrNoError;
  }
  delete base01;

  GrammarReturnerBase *base02 = new GrammarReturnerBase(eNamespaceName, "");
  int32 tryLast02 = index;
  if (INVOKE(NamespaceAlias, index, tryLast02, curBlock, base02, NOT_OPT))
  {
    if (returner)
    {
      returner -> addChild(base02);
    }
    else
    {
      delete base02;
    }
    lastIndex = tryLast02;
    return eGrmErrNoError;
  }
  delete base02;

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleNamespaceAlias(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  GrammarReturnerBase *base = new GrammarReturnerBase(eNamespaceAlias, "");
  int32 tryLast = index;
  if (INVOKE(Identifier, index, tryLast, curBlock, base, NOT_OPT))
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

uint32 GrammarAnalyzer::handleTypedefName(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  GrammarReturnerBase *base = new GrammarReturnerBase(eTypedefName, "");
  int32 tryLast = index;
  if (INVOKE(Identifier, index, tryLast, curBlock, base, NOT_OPT))
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

uint32 GrammarAnalyzer::handleEnumName(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  GrammarReturnerBase *base = new GrammarReturnerBase(eEnumName, "");
  int32 tryLast = index;
  if (INVOKE(Identifier, index, tryLast, curBlock, base, NOT_OPT))
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
  GrammarReturnerBase * base001 = new GrammarReturnerBase(eUnqualifiedId, "");
  int32 tryLast001 = index;
  bool ret001 = INVOKE(OperatorFunctionId, index, tryLast001, curBlock, base001, NOT_OPT);
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

  GrammarReturnerBase * base002 = new GrammarReturnerBase(eUnqualifiedId, "");
  int32 tryLast002 = index;
  bool ret002 = INVOKE(ConversionFunctionId, index, tryLast002, curBlock, base002, NOT_OPT);
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

  GrammarReturnerBase * base003 = new GrammarReturnerBase(eUnqualifiedId, "");
  int32 tryLast003 = index;
  bool ret003 = INVOKE(LiteralOperatorId, index, tryLast003, curBlock, base003, NOT_OPT);
  if (ret003)
  {
    if (returner)
    {
      returner -> addChild(base003);
    }
    else
    {
      delete base003;
    }
    lastIndex = tryLast003;
    return eGrmErrNoError;
  }
  delete base003;

  GrammarReturnerBase * base004 = new GrammarReturnerBase(eUnqualifiedId, "");
  int32 tryLast004 = index;
  bool ret004 = INVOKE(TemplateId, index, tryLast004, curBlock, base004, NOT_OPT);
  if (ret004)
  {
    if (returner)
    {
      returner -> addChild(base004);
    }
    else
    {
      delete base004;
    }
    lastIndex = tryLast004;
    return eGrmErrNoError;
  }
  delete base004;

  GrammarReturnerBase * base005 = new GrammarReturnerBase(eUnqualifiedId, "");
  int32 tryLast005 = index;
  bool ret005 = EXPECT(index, tryLast005, "~", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(ClassName, tryLast005 + 1, tryLast005, curBlock, base005, NOT_OPT);
  if (ret005)
  {
    if (returner)
    {
      returner -> addChild(base005);
    }
    else
    {
      delete base005;
    }
    lastIndex = tryLast005;
    return eGrmErrNoError;
  }
  delete base005;

  GrammarReturnerBase * base006 = new GrammarReturnerBase(eUnqualifiedId, "");
  int32 tryLast006 = index;
  bool ret006 = EXPECT(index, tryLast006, "~", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(DecltypeSpecifier, tryLast006 + 1, tryLast006, curBlock, base006, NOT_OPT);
  if (ret006)
  {
    if (returner)
    {
      returner -> addChild(base006);
    }
    else
    {
      delete base006;
    }
    lastIndex = tryLast006;
    return eGrmErrNoError;
  }
  delete base006;

  GrammarReturnerBase * base007 = new GrammarReturnerBase(eUnqualifiedId, "");
  int32 tryLast007 = index;
  bool ret007 = INVOKE(Identifier, tryLast007, tryLast007, curBlock, base007, NOT_OPT);
  if (ret007)
  {
    if (returner)
    {
      returner -> addChild(base007);
    }
    else
    {
      delete base007;
    }
    lastIndex = tryLast007;
    return eGrmErrNoError;
  }
  delete base007;

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleQualifiedId(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  GrammarReturnerBase * base003 = new GrammarReturnerBase(eQualifiedId, "");
  int32 tryLast003 = index;
  bool ret003 = EXPECT(index, tryLast003, "::", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(OperatorFunctionId, tryLast003 + 1, tryLast003, curBlock, base003, NOT_OPT);
  if (ret003)
  {
    if (returner)
    {
      returner -> addChild(base003);
    }
    else
    {
      delete base003;
    }
    lastIndex = tryLast003;
    return eGrmErrNoError;
  }
  delete base003;

  GrammarReturnerBase * base004 = new GrammarReturnerBase(eQualifiedId, "");
  int32 tryLast004 = index;
  bool ret004 = EXPECT(index, tryLast004, "::", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(LiteralOperatorId, tryLast004 + 1, tryLast004, curBlock, base004, NOT_OPT);
  if (ret004)
  {
    if (returner)
    {
      returner -> addChild(base004);
    }
    else
    {
      delete base004;
    }
    lastIndex = tryLast004;
    return eGrmErrNoError;
  }
  delete base004;

  GrammarReturnerBase * base005 = new GrammarReturnerBase(eQualifiedId, "");
  int32 tryLast005 = index;
  bool ret005 = EXPECT(index, tryLast005, "::", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(TemplateId, tryLast005 + 1, tryLast005, curBlock, base005, NOT_OPT);
  if (ret005)
  {
    if (returner)
    {
      returner -> addChild(base005);
    }
    else
    {
      delete base005;
    }
    lastIndex = tryLast005;
    return eGrmErrNoError;
  }
  delete base005;

  GrammarReturnerBase * base002 = new GrammarReturnerBase(eQualifiedId, "");
  int32 tryLast002 = index;
  bool ret002 = EXPECT(index, tryLast002, "::", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Identifier, tryLast002 + 1, tryLast002, curBlock, base002, NOT_OPT);
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

  GrammarReturnerBase * base001 = new GrammarReturnerBase(eQualifiedId, "");
  int32 tryLast001 = index;
  bool ret001 = EXPECT(index, tryLast001, "::", IS_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(NestNameSpecifier, tryLast001 + 1, tryLast001, curBlock, base001, NOT_OPT);
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
  
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleDecltypeSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  GrammarReturnerBase * base = new GrammarReturnerBase(eDecltypeSpecifier, "");
  int32 tryLast = index;
  bool ret = EXPECT(index, tryLast, "decltype", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLast + 1, tryLast, "(", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Expression, tryLast + 1, tryLast, curBlock, base, NOT_OPT) &&
    EXPECT(tryLast + 1, tryLast, ")", NOT_OPT, NOT_IN_ONE_LINE);
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

uint32 GrammarAnalyzer::handleExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  GrammarReturnerBase *base = new GrammarReturnerBase(eExpression, "");
  bool ret = INVOKE(AssignmentExpression, index, tryLast, curBlock, base, NOT_OPT);
  if (ret)
  {
    int32 tryLastA = tryLast;
    while (EXPECT(tryLastA + 1, tryLastA, ",", NOT_OPT, NOT_IN_ONE_LINE) &&
        INVOKE(AssignmentExpression, tryLastA + 1, tryLastA, curBlock, base, NOT_OPT))  
    {
      tryLast = tryLastA;
    }

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

uint32 GrammarAnalyzer::handleAssignmentExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast001 = index;
  GrammarReturnerBase *base001 = new GrammarReturnerBase(eAssignmentExpression, "");
  if (INVOKE(ThrowExpression, index, tryLast001, curBlock, base001, NOT_OPT))
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

  int32 tryLast002 = index;
  GrammarReturnerBase *base002 = new GrammarReturnerBase(eAssignmentExpression, "");
  bool ret002 = INVOKE(LogicalOrExpression, index, tryLast002, curBlock, base002, NOT_OPT) &&
    INVOKE(AssignmentOperator, tryLast002 + 1, tryLast002, curBlock, base002, NOT_OPT) &&
    INVOKE(InitializerClause, tryLast002 + 1, tryLast002, curBlock, base002, NOT_OPT);
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
  
  int32 tryLast003 = index;
  GrammarReturnerBase *base003 = new GrammarReturnerBase(eAssignmentExpression, "");
  if (INVOKE(ConditionalExpression, index, tryLast003, curBlock, base003, NOT_OPT))
  {
    if (returner)
    {
      returner -> addChild(base003);
    }
    else
    {
      delete base003;
    }
    lastIndex = tryLast003;
    return eGrmErrNoError;
  }
  delete base003;

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleConditionalExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast001 = index;
  GrammarReturnerBase * base001 = new GrammarReturnerBase(eConditionalExpression, "");
  bool ret001 = INVOKE(LogicalOrExpression, index, tryLast001, curBlock, base001, NOT_OPT);
  if (ret001)
  {
    int32 tryLast002 = tryLast001;
    GrammarReturnerBase * base002 = new GrammarReturnerBase(eConditionalExpression, "");
    bool ret002 = EXPECT(tryLast002 + 1, tryLast002, "?", NOT_OPT, NOT_IN_ONE_LINE) &&
      INVOKE(Expression, tryLast002 + 1, tryLast002, curBlock, base002, NOT_OPT)&&
      EXPECT(tryLast002 + 1, tryLast002, ":", NOT_OPT, NOT_IN_ONE_LINE) &&
      INVOKE(AssignmentExpression, tryLast002 + 1, tryLast002, curBlock, base002, NOT_OPT);
    if (ret002)
    {
      if (returner)
      {
        base001 -> mergeChild(base002);
        returner -> addChild( base001 );
        
      }
      else
      {
        delete base002;
        delete base001;
      }
      lastIndex = tryLast002;
      return eGrmErrNoError;
    }
    delete base002;
    if (returner)
    {
      returner -> addChild( base001 );
    }
    else
    {
      delete base001;
    }
    lastIndex = tryLast001;
    return eGrmErrNoError;
  }
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleLogicalOrExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  GrammarReturnerBase *base = new GrammarReturnerBase(eLogicalOrExpression, "");
  bool ret = INVOKE(LogicalAndExpression, index, tryLast, curBlock, base, NOT_OPT);
  if (ret)
  {
    int32 tryLastA = tryLast;
    while (EXPECT(tryLastA + 1, tryLastA, "||", NOT_OPT, NOT_IN_ONE_LINE) &&
        INVOKE(LogicalAndExpression, tryLastA + 1, tryLastA, curBlock, base, NOT_OPT))  
    {
      tryLast = tryLastA;
    }

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

uint32 GrammarAnalyzer::handleLogicalAndExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  GrammarReturnerBase *base = new GrammarReturnerBase(eLogicalAndExpression, "");
  bool ret = INVOKE(InclusiveOrExpression, index, tryLast, curBlock, base, NOT_OPT);
  if (ret)
  {
    int32 tryLastA = tryLast;
    while (EXPECT(tryLastA + 1, tryLastA, "&&", NOT_OPT, NOT_IN_ONE_LINE) &&
        INVOKE(InclusiveOrExpression, tryLastA + 1, tryLastA, curBlock, base, NOT_OPT))  
    {
      tryLast = tryLastA;
    }

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

uint32 GrammarAnalyzer::handleInclusiveOrExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  GrammarReturnerBase *base = new GrammarReturnerBase(eInclusiveOrExpression, "");
  bool ret = INVOKE(ExclusiveOrExpression, index, tryLast, curBlock, base, NOT_OPT);
  if (ret)
  {
    int32 tryLastA = tryLast;
    while (EXPECT(tryLastA + 1, tryLastA, "|", NOT_OPT, NOT_IN_ONE_LINE) &&
        INVOKE(ExclusiveOrExpression, tryLastA + 1, tryLastA, curBlock, base, NOT_OPT))  
    {
      tryLast = tryLastA;
    }

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

uint32 GrammarAnalyzer::handleExclusiveOrExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  GrammarReturnerBase *base = new GrammarReturnerBase(eExclusiveOrExpression, "");
  bool ret = INVOKE(AndExpression, index, tryLast, curBlock, base, NOT_OPT);
  if (ret)
  {
    int32 tryLastA = tryLast;
    while (EXPECT(tryLastA + 1, tryLastA, "^", NOT_OPT, NOT_IN_ONE_LINE) &&
        INVOKE(AndExpression, tryLastA + 1, tryLastA, curBlock, base, NOT_OPT))  
    {
      tryLast = tryLastA;
    }

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

uint32 GrammarAnalyzer::handleAndExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  GrammarReturnerBase *base = new GrammarReturnerBase(eAndExpression, "");
  bool ret = INVOKE(EqualityExpression, index, tryLast, curBlock, base, NOT_OPT);
  if (ret)
  {
    int32 tryLastA = tryLast;
    while (EXPECT(tryLastA + 1, tryLastA, "&", NOT_OPT, NOT_IN_ONE_LINE) &&
        INVOKE(EqualityExpression, tryLastA + 1, tryLastA, curBlock, base, NOT_OPT))  
    {
      tryLast = tryLastA;
    }

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

uint32 GrammarAnalyzer::handleEqualityExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  GrammarReturnerBase *base = new GrammarReturnerBase(eEqualityExpression, "");
  bool ret = INVOKE(RelationalExpression, index, tryLast, curBlock, base, NOT_OPT);
  if (ret)
  {
    int32 tryLastA = tryLast;
    int32 tryLastB = tryLast;
    while (
        (EXPECT(tryLastA + 1, tryLastA, "==", NOT_OPT, NOT_IN_ONE_LINE) &&
        INVOKE(RelationalExpression, tryLastA + 1, tryLastA, curBlock, base, NOT_OPT)) ||
        (EXPECT(tryLastB + 1, tryLastB, "!=", NOT_OPT, NOT_IN_ONE_LINE) &&
        INVOKE(RelationalExpression, tryLastB + 1, tryLastB, curBlock, base, NOT_OPT))
        )
    {
      if (tryLastA > tryLastB)
      {
        tryLast = tryLastA;
      }
      else
      {
        tryLast = tryLastB;
      }
      tryLastA = tryLast;
      tryLastB = tryLast;
    }

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

uint32 GrammarAnalyzer::handleRelationalExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  GrammarReturnerBase *base = new GrammarReturnerBase(eRelationalExpression, "");
  bool ret = INVOKE(ShiftExpression, index, tryLast, curBlock, base, NOT_OPT);
  if (ret)
  {
    int32 tryLastA = tryLast;
    int32 tryLastB = tryLast;
    int32 tryLastC = tryLast;
    int32 tryLastD = tryLast;
    while (
        (EXPECT(tryLastA + 1, tryLastA, "<", NOT_OPT, NOT_IN_ONE_LINE) &&
        INVOKE(ShiftExpression, tryLastA + 1, tryLastA, curBlock, base, NOT_OPT)) ||
        (EXPECT(tryLastB + 1, tryLastB, ">", NOT_OPT, NOT_IN_ONE_LINE) &&
        INVOKE(ShiftExpression, tryLastB + 1, tryLastB, curBlock, base, NOT_OPT)) ||
        (EXPECT(tryLastC + 1, tryLastC, "<=", NOT_OPT, NOT_IN_ONE_LINE) &&
        INVOKE(ShiftExpression, tryLastC + 1, tryLastC, curBlock, base, NOT_OPT)) ||
        (EXPECT(tryLastD + 1, tryLastD, ">=", NOT_OPT, NOT_IN_ONE_LINE) &&
        INVOKE(ShiftExpression, tryLastD + 1, tryLastD, curBlock, base, NOT_OPT))
        )
    {
      int32 max = tryLastA > tryLastB ? tryLastA : tryLastB;
      max = tryLastC > max ? tryLastC : max;
      max = tryLastD > max ? tryLastD : max;

      tryLast = max;

      tryLastA = tryLast;
      tryLastB = tryLast;
      tryLastC = tryLast;
      tryLastD = tryLast;
    }

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

uint32 GrammarAnalyzer::handleShiftExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  GrammarReturnerBase *base = new GrammarReturnerBase(eShiftExpression, "");
  bool ret = INVOKE(AdditiveExpression, index, tryLast, curBlock, base, NOT_OPT);
  if (ret)
  {
    int32 tryLastA = tryLast;
    int32 tryLastB = tryLast;
    while (
        (EXPECT(tryLastA + 1, tryLastA, "<<", NOT_OPT, NOT_IN_ONE_LINE) &&
        INVOKE(AdditiveExpression, tryLastA + 1, tryLastA, curBlock, base, NOT_OPT)) ||
        (EXPECT(tryLastB + 1, tryLastB, ">>", NOT_OPT, NOT_IN_ONE_LINE) &&
        INVOKE(AdditiveExpression, tryLastB + 1, tryLastB, curBlock, base, NOT_OPT))
        )
    {
      if (tryLastA > tryLastB)
      {
        tryLast = tryLastA;
      }
      else
      {
        tryLast = tryLastB;
      }
      tryLastA = tryLast;
      tryLastB = tryLast;
    }

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

uint32 GrammarAnalyzer::handleAdditiveExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  GrammarReturnerBase *base = new GrammarReturnerBase(eAdditiveExpression, "");
  bool ret = INVOKE(MultiplicativeExpression, index, tryLast, curBlock, base, NOT_OPT);
  if (ret)
  {
    int32 tryLastA = tryLast;
    int32 tryLastB = tryLast;
    while (
        (EXPECT(tryLastA + 1, tryLastA, "+", NOT_OPT, NOT_IN_ONE_LINE) &&
        INVOKE(MultiplicativeExpression, tryLastA + 1, tryLastA, curBlock, base, NOT_OPT)) ||
        (EXPECT(tryLastB + 1, tryLastB, "-", NOT_OPT, NOT_IN_ONE_LINE) &&
        INVOKE(MultiplicativeExpression, tryLastB + 1, tryLastB, curBlock, base, NOT_OPT))
        )
    {
      if (tryLastA > tryLastB)
      {
        tryLast = tryLastA;
      }
      else
      {
        tryLast = tryLastB;
      }
      tryLastA = tryLast;
      tryLastB = tryLast;
    }

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

uint32 GrammarAnalyzer::handleMultiplicativeExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  GrammarReturnerBase *base = new GrammarReturnerBase(eMultiplicativeExpression, "");
  bool ret = INVOKE(PmExpression, index, tryLast, curBlock, base, NOT_OPT);
  if (ret)
  {
    int32 tryLastA = tryLast;
    int32 tryLastB = tryLast;
    int32 tryLastC = tryLast;
    while (
        (EXPECT(tryLastA + 1, tryLastA, "*", NOT_OPT, NOT_IN_ONE_LINE) &&
        INVOKE(PmExpression, tryLastA + 1, tryLastA, curBlock, base, NOT_OPT)) ||
        (EXPECT(tryLastB + 1, tryLastB, "/", NOT_OPT, NOT_IN_ONE_LINE) &&
        INVOKE(PmExpression, tryLastB + 1, tryLastB, curBlock, base, NOT_OPT)) ||
        (EXPECT(tryLastC + 1, tryLastC, "%", NOT_OPT, NOT_IN_ONE_LINE) &&
        INVOKE(PmExpression, tryLastC + 1, tryLastC, curBlock, base, NOT_OPT))
        )
    {
      int32 max = tryLastA > tryLastB ? tryLastA : tryLastB;
      max = tryLastC > max ? tryLastC : max;
      tryLast = max;
      tryLastA = tryLast;
      tryLastB = tryLast;
      tryLastC = tryLast;
    }

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

uint32 GrammarAnalyzer::handlePmExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  GrammarReturnerBase *base = new GrammarReturnerBase(ePmExpression, "");
  bool ret = INVOKE(CastExpression, index, tryLast, curBlock, base, NOT_OPT);
  if (ret)
  {
    int32 tryLastA = tryLast;
    int32 tryLastB = tryLast;
    while (
        (EXPECT(tryLastA + 1, tryLastA, ".*", NOT_OPT, NOT_IN_ONE_LINE) &&
        INVOKE(CastExpression, tryLastA + 1, tryLastA, curBlock, base, NOT_OPT)) ||
        (EXPECT(tryLastB + 1, tryLastB, "->*", NOT_OPT, NOT_IN_ONE_LINE) &&
        INVOKE(CastExpression, tryLastB + 1, tryLastB, curBlock, base, NOT_OPT))
        )
    {
      int32 max = tryLastA > tryLastB ? tryLastA : tryLastB;
      tryLast = max;
      tryLastA = tryLast;
      tryLastB = tryLast;
    }

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

uint32 GrammarAnalyzer::handleCastExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast001 = index;
  GrammarReturnerBase * base001 = new GrammarReturnerBase(eCastExpression, "");
  bool ret001 = INVOKE(UnaryExpression, index, tryLast001, curBlock, base001, NOT_OPT) ;
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

  int32 tryLast002 = index;
  GrammarReturnerBase * base002 = new GrammarReturnerBase(eCastExpression, "");
  bool ret002 = EXPECT(index, tryLast002, "(", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(TypeId, tryLast002 + 1, tryLast002, curBlock, base002, NOT_OPT)&&
    EXPECT(tryLast002 + 1, tryLast002, ")", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(CastExpression, tryLast002 + 1, tryLast002, curBlock, base002, NOT_OPT) ;
  if (ret002)
  {
    lastIndex = tryLast002;
    if (returner)
    {
      returner -> addChild(base002);
    }
    else
    {
      delete base002;
    }
    return eGrmErrNoError;
  }
  delete base002;

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
  int32 tryLast003 = index;
  bool ret003 = EXPECT(index, tryLast003, "...", NOT_OPT,NOT_IN_ONE_LINE);
  if (ret003)
  {
    if (returner)
    {
      GrammarReturnerBase * base003 = new GrammarReturnerBase(eAbstractDeclarator, "...");
      returner -> addChild( base003 );
    }
    lastIndex = tryLast003;
    return eGrmErrNoError;
  }

  int32 tryLast001 = index;
  GrammarReturnerBase * base001 = new GrammarReturnerBase(eAbstractDeclarator, "");
  bool ret001 = INVOKE(PtrAbstractDeclarator, index, tryLast001, curBlock, base001, NOT_OPT);
  if (ret001)
  {
    if (returner)
    {
      returner -> addChild( base001 );
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
  GrammarReturnerBase * base002 = new GrammarReturnerBase(eAbstractDeclarator, "");
  bool ret002 = INVOKE(NoptrAbstractDeclarator, index, tryLast002, curBlock, base002, IS_OPT) &&
    INVOKE(ParametersAndQualifiers, tryLast002 + 1, tryLast002, curBlock, base002, NOT_OPT) &&
    INVOKE(TrailingReturnType, tryLast002 + 1, tryLast002, curBlock, base002, NOT_OPT);
  if (ret002)
  {
    if (returner)
    {
      returner -> addChild( base002 );
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

uint32 GrammarAnalyzer::handlePtrAbstractDeclarator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast001 = index;
  GrammarReturnerBase * base001 = new GrammarReturnerBase(ePtrAbstractDeclarator, "");
  bool ret001 = INVOKE(NoptrAbstractDeclarator, index, tryLast001, curBlock, base001, NOT_OPT);
  if (ret001)
  {
    if (returner)
    {
      returner -> addChild( base001 );
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
  GrammarReturnerBase * base002 = new GrammarReturnerBase(ePtrAbstractDeclarator, "");
  bool ret002 = INVOKE(PtrOperator, index, tryLast002, curBlock, base002, NOT_OPT) &&
    INVOKE(PtrAbstractDeclarator, tryLast002 + 1, tryLast002, curBlock, base002, IS_OPT);
  if (ret002)
  {
    if (returner)
    {
      returner -> addChild( base002 );
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

uint32 GrammarAnalyzer::handleNoptrAbstractDeclarator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast001 = index - 1;
  bool inLoop = false;
  GrammarReturnerBase * base001 = new GrammarReturnerBase(eNoptrAbstractDeclarator, "");
  while(INVOKE(NoptrAbstractDeclarator, tryLast001 + 1, tryLast001, curBlock, base001, NOT_OPT  ))
  {
    inLoop = true;
  }

  int32 tryLast001A = tryLast001;
  GrammarReturnerBase * base001A = new GrammarReturnerBase(eNoptrAbstractDeclarator, "");
  bool ret001A = INVOKE(ParametersAndQualifiers, tryLast001A + 1, tryLast001A, curBlock, base001A, NOT_OPT);
  if (ret001A)
  {
    if (returner)
    {
      base001 -> mergeChild(base001A);
      delete base001A;
      returner -> addChild(base001);
    }
    else
    {
      delete base001A;
      delete base001;
    }
    lastIndex = tryLast001A;
    return eGrmErrNoError;
  }

  int32 tryLast001B = tryLast001;
  GrammarReturnerBase * base001B = new GrammarReturnerBase(eNoptrAbstractDeclarator, "");
  bool ret001B = EXPECT(tryLast001B + 1, tryLast001B, "[", NOT_OPT, NOT_OPT) &&
    INVOKE(ParametersAndQualifiers, tryLast001B + 1, tryLast001B, curBlock, base001B, NOT_OPT) &&
    EXPECT(tryLast001B + 1, tryLast001B, "]", NOT_OPT, NOT_OPT) &&
    INVOKE(Attributes, tryLast001B + 1, tryLast001B, curBlock, base001B, IS_OPT);
  if (ret001B)
  {
    if (returner)
    {
      base001 -> mergeChild(base001B);
      delete base001B;
      returner -> addChild(base001);
    }
    else
    {
      delete base001B;
      delete base001;
    }
    lastIndex = tryLast001B;
    return eGrmErrNoError;
  }

  if (false == inLoop)
  {
    int32 tryLast002 = index;
    GrammarReturnerBase * base002 = new GrammarReturnerBase(eNoptrAbstractDeclarator, "");
    bool ret002 = EXPECT(tryLast002 , tryLast002, "(", NOT_OPT, NOT_OPT) &&
      INVOKE(PtrAbstractDeclarator, tryLast002 + 1, tryLast002, curBlock, base002, NOT_OPT) &&
      EXPECT(tryLast002 + 1, tryLast002, ")", NOT_OPT, NOT_OPT);
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
  }

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleConstantExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  GrammarReturnerBase * base = new GrammarReturnerBase(eConstantExpression, "");
  bool ret = INVOKE(ConditionalExpression, index,  tryLast, curBlock, base, NOT_OPT);
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

uint32 GrammarAnalyzer::handleUnaryExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast001 = index;
  GrammarReturnerBase * base001 = new GrammarReturnerBase(eUnaryExpression, "");
  bool ret001 = EXPECT(index, tryLast001, "sizeof", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLast001 + 1, tryLast001, "...", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLast001 + 1, tryLast001, "(", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Identifier, tryLast001 + 1, tryLast001, curBlock, base001, NOT_OPT) &&
    EXPECT(tryLast001 + 1, tryLast001, ")", NOT_OPT, NOT_IN_ONE_LINE);
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

  int32 tryLast002 = index;
  GrammarReturnerBase * base002 = new GrammarReturnerBase(eUnaryExpression, "");
  bool ret002 = EXPECT(index, tryLast002, "sizeof", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(UnaryExpression, tryLast002 + 1, tryLast002, curBlock, base002, NOT_OPT);
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

  int32 tryLast003 = index;
  GrammarReturnerBase * base003 = new GrammarReturnerBase(eUnaryExpression, "");
  bool ret003 = EXPECT(index, tryLast003, "sizeof", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLast003 + 1, tryLast003, "(", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(TypeId, tryLast003 + 1, tryLast003, curBlock, base003, NOT_OPT) &&
    EXPECT(tryLast003 + 1, tryLast003, ")", NOT_OPT, NOT_IN_ONE_LINE);
  if (ret003)
  {
    if (returner)
    {
      returner -> addChild(base003);
    }
    else
    {
      delete base003;
    }
    lastIndex = tryLast003;
    return eGrmErrNoError;
  }
  delete base003;

  int32 tryLast004 = index;
  GrammarReturnerBase * base004 = new GrammarReturnerBase(eUnaryExpression, "");
  bool ret004 = EXPECT(index, tryLast004, "alignof", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLast004 + 1, tryLast004, "(", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(TypeId, tryLast004 + 1, tryLast004, curBlock, base004, NOT_OPT) &&
    EXPECT(tryLast004 + 1, tryLast004, ")", NOT_OPT, NOT_IN_ONE_LINE);
  if (ret004)
  {
    if (returner)
    {
      returner -> addChild(base004);
    }
    else
    {
      delete base004;
    }
    lastIndex = tryLast004;
    return eGrmErrNoError;
  }
  delete base004;

  int32 tryLast005 = index;
  GrammarReturnerBase * base005 = new GrammarReturnerBase(eUnaryExpression, "");
  bool ret005 = EXPECT(index, tryLast005, "++", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(CastExpression, tryLast005 + 1, tryLast005, curBlock, base005, NOT_OPT);
  if (ret005)
  {
    if (returner)
    {
      returner -> addChild(base005);
    }
    else
    {
      delete base005;
    }
    lastIndex = tryLast005;
    return eGrmErrNoError;
  }
  delete base005;

  int32 tryLast006 = index;
  GrammarReturnerBase * base006 = new GrammarReturnerBase(eUnaryExpression, "");
  bool ret006 = EXPECT(index, tryLast006, "--", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(CastExpression, tryLast006 + 1, tryLast006, curBlock, base006, NOT_OPT);
  if (ret006)
  {
    if (returner)
    {
      returner -> addChild(base006);
    }
    else
    {
      delete base006;
    }
    lastIndex = tryLast006;
    return eGrmErrNoError;
  }
  delete base006;

  int32 tryLast007 = index;
  GrammarReturnerBase * base007 = new GrammarReturnerBase(eUnaryExpression, "");
  bool ret007 = INVOKE(NewExpression, tryLast007 , tryLast007, curBlock, base007, NOT_OPT);
  if (ret007)
  {
    if (returner)
    {
      returner -> addChild(base007);
    }
    else
    {
      delete base007;
    }
    lastIndex = tryLast007;
    return eGrmErrNoError;
  }
  delete base007;

  int32 tryLast008 = index;
  GrammarReturnerBase * base008 = new GrammarReturnerBase(eUnaryExpression, "");
  bool ret008 = INVOKE(DeleteExpression, tryLast008 , tryLast008, curBlock, base008, NOT_OPT);
  if (ret008)
  {
    if (returner)
    {
      returner -> addChild(base008);
    }
    else
    {
      delete base008;
    }
    lastIndex = tryLast008;
    return eGrmErrNoError;
  }
  delete base008;

  int32 tryLast009 = index;
  GrammarReturnerBase * base009 = new GrammarReturnerBase(eUnaryExpression, "");
  bool ret009 = INVOKE(NoexceptExpression, tryLast009 , tryLast009, curBlock, base009, NOT_OPT);
  if (ret009)
  {
    if (returner)
    {
      returner -> addChild(base009);
    }
    else
    {
      delete base009;
    }
    lastIndex = tryLast009;
    return eGrmErrNoError;
  }
  delete base009;

  int32 tryLast0010 = index;
  GrammarReturnerBase * base010 = new GrammarReturnerBase(eUnaryExpression, "");
  bool ret0010 = INVOKE(PostfixExpression, tryLast0010 , tryLast0010, curBlock, base010, NOT_OPT);
  if (ret0010)
  {
    if (returner)
    {
      returner -> addChild(base010);
    }
    else
    {
      delete base010;
    }
    lastIndex = tryLast0010;
    return eGrmErrNoError;
  }
  delete base010;

  int32 tryLast0011 = index;
  GrammarReturnerBase * base011 = new GrammarReturnerBase(eUnaryExpression, "");
  bool ret0011 = INVOKE(UnaryOperator, index, tryLast0011, curBlock, base011, NOT_OPT) &&
    INVOKE(CastExpression, tryLast0011 + 1, tryLast0011, curBlock, base011, NOT_OPT);
  if (ret0011)
  {
    if (returner)
    {
      returner -> addChild(base011);
    }
    else
    {
      delete base011;
    }
    lastIndex = tryLast0011;
    return eGrmErrNoError;
  }
  delete base011;

  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleNoexceptExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  GrammarReturnerBase *base = new GrammarReturnerBase(eNoexceptExpression, "");
  int32 tryLast = index;
  bool ret = EXPECT(index, tryLast, "noexcept", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLast + 1, tryLast, "(", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Expression, tryLast + 1, tryLast, curBlock, base, NOT_OPT) &&
    EXPECT(tryLast + 1, tryLast, ")", NOT_OPT, NOT_IN_ONE_LINE) ;
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

uint32 GrammarAnalyzer::handlePostfixExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLastLoop = index - 1;
  bool inLoop = false;
  GrammarReturnerBase * baseLoop = new GrammarReturnerBase(ePostfixExpression, "");
  while (INVOKE(PostfixExpression, tryLastLoop + 1, tryLastLoop, curBlock, baseLoop, NOT_OPT))
  {
    inLoop = true;
  }
  if (inLoop)
  {
    bool inDo = false;
    GrammarReturnerBase * baseLoop001 = new GrammarReturnerBase(ePostfixExpression, "");
    GrammarReturnerBase * baseLoop002 = new GrammarReturnerBase(ePostfixExpression, "");
    GrammarReturnerBase * baseLoop003 = new GrammarReturnerBase(ePostfixExpression, "");
    GrammarReturnerBase * baseLoop004 = new GrammarReturnerBase(ePostfixExpression, "");
    GrammarReturnerBase * baseLoop005 = new GrammarReturnerBase(ePostfixExpression, "");
    GrammarReturnerBase * baseLoop006 = new GrammarReturnerBase(ePostfixExpression, "");
    GrammarReturnerBase * baseLoop007 = new GrammarReturnerBase(ePostfixExpression, "");
    do{
      inDo = false;
      int32 tryLastLoop001 = tryLastLoop;
      bool retLoop001 = EXPECT(tryLastLoop001 + 1, tryLastLoop001, "[", NOT_OPT, NOT_IN_ONE_LINE) &&
        INVOKE(Expression, tryLastLoop001 + 1, tryLastLoop001, curBlock, baseLoop001, NOT_OPT) &&
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
        INVOKE(BracedInitList, tryLastLoop002 + 1, tryLastLoop002, curBlock, baseLoop002, IS_OPT) &&
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
        INVOKE(ExpressionList, tryLastLoop003 + 1, tryLastLoop003, curBlock, baseLoop003, IS_OPT) &&
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
        INVOKE(PseudoDestructorName, tryLastLoop006 + 1, tryLastLoop006, curBlock, baseLoop004, NOT_OPT);
      if (retLoop006)
      {
        lastIndex = tryLastLoop006;
        tryLastLoop = tryLastLoop006;
        inDo = true;
        continue;
      }

      int32 tryLastLoop007 = tryLastLoop;
      bool retLoop007 = EXPECT(tryLastLoop007 + 1, tryLastLoop007, "->", NOT_OPT, NOT_IN_ONE_LINE) &&
        INVOKE(PseudoDestructorName, tryLastLoop007 + 1, tryLastLoop007, curBlock, baseLoop005, NOT_OPT);
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
        INVOKE(IdExpression, tryLastLoop008 + 1, tryLastLoop008, curBlock, baseLoop006, NOT_OPT);
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
        INVOKE(IdExpression, tryLastLoop009 + 1, tryLastLoop009, curBlock, baseLoop007, NOT_OPT);
      if (retLoop009)
      {
        lastIndex = tryLastLoop009;
        tryLastLoop = tryLastLoop009;
        inDo = true;
        continue;
      }

    }while(inDo);

    if (returner)
    {
      baseLoop->mergeChild(baseLoop001);
      baseLoop->mergeChild(baseLoop002);
      baseLoop->mergeChild(baseLoop003);
      baseLoop->mergeChild(baseLoop004);
      baseLoop->mergeChild(baseLoop005);
      baseLoop->mergeChild(baseLoop006);
      baseLoop->mergeChild(baseLoop007);
      returner -> addChild(baseLoop);
    }
    else
    {
      delete baseLoop;
    }
    delete baseLoop001;
    delete baseLoop002;
    delete baseLoop003;
    delete baseLoop004;
    delete baseLoop005;
    delete baseLoop006;
    delete baseLoop007;

    lastIndex = tryLastLoop;

    return eGrmErrNoError;
  }

  int32 tryLastA = index;
  GrammarReturnerBase * baseA = new GrammarReturnerBase(ePostfixExpression, "");
  bool retA = EXPECT(index, tryLastA, "dynamic_cast", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLastA + 1, tryLastA, "<", NOT_OPT, NOT_OPT) &&
    INVOKE(TypeId, tryLastA + 1, tryLastA, curBlock, baseA, NOT_OPT) &&
    EXPECT(tryLastA + 1, tryLastA, ">", NOT_OPT, NOT_OPT) &&
    EXPECT(tryLastA + 1, tryLastA, "(", NOT_OPT, NOT_OPT) &&
    INVOKE(Expression, tryLastA + 1, tryLastA, curBlock, baseA, NOT_OPT) &&
    EXPECT(tryLastA + 1, tryLastA, ")", NOT_OPT, NOT_OPT);
  if (retA)
  {
    lastIndex = tryLastA;
    if (returner)
    {
      returner -> addChild( baseA );
    }
    else
    {
      delete baseA;
    }
    return eGrmErrNoError;
  }
  delete baseA;

  GrammarReturnerBase * baseB = new GrammarReturnerBase(ePostfixExpression, "");
  int32 tryLastB = index;
  bool retB = EXPECT(index, tryLastB, "static_cast", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLastB + 1, tryLastB, "<", NOT_OPT, NOT_OPT) &&
    INVOKE(TypeId, tryLastB + 1, tryLastB, curBlock, baseB, NOT_OPT) &&
    EXPECT(tryLastB + 1, tryLastB, ">", NOT_OPT, NOT_OPT) &&
    EXPECT(tryLastB + 1, tryLastB, "(", NOT_OPT, NOT_OPT) &&
    INVOKE(Expression, tryLastB + 1, tryLastB, curBlock, baseB, NOT_OPT) &&
    EXPECT(tryLastB + 1, tryLastB, ")", NOT_OPT, NOT_OPT);
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

  GrammarReturnerBase * baseC = new GrammarReturnerBase(ePostfixExpression, "");
  int32 tryLastC = index;
  bool retC = EXPECT(index, tryLastC, "reinterpret_cast", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLastC + 1, tryLastC, "<", NOT_OPT, NOT_OPT) &&
    INVOKE(TypeId, tryLastC + 1, tryLastC, curBlock, baseC, NOT_OPT) &&
    EXPECT(tryLastC + 1, tryLastC, ">", NOT_OPT, NOT_OPT) &&
    EXPECT(tryLastC + 1, tryLastC, "(", NOT_OPT, NOT_OPT) &&
    INVOKE(Expression, tryLastC + 1, tryLastC, curBlock, baseC, NOT_OPT) &&
    EXPECT(tryLastC + 1, tryLastC, ")", NOT_OPT, NOT_OPT);
  if (retC)
  {
    lastIndex = tryLastC;
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

  GrammarReturnerBase * baseD = new GrammarReturnerBase(ePostfixExpression, "");
  int32 tryLastD = index;
  bool retD = EXPECT(index, tryLastD, "const_cast", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLastD + 1, tryLastD, "<", NOT_OPT, NOT_OPT) &&
    INVOKE(TypeId, tryLastD + 1, tryLastD, curBlock, baseD, NOT_OPT) &&
    EXPECT(tryLastD + 1, tryLastD, ">", NOT_OPT, NOT_OPT) &&
    EXPECT(tryLastD + 1, tryLastD, "(", NOT_OPT, NOT_OPT) &&
    INVOKE(Expression, tryLastD + 1, tryLastD, curBlock, baseD, NOT_OPT) &&
    EXPECT(tryLastD + 1, tryLastD, ")", NOT_OPT, NOT_OPT);
  if (retD)
  {
    if (returner)
    {
      returner -> addChild(baseD);
    }
    else
    {
      delete baseD;
    }
    lastIndex = tryLastD;
    return eGrmErrNoError;
  }
  delete baseD;

  int32 tryLastE = index;
  GrammarReturnerBase * baseE = new GrammarReturnerBase(ePostfixExpression, "");
  bool retE = EXPECT(index, tryLastE, "typeid", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLastE + 1, tryLastE, "(", NOT_OPT, NOT_OPT) &&
    INVOKE(Expression, tryLastE + 1, tryLastE, curBlock, baseE, NOT_OPT) &&
    EXPECT(tryLastE + 1, tryLastE, ")", NOT_OPT, NOT_OPT);
  if (retE)
  {
    lastIndex = tryLastE;
    if (returner)
    {
      returner -> addChild( baseE );
    }
    else
    {
      delete baseE;
    }
    return eGrmErrNoError;
  }
  delete baseE;

  GrammarReturnerBase * baseF = new GrammarReturnerBase(ePostfixExpression, "");
  int32 tryLastF = index;
  bool retF = EXPECT(index, tryLastF, "typeid", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLastF + 1, tryLastF, "(", NOT_OPT, NOT_OPT) &&
    INVOKE(TypeId, tryLastF + 1, tryLastF, curBlock, baseF, NOT_OPT) &&
    EXPECT(tryLastF + 1, tryLastF, ")", NOT_OPT, NOT_OPT);
  if (retF)
  {
    if (returner)
    {
      returner -> addChild(baseF);
    }
    else
    {
      delete baseF;
    }
    lastIndex = tryLastF;
    return eGrmErrNoError;
  }
  delete baseF;

  int32 tryLast001 = index; 
  GrammarReturnerBase * base001 = new GrammarReturnerBase(ePostfixExpression, "");
  bool ret001 = INVOKE(PrimaryExpression, index, tryLast001, curBlock, base001, NOT_OPT );
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
  
  int32 tryLast002 = index;
  GrammarReturnerBase * base002 = new GrammarReturnerBase(ePostfixExpression, "");
  bool ret002 = INVOKE(SimpleTypeSpecifier, index, tryLast002, curBlock, base002, NOT_OPT ) &&
    EXPECT(tryLast002 + 1,tryLast002, "(", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(ExpressionList, tryLast002 + 1, tryLast002, curBlock, base002, IS_OPT) &&
    EXPECT(tryLast002 + 1,tryLast002, ")", NOT_OPT, NOT_IN_ONE_LINE); 
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

  int32 tryLast003 = index;
  GrammarReturnerBase * base003 = new GrammarReturnerBase(ePostfixExpression, "");
  bool ret003 = INVOKE(TypenameSpecifier, index, tryLast003, curBlock, base003, NOT_OPT ) &&
    EXPECT(tryLast003 + 1,tryLast003, "(", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(ExpressionList, tryLast003 + 1, tryLast003, curBlock, base003, IS_OPT) &&
    EXPECT(tryLast003 + 1,tryLast003, ")", NOT_OPT, NOT_IN_ONE_LINE); 
  if (ret003)
  {
    if (returner)
    {
      returner -> addChild(base003);
    }
    else
    {
      delete base003;
    }
    lastIndex = tryLast003;
    return eGrmErrNoError;
  }
  delete base003;

  int32 tryLast004 = index;
  GrammarReturnerBase * base004 = new GrammarReturnerBase(ePostfixExpression, "");
  bool ret004 = INVOKE(SimpleTypeSpecifier, index, tryLast004, curBlock, base004, NOT_OPT ) &&
    INVOKE(BracedInitList, tryLast004 + 1, tryLast004, curBlock, base004, NOT_OPT); 
  if (ret004)
  {
    if (returner)
    {
      returner -> addChild(base004);
    }
    else
    {
      delete base004;
    }
    lastIndex = tryLast004;
    return eGrmErrNoError;
  }
  delete base004;

  GrammarReturnerBase * base005 = new GrammarReturnerBase(ePostfixExpression, "");
  int32 tryLast005 = index;
  bool ret005 = INVOKE(TypenameSpecifier, index, tryLast005, curBlock, returner, NOT_OPT ) &&
    INVOKE(BracedInitList, tryLast005 + 1, tryLast005, curBlock, returner, NOT_OPT) ;
  if (ret005)
  {
    if (returner)
    {
      returner -> addChild(base005);
    }
    else
    {
      delete base005;
    }
    lastIndex = tryLast005;
    return eGrmErrNoError;
  }
  delete base005;


  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handlePseudoDestructorName(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  GrammarReturnerBase * base001 = new GrammarReturnerBase(ePseudoDestructorName, "");
  int32 tryLast001 = index;
  bool ret001 = EXPECT(index, tryLast001, "~", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(DecltypeSpecifier, tryLast001 + 1, tryLast001, curBlock, base001, NOT_OPT);
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

  GrammarReturnerBase * base002 = new GrammarReturnerBase(ePseudoDestructorName, "");
  int32 tryLast002 = index;
  bool ret002 = EXPECT(index, tryLast002, "::", IS_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(NestNameSpecifier, tryLast002 + 1, tryLast002, curBlock, base002, IS_OPT) &&
    INVOKE(TypeName, tryLast002 + 1, tryLast002, curBlock, base002, NOT_OPT) &&
    EXPECT(tryLast002 + 1, tryLast002, "::", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLast002 + 1, tryLast002, "~", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(TypeName, tryLast002 + 1, tryLast002, curBlock, base002, NOT_OPT);
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

  GrammarReturnerBase * base003 = new GrammarReturnerBase(ePseudoDestructorName, "");
  int32 tryLast003 = index;
  bool ret003 = EXPECT(index, tryLast003, "::", IS_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(NestNameSpecifier, tryLast003 + 1, tryLast003, curBlock, base003, NOT_OPT) &&
    EXPECT(tryLast003 + 1, tryLast003, "template", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(SimpleTemplateId, tryLast003 + 1, tryLast003, curBlock, base003, NOT_OPT) &&
    EXPECT(tryLast003 + 1, tryLast003, "::", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLast003 + 1, tryLast003, "~", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(TypeName, tryLast003 + 1, tryLast003, curBlock, base003, NOT_OPT);
  if (ret003)
  {
    if (returner)
    {
      returner -> addChild(base003);
    }
    else
    {
      delete base003;
    }
    lastIndex = tryLast003;
    return eGrmErrNoError;
  }
  delete base003;

  GrammarReturnerBase * base004 = new GrammarReturnerBase(ePseudoDestructorName, "");
  int32 tryLast004 = index;
  bool ret004 = EXPECT(index, tryLast004, "::", IS_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(NestNameSpecifier, tryLast004 + 1, tryLast004, curBlock, base004, IS_OPT) &&
    EXPECT(tryLast004 + 1, tryLast004, "~", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(NestNameSpecifier, tryLast004 + 1, tryLast004, curBlock, base004, NOT_OPT);
  if (ret004)
  {
    if (returner)
    {
      returner -> addChild(base004);
    }
    else
    {
      delete base004;
    }
    lastIndex = tryLast004;
    return eGrmErrNoError;
  }
  delete base004;
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleTypenameSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  GrammarReturnerBase *base = new GrammarReturnerBase(eTypenameSpecifier, "");
  int32 tryLast = index;
  bool ret = EXPECT(index, tryLast, "typename", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLast + 1, tryLast, "::", IS_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(NestNameSpecifier, tryLast + 1, tryLast, curBlock, base, NOT_OPT);
  if (ret)
  {
    int32 tryLast001 = tryLast;
    GrammarReturnerBase *base001 = new GrammarReturnerBase(eTypenameSpecifier, "");
    bool ret1 = INVOKE(Identifier, tryLast001 + 1, tryLast001, curBlock, base001, NOT_OPT);
    if (ret1)
    {
      if (returner)
      {
        base -> mergeChild(base001);
        returner -> addChild(base);
      }
      else
      {
        delete base;
      }
      delete base001;
      lastIndex = tryLast001;
      return eGrmErrNoError;
    }
    delete base001;

    int32 tryLast002 = tryLast;
    GrammarReturnerBase *base002 = new GrammarReturnerBase(eTypenameSpecifier, "");
    bool ret2 = EXPECT(tryLast002 + 1, tryLast002, "template", IS_OPT, NOT_IN_ONE_LINE) &&
      INVOKE(SimpleTemplateId, tryLast002 + 1, tryLast002, curBlock, base002, NOT_OPT);
    if (ret2)
    {
      if (returner)
      {
        base -> mergeChild(base002);
        returner -> addChild(base);
      }
      else
      {
        delete base;
      }
      delete base002;
      lastIndex = tryLast002;
      return eGrmErrNoError;
    }
    delete base002;
  }
  delete base;
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleBracedInitList(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast001 = index;
  bool ret001 = EXPECT(index, tryLast001, "{", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLast001 + 1, tryLast001, "}", NOT_OPT, NOT_IN_ONE_LINE);
  if (ret001)
  {
    if (returner)
    {
      GrammarReturnerBase *base001 = new GrammarReturnerBase(eBracedInitList, "{}");
      returner -> addChild(base001);
    }
    lastIndex = tryLast001;
    return eGrmErrNoError;
  }

  int32 tryLast002 = index;
  GrammarReturnerBase *base002 = new GrammarReturnerBase(eBracedInitList, "{}");
  bool ret002 = EXPECT(index, tryLast002, "{", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(InitializerList, tryLast002 + 1, tryLast002, curBlock, base002, NOT_OPT) &&
    EXPECT(tryLast002 + 1, tryLast002, ",", IS_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLast002 + 1, tryLast002, "}", NOT_OPT, NOT_IN_ONE_LINE);
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

uint32 GrammarAnalyzer::handleExpressionList(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  GrammarReturnerBase *base = new GrammarReturnerBase(eExpressionList, "{}");
  bool ret = INVOKE(InitializerList, index, tryLast, curBlock, base, NOT_OPT);
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

uint32 GrammarAnalyzer::handleInitializerList(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  GrammarReturnerBase *base = new GrammarReturnerBase(eInitializerList, "");
  bool ret = INVOKE(InitializerClause, tryLast, tryLast, curBlock, base, NOT_OPT);
  if (ret)
  {
    lastIndex = tryLast;
    while(EXPECT(tryLast + 1, tryLast, ",", NOT_OPT, NOT_IN_ONE_LINE) &&
        INVOKE(InitializerClause, tryLast + 1, tryLast, curBlock, base, NOT_OPT))
    {
      lastIndex = tryLast;
    }
    EXPECT(lastIndex + 1, lastIndex, "...", NOT_OPT, NOT_IN_ONE_LINE);
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

uint32 GrammarAnalyzer::handlePrimaryExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast001 = index;
  if (EXPECT(index, tryLast001, "this", NOT_OPT, NOT_IN_ONE_LINE)) 
  {
    if (returner)
    {
      GrammarReturnerBase * base = new GrammarReturnerBase(ePrimaryExpression, "this");
      returner -> addChild(base);
    }
    lastIndex = tryLast001;
    return eGrmErrNoError;
  }

  int32 tryLast002 = index;
  GrammarReturnerBase * base002 = new GrammarReturnerBase(ePrimaryExpression, "");
  bool ret002 = EXPECT(index, tryLast002, "(", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(Expression, tryLast002 + 1, tryLast002, curBlock, base002, NOT_OPT) &&
    EXPECT(tryLast002 + 1, tryLast002, ")", NOT_OPT, NOT_IN_ONE_LINE);
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

  int32 tryLast003 = index;
  GrammarReturnerBase * base003 = new GrammarReturnerBase(ePrimaryExpression, "");
  bool ret003 = INVOKE(Literal, tryLast003, tryLast003, curBlock, base003, NOT_OPT);
  if (ret003)
  {
    if (returner)
    {
      returner -> addChild(base003);
    }
    else
    {
      delete base003;
    }
    lastIndex = tryLast003;
    return eGrmErrNoError;
  }
  delete base003;

  int32 tryLast004 = index;
  GrammarReturnerBase * base004 = new GrammarReturnerBase(ePrimaryExpression, "");
  bool ret004 = INVOKE(IdExpression, index, tryLast004, curBlock, base004, NOT_OPT);
  if (ret004)
  {
    if (returner)
    {
      returner -> addChild(base004);
    }
    else
    {
      delete base004;
    }
    lastIndex = tryLast004;
    return eGrmErrNoError;
  }
  delete base004;

  int32 tryLast005 = index;
  GrammarReturnerBase * base005 = new GrammarReturnerBase(ePrimaryExpression, "");
  bool ret005 = INVOKE(LambdaExpression, index, tryLast005, curBlock, base005, NOT_OPT);
  if (ret005)
  {
    if (returner)
    {
      returner -> addChild(base005);
    }
    else
    {
      delete base005;
    }
    lastIndex = tryLast005;
    return eGrmErrNoError;
  }
  delete base005;
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleDeleteExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  GrammarReturnerBase * base002 = new GrammarReturnerBase(eDeleteExpression, "");
  int32 tryLast002 = index;
  bool ret002 = EXPECT(index, tryLast002, "::", IS_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLast002 + 1, tryLast002, "delete", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLast002 + 1, tryLast002, "[", NOT_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLast002 + 1, tryLast002, "]", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(CastExpression, tryLast002 + 1, tryLast002, curBlock, base002, NOT_OPT);
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

  GrammarReturnerBase * base001 = new GrammarReturnerBase(eDeleteExpression, "");
  int32 tryLast001 = index;
  bool ret001 = EXPECT(index, tryLast001, "::", IS_OPT, NOT_IN_ONE_LINE) &&
    EXPECT(tryLast001 + 1, tryLast001, "delete", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(CastExpression, tryLast001 + 1, tryLast001, curBlock, base001, NOT_OPT);
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
  
  return eGrmErrUnknown;
}

uint32 GrammarAnalyzer::handleInitializerClause(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  GrammarReturnerBase * base001 = new GrammarReturnerBase(eInitializerClause, "");
  int32 tryLast001 = index;
  bool ret001 = INVOKE(BracedInitList, tryLast001, tryLast001, curBlock, base001, NOT_OPT);
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

  GrammarReturnerBase * base002 = new GrammarReturnerBase(eInitializerClause, "");
  int32 tryLast002 = index;
  bool ret002 = INVOKE(AssignmentExpression, tryLast002, tryLast002, curBlock, base002, NOT_OPT);
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

uint32 GrammarAnalyzer::handleThrowExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  GrammarReturnerBase * base = new GrammarReturnerBase(eThrowExpression, "");
  int32 tryLast = index;
  bool ret = EXPECT(index, tryLast, "throw", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(AssignmentExpression, tryLast + 1, tryLast, curBlock, base, IS_OPT);
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

uint32 GrammarAnalyzer::handleConversionFunctionId(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  GrammarReturnerBase * base = new GrammarReturnerBase(eConversionFunctionId, "");
  int32 tryLast = index;
  bool ret = EXPECT(index, tryLast, "operator", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(ConversionTypeId, tryLast + 1, tryLast, curBlock, base, NOT_OPT);
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

uint32 GrammarAnalyzer::handleConversionTypeId(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  GrammarReturnerBase * base = new GrammarReturnerBase(eConversionTypeId, "");
  int32 tryLast = index;
  bool ret = INVOKE(TypeSpecifierSeq, index, tryLast, curBlock, base, NOT_OPT) &&
    INVOKE(ConversionDeclarator, tryLast + 1, tryLast, curBlock, base, IS_OPT);
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

uint32 GrammarAnalyzer::handleConversionDeclarator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  GrammarReturnerBase * base = new GrammarReturnerBase(eConversionDeclarator, "");
  int32 tryLast = index;
  bool ret = INVOKE(PtrOperator, index, tryLast, curBlock, base, NOT_OPT) &&
    INVOKE(ConversionDeclarator, tryLast + 1, tryLast, curBlock, base, IS_OPT);
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

uint32 GrammarAnalyzer::handleOperatorFunctionId(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  GrammarReturnerBase * base = new GrammarReturnerBase(eOperatorFunctionId, "");
  bool ret = EXPECT(index, tryLast, "operator", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(OverloadableOperator, tryLast + 1, tryLast, curBlock, base, NOT_IN_ONE_LINE) ;
  if (ret)
  {
    lastIndex = tryLast;
    GrammarReturnerBase *baseA = new GrammarReturnerBase(eOperatorFunctionId, "");
    int32 tryLastA = tryLast;
    bool retA=  EXPECT(tryLastA + 1, tryLastA, "<", NOT_OPT, NOT_IN_ONE_LINE) &&
      INVOKE(TemplateArgumentList, tryLastA + 1, tryLastA, curBlock, baseA, IS_OPT) &&
      EXPECT(tryLastA + 1, tryLastA, ">", NOT_OPT, NOT_IN_ONE_LINE);
    if (retA)
    {
      lastIndex = tryLastA;
      base -> mergeChild(baseA);
    }
    delete baseA;

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
  int32 tryLast001 = index;
  GrammarReturnerBase *base001 = new GrammarReturnerBase(eTemplateId, "");
  bool ret001 = INVOKE(SimpleTemplateId, index, tryLast001, curBlock, base001, NOT_OPT);
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

  int32 tryLast002 = index;
  GrammarReturnerBase *base002 = new GrammarReturnerBase(eTemplateId, "");
  bool ret002 = INVOKE(OperatorFunctionId, index, tryLast002, curBlock, base002, NOT_OPT) &&
    EXPECT(tryLast002 + 1, tryLast002, "<", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(TemplateArgumentList, tryLast002 + 1, tryLast002, curBlock, base002, IS_OPT) &&
    EXPECT(tryLast002 + 1, tryLast002, ">", NOT_OPT, NOT_IN_ONE_LINE);
    
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

  int32 tryLast003 = index;
  GrammarReturnerBase *base003 = new GrammarReturnerBase(eTemplateId, "");
  bool ret003 = INVOKE(LiteralOperatorId, index, tryLast003, curBlock, base003, NOT_OPT) &&
    EXPECT(tryLast003 + 1, tryLast003, "<", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(TemplateArgumentList, tryLast003 + 1, tryLast003, curBlock, base003, IS_OPT) &&
    EXPECT(tryLast003 + 1, tryLast003, ">", NOT_OPT, NOT_IN_ONE_LINE);
  if (ret003)
  {
    if (returner)
    {
      returner -> addChild(base003);
    }
    else
    {
      delete base003;
    }
    lastIndex = tryLast003;
    return eGrmErrNoError;
  }
  delete base003;

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
  int32 tryLast = index;
  GrammarReturnerBase *base = new GrammarReturnerBase(eTemplateArgument, "");
  bool ret = INVOKE(TemplateArgument, index, tryLast, curBlock, base, NOT_OPT);
  if (ret)
  {
    lastIndex = tryLast;
    while (EXPECT(tryLast + 1, tryLast, ",", NOT_OPT, NOT_IN_ONE_LINE) &&
        INVOKE(TemplateArgument, tryLast + 1, tryLast, curBlock, base, NOT_OPT))
    {
      lastIndex = tryLast;
    }

    EXPECT(lastIndex + 1, lastIndex, "...", NOT_OPT, NOT_IN_ONE_LINE);

    if (returner)
    {
      returner -> addChild( base );
    }
    else
    {
      delete base;
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
  int32 tryLast = index;
  GrammarReturnerBase * base = new GrammarReturnerBase(eClassSpecifier, "");
  bool ret = INVOKE(ClassHead, index, tryLast, curBlock, base, NOT_OPT) &&
    EXPECT(tryLast + 1, tryLast, "{", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(MemberSpecification, tryLast + 1, tryLast, curBlock, base, NOT_OPT) &&
    EXPECT(tryLast + 1, tryLast, "}", NOT_OPT, NOT_IN_ONE_LINE);
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

uint32 GrammarAnalyzer::handleClassHead(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner)
{
  int32 tryLast = index;
  GrammarReturnerBase *base = new GrammarReturnerBase(eClassHead, "");
  bool ret = INVOKE(ClassKey, index, tryLast, curBlock, base, NOT_OPT) &&
    INVOKE(Attributes, tryLast + 1, tryLast, curBlock, base, IS_OPT);
  if (ret)
  {
    GrammarReturnerBase *base01 = new GrammarReturnerBase(eClassHead, "");
    int32 tryLast01 = tryLast;
    bool ret01 = INVOKE(ClassHeadName, tryLast01 + 1, tryLast01, curBlock, base01, NOT_OPT) &&
      INVOKE(ClassVirtSpecifierSeq, tryLast01 + 1, tryLast01, curBlock, base01, IS_OPT) &&
      INVOKE(BaseClause, tryLast01 + 1, tryLast01, curBlock, base01, IS_OPT);
    if (ret01)
    {
      if (returner)
      {
        base -> mergeChild(base01);
        returner -> addChild(base);
      }
      else
      {
        delete base01;
        delete base;
      }
      lastIndex = tryLast01;
      return eGrmErrNoError;
    }
    delete base01;

    GrammarReturnerBase *base02 = new GrammarReturnerBase(eClassHead, "");
    int32 tryLast02 = tryLast;
    bool ret02 = INVOKE(BaseClause, tryLast02 + 1, tryLast02, curBlock, base02, IS_OPT);
    //actually it should always be true...
    if (ret02)
    {
      if (returner)
      {
        base -> mergeChild(base02);
        returner -> addChild(base);
      }
      else
      {
        delete base02;
        delete base;
      }
      lastIndex = tryLast02;
      return eGrmErrNoError;
    }
    delete base02;
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
  GrammarReturnerBase *base = new GrammarReturnerBase(eLinkageSpecification, "");
  bool ret = EXPECT(tryLast, tryLast, "extern", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(ExternLiteral, tryLast + 1, tryLast, curBlock, base, NOT_OPT);
  if (ret)
  {

    GrammarReturnerBase *base01 = new GrammarReturnerBase(eLinkageSpecification, "");
    int32 tryLastA = tryLast;
    bool retA = EXPECT(tryLastA + 1, tryLastA, "{", NOT_OPT, NOT_IN_ONE_LINE) &&
      INVOKE(DeclarationSeq, tryLastA + 1, tryLastA, curBlock, base01, IS_OPT) &&
      EXPECT(tryLastA + 1, tryLastA, "}", NOT_OPT, NOT_IN_ONE_LINE);
    if (retA)
    {
      lastIndex = tryLastA;
      if (returner)
      {
        base -> mergeChild(base01);
        returner -> addChild(base);
      }
      else
      {
        delete base01;
      }
      return eGrmErrNoError;
    }
    delete base01;

    int32 tryLastB = tryLast;
    GrammarReturnerBase *base02 = new GrammarReturnerBase(eLinkageSpecification, "");
    bool retB = INVOKE(Declaration, tryLastA + 1, tryLastA, curBlock, base02, NOT_OPT) ;
    if (retB)
    {
      lastIndex = tryLastB;
      if (returner)
      {
        base -> mergeChild(base02);
        returner -> addChild(base);
      }
      else
      {
        delete base02;
      }
      return eGrmErrNoError;
    }
    delete base02;
  }
  delete base;
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
  GrammarReturnerBase *baseA = new GrammarReturnerBase(eMemberSpecification,"");
  bool retA = INVOKE(MemberDeclaration, index, tryLastA, curBlock, baseA, NOT_OPT) &&
    INVOKE(MemberSpecification, tryLastA + 1,tryLastA, curBlock, baseA, IS_OPT);
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

  int32 tryLastB = index;
  GrammarReturnerBase *baseB = new GrammarReturnerBase(eMemberSpecification,"");
  bool retB = INVOKE(AccessSpecifier, index, tryLastB, curBlock, baseB, NOT_OPT) &&
    EXPECT(tryLastB + 1, tryLastB, ":", NOT_OPT, NOT_IN_ONE_LINE) &&
    INVOKE(MemberSpecification, tryLastB + 1,tryLastB, curBlock, baseB, IS_OPT);
  if (retB)
  {
    lastIndex = tryLastB;
    if (returner)
    {
      returner -> addChild(baseB);
    }
    else
    {
      delete baseB;
    }
    return eGrmErrNoError;
  }
  delete baseB;

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
