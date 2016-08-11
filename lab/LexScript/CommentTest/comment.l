%{
#include <stdio.h>
#include <stdlib.h>
%}
%x _comment_
%%
"//" {BEGIN(_comment_);}
<_comment_>.*\\[\t ]*\n {}
<_comment_>.*\n {BEGIN(INITIAL);}
%%

int main(int argc, char ** argv){
  FILE* f = fopen("test","r");
  yyrestart(f);
  yylex();
  fclose(f);
}
