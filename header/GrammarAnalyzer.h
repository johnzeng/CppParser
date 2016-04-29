#ifndef GRAMMARANALYZER_H
#define GRAMMARANALYZER_H

#include "LexData.h"

enum GrammarNodeType
{
  eGrammarNodeTopNode,   //this node is going to be the top class node in grammar analyzer

  eGrammarNodeBlock,
  eGrammarNodeDataTypeDefine,
  eGrammarNodeVarDefine,
  eGrammarNodeStatment,
}

class GrammarNode {
public:
  GrammarNode(GrammarNode* father);
  virtual ~GrammarData ();

private:
  GrammarNode *mFather;
  vector<GrammarNode*> mChildrens;
  uint32 mNodeType;
};

enum DataTypeEnum{
  eDataTypeUnknow,
  eDataTypeStruct,
  eDataTypeEnum,
  eDataTypeClass,
  eDataTypeUnion,
  eDataTypePtr
}

class DataTypeDefine:public GrammarNode {
public:
  DataTypeDefine (string word);
  virtual ~DataTypeDefine ();

private:
  vector<string> mKeyWrods;
  string signature;
  uint32 mDateType;
};

class VarDefine:public GrammarNode {
public:
  VarDefine ();
  virtual ~VarDefine ();

private:
  DataTypeDefine* mDataType;
};

class ClassDefine: public DataTypeDefine{
public:
  ClassDefine(string word);
  virtual ~ClassDefine();
private:
  vector<DataTypeDefine*> mFields;
}

class StructDefine: public DataTypeDefine {
public:
  StructDefine ();
  virtual ~StructDefine ();

private:
  vector<DataTypeDefine*> mFields;
};

class EnumDefine: public DataTypeDefine{
public:
  EnumDefine ();
  virtual ~EnumDefine ();

private:
  vector<string> mFields;
};

class UnionDefine:public DataTypeDefine {
public:
  UnionDefine ();
  virtual ~UnionDefine ();

private:
  vector<DataTypeDefine*> mFields;
};


class GrammarAnalyzer {
public:
  GrammarAnalyzer (LexRecList recList);
  virtual ~GrammarAnalyzer ();

private:
  LexRecList mRecList;

  
};

#endif /* end of include guard: GRAMMARANALYZER_H */
