#ifndef GRAMMARANALYZER_H
#define GRAMMARANALYZER_H

#include "LexData.h"
#include "GrammarData.h"
class GrammarAnalyzer {
public:
  GrammarAnalyzer (LexRecList recList);
  virtual ~GrammarAnalyzer ();

  uint32 doAnalyze();
protected:
  uint32 blockHeartBeat(int32 index,int32& lastIndex, GrammarBlock& curBlock);

  uint32 handleEnum(int index, int& lastIndex, GrammarBlock& curBlock);
  uint32 handleEnumId(int, index, int& lastIndex, GrammarBlock &curBlock);
  uint32 handleEnumFieldName(int index, int& lastIndex, GrammarBlock& curBlock);
  uint32 handleEnumFieldEqual(int index, int& lastIndex, GrammarBlock& curBlock);
  uint32 handleEnumFieldValue(int index, int& lastIndex, GrammarBlock& curBlock);

  //they should be some common method, but I don't sure how to carry the info from invoker now.
  uint32 handleSemicolon(int index, int& lastIndex, GrammarBlock& curBlock);
  uint32 handleLeftBrace(int index, int& lastIndex, GrammarBlock& curBlock);
  uint32 handleRightBrace(int index, int& lastIndex, GrammarBlock& curBlock);
private:
  LexRecList mRecList;

  GrammarBlock mTopBlock;
};

#endif /* end of include guard: GRAMMARANALYZER_H */
