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

uint32 GrammarAnalyzer::doAnalyze()
{
  int index = 0;
  int listSize = mRecList.size();
  while (index < listSize)
  {
    int32 nextIndex = index;
    uint32 ret = eGrammarErrorNoError;
    if(eGrammarErrorNoError != (ret = blockHeartBeat(index, nextIndex, &mTopBlock) ))
    {
      return ret;
    }
    index = nextIndex + 1;
  }
  return eGrammarErrorNoError;
}

uint32 GrammarAnalyzer::blockHeartBeat(int32 index, int32& lastIndex, GrammarBlock* curBlock)
{
  JZFUNC_BEGIN_LOG();
  int nextIndex = index;
  uint32 ret = handleEnum(index, nextIndex, curBlock) /* || handleClass*/;
  if (ret == eGrammarErrorNoError)
  {
    lastIndex = nextIndex;
    JZFUNC_END_LOG();
    return ret;
  }
  JZFUNC_END_LOG();
  return eGrammarErrorUnknown;
}

uint32 GrammarAnalyzer::handleEnum(int32 index, int32& lastIndex, GrammarBlock* curBlock)
{
  JZFUNC_BEGIN_LOG();
  if(mRecList.size() <= index)
  {
    JZFUNC_END_LOG();
    return eGrammarErrorFileEnd;
  }
  if (mRecList[index].word != "enum")
  {
    JZWRITE_DEBUG("get key word %s", mRecList[index].word.c_str());
    JZFUNC_END_LOG();
    return eGrammarErrorNotEnum;
  }
  lastIndex = index;
  JZFUNC_END_LOG();
  return handleEnumId(index + 1, lastIndex, curBlock);
}

uint32 GrammarAnalyzer::handleEnumId(int index, int& lastIndex, GrammarBlock* curBlock)
{
  if(mRecList.size() <= index)
  {
    return eGrammarErrorFileEnd;
  }
  //need to check the id
  if (false == isLegalVarIdentify(mRecList[index].word, curBlock))
  {
    return eGrammarErrorDoubleDefinedDataType;
  }
  lastIndex = index;
  string id = mRecList[index].word;
  EnumDefine *curEnum = new EnumDefine(id);

  uint32 ret = expect(";",index + 1);

  if (ret == eGrammarErrorNoError)
  {
    curEnum->setFather(curBlock);
    lastIndex = index;
    JZFUNC_END_LOG();
    return ret;
  }

  GrammarBlock* body = new GrammarBlock();
  curEnum->addBody(body);

  ret = handleLeftBrace(index + 1, lastIndex, body);
  if (ret == eGrammarErrorNoError)
  {
    curBlock->addDataTypeDefine(curEnum);
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
    return eGrammarErrorNotLeftBrace;
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
    if(expect(";", index + 1) == eGrammarErrorNoError)
    {
      lastIndex++;
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
    if(index - 1 < 0){
      JZFUNC_END_LOG();
      return eGrammarErrorUnknown;
    }
    if (mRecList[index - 1].word == ",")
    {
      return eGrammarErrorUnexpectedCommon;
    }
    return handleEnumFieldName(index + 1, lastIndex, curBlock);
  }

  if(true == isLegalVarIdentify(mRecList[index].word,curBlock))
  {
    string fieldName = mRecList[index].word;
    EnumDefine *define = dynamic_cast<EnumDefine*>(curBlock->getFather());
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

bool GrammarAnalyzer::isLegalVarIdentify(const string& id, GrammarBlock* curBlock)
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
      return false;
    }
  }
  return true;
}

uint32 GrammarAnalyzer::handleStatement(int index, int& lastIndex, GrammarBlock* curBlock)
{
  return eGrammarErrorNoError;
}

GrammarBlock* GrammarAnalyzer::getTopBlock()
{
  return &mTopBlock;
}
