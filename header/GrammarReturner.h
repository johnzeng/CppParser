#ifndef GRAMMARRETURNER_H
#define GRAMMARRETURNER_H

#include "JZCommonDefine.h"
using namespace std;

enum ReturnerType{
  eGrmTop,
  eDeclarationSeq,
  eDeclaration,
  eEnumeratorDefinition,
};

class GrammarReturnerBase
{
public:
  GrammarReturnerBase(int32 type, string key);
  ~GrammarReturnerBase();

  void addChild(GrammarReturnerBase* child);
private:
  int32 mRetType;
  string mKey;
  vector<GrammarReturnerBase*> mChildren;
};

#endif /* end of include guard: GRAMMARRETURNER_H */
