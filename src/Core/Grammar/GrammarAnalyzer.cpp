#include "GrammarAnalyzer.h"
#include "GrammarUtil.h"

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

uint32 GrammarAnalyzer::blockHeartBeat(int32 index, int32& lastIndex, GrammarBlock* curBlock)
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

uint32 GrammarAnalyzer::handleEnum(int32 index, int32& lastIndex, GrammarBlock* curBlock)
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

uint32 GrammarAnalyzer::handleEnumId(int index, int& lastIndex, GrammarBlock* curBlock)
{
  if(mRecList.size() <= index)
  {
    return eGrammarErrorFileEnd;
  }
  //need to check the id
  if (false == isLeagalVarIdentify(mRecList[index].word))
  {
    return eGrammarErrorDoubleDefinedDataType;
  }
  lastIndex = index;
  string id = mRecList[index].word;
  EnumDefine *curEnum = new EnumDefine(id);

  uint32 ret = handleSemicolon(index + 1,lastIndex, curBlock);

  if (ret == eGrammarErrorNoError)
  {
    curEnum->setFather(curBlock);
    JZFUNC_END_LOG();
    return ret;
  }

  GrammarBlock* body = new GrammarBlock();
  curEnum->addBody(body);

  ret = handleLeftBrace(index + 1, lastIndex, body);
  if (ret == eGrammarErrorNoError)
  {
    curEnum->setFather(curBlock);
    JZFUNC_END_LOG();
    return ret;
  }

  return eGrammarErrorUnknown;
}

uint32 GrammarAnalyzer::handleLeftBrace(int index, int& lastIndex, GrammarBlock* curBlock)
{
  if(mRecList.size() <= index)
  {
    return eGrammarErrorFileEnd;
  }
  //need to check the id
  if (mRecList[index].word != "{")
  {
    return eGrammarErrorNotSemicolon;
  }

  auto father = curBlock->getFather();
  uint32 fatherNodeType = father == NULL ? eGrammarBlockTop : father->getType();

  lastIndex = index;
  uint32 ret = eGrammarErrorNoError;
  switch(fatherNodeType)
  {
    case eGrammarNodeTopNode:
      ret = blockHeartBeat(index + 1, lastIndex, curBlock);
      break;
    case eDataTypeEnum:
      ret = handleEnumFieldName(index + 1, lastIndex, curBlock);
      break;
    default:
    {
      break;  
    }
  }
  return ret;
}

uint32 GrammarAnalyzer::handleEnumFieldName(int index, int& lastIndex, GrammarBlock* curBlock)
{
  if(mRecList.size() <= index)
  {
    return eGrammarErrorFileEnd;
  }
  lastIndex = index;

  if(expect("}", index) == eGrammarErrorNoError)
  {
    if(expect(";", lastIndex + 1) == eGrammarErrorNoError)
    {
      return eGrammarErrorNoError;
    }
    else
    {
      JZWRITE_ERROR("missing ; at the end of enum define");
      return eGrammarErrorMissingSemicolon;
    }
  }

  if(expect(",", index) == eGrammarErrorNoError)
  {
    return handleEnumFieldName(index + 1, lastIndex, curBlock);
  }

  if(true == isLeagalVarIdentify(mRecList[index].word))
  {
    string fieldName = mRecList[index].word;
    EnumDefine *define = dynamic_cast<EnumDefine*>(curBlock.getFather());
    if(NULL == define)
    {
      return eGrammarErrorUnknown;
    }
    if(expect("=", index + 1) == eGrammarErrorNoError)
    {
      uint32 value = 0;
      uint32 getValeRet = handleStatement(index + 2, lastIndex, curBlock);
      if(getValeRet != eGrammarErrorNoError)
      {
        //should check value type here;
        return getValeRet;
      }
      uint32 addRet = define->addField(fieldName,value);
      if(addRet != eGrammarErrorNoError)
      {
        return addRet;
      }
      lastIndex = index + 2;
      return handleEnumFieldName(index + 3, lastIndex, curBlock);
    }
    else
    {
      uint32 addRet = define->addField(fieldName);
      if(addRet != eGrammarErrorNoError)
      {
        return addRet;
      }
      lastIndex = index + 1;
      return handleEnumFieldName(index + 2, lastIndex, curBlock);
    }
  }

  return eGrammarErrorNoError;
}

uint32 GrammarAnalyzer::expect(const string& expected, int index)
{
  if(mRecList.size() <= index)
  {
    return eGrammarErrorFileEnd;
  }
  //need to check the id
  if (mRecList[index].word != expected)
  {
    JZWRITE_DEBUG("expected %s not match", expected.c_str());
    return eGrammarErrorNotExpected;
  }
  return eGrammarErrorNoError;
}

bool GrammarAnalyzer::isLeagalVarIdentify(const string id, GrammarBlock* curBlock)
{
  const VarDefine* def = curBlock->getVarDef(id);
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
        return false;
      }
    }
    char curChar = id[i];
    if (false == LexUtil::isIdentifyChar(curChar))
    {
      return false
    }
  }
  return true;
}

uint32 GrammarAnalyzer::handleStatement(int index, int& lastIndex, GrammarBlock* curBlock)
{
  return eGrammarErrorNoError;
}
