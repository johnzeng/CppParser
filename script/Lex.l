%{
#include <stdio.h>
#include <stdlib.h>
%}

%x _line_comment_
%x _block_comment_

%%

"/*" {BEGIN(_block_comment_);}
<_block_comment_>"*/" {BEGIN(INITIAL);}
<_block_comment_>. {/*ignore all other words*/}

"//" {BEGIN(_line_comment_);}
<_line_comment_>.*\\[\t ]*\n {}
<_line_comment_>.*\n {BEGIN(INITIAL);}

%%

