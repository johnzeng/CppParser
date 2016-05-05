#include "GrammarAnalyzer.h"

GrammarAnalyzer::GrammarAnalyzer(LexRecList list):
  mRecList(list),
  mTopBlock(GrammarBlock::createTopNode())
{

}

uint32 GrammarAnalyzer::doAnalyze()
{
  int index = 0;
  int listSize = mRecList.size();
  while (index < listSize)
  {
    int32 nextIndex = index;
    uint32 ret = eGrammarErrorNoError;
    if(eGrammarErrorNoError != (ret = blockHeartBeat(index, nextIndex, mTopBlock) ))
    {
      return ret;
    }
  }
  return eGrammarErrorNoError;
}

uint32 GrammarAnalyzer::blockHeartBeat(int32 index, int32& lastIndex, GrammarBlock& curBlock)
{
  int nextIndex = index;
  uint32 ret = handleEnum(index, nextIndex, mTopBlock) /* || handleClass*/;
  if (ret != eGrammarErrorNoError || ret != eGrammarErrorNotEnum)
  {
    index = nextIndex;
    return ret;
  }
  return eGrammarErrorUnknown;
}

uint32 GrammarAnalyzer::handleEnum(int32 index, int32& lastIndex, GrammarBlock& curBlock)
{
  if(mRecList.size() <= index)
  {
    return eGrammarErrorFileEnd;
  }
  if (mRecList[index].word != "enum")
  {
    return eGrammarErrorNotEnum;
  }
  lastIndex = index;
  return handleEnumId(index + 1, lastIndex, curBlock);
}

uint32 GrammarAnalyzer::handleEnumId(int index, int& lastIndex, GrammarBlock &curBlock)
{
  if(mRecList.size() <= index)
  {
    return eGrammarErrorFileEnd;
  }
  //need to check the id
//  if (mRecList[index].word != "enum")
//  {
//    return eGrammarErrorNotEnum;
//  }
  lastIndex = index;
  string id = mRecList[index].word;
  EnumDefine *curEnum = new EnumDefine(id);
  GrammarBlock* body = new GrammarBlock();
  curEnum->addBody(body);
}
