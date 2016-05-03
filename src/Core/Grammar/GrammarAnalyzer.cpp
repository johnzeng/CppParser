#include "GrammarAnalyzer.h"

GrammarNode::GrammarNode():
  mFather(NULL)
{
}

GrammarNode::~GrammarNode()
{
  auto it = mChildrens.begin();
  for(;it != mChildrens.end(); it++)
  {
    delete (*it);
  }
}

GrammarBlock* GrammarBlock::getTopNode()
{
    static GrammarBlock* instance = NULL;
    if (NULL == instance)
    {
      instance = new GrammarBlock();
      instance->mFather = instance;
      instance->mNodeType = eGrammarNodeTopNode;

      //add other init for top class node
//      BasicDefine intDefine = BasicDefine("int");
//      BasicDefine longDefine = BasicDefine("long");
//      BasicDefine doubleDefine = BasicDefine("double");
//      BasicDefine charDefine = BasicDefine("char");
//      BasicDefine doubleDefine = BasicDefine("double");
//      BasicDefine floatDefine = BasicDefine("float");
    }
    return instance;
}

DataTypeDefine::DataTypeDefine()
{
}
DataTypeDefine::~DataTypeDefine()
{
}

BasicDefine::BasicDefine(string keyWord)
{
  mKeyWrods = keyWord;
  signature = "__" + mKeyWrods;
  mDataType = eDataTypeBasic;
}

