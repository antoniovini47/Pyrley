%{
#include <stdio.h>
#include <string.h>
#include "sintatico.tab.h"

extern YYSTYPE yylval;
char string_buffer[1024];
int string_buffer_ptr = 0;
%}

%x STRING_STATE

ESPACO [ \t]
DIGITO [0-9]
LETRA [A-Za-z_]

DELIM {ESPACO}+
NUM {DIGITO}+
ID {LETRA}({LETRA}|{DIGITO})*

%%

{DELIM}     { /* Ignore delimiters */ }
\n          { return TK_NEWLINE; }
#.*\n       { return TK_NEWLINE; /* Comments return newline */ }

\"          { string_buffer_ptr = 0; BEGIN(STRING_STATE); }
<STRING_STATE>[^\"\n\\]+ { 
    strncpy(string_buffer + string_buffer_ptr, yytext, strlen(yytext));
    string_buffer_ptr += strlen(yytext);
}
<STRING_STATE>\\[nt\"]  {
    if(yytext[1] == 'n') string_buffer[string_buffer_ptr++] = '\n';
    else if(yytext[1] == 't') string_buffer[string_buffer_ptr++] = '\t';
    else if(yytext[1] == '\"') string_buffer[string_buffer_ptr++] = '\"';
}
<STRING_STATE>\"        { 
    BEGIN(INITIAL);
    string_buffer[string_buffer_ptr] = '\0';
    yylval.label = strdup(string_buffer);
    return TK_STRING;
}
<STRING_STATE>\n       { 
    printf("Erro: string não terminada\n"); 
    BEGIN(INITIAL); 
    return TK_NEWLINE;
}
<STRING_STATE>.        { /* Ignora outros caracteres dentro da string */ }

"def"       { return TK_DEF; }
"return"    { return TK_RETURN; }
"if"        { return TK_IF; }
"else"      { return TK_ELSE; }
"while"     { return TK_WHILE; }
"for"       { return TK_FOR; }
"in"        { return TK_IN; }
"print"     { return TK_PRINT; }
"and"       { return TK_AND; }
"or"        { return TK_OR; }
"not"       { return TK_NOT; }

"=="        { return TK_EQ; }
"!="        { return TK_NEQ; }
"<"         { return TK_LT; }
">"         { return TK_GT; }
"<="        { return TK_LE; }
">="        { return TK_GE; }
"="         { return TK_ASSIGN; }
"+"         { return TK_PLUS; }
"-"         { return TK_MINUS; }
"*"         { return TK_TIMES; }
"/"         { return TK_DIVIDE; }
"("         { return TK_LPAREN; }
")"         { return TK_RPAREN; }
":"         { return TK_COLON; }
","         { return TK_COMMA; }

{NUM}       { yylval.label = strdup(yytext); return TK_NUM; }
{ID}        { yylval.label = strdup(yytext); return TK_ID; }

.           { printf("Caractere não reconhecido: %s\n", yytext); }

%%

int yywrap() {
    return 1;
}