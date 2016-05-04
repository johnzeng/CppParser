#include "GrammarAnalyzer.h"
#include "StringUtil.h"
#include "JZLogger.h"

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
      JZFUNC_BEGIN_LOG();
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
      JZFUNC_END_LOG();
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

string DataTypeDefine::getKeyWord(int index)
{
  if(index < mKeyWords.size())
  {
    return mKeyWords[index];
  }
  else
  {
    return "";
  }
}

void GrammarNode::setFather(GrammarNode* father)
{
  mFather = father;
}

uint32 GrammarBlock::addDataTypeDefine(DataTypeDefine dataType)
{
  dataType.setFather(this) ;

  string sig = dataType.getSignature();

  auto it = mDataTypeList.find(sig);
  if(mDataTypeList.end() != it)
  {
    return eGrammarErrorDoubleDefinedDataType;
  }
  mDataTypeList[sig] = dataType;
  mChildrens.push_back(&mDataTypeList[sig]);

  return eGrammarErrorNoError;
}

VarDefine::VarDefine(string id, DataTypeDefine* define)
{
  mIdentify = id;
  mDataType = define;
  if (NULL == define)
  {
    JZWRITE_ERROR("NULL define");
  }
}
