#include "GrammarReturner.h"
#include "JZMacroFunc.h"

map<int32, string> GrammarReturnerBase::sNameMap;

void GrammarReturnerBase::initMap()
{
  sNameMap[eGrmTop] = "GrmTop";
  sNameMap[eTypeId] = "TypeId";
  sNameMap[eCapture] = "Capture";
  sNameMap[eCaptureDefault] = "CaptureDefault";
  sNameMap[eCaptureList] = "CaptureList";
  sNameMap[eTypeIdList] = "TypeIdList";
  sNameMap[eNoexceptSpecification] = "NoexceptSpecification";
  sNameMap[eDynamicExceptionSpecification] = "DynamicExceptionSpecification";
  sNameMap[eExceptionSpecification] = "ExceptionSpecification";
  sNameMap[eTrailingReturnType] = "TrailingReturnType";
  sNameMap[eTrailingTypeSpecifierSeq] = "TrailingTypeSpecifierSeq";
  sNameMap[eFunctionTryBlock] = "FunctionTryBlock";
  sNameMap[eExceptionDeclaration] = "ExceptionDeclaration";
  sNameMap[eAliasDeclaration] = "AliasDeclaration";
  sNameMap[eExplicitInstantiation] = "ExplicitInstantiation";
  sNameMap[eHandlerSeq] = "HandlerSeq";
  sNameMap[eHandler] = "Handler";
  sNameMap[eTryBlock] = "TryBlock";
  sNameMap[eUsingDirective] = "UsingDirective";
  sNameMap[eStatic_assertDeclaration] = "Static_assertDeclaration";
  sNameMap[eExplicitSpecification] = "ExplicitSpecification";
  sNameMap[eTemplateParameter] = "TemplateParameter";
  sNameMap[eTypeParameter] = "TypeParameter";
  sNameMap[eTemplateParameterList] = "TemplateParameterList";
  sNameMap[eTemplateDeclaration] = "TemplateDeclaration";
  sNameMap[eNoptrNewDeclarator] = "NoptrNewDeclarator";
  sNameMap[eNewInitializer] = "NewInitializer";
  sNameMap[eDeclarationStatement] = "DeclarationStatement";
  sNameMap[eNewDeclarator] = "NewDeclarator";
  sNameMap[eNewTypeId] = "NewTypeId";
  sNameMap[eNewExpression] = "NewExpression";
  sNameMap[eNewPlacement] = "NewPlacement";
  sNameMap[eForRangeInitializer] = "ForRangeInitializer";
  sNameMap[eForRangeDeclaration] = "ForRangeDeclaration";
  sNameMap[eForInitStatement] = "ForInitStatement";
  sNameMap[eIterationStatement] = "IterationStatement";
  sNameMap[eCondition] = "Condition";
  sNameMap[eSelectionStatement] = "SelectionStatement";
  sNameMap[eJumpStatement] = "JumpStatement";
  sNameMap[eElaboratedTypeSpecifier] = "ElaboratedTypeSpecifier";
  sNameMap[eLabeledStatement] = "LabeledStatement";
  sNameMap[eVirtSpecifierSeq] = "VirtSpecifierSeq";
  sNameMap[eMemberDeclarator] = "MemberDeclarator";
  sNameMap[eMemberDeclaratorList] = "MemberDeclaratorList";
  sNameMap[eMemberDeclaration] = "MemberDeclaration";
  sNameMap[eMemInitializer] = "MemInitializer";
  sNameMap[eMemInitializerList] = "MemInitializerList";
  sNameMap[eMemInitializerId] = "MemInitializerId";
  sNameMap[eAttibuteDeclaration] = "AttibuteDeclaration";
  sNameMap[eCtorInitializer] = "CtorInitializer";
  sNameMap[eUsingDeclaration] = "UsingDeclaration";
  sNameMap[eAsmDeclaration] = "AsmDeclaration";
  sNameMap[eUnnamedNamespaceDefinition] = "UnnamedNamespaceDefinition";
  sNameMap[eExtensionNamespaceDefinition] = "ExtensionNamespaceDefinition";
  sNameMap[eNamespaceBody] = "NamespaceBody";
  sNameMap[eOriginalNamespaceName] = "OriginalNamespaceName";
  sNameMap[eOriginalNamespaceDefinition] = "OriginalNamespaceDefinition";
  sNameMap[eNamedNamespaceDefinition] = "NamedNamespaceDefinition";
  sNameMap[eNamespaceDefinition] = "NamespaceDefinition";
  sNameMap[eQualifiedNamespaceSpecifier] = "QualifiedNamespaceSpecifier";
  sNameMap[eNamespaceAliasDefinition] = "NamespaceAliasDefinition";
  sNameMap[eOpaqueEnumDeclaration] = "OpaqueEnumDeclaration";
  sNameMap[eBraceOrEqualInitializer] = "BraceOrEqualInitializer";
  sNameMap[eInitializer] = "Initializer";
  sNameMap[eEmptyDeclaration] = "EmptyDeclaration";
  sNameMap[eInitDeclaratorList] = "InitDeclaratorList";
  sNameMap[eInitDeclarator] = "InitDeclarator";
  sNameMap[eClassOrDecltype] = "ClassOrDecltype";
  sNameMap[eBaseSpecifier] = "BaseSpecifier";
  sNameMap[eBaseTypeSpecifier] = "BaseTypeSpecifier";
  sNameMap[eBaseSpecifierList] = "BaseSpecifierList";
  sNameMap[eClassVirtSpecifierSeq] = "ClassVirtSpecifierSeq";
  sNameMap[eClassVirtSpecifier] = "ClassVirtSpecifier";
  sNameMap[eBaseClause] = "BaseClause";
  sNameMap[eEnumSpecifier] = "EnumSpecifier";
  sNameMap[eEnumerator] = "Enumerator";
  sNameMap[eEnumeratorList] = "EnumeratorList";
  sNameMap[eEnumBase] = "EnumBase";
  sNameMap[eEnumHead] = "EnumHead";
  sNameMap[eStatement] = "Statement";
  sNameMap[eExpressionStatement] = "ExpressionStatement";
  sNameMap[eParameterDeclaration] = "ParameterDeclaration";
  sNameMap[eLambdaDeclarator] = "LambdaDeclarator";
  sNameMap[eLambdaIntroducer] = "LambdaIntroducer";
  sNameMap[eLambdaExpression] = "LambdaExpression";
  sNameMap[eLambdaCapture] = "LambdaCapture";
  sNameMap[eParameterDeclarationClause] = "eParameterDeclarationClause";
  sNameMap[eParameterDeclarationList] = "eParameterDeclarationList";
  sNameMap[eParametersAndQualifiers] = "ParametersAndQualifiers";
  sNameMap[eStatementSeq] = "StatementSeq";
  sNameMap[eTemplateArgument] = "TemplateArgument";
  sNameMap[eTemplateArgumentList] = "TemplateArgumentList";
  sNameMap[eTemplateId] = "TemplateId";
  sNameMap[eOperatorFunctionId] = "OperatorFunctionId";
  sNameMap[eConversionDeclarator] = "ConversionDeclarator";
  sNameMap[eThrowExpression] = "ThrowExpression";
  sNameMap[eConversionFunctionId] = "ConversionFunctionId";
  sNameMap[eConversionTypeId] = "ConversionTypeId";
  sNameMap[eInitializerClause] = "InitializerClause";
  sNameMap[eDeleteExpression] = "DeleteExpression";
  sNameMap[ePrimaryExpression] = "PrimaryExpression";
  sNameMap[eInitializerList] = "InitializerList";
  sNameMap[eExpressionList] = "ExpressionList";
  sNameMap[eBracedInitList] = "BracedInitList";
  sNameMap[eTypenameSpecifier] = "TypenameSpecifier";
  sNameMap[ePseudoDestructorName] = "PseudoDestructorName";
  sNameMap[eNoexceptExpression] = "NoexceptExpression";
  sNameMap[ePostfixExpression] = "PostfixExpression";
  sNameMap[eUnaryExpression] = "UnaryExpression";
  sNameMap[eConstantExpression] = "ConstantExpression";
  sNameMap[eNoptrAbstractDeclarator] = "NoptrAbstractDeclarator";
  sNameMap[ePtrAbstractDeclarator] = "PtrAbstractDeclarator";
  sNameMap[eAbstractDeclarator] = "AbstractDeclarator";
  sNameMap[eCastExpression] = "CastExpression";
  sNameMap[ePmExpression] = "PmExpression";
  sNameMap[eMultiplicativeExpression] = "MultiplicativeExpression";
  sNameMap[eAdditiveExpression] = "AdditiveExpression";
  sNameMap[eShiftExpression] = "ShiftExpression";
  sNameMap[eRelationalExpression] = "RelationalExpression";
  sNameMap[eExclusiveOrExpression] = "ExclusiveOrExpression";
  sNameMap[eInclusiveOrExpression] = "InclusiveOrExpression";
  sNameMap[eLogicalOrExpression] = "LogicalOrExpression";
  sNameMap[eLogicalAndExpression] = "LogicalAndExpression";
  sNameMap[eEqualityExpression] = "EqualityExpression";
  sNameMap[eAndExpression] = "AndExpression";
  sNameMap[eConditionalExpression] = "ConditionalExpression";
  sNameMap[eAssignmentExpression] = "AssignmentExpression";
  sNameMap[eExpression] = "Expression";
  sNameMap[eDecltypeSpecifier] = "DecltypeSpecifier";
  sNameMap[eEnumName] = "EnumName";
  sNameMap[eTypedefName] = "TypedefName";
  sNameMap[eNamespaceAlias] = "NamespaceAlias";
  sNameMap[eNamespaceName] = "NamespaceName";
  sNameMap[eNonPtrDeclarator] = "NonPtrDeclarator";
  sNameMap[eMemberSpecification] = "MemberSpecification";
  sNameMap[eClassHead] = "ClassHead";
  sNameMap[eClassSpecifier] = "ClassSpecifier";
  sNameMap[eTypeSpecifier] = "TypeSpecifier";
  sNameMap[eLinkageSpecification] = "LinkageSpecification";
  sNameMap[eLiteral] = "Literal";
  sNameMap[eExternLiteral] = "ExternLiteral";
  sNameMap[eClassKey] = "ClassKey";
  sNameMap[eEnumKey] = "EnumKey";
  sNameMap[eOverloadableOperator] = "OverloadableOperator";
  sNameMap[eAssignmentOperator] = "AssignmentOperator";
  sNameMap[eVirtSpecifier] = "VirtSpecifier";
  sNameMap[ePureSpecifier] = "PureSpecifier";
  sNameMap[eAccessSpecifier] = "AccessSpecifier";
  sNameMap[eUnaryOperator] = "UnaryOperator";
  sNameMap[eCVQualifier] = "CVQualifier";
  sNameMap[eStorageClassSpecifier] = "StorageClassSpecifier";
  sNameMap[eFunctionSpecifier] = "FunctionSpecifier";
  sNameMap[eRefQualifier] = "RefQualifier";
  sNameMap[eDeclSpecifierSeq] = "DeclSpecifierSeq";
  sNameMap[eDeclSpecifier] = "DeclSpecifier";
  sNameMap[eUnqualifiedId] = "UnqualifiedId";
  sNameMap[eQualifiedId] = "QualifiedId";
  sNameMap[eBlockDeclaration] = "BlockDeclaration";
  sNameMap[eTrailingTypeSpecifier] = "TrailingTypeSpecifier";
  sNameMap[eTypeName] = "TypeName";
  sNameMap[eIdentifier] = "Identifier";
  sNameMap[eSimpleTemplateId] = "SimpleTemplateId";
  sNameMap[eClassHeadName] = "ClassHeadName";
  sNameMap[eClassName] = "ClassName";
  sNameMap[eTemplateName] = "TemplateName";
  sNameMap[eLiteralOperatorId] = "LiteralOperatorId";
  sNameMap[eIdExpression] = "IdExpression";
  sNameMap[eDeclaratorId] = "DeclaratorId";
  sNameMap[ePtrOperator] = "PtrOperator";
  sNameMap[eNestNameSpecifier] = "NestNameSpecifier";
  sNameMap[ePtrDeclarator] = "PtrDeclarator";
  sNameMap[eCompoundStatement] = "CompoundStatement";
  sNameMap[eFunctionBody] = "FunctionBody";
  sNameMap[eFunctionDefinition] = "FunctionDefinition";
  sNameMap[eCVQualifierSeq] = "CVQualifierSeq";
  sNameMap[eSimpleDeclaration] = "SimpleDeclaration";
  sNameMap[eDeclarationSeq] = "DeclarationSeq";
  sNameMap[eDeclaration] = "Declaration";
  sNameMap[eEnumeratorDefinition] = "EnumeratorDefinition";
  sNameMap[eDeclarator] = "Declarator";
  sNameMap[eSimpleTypeSpecifier] = "SimpleTypeSpecifier";
  sNameMap[eTypeSpecifierSeq] = "TypeSpecifierSeq";

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
