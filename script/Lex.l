%{
#include <stdio.h>
#include <stdlib.h>
#include <string>
using namespace std;

static string S_StrBuf = "";
//how should I record string?
%}

%option c++
%option noyywrap

%x _line_comment_
%x _block_comment_
%x _string_
%x _char_

%%

.*\\[\t ]*\n {/*eat the new line backslash by default*/}

"/*" {BEGIN(_block_comment_);}
<_block_comment_>"*/" {BEGIN(INITIAL);}
<_block_comment_>. {/*ignore all other words*/}

"//" {BEGIN(_line_comment_);}
<_line_comment_>.*\\[\t ]*\n {}
<_line_comment_>.*\n {BEGIN(INITIAL);}

"\"" {BEGIN(_string_);}
<_string_>"\\\""  {printf("\\\"");}
<_string_>.*\\[\t ]*\n {S_StrBuf += "1";}
<_string_>"\"" {BEGIN(INITIAL);}

"'" {BEGIN(_char_);}
<_char_>"\\'" {printf("\\'");}
<_char_>.*\\[\t ]*\n {}
<_char_>"'" {BEGIN(INITIAL);}

0[1-7][0-7]+ {/*Octal*/}
0x[1-9a-fA-F][0-9a-fA-F]+ {/*Hex*/}
[1-9][0-9]+\.[0-9]+[f] {/*float*/}
[1-9][0-9]+\.[0-9]+[d] {/*double*/}
[1-9][0-9]+ {/*Decimal*/}


"auto" |
"int" |
"float" |
"double" |
"if" |
"else" |
"switch" |
"case" |
"long" |
"class" |
"struct" |
"char" |
"const" |
"while" |
"do" |
"public" |
"private" |
"protected" |
"enum" |
"union" |
"typedef" |
"static" |
"inline" |
"void" |
"return" |
"asm" |
"catch" |
"const_cast" |
"continue" |
"default" |
"delete" |
"dynamic_cast" |
"explicit" |
"extern" |
"false" |
"true" |
"for" |
"goto" |
"mutable" |
"operator" |
"namespace" |
"new" |
"register" |
"reinterpret_cast" |
"short" |
"signed" |
"sizeof" |
"static_cast" |
"template" |
"this" |
"throw" |
"try" |
"typename" |
"unsigned" |
"virtual" |
"volatile" |
"wchar_t" {/*tokens*/printf("[key:%s]", yytext);}

%%
int main( int /* argc */, char** /* argv */ )
{
   FlexLexer* lexer = new yyFlexLexer;
   while(lexer->yylex() != 0)
       ;
   return 0;
}
