#include "GrammarAnalyzer.h"
#include "StringUtil.h"

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
      vector<string> basicInt;
      basicInt.push_back("int");
      BasicDefine intDefine = BasicDefine(basicInt);
      instance->addDataTypeDefine(intDefine);

      vector<string> basicUnsignedInt;
      basicUnsignedInt.push_back("unsigned");
      basicUnsignedInt.push_back("int");
      BasicDefine uintDefine = BasicDefine(basicUnsignedInt);
      instance->addDataTypeDefine(uintDefine);

      vector<string> basicLong;
      basicLong.push_back("long");
      BasicDefine longDefine = BasicDefine(basicLong);
      instance->addDataTypeDefine(longDefine);

      vector<string> basicLongLong;
      basicLongLong.push_back("long");
      basicLongLong.push_back("long");
      BasicDefine longLongDefine = BasicDefine(basicLongLong);
      instance->addDataTypeDefine(longLongDefine);
    }
    return instance;
}

DataTypeDefine::DataTypeDefine()
{
}
DataTypeDefine::~DataTypeDefine()
{
}

string DataTypeDefine::getSignature()
{
  return mSignature;
}

BasicDefine::BasicDefine(vector<string> keyWord)
{
  mKeyWords = keyWord;
  string tmpSig = "";
  for (int i = 0; i < keyWord.size(); i++) 
  {
    uint32 size = keyWord[i].size();
    tmpSig += "_" + StringUtil::tostr(size) + keyWord[i];
  }
  mSignature = tmpSig;
  mDataType = eDataTypeBasic;
}

