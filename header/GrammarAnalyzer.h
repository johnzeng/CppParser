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
  GrammarNode();
  virtual ~GrammarData ();


private:
  GrammarNode *mFather;
  vector<GrammarNode*> mChildrens;
  uint32 mNodeType;
};

enum DataTypeEnum{
  eDataTypeUnknow,
  eDataTypeNormal,
  eDataTypeStruct,
  eDataTypeEnum,
  eDataTypeClass,
  eDataTypeUnion,
  eDataTypeFunc,
  eDataTypePtr
};

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
  bool isStatic;
  bool isVirtual;
  bool isConst;
};

enum GrammarBlockType{
  eGrammarBlockTop,
  eGrammarBlockFunc,
  eGrammarBlockWhile,
  eGrammarBlockFor,
  eGrammarBlockIf,
  eGrammarBlockElse
};

class GrammarBlock: public GrammarNode {
public:
  GrammarBlock ();
  virtual ~GrammarBlock ();

  static GrammarBlock* getTopNode();

  uint32 addDataTypeDefine(string key, DataTypeDefine dataType);
  uint32 addVarDefine(string key, VarDefine var);
private:
  map<string,DataTypeDefine> mDataTypeList;
  map<string,VarDefine> mVarList;
};

class BasicDefine:public DataTypeDefine {
public:
  BasicDefine (vector<string> keyWords);
  virtual ~BasicDefine ();

private:
  vector<string> keyWords;
};

class PointerDefine:public DataTypeDefine {
public:
  PointerDefine ();
  virtual ~PointerDefine ();

private:
  DataTypeDefine* mPointerType;
  
};


class FunctionDefine: public DataTypeDefine {
public:
  FunctionDefine ();
  virtual ~FunctionDefine ();

private:
  vector<DataTypeDefine*> inputs;
  DataTypeDefine* output;
};


class ClassDefine: public DataTypeDefine{
public:
  ClassDefine(string word);
  virtual ~ClassDefine();
private:
  map<string,DataTypeDefine*> mFields;
}

class StructDefine: public DataTypeDefine {
public:
  StructDefine ();
  virtual ~StructDefine ();

private:
  map<string,DataTypeDefine*> mFields;
};

class EnumDefine: public DataTypeDefine{
public:
  EnumDefine ();
  virtual ~EnumDefine ();

private:
  map<string, value> mFields;
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
