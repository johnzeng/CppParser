#include "GrammarReturner.h"
#include "JZMacroFunc.h"

map<int32, string> GrammarReturnerBase::sNameMap;

void GrammarReturnerBase::initMap()
{
  sNameMap[eGrmTop] = "eGrmTop";
  sNameMap[eTypeId] = "eTypeId";
  sNameMap[eBlockDeclaration] = "eBlockDeclaration";
  sNameMap[eTrailingTypeSpecifier] = "eTrailingTypeSpecifier";
  sNameMap[eTypeName] = "eTypeName";
  sNameMap[eSimpleTemplateId] = "eSimpleTemplateId";
  sNameMap[eClassHeadName] = "eClassHeadName";
  sNameMap[eClassName] = "eClassName";
  sNameMap[eTemplateName] = "eTemplateName";
  sNameMap[eLiteralOperatorId] = "eLiteralOperatorId";
  sNameMap[eIdExpression] = "eIdExpression";
  sNameMap[eDeclaratorId] = "eDeclaratorId";
  sNameMap[ePtrOperator] = "ePtrOperator";
  sNameMap[eNestNameSpecifier] = "eNestNameSpecifier";
  sNameMap[ePtrDeclarator] = "ePtrDeclarator";
  sNameMap[eCompoundStatement] = "eCompoundStatement";
  sNameMap[eFunctionBody] = "eFunctionBody";
  sNameMap[eFunctionDefinition] = "eFunctionDefinition";
  sNameMap[eCVQualifierSeq] = "eCVQualifierSeq";
  sNameMap[eSimpleDeclaration] = "eSimpleDeclaration";
  sNameMap[eDeclarationSeq] = "eDeclarationSeq";
  sNameMap[eDeclaration] = "eDeclaration";
  sNameMap[eEnumeratorDefinition] = "eEnumeratorDefinition";
  sNameMap[eDeclarator] = "eDeclarator";
  sNameMap[eSimpleTypeSpecifier] = "eSimpleTypeSpecifier";
  sNameMap[eTypeSpecifierSeq] = "eTypeSpecifierSeq";

}

GrammarReturnerBase::GrammarReturnerBase(int32 type, string key):
  mRetType(type),
  mKey(key)
{
}

GrammarReturnerBase::~GrammarReturnerBase()
{
  int i = 0;
  for (i = 0; i < mChildren.size(); i++) 
  {
    JZSAFE_DELETE(mChildren[i]);
  }
}

void GrammarReturnerBase::addChild(GrammarReturnerBase* child)
{
  if (NULL == child)
  {
    return;
  }
  mChildren.push_back(child);
}

void GrammarReturnerBase::printAllChild(int tabNum)
{
  if (0 == sNameMap.size())
  {
    initMap();
  }
  string toPrint = "";
  for (int i = 0; i < tabNum; i++) {
    toPrint += "|\t";
  }

  toPrint += sNameMap[mRetType];
  
  printf("%s\n", toPrint.c_str());

  for (int i = 0; i < mChildren.size(); i++) {
    mChildren[i]->printAllChild(tabNum + 1);
  }
}

int32 GrammarReturnerBase::getType() const
{
  return mRetType;
}

const string& GrammarReturnerBase::getKey() const
{
  return mKey;
}

GrammarReturnerBase* GrammarReturnerBase::getChild(int32 index) const
{
  if (index < mChildren.size())
  {
    return mChildren[index];
  }
  else
  {
    return NULL;
  }
}

void GrammarReturnerBase::mergeChild(GrammarReturnerBase* node)
{
  for (int i = 0; i < node->mChildren.size(); i++) {
    addChild(node->getChild(i));
  }
  node->mChildren.clear();
}
