#ifndef GRAMMARDATA_H
#define GRAMMARDATA_H

#include "LexData.h"

/*********************************************************
  All enum begin here 
 ********************************************************/

enum GrammarReturnCode
{
  eGrmErrNoError = 0,

  eGrmErrNotEnum = 1,
  eGrmErrNotLeftBrace = 2,
  eGrmErrNotExpected = 3,
  eGrmErrNotAttri = 4,
  eGrmErrNotStorageClassSpecifier = 5,
  eGrmErrNotFunctionSpecifier = 6,
  eGrmErrNotAccessSpecifier = 7,
  eGrmErrNotPure = 8,

  eGrmErrReasonableErrors = 99,

  eGrmErrDoubleDefinedDataType = 100,
  eGrmErrDoubleDefinedVar = 101,

  eGrmErrFileEnd = 1000,
  eGrmErrMissingSemicolon = 1001,
  eGrmErrUnexpectedCommon = 1002,
  eGrmErrNotCVQualifier = 1003,
  eGrmErrDoubleCVQualifier = 1004,
  eGrmErrNotUnaryOperator = 1005,
  eGrmErrExpectNextDblSqBracket = 1006,
  eGrmErrUnexpDblSqlBracket = 1007,
  eGrmErrUnexpRightBracket = 1008,

  eGrmErrUnknown,
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

enum GrammarState
{
  eGramIsNothing,
  eGramIsConst,
  eGramIsVolatile,
  eGramIsStar,
  eGramIsAnd,
  eGramIsPlus,
  eGramIsMinus,
  eGramIsBone,
  eGramIsWave,
  eGramIsAuto,
  eGramIsRegister,
  eGramIsStatic,
  eGramIsThreadLocal,
  eGramIsExtern,
  eGramIsMutable,
  eGramIsInline,
  eGramIsVirtual,
  eGramIsExplicit,
  eGramIsPrivate,
  eGramIsPublic,
  eGramIsProtected,
  eGramIsPureSpecifier,
};

enum EnumType{
  eEnumBasic,
  eEnumClass,
  eEnumStruct,
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
  GrammarNode* getFather(){return mFather;}
  uint32 getType()const{return mNodeType;}

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

  VarDefine* getVarDefInBlock(const string& key);

  VarDefine* getVarDef(const string& key);
  DataTypeDefine* getDataDef(const string& key);

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

  uint32 addField(const string& id);
  uint32 addField(const string& id, int value);

  bool fieldExist(const string& id);
private:
  uint32 mEnumType;
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
