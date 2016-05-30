#ifndef GRAMMARANALYZER_H
#define GRAMMARANALYZER_H

#include "LexData.h"
#include "GrammarData.h"
#include "LoopBreaker.h"

class GrammarReturnerBase;


class GrammarAnalyzer {
public:
  GrammarAnalyzer (LexRecList recList);
  virtual ~GrammarAnalyzer ();

  uint32 doAnalyze();

  GrammarBlock *getTopBlock();
protected:

  uint32 handleLiteralOperatorId(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleCaptureList(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleCapture(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleCaptureDefault(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleLambdaIntroducer(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleLambdaCapture(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleLambdaExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleLambdaDeclarator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleDynamicExceptionSpecification(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleNoexceptSpecification(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleExceptionDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleHandler(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleHandlerSeq(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleTypeParameter(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleNewPlacement(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleTemplateParameterList(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleTemplateParameter(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleNewTypeId(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleTypeIdList(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleNewDeclarator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleNoptrNewDeclarator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleNewInitializer(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleForInitStatement(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleForRangeDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleForRangeInitializer(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleCondition(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleElaboratedTypeSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleMemberDeclarator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleMemberDeclaratorList(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleMemberDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleBraceOrEqualInitializer(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleExceptionSpecification(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleParameterDeclarationList(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleParameterDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleStatement(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleParameterDeclarationClause(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleStatementSeq(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleFunctionTryBlock(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleCtorInitializer(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleMemInitializerList(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleMemInitializer(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleMemInitializerId(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleCompoundStatement(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleIdentifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleNamespaceName(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleOriginalNamespaceName(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleNamespaceAlias(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleTypeName(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleClassName(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleEnumName(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleTypedefName(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleSimpleTemplateId(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleTemplateId(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleDeclarator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleDeclaratorId(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleIdExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleUnqualifiedId(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleQualifiedId(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleDeclSpecifierSeq(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleDeclSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleFunctionBody(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handlePtrDeclarator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleNonPtrDeclarator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleParametersAndQualifiers(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleTrailingReturnType(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handlePtrOperator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleNestNameSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleOperatorFunctionId(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleConversionFunctionId(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleDecltypeSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleAssignmentExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleTypeSpecifierSeq(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleAbstractDeclarator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handlePtrAbstractDeclarator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleNoptrAbstractDeclarator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleConstantExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handlePrimaryExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleExpressionList(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleBracedInitList(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handlePseudoDestructorName(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleTypenameSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleInitializerList(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleInitializer(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleConversionTypeId(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleConversionDeclarator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleFunctionDefinition(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleTypeSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleTrailingTypeSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleTrailingTypeSpecifierSeq(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleClassSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleEnumSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleSimpleTypeSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleThrowExpression(int index, int&lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleLogicalOrExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleInitializerClause(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleConditionalExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleLogicAndExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleInclusiveOrExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleExclusiveOrExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleAndExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleEqualityExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleRelationalExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleShiftExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleAdditiveExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleMultiplicativeExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handlePmExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleCastExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleUnaryExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleTypeId(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handlePostfixExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleNoexceptExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleNewExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleDeleteExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleTemplateArgumentList(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleTemplateArgument(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleLabeledStatement(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleExpressionStatement(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleDeclarationStatement(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleSelectionStatement(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleIterationStatement(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleJumpStatement(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleTryBlock(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleTemplateName(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleEnumeratorList(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleEnumHead(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleEnumeratorDefinition(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleEnumBase(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleEnumerator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleClassHead(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleMemberSpecification(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleClassHeadName(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleBaseClause(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleClassVirtSpecifierSeq(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleBaseSpecifierList(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleBaseSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleBaseTypeSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleAttributes(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleClassOrDecltype(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleDeclarationSeq(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleBlockDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleSimpleDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleAsmDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleUsingDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleUsingDirective(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleStatic_assertDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleAliasDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleOpaqueEnumDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleTemplateDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleExplicitInstantiation(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleExplicitSpecification(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleLinkageSpecification(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleNamespaceDefinition(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleNamespaceAliasDefinition(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleEmptyDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleAttibuteDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleInitDeclaratorList(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleInitDeclarator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleCVQualifierSeq(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleRefQualifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret = NULL);
  uint32 handleCVQualifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret = NULL);
  uint32 handleUnaryOperator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret = NULL);
  uint32 handleConstExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret = NULL);
  uint32 handleFunctionSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret = NULL);
  uint32 handleStorageClassSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret = NULL);
  uint32 handleAccessSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret = NULL);
  uint32 handlePureSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret = NULL);
  uint32 handleVirtSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret = NULL);
  uint32 handleVirtSpecifierSeq(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret = NULL);
  uint32 handleAssignmentOperator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret = NULL);
  uint32 handleLiteral(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret = NULL);
  uint32 handleExternLiteral(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret = NULL);
  uint32 handleStringLiteral(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret = NULL);
  uint32 handleEnumKey(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret = NULL);
  uint32 handleClassKey(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret = NULL);
  uint32 handleOverloadableOperator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret = NULL);
  uint32 handleQualifiedNamespaceSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret = NULL);
  uint32 handleNamedNamespaceDefinition(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret = NULL);
  uint32 handleUnnamedNamespaceDefinition(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret = NULL);
  uint32 handleOriginalNamespaceDefinition(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret = NULL);
  uint32 handleExtensionNamespaceDefinition(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret = NULL);
  uint32 handleNamespaceBody(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret = NULL);

  typedef uint32(GrammarAnalyzer::*handler)(int,int&,GrammarBlock*, GrammarReturnerBase* ret);

  bool invoke(handler han, const string& func, const int line, const int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret,bool isOpt);

  bool invoke(const string& func, const int line, const int index, int& lastIndex, const string& key, bool isOpt, bool inOoneLine = false);

  uint32 expect(const string& expected,int index, bool oneLine = false);

  bool isLegalVarIdentify(const string& id, GrammarBlock* curBlock);

private:

  LexRecList mRecList;

  GrammarBlock mTopBlock;
  
  LoopBreaker mLoopBreaker;
};

//this macro should only be used in this file
//
#define IS_OPT true
#define NOT_OPT false
#define IN_ONE_LINE true
#define NOT_IN_ONE_LINE false

#define INVOKE(handler, index, lastIndex, curBlock, returner, isOpt) invoke(&GrammarAnalyzer::handle ## handler, __func__, __LINE__,index, lastIndex, curBlock, returner, isOpt)

#define EXPECT(index,lastIndex, key, isOpt, inOneLine) invoke(__func__,__LINE__,index, lastIndex, key, isOpt, inOneLine)

#endif /* end of include guard: GRAMMARANALYZER_H */
