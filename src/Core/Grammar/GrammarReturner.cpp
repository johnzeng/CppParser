#include "GrammarReturner.h"
#include "JZMacroFunc.h"

map<int32, string> GrammarReturnerBase::sNameMap;

void GrammarReturnerBase::initMap()
{
  sNameMap[eGrmTop] = "eGrmTop";
  sNameMap[eTypeId] = "eTypeId";
  sNameMap[eCapture] = "eCapture";
  sNameMap[eCaptureDefault] = "eCaptureDefault";
  sNameMap[eCaptureList] = "eCaptureList";
  sNameMap[eTypeIdList] = "eTypeIdList";
  sNameMap[eNoexceptSpecification] = "eNoexceptSpecification";
  sNameMap[eDynamicExceptionSpecification] = "eDynamicExceptionSpecification";
  sNameMap[eExceptionSpecification] = "eExceptionSpecification";
  sNameMap[eTrailingReturnType] = "eTrailingReturnType";
  sNameMap[eTrailingTypeSpecifierSeq] = "eTrailingTypeSpecifierSeq";
  sNameMap[eFunctionTryBlock] = "eFunctionTryBlock";
  sNameMap[eExceptionDeclaration] = "eExceptionDeclaration";
  sNameMap[eAliasDeclaration] = "eAliasDeclaration";
  sNameMap[eExplicitInstantiation] = "eExplicitInstantiation";
  sNameMap[eHandlerSeq] = "eHandlerSeq";
  sNameMap[eHandler] = "eHandler";
  sNameMap[eTryBlock] = "eTryBlock";
  sNameMap[eUsingDirective] = "eUsingDirective";
  sNameMap[eStatic_assertDeclaration] = "eStatic_assertDeclaration";
  sNameMap[eExplicitSpecification] = "eExplicitSpecification";
  sNameMap[eTemplateParameter] = "eTemplateParameter";
  sNameMap[eTypeParameter] = "eTypeParameter";
  sNameMap[eTemplateParameterList] = "eTemplateParameterList";
  sNameMap[eTemplateDeclaration] = "eTemplateDeclaration";
  sNameMap[eNoptrNewDeclarator] = "eNoptrNewDeclarator";
  sNameMap[eNewInitializer] = "eNewInitializer";
  sNameMap[eDeclarationStatement] = "eDeclarationStatement";
  sNameMap[eNewDeclarator] = "eNewDeclarator";
  sNameMap[eNewTypeId] = "eNewTypeId";
  sNameMap[eNewExpression] = "eNewExpression";
  sNameMap[eNewPlacement] = "eNewPlacement";
  sNameMap[eForRangeInitializer] = "eForRangeInitializer";
  sNameMap[eForRangeDeclaration] = "eForRangeDeclaration";
  sNameMap[eForInitStatement] = "eForInitStatement";
  sNameMap[eIterationStatement] = "eIterationStatement";
  sNameMap[eCondition] = "eCondition";
  sNameMap[eSelectionStatement] = "eSelectionStatement";
  sNameMap[eJumpStatement] = "eJumpStatement";
  sNameMap[eElaboratedTypeSpecifier] = "eElaboratedTypeSpecifier";
  sNameMap[eLabeledStatement] = "eLabeledStatement";
  sNameMap[eVirtSpecifierSeq] = "eVirtSpecifierSeq";
  sNameMap[eMemberDeclarator] = "eMemberDeclarator";
  sNameMap[eMemberDeclaratorList] = "eMemberDeclaratorList";
  sNameMap[eMemberDeclaration] = "eMemberDeclaration";
  sNameMap[eMemInitializer] = "eMemInitializer";
  sNameMap[eMemInitializerList] = "eMemInitializerList";
  sNameMap[eMemInitializerId] = "eMemInitializerId";
  sNameMap[eAttibuteDeclaration] = "eAttibuteDeclaration";
  sNameMap[eCtorInitializer] = "eCtorInitializer";
  sNameMap[eUsingDeclaration] = "eUsingDeclaration";
  sNameMap[eAsmDeclaration] = "eAsmDeclaration";
  sNameMap[eUnnamedNamespaceDefinition] = "eUnnamedNamespaceDefinition";
  sNameMap[eExtensionNamespaceDefinition] = "eExtensionNamespaceDefinition";
  sNameMap[eNamespaceBody] = "eNamespaceBody";
  sNameMap[eOriginalNamespaceName] = "eOriginalNamespaceName";
  sNameMap[eOriginalNamespaceDefinition] = "eOriginalNamespaceDefinition";
  sNameMap[eNamedNamespaceDefinition] = "eNamedNamespaceDefinition";
  sNameMap[eNamespaceDefinition] = "eNamespaceDefinition";
  sNameMap[eQualifiedNamespaceSpecifier] = "eQualifiedNamespaceSpecifier";
  sNameMap[eNamespaceAliasDefinition] = "eNamespaceAliasDefinition";
  sNameMap[eOpaqueEnumDeclaration] = "eOpaqueEnumDeclaration";
  sNameMap[eBraceOrEqualInitializer] = "eBraceOrEqualInitializer";
  sNameMap[eInitializer] = "eInitializer";
  sNameMap[eEmptyDeclaration] = "eEmptyDeclaration";
  sNameMap[eInitDeclaratorList] = "eInitDeclaratorList";
  sNameMap[eInitDeclarator] = "eInitDeclarator";
  sNameMap[eClassOrDecltype] = "eClassOrDecltype";
  sNameMap[eBaseSpecifier] = "eBaseSpecifier";
  sNameMap[eBaseTypeSpecifier] = "eBaseTypeSpecifier";
  sNameMap[eBaseSpecifierList] = "eBaseSpecifierList";
  sNameMap[eClassVirtSpecifierSeq] = "eClassVirtSpecifierSeq";
  sNameMap[eClassVirtSpecifier] = "eClassVirtSpecifier";
  sNameMap[eBaseClause] = "eBaseClause";
  sNameMap[eEnumSpecifier] = "eEnumSpecifier";
  sNameMap[eEnumerator] = "eEnumerator";
  sNameMap[eEnumeratorList] = "eEnumeratorList";
  sNameMap[eEnumBase] = "eEnumBase";
  sNameMap[eEnumHead] = "eEnumHead";
  sNameMap[eStatement] = "eStatement";
  sNameMap[eExpressionStatement] = "eExpressionStatement";
  sNameMap[eParameterDeclaration] = "eParameterDeclaration";
  sNameMap[eLambdaDeclarator] = "eLambdaDeclarator";
  sNameMap[eLambdaIntroducer] = "eLambdaIntroducer";
  sNameMap[eLambdaExpression] = "eLambdaExpression";
  sNameMap[eLambdaCapture] = "eLambdaCapture";
  sNameMap[eParameterDeclarationClause] = "eeParameterDeclarationClause";
  sNameMap[eParameterDeclarationList] = "eeParameterDeclarationList";
  sNameMap[eParametersAndQualifiers] = "eParametersAndQualifiers";
  sNameMap[eStatementSeq] = "eStatementSeq";
  sNameMap[eTemplateArgument] = "eTemplateArgument";
  sNameMap[eTemplateArgumentList] = "eTemplateArgumentList";
  sNameMap[eTemplateId] = "eTemplateId";
  sNameMap[eOperatorFunctionId] = "eOperatorFunctionId";
  sNameMap[eConversionDeclarator] = "eConversionDeclarator";
  sNameMap[eThrowExpression] = "eThrowExpression";
  sNameMap[eConversionFunctionId] = "eConversionFunctionId";
  sNameMap[eConversionTypeId] = "eConversionTypeId";
  sNameMap[eInitializerClause] = "eInitializerClause";
  sNameMap[eDeleteExpression] = "eDeleteExpression";
  sNameMap[ePrimaryExpression] = "ePrimaryExpression";
  sNameMap[eInitializerList] = "eInitializerList";
  sNameMap[eExpressionList] = "eExpressionList";
  sNameMap[eBracedInitList] = "eBracedInitList";
  sNameMap[eTypenameSpecifier] = "eTypenameSpecifier";
  sNameMap[ePseudoDestructorName] = "ePseudoDestructorName";
  sNameMap[eNoexceptExpression] = "eNoexceptExpression";
  sNameMap[ePostfixExpression] = "ePostfixExpression";
  sNameMap[eUnaryExpression] = "eUnaryExpression";
  sNameMap[eConstantExpression] = "eConstantExpression";
  sNameMap[eNoptrAbstractDeclarator] = "eNoptrAbstractDeclarator";
  sNameMap[ePtrAbstractDeclarator] = "ePtrAbstractDeclarator";
  sNameMap[eAbstractDeclarator] = "eAbstractDeclarator";
  sNameMap[eCastExpression] = "eCastExpression";
  sNameMap[ePmExpression] = "ePmExpression";
  sNameMap[eMultiplicativeExpression] = "eMultiplicativeExpression";
  sNameMap[eAdditiveExpression] = "eAdditiveExpression";
  sNameMap[eShiftExpression] = "eShiftExpression";
  sNameMap[eRelationalExpression] = "eRelationalExpression";
  sNameMap[eExclusiveOrExpression] = "eExclusiveOrExpression";
  sNameMap[eInclusiveOrExpression] = "eInclusiveOrExpression";
  sNameMap[eLogicalOrExpression] = "eLogicalOrExpression";
  sNameMap[eLogicalAndExpression] = "eLogicalAndExpression";
  sNameMap[eEqualityExpression] = "eEqualityExpression";
  sNameMap[eAndExpression] = "eAndExpression";
  sNameMap[eConditionalExpression] = "eConditionalExpression";
  sNameMap[eAssignmentExpression] = "eAssignmentExpression";
  sNameMap[eExpression] = "eExpression";
  sNameMap[eDecltypeSpecifier] = "eDecltypeSpecifier";
  sNameMap[eEnumName] = "eEnumName";
  sNameMap[eTypedefName] = "eTypedefName";
  sNameMap[eNamespaceAlias] = "eNamespaceAlias";
  sNameMap[eNamespaceName] = "eNamespaceName";
  sNameMap[eNonPtrDeclarator] = "eNonPtrDeclarator";
  sNameMap[eMemberSpecification] = "eMemberSpecification";
  sNameMap[eClassHead] = "eClassHead";
  sNameMap[eClassSpecifier] = "eClassSpecifier";
  sNameMap[eTypeSpecifier] = "eTypeSpecifier";
  sNameMap[eLinkageSpecification] = "eLinkageSpecification";
  sNameMap[eLiteral] = "eLiteral";
  sNameMap[eExternLiteral] = "eExternLiteral";
  sNameMap[eClassKey] = "eClassKey";
  sNameMap[eEnumKey] = "eEnumKey";
  sNameMap[eOverloadableOperator] = "eOverloadableOperator";
  sNameMap[eAssignmentOperator] = "eAssignmentOperator";
  sNameMap[eVirtSpecifier] = "eVirtSpecifier";
  sNameMap[ePureSpecifier] = "ePureSpecifier";
  sNameMap[eAccessSpecifier] = "eAccessSpecifier";
  sNameMap[eUnaryOperator] = "eUnaryOperator";
  sNameMap[eCVQualifier] = "eCVQualifier";
  sNameMap[eStorageClassSpecifier] = "eStorageClassSpecifier";
  sNameMap[eFunctionSpecifier] = "eFunctionSpecifier";
  sNameMap[eRefQualifier] = "eRefQualifier";
  sNameMap[eDeclSpecifierSeq] = "eDeclSpecifierSeq";
  sNameMap[eDeclSpecifier] = "eDeclSpecifier";
  sNameMap[eUnqualifiedId] = "eUnqualifiedId";
  sNameMap[eQualifiedId] = "eQualifiedId";
  sNameMap[eBlockDeclaration] = "eBlockDeclaration";
  sNameMap[eTrailingTypeSpecifier] = "eTrailingTypeSpecifier";
  sNameMap[eTypeName] = "eTypeName";
  sNameMap[eIdentifier] = "eIdentifier";
  sNameMap[eSimpleTemplateId] = "eSimpleTemplateId";
  sNameMap[eClassHeadName] = "eClassHeadName";
  sNameMap[eClassName] = "eClassName";
  sNameMap[eTemplateName] = "eTemplateName";
  sNameMap[eLiteralOperatorId] = "eLiteralOperatorId";
  sNameMap[eIdExpression] = "eIdExpression";
  sNameMap[eDeclaratorId] = "eDeclaratorId";
  sNameMap[ePtrOperator] = "ePtrOperator";
  sNameMap[eNestNameSpecifier] = "eNestNameSpecifier";
  sNameMap[ePtrDeclarator] = "ePtrDeclarator";
  sNameMap[eCompoundStatement] = "eCompoundStatement";
  sNameMap[eFunctionBody] = "eFunctionBody";
  sNameMap[eFunctionDefinition] = "eFunctionDefinition";
  sNameMap[eCVQualifierSeq] = "eCVQualifierSeq";
  sNameMap[eSimpleDeclaration] = "eSimpleDeclaration";
  sNameMap[eDeclarationSeq] = "eDeclarationSeq";
  sNameMap[eDeclaration] = "eDeclaration";
  sNameMap[eEnumeratorDefinition] = "eEnumeratorDefinition";
  sNameMap[eDeclarator] = "eDeclarator";
  sNameMap[eSimpleTypeSpecifier] = "eSimpleTypeSpecifier";
  sNameMap[eTypeSpecifierSeq] = "eTypeSpecifierSeq";

}

GrammarReturnerBase::GrammarReturnerBase(int32 type, string key):
  mRetType(type),
  mKey(key)
{
}

GrammarReturnerBase::~GrammarReturnerBase()
{
  int i = 0;
  for (i = 0; i < mChildren.size(); i++) 
  {
    JZSAFE_DELETE(mChildren[i]);
  }
}

void GrammarReturnerBase::addChild(GrammarReturnerBase* child)
{
  if (NULL == child)
  {
    return;
  }
  mChildren.push_back(child);
}

void GrammarReturnerBase::printAllChild(int tabNum)
{
  if (0 == sNameMap.size())
  {
    initMap();
  }
  string toPrint = "";
  for (int i = 0; i < tabNum; i++) {
    toPrint += "|  ";
  }

  toPrint += sNameMap[mRetType];
  if (mKey != "")
  {
    toPrint += ":" + mKey;
  }
  
  printf("%s\n", toPrint.c_str());

  for (int i = 0; i < mChildren.size(); i++) {
    mChildren[i]->printAllChild(tabNum + 1);
  }
}

int32 GrammarReturnerBase::getType() const
{
  return mRetType;
}

const string& GrammarReturnerBase::getKey() const
{
  return mKey;
}

GrammarReturnerBase* GrammarReturnerBase::getChild(int32 index) const
{
  if (index < mChildren.size())
  {
    return mChildren[index];
  }
  else
  {
    return NULL;
  }
}

void GrammarReturnerBase::mergeChild(GrammarReturnerBase* node)
{
  for (int i = 0; i < node->mChildren.size(); i++) {
    addChild(node->getChild(i));
  }
  node->mChildren.clear();
}
