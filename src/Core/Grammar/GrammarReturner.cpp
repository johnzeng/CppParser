#include "GrammarReturner.h"
#include "JZMacroFunc.h"

map<int32, string> GrammarReturnerBase::sNameMap;

void GrammarReturnerBase::initMap()
{
  sNameMap[eGrmTop] = "eGrmTop";
  sNameMap[eDeclarationSeq] = "eDeclarationSeq";
  sNameMap[eDeclaration] = "eDeclaration";
  sNameMap[eEnumeratorDefinition] = "eEnumeratorDefinition";

}

GrammarReturnerBase::GrammarReturnerBase(int32 type, string key):
  mRetType(type),
  mKey(key)
{
  if (0 == sNameMap.size())
  {
    initMap();
  }
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
