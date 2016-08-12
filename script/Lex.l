%{
#include <stdio.h>
#include <stdlib.h>

//how should I record string?
%}

%x _line_comment_
%x _block_comment_
%x _string_

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
<_string_>.*\\[\t ]*\n {}
<_string_>"\"" {BEGIN(INITIAL);}

%%

