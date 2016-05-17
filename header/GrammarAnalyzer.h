#ifndef GRAMMARANALYZER_H
#define GRAMMARANALYZER_H

#include "LexData.h"
#include "GrammarData.h"
#include "LoopBreaker.h"
#include "GrammarReturner.h"


class GrammarAnalyzer {
public:
  GrammarAnalyzer (LexRecList recList);
  virtual ~GrammarAnalyzer ();

  uint32 doAnalyze();

  GrammarBlock *getTopBlock();
protected:


  uint32 handleBraceOrEqualInitializer(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleExceptionSpeciafier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleParameterDeclarationList(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleParameterDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleStatement(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleParameterDeclarationClause(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleStatementSeq(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleFunctionTryBlock(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleCtorInitializer(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleCompoundStatement(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleIdentifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleNameSpaceName(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
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
  uint32 handleParameterAndQualifiers(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleTrailingReturenType(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handlePtrOperator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleNestNameSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleOperatorFunctionId(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleConversionFunctionId(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleLiteralOperatorId(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
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
  uint32 handleFuncDefinition(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleTypeSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleTrailingTypeSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleClassSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleEnumSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleSimpleTypeSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleThrowExpression(int index, int&lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleLogicOrExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
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
  uint32 handleTemplateDecaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleExplicitInstantiation(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleExplicitSpecification(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleLinkageSpecification(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleNamespaceDefinition(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleNamespaceAliasDefinition(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleEmptyDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleAttibuteDeclaration(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleInitDeclaratorList(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 handleInitDeclarator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);

  uint32 expect(const string& expected,int index, bool oneLine = false);

  bool isLegalVarIdentify(const string& id, GrammarBlock* curBlock);

  uint32 handleCVQualifierSeq(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* returner = NULL);
  uint32 getRefQualifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret = NULL);
  uint32 getCVQualifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret = NULL);
  uint32 getUnaryOperator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret = NULL);
  uint32 getConstExpression(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret = NULL);
  uint32 getFunctionSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret = NULL);
  uint32 getStorageClassSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret = NULL);
  uint32 getAccessSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret = NULL);
  uint32 getPureSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret = NULL);
  uint32 getVirtSpecifier(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret = NULL);
  uint32 getAssignmentOperator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret = NULL);
  uint32 getLiteral(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret = NULL);
  uint32 getEnumKey(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret = NULL);
  uint32 getClassKey(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret = NULL);
  uint32 getOverloadableOperator(int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret = NULL);

  typedef uint32(GrammarAnalyzer::*handler)(int,int&,GrammarBlock*, GrammarReturnerBase* ret);

  bool invoke(handler han, const string& func, const int line, const int index, int& lastIndex, GrammarBlock* curBlock, GrammarReturnerBase* ret,bool isOpt);

  bool invoke(const string& func, const int line, const int index, int& lastIndex, const string& key, bool isOpt, bool inOoneLine = false);

protected:


private:

  LexRecList mRecList;

  GrammarBlock mTopBlock;
  
  LoopBreaker mLoopBreaker;
};

//this macro should only be used in this file
//
#define INVOKE(handler, index, lastIndex, curBlock, returner, isOpt) invoke(&GrammarAnalyzer::handle ## handler, __func__, __LINE__,index, lastIndex, curBlock, returner, isOpt)

#define EXPECT(index,lastIndex, key, isOpt, inOneLine) invoke(__func__,__LINE__,index, lastIndex, key, isOpt, inOneLine)

#endif /* end of include guard: GRAMMARANALYZER_H */
