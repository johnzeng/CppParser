%{

%}

%token STRING

%%
number
: STRING{ $$ = atoi(lexer.YYText());std::cout << "Parser value "<<$$<<std::endl;}
%%

