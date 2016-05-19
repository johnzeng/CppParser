#include "LoopBreaker.h"
#include "StringUtil.h"

LoopBreaker::LoopBreaker(){}
LoopBreaker::~LoopBreaker(){}

bool LoopBreaker::insert(const string& file, int line, int index)
{
  if (false == check(file, line, index))
  {
    return false;
  }

  mLoopMark[getKey(file, line, index)] = 1;
  return true;
}

bool LoopBreaker::check(const string& file, int line, int index)
{
  auto it = mLoopMark.find(getKey(file,line, index)) ;
  if (mLoopMark.end() == it)
  {
    return true;
  }
  return it->second == 0;
}

void LoopBreaker::remomve(const string& file, int line, int index)
{
  mLoopMark[getKey(file, line, index)] = 0;
}

string LoopBreaker::getKey(const string& file, int line, int index)
{
  return file + ":" + StringUtil::tostr(line) + ":" + StringUtil::tostr(index);
}
