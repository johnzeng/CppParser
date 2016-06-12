# CppParser
[![Build Status](https://travis-ci.org/johnzeng/CppParser.svg?branch=master)](https://travis-ci.org/johnzeng/CppParser)[![Coverage Status](https://coveralls.io/repos/github/johnzeng/CppParser/badge.svg?branch=master)](https://coveralls.io/github/johnzeng/CppParser?branch=master)

This SimpleCompletor is aiming to create a completor for vim ,which is easy to config,and good enough to complete the c,cpp coding input.
First work is to complete the c codes in property way.

###how to build:
Just use make is enough.
##Mac
#dependency:

```
1, clang
2, make
3, cmake if you wanna run test
```

#build lib:

```make```

#running test:

```make test```

running test need gtest support, I already add checkout comment into makefile. 

##linux
#dependency:

```
1,gcc
2,make
3,cmkae if you wanna run test
```

#build lib:

```make```

#running test:

```make travis```

this target is used to run test on travis, don't feel strange for this. you can also use `make test debug_var=2` to run the test

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
GrmTop
|  DeclarationSeq
|  |  Declaration
|  |  |  NamespaceDefinition
|  |  |  |  NamedNamespaceDefinition
|  |  |  |  |  OriginalNamespaceDefinition
|  |  |  |  |  |  Identifier:JZNamespace
|  |  |  |  |  |  NamespaceBody
|  |  |  |  |  |  |  DeclarationSeq
|  |  |  |  |  |  |  |  Declaration
|  |  |  |  |  |  |  |  |  BlockDeclaration
|  |  |  |  |  |  |  |  |  |  SimpleDeclaration
|  |  |  |  |  |  |  |  |  |  |  DeclSpecifierSeq
|  |  |  |  |  |  |  |  |  |  |  |  DeclSpecifier
|  |  |  |  |  |  |  |  |  |  |  |  |  TypeSpecifier
|  |  |  |  |  |  |  |  |  |  |  |  |  |  ClassSpecifier
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  ClassHead
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  ClassKey:class
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  ClassHeadName
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  ClassName
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  Identifier:test
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  MemberSpecification
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  MemberDeclaration
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  DeclSpecifierSeq
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  DeclSpecifier
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  TypeSpecifier
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  TrailingTypeSpecifier
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  SimpleTypeSpecifier:long
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  DeclSpecifier
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  TypeSpecifier
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  TrailingTypeSpecifier
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  SimpleTypeSpecifier:int
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  MemberDeclaratorList
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  MemberDeclarator
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  Declarator
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  PtrDeclarator
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  NonPtrDeclarator
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  NonPtrDeclarator
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  DeclaratorId
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  ClassName
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  Identifier:b
|  |  Declaration
|  |  |  BlockDeclaration
|  |  |  |  SimpleDeclaration
|  |  Declaration
|  |  |  BlockDeclaration
|  |  |  |  UsingDirective
|  |  |  |  |  NamespaceName
|  |  |  |  |  |  OriginalNamespaceName
|  |  |  |  |  |  |  Identifier:JZNamespace
|  |  Declaration
|  |  |  BlockDeclaration
|  |  |  |  SimpleDeclaration
|  |  |  |  |  InitDeclaratorList
|  |  |  |  |  |  InitDeclarator
|  |  |  |  |  |  |  Declarator
|  |  |  |  |  |  |  |  PtrDeclarator
|  |  |  |  |  |  |  |  |  NonPtrDeclarator
|  |  |  |  |  |  |  |  |  |  NonPtrDeclarator
|  |  |  |  |  |  |  |  |  |  |  DeclaratorId
|  |  |  |  |  |  |  |  |  |  |  |  NestNameSpecifier
|  |  |  |  |  |  |  |  |  |  |  |  |  NestNameSpecifier
|  |  |  |  |  |  |  |  |  |  |  |  |  |  TypeName
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  ClassName
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  Identifier:JZNamespace
|  |  |  |  |  |  |  |  |  |  |  |  |  NestNameSpecifier
|  |  |  |  |  |  |  |  |  |  |  |  |  |  TypeName
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  ClassName
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  Identifier:test
|  |  |  |  |  |  |  |  |  |  |  |  ClassName
|  |  |  |  |  |  |  |  |  |  |  |  |  Identifier:b
|  |  |  |  |  |  |  Initializer
|  |  |  |  |  |  |  |  BraceOrEqualInitializer
|  |  |  |  |  |  |  |  |  InitializerClause
|  |  |  |  |  |  |  |  |  |  AssignmentExpression
|  |  |  |  |  |  |  |  |  |  |  ConditionalExpression
|  |  |  |  |  |  |  |  |  |  |  |  LogicalOrExpression
|  |  |  |  |  |  |  |  |  |  |  |  |  LogicalAndExpression
|  |  |  |  |  |  |  |  |  |  |  |  |  |  InclusiveOrExpression
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  ExclusiveOrExpression
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  AndExpression
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  EqualityExpression
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  RelationalExpression
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  ShiftExpression
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  AdditiveExpression
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  MultiplicativeExpression
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  PmExpression
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  CastExpression
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  UnaryExpression
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  PostfixExpression
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  PostfixExpression
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  PrimaryExpression
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  Literal:1
```
##2016/05/22
All BNF is trans to code. Now we need to debug and raise the cover rage.

##2016/05/14
Release 0.1.3, fixed most of the preprocessing bugs, and already begin to hack the grammar part.

##2016/03/29
Adding gtest into this project and we are going to do test now!

##2015/08/05
Preprocessing stage is complete.More test is needed to check if I do the right work,it may take time until I decide to wirte next stage.


