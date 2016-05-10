#ifndef GRAMMARANALYZER_H
#define GRAMMARANALYZER_H

#include "LexData.h"
#include "GrammarData.h"
class GrammarAnalyzer {
public:
  GrammarAnalyzer (LexRecList recList);
  virtual ~GrammarAnalyzer ();

  uint32 doAnalyze();

  GrammarBlock *getTopBlock();
protected:
  uint32 blockHeartBeat(int32 index,int32& lastIndex, GrammarBlock* curBlock);

  uint32 handleEnum(int index, int& lastIndex, GrammarBlock* curBlock);
  uint32 handleEnumId(int index, int& lastIndex, GrammarBlock *curBlock);
  uint32 handleEnumFieldName(int index, int& lastIndex, GrammarBlock* curBlock);

  //they should be some common method, but I don't sure how to carry the info from invoker now.
  //they may be covered by expected..
//  uint32 handleSemicolon(int index, int& lastIndex, GrammarBlock* curBlock);
  uint32 handleLeftBrace(int index, int& lastIndex, GrammarBlock* curBlock);
//  uint32 handleRightBrace(int index, int& lastIndex, GrammarBlock* curBlock);

  uint32 expect(const string& expected,int index);

  bool isLegalVarIdentify(const string& id, GrammarBlock* curBlock);

  //return the statement's type; This will be a long check.... 
  uint32 handleStatement(int index, int& lastIndex, GrammarBlock* curBlock, DataTypeDefine& retDefine);

  uint32 handleStatementLeftBracket(int index, int& lastIndex, GrammarBlock* curBlock, DataTypeDefine& retDefine);
  uint32 expectAConstInt(int index, int& lastIndex, GrammarBlock* curBlock);
  uint32 expectAVar(int index, int& lastIndex, GrammarBlock* curBlock, DataTypeDefine& retDefine);
  uint32 getVarCalResult(const string& op,DataTypeDefine* lVar, DataTypeDefine* rVar, DataTypeDefine& retVar);

  uint32 handleCVQualifierSeq(int index, int& lastIndex, GrammarBlock* curBlock);
  uint32 getCVQualifier(int index, int& lastIndex, uint32 &ret);
private:
  LexRecList mRecList;

  GrammarBlock mTopBlock;
};

#endif /* end of include guard: GRAMMARANALYZER_H */
