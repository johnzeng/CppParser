#include "GrammarAnalyzer.h"

GrammarNode::GrammarNode(GrammarNode* father):
  mFather(father)
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

GrammarNode* GrammarNode::getTopNode()
{
    static GrammarNode* instance = NULL;
    if (NULL == instance)
    {
      instance = new GrammarNode(NULL);
      instance->mFather = instance;
      instance->mNodeType = eGrammarNodeTopNode;

      //add other init for top class node
    }
    return instance;
}

DataTypeDefine::DataTypeDefine()
{
}
DataTypeDefine::~DataTypeDefine()
{
}

NormalDefine::NormalDefine(string keyWord)
{
  mKeyWrods = keyWord;
  signature = "__" + mKeyWrods;
  mDataType = eDataTypeNormal;
}

