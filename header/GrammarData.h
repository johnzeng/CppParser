#ifndef GRAMMARDATA_H
#define GRAMMARDATA_H

#include "LexData.h"

/*********************************************************
  All enum begin here 
 ********************************************************/

enum GrammarReturnCode
{
  eGrammarErrorNoError = 0,

  eGrammarErrorNotEnum = 1,
  eGrammarErrorNotLeftBrace = 2,
  eGrammarErrorNotExpected = 3,

  eGrammarErrorReasonableErrors = 99,

  eGrammarErrorDoubleDefinedDataType = 100,
  eGrammarErrorDoubleDefinedVar = 101,

  eGrammarErrorFileEnd = 1000,
  eGrammarErrorMissingSemicolon = 1001,

  eGrammarErrorUnknown,
};

enum GrammarNodeType
{
  eGrammarNodeTopNode,   //this node is going to be the top class node in grammar analyzer

  eGrammarNodeBlock,
  eGrammarNodeDataTypeDefine,
  eGrammarNodeVarDefine,
  eGrammarNodeStatment,

  eDataTypeUnknow,
  eDataTypeBasic,
  eDataTypeStruct,
  eDataTypeEnum,
  eDataTypeClass,
  eDataTypeUnion,
  eDataTypeFunc,
  eDataTypePtr,

  eGrammarBlockTop,
  eGrammarBlockFunc,
  eGrammarBlockWhile,
  eGrammarBlockFor,
  eGrammarBlockIf,
  eGrammarBlockElse,
};

/*********************************************************
  All class define start here 
 ********************************************************/

class GrammarNode;
class DataTypeDefine;
class VarDefine;
class GrammarBlock;
class ClassDefine;
class EnumDefine;
class StructDefine;
class FunctionDefine;

class GrammarNode {
public:
  GrammarNode();
  virtual ~GrammarNode();

  void setFather(GrammarNode* father);
  const GrammarNode* getFather(){return mFather;}
  uint32 getType(){return mNodeType;}

protected:
  GrammarNode *mFather;
  vector<GrammarNode*> mChildrens;
  uint32 mNodeType;
};

class DataTypeDefine:public GrammarNode {
public:
  DataTypeDefine ();
  virtual ~DataTypeDefine ();

  string getSignature();
  string getKeyWord(int i);

  uint32 addBody(GrammarBlock* body);
protected:
  vector<string> mKeyWords;
  string mSignature;
  GrammarBlock* mBody;
};

class VarDefine:public GrammarNode {
public:
  VarDefine(){};
  uint32 init (string id, DataTypeDefine* define);
  virtual ~VarDefine ();

  string getId();

private:
  DataTypeDefine* mDataType;
  string mIdentify;
  bool mIsStatic;
  bool mIsVirtual;
  bool mIsConst;
};

class GrammarBlock: public GrammarNode {
public:
  GrammarBlock ();
  virtual ~GrammarBlock ();

  static GrammarBlock createTopNode();

  uint32 addDataTypeDefine(DataTypeDefine* dataType);
  uint32 addVarDefine(VarDefine* var);

  const VarDefine* getVarDef(const string key);
private:
  map<string,DataTypeDefine*> mDataTypeList;
  map<string,VarDefine*> mVarList;
};

class BasicDefine:public DataTypeDefine {
public:
  BasicDefine (vector<string> keyWords);
  virtual ~BasicDefine ();

private:
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
};

class StructDefine: public DataTypeDefine {
public:
  StructDefine ();
  virtual ~StructDefine ();

private:
  map<string,DataTypeDefine*> mFields;
};

class EnumDefine: public DataTypeDefine{
public:
  EnumDefine (string id);
  virtual ~EnumDefine ();

  uint32 addField(string id);
  utin32 addField(string id, int value);

private:
  map<string, int> mFields;
};

class UnionDefine:public DataTypeDefine {
public:
  UnionDefine ();
  virtual ~UnionDefine ();

private:
  map<string,DataTypeDefine*> mFields;
};


#endif /* end of include guard: GRAMMARDATA_H */
