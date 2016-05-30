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
  eVirtSpecifier,
  eTypeId,
  eAssignmentExpression,
  eExpression,
  eDecltypeSpecifier,
  eEnumName,
  eTypedefName,
  eNamespaceAlias,
  eNamespaceName,
  eNonPtrDeclarator,
  eMemberSpecification,
  eClassHead,
  eClassSpecifier,
  eTypeSpecifier,
  eLinkageSpecification,
  eEnumKey,
  eClassKey,
  eLiteral,
  eExternLiteral,
  eOverloadableOperator,
  eAssignmentOperator,
  ePureSpecifier,
  eRefQualifier,
  eFunctionSpecifier,
  eStorageClassSpecifier,
  eCVQualifier,
  eUnaryOperator,
  eAccessSpecifier,
  eDeclSpecifierSeq,
  eDeclSpecifier,
  eUnqualifiedId,
  eQualifiedId,
  eBlockDeclaration,
  eTrailingTypeSpecifier,
  eTypeName,
  eIdentifier,
  eClassName,
  eClassHeadName,
  eSimpleTemplateId,
  eTemplateName,
  eLiteralOperatorId,
  eIdExpression,
  eDeclaratorId,
  ePtrOperator,
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
