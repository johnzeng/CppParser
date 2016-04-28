#ifndef GRAMMARANALYZER_H
#define GRAMMARANALYZER_H

#include "LexData.h"

enum DataTypeEnum{
  eDataTypeUnknow,
  eDataTypeStruct,
  eDataTypeEnum,
  eDataTypeClass,
  eDataTypeUnion,
  eDataTypePtr
}
class DataTypeDefine {
public:
  DataTypeDefine (string word);
  virtual ~DataTypeDefine ();

private:
  string mWrod;
  uint32 mDateType;
};

class GrammarNode {
public:
  GrammarNode(GrammarNode* father);
  virtual ~GrammarData ();

private:

};

class GrammarAnalyzer {
public:
  GrammarAnalyzer (LexRecList recList);
  virtual ~GrammarAnalyzer ();

private:
  LexRecList mRecList;

  
};

#endif /* end of include guard: GRAMMARANALYZER_H */
