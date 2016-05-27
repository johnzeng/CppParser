#ifndef GRAMMARRETURNER_H
#define GRAMMARRETURNER_H

#include "JZCommonDefine.h"
using namespace std;

enum ReturnerType{
  eGrmTop,
  eDeclarationSeq,
  eDeclaration,
  eEnumeratorDefinition,
  eDeclarator,
  eSimpleTypeSpecifier,
  eTypeSpecifierSeq,
  eTypeId,
  eBlockDeclaration,
  eTrailingTypeSpecifier,
  eTypeName,
  eNestNameSpecifier,
  ePtrDeclarator,
  eCompoundStatement,
  eFunctionBody,
  eFunctionDefinition,
  eCVQualifierSeq,
  eSimpleDeclaration,
};

class GrammarReturnerBase
{
public:
  GrammarReturnerBase(int32 type, string key);
  ~GrammarReturnerBase();

  void addChild(GrammarReturnerBase* child);
  void mergeChild(GrammarReturnerBase* node);

  GrammarReturnerBase* getChild(int index) const;
  int32 getType() const;
  const string& getKey() const;

  void printAllChild(int tabNum);
private:

  void initMap();
  static map<int32, string> sNameMap;

  int32 mRetType;
  string mKey;
  vector<GrammarReturnerBase*> mChildren;
};

#endif /* end of include guard: GRAMMARRETURNER_H */
