# CppParser
[![Build Status](https://travis-ci.org/johnzeng/CppParser.svg?branch=develop)](https://travis-ci.org/johnzeng/CppParser)[![Coverage Status](https://coveralls.io/repos/github/johnzeng/CppParser/badge.svg?branch=develop)](https://coveralls.io/github/johnzeng/CppParser?branch=develop)

This SimpleCompletor is aiming to create a completor for vim ,which is easy to config,and good enough to complete the c,cpp coding input.
First work is to complete the c codes in property way.

##2016/06/06
update a sample for grammar analyze result:

here is the original file:
```cpp
namespace JZNamespace{
  class test{
    long int b;
  };
};
using namespace JZNamespace;

JZNamespace::test::b = 1;
```

And the grammar tree is look like this:

```shell
eGrmTop
|  eDeclarationSeq
|  |  eDeclaration
|  |  |  eNamespaceDefinition
|  |  |  |  eNamedNamespaceDefinition
|  |  |  |  |  eOriginalNamespaceDefinition
|  |  |  |  |  |  eIdentifier:JZNamespace
|  |  |  |  |  |  eNamespaceBody
|  |  |  |  |  |  |  eDeclarationSeq
|  |  |  |  |  |  |  |  eDeclaration
|  |  |  |  |  |  |  |  |  eBlockDeclaration
|  |  |  |  |  |  |  |  |  |  eSimpleDeclaration
|  |  |  |  |  |  |  |  |  |  |  eDeclSpecifierSeq
|  |  |  |  |  |  |  |  |  |  |  |  eDeclSpecifier
|  |  |  |  |  |  |  |  |  |  |  |  |  eTypeSpecifier
|  |  |  |  |  |  |  |  |  |  |  |  |  |  eClassSpecifier
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  eClassHead
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  eClassKey:class
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  eClassHeadName
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  eClassName
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  eIdentifier:test
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  eMemberSpecification
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  eMemberDeclaration
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  eDeclSpecifierSeq
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  eDeclSpecifier
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  eTypeSpecifier
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  eTrailingTypeSpecifier
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  eSimpleTypeSpecifier:long
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  eDeclSpecifier
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  eTypeSpecifier
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  eTrailingTypeSpecifier
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  eSimpleTypeSpecifier:int
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  eMemberDeclaratorList
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  eMemberDeclarator
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  eDeclarator
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  ePtrDeclarator
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  eNonPtrDeclarator
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  eNonPtrDeclarator
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  eDeclaratorId
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  eIdExpression
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  eUnqualifiedId
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  eIdentifier:b
|  |  eDeclaration
|  |  |  eBlockDeclaration
|  |  |  |  eSimpleDeclaration
|  |  eDeclaration
|  |  |  eBlockDeclaration
|  |  |  |  eUsingDirective
|  |  |  |  |  eNamespaceName
|  |  |  |  |  |  eOriginalNamespaceName
|  |  |  |  |  |  |  eIdentifier:JZNamespace
|  |  eDeclaration
|  |  |  eBlockDeclaration
|  |  |  |  eSimpleDeclaration
|  |  |  |  |  eInitDeclaratorList
|  |  |  |  |  |  eInitDeclarator
|  |  |  |  |  |  |  eDeclarator
|  |  |  |  |  |  |  |  ePtrDeclarator
|  |  |  |  |  |  |  |  |  eNonPtrDeclarator
|  |  |  |  |  |  |  |  |  |  eNonPtrDeclarator
|  |  |  |  |  |  |  |  |  |  |  eDeclaratorId
|  |  |  |  |  |  |  |  |  |  |  |  eIdExpression
|  |  |  |  |  |  |  |  |  |  |  |  |  eUnqualifiedId
|  |  |  |  |  |  |  |  |  |  |  |  |  |  eIdentifier:JZNamespace
|  |  |  |  |  |  |  |  |  |  eNonPtrDeclarator
|  |  |  |  |  |  |  |  |  |  |  eDeclaratorId
|  |  |  |  |  |  |  |  |  |  |  |  eIdExpression
|  |  |  |  |  |  |  |  |  |  |  |  |  eQualifiedId
|  |  |  |  |  |  |  |  |  |  |  |  |  |  eIdentifier:test
|  |  |  |  |  |  |  |  |  |  eNonPtrDeclarator
|  |  |  |  |  |  |  |  |  |  |  eDeclaratorId
|  |  |  |  |  |  |  |  |  |  |  |  eIdExpression
|  |  |  |  |  |  |  |  |  |  |  |  |  eQualifiedId
|  |  |  |  |  |  |  |  |  |  |  |  |  |  eIdentifier:b
|  |  |  |  |  |  |  eInitializer
|  |  |  |  |  |  |  |  eBraceOrEqualInitializer
|  |  |  |  |  |  |  |  |  eInitializerClause
|  |  |  |  |  |  |  |  |  |  eAssignmentExpression
|  |  |  |  |  |  |  |  |  |  |  eConditionalExpression
|  |  |  |  |  |  |  |  |  |  |  |  eLogicalOrExpression
|  |  |  |  |  |  |  |  |  |  |  |  |  eLogicalAndExpression
|  |  |  |  |  |  |  |  |  |  |  |  |  |  eInclusiveOrExpression
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  eExclusiveOrExpression
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  eAndExpression
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  eEqualityExpression
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  eRelationalExpression
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  eShiftExpression
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  eAdditiveExpression
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  eMultiplicativeExpression
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  ePmExpression
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  eCastExpression
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  eUnaryExpression
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  ePostfixExpression
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  ePostfixExpression
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  ePrimaryExpression
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  eLiteral:1
```
##2016/05/22
All BNF is trans to code. Now we need to debug and raise the cover rage.

##2016/05/14
Release 0.1.3, fixed most of the preprocessing bugs, and already begin to hack the grammar part.

##2016/03/29
Adding gtest into this project and we are going to do test now!

##2015/08/05
Preprocessing stage is complete.More test is needed to check if I do the right work,it may take time until I decide to wirte next stage.


