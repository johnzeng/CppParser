/*********************************************************
  put some simple api into this file 
 ********************************************************/


#include "GrammarAnalyzer.h"
#include "GrammarUtil.h"
#include "LexUtil.h"
#include "JZLogger.h"

GrammarAnalyzer::GrammarAnalyzer(LexRecList list):
  mRecList(list),
  mTopBlock(GrammarBlock::createTopNode())
{
}

GrammarAnalyzer::~GrammarAnalyzer()
{
}

GrammarBlock* GrammarAnalyzer::getTopBlock()
{
  return &mTopBlock;
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
        JZFUNC_END_LOG();
        return false;
      }
      else if (true == LexUtil::isInterpunction(id[i]))
      {
        JZFUNC_END_LOG();
        return false;
      }
    }
    char curChar = id[i];
    if (false == LexUtil::isIdentifyChar(curChar))
    {
      JZFUNC_END_LOG();
      return false;
    }
  }
  return true;
}
