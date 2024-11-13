%{
#include <stdio.h>
#include <string.h>

// Declare token types
#define TK_DEF 1
#define TK_RETURN 2
#define TK_IF 3
#define TK_ELSE 4
#define TK_WHILE 5
#define TK_FOR 6
#define TK_IN 7
#define TK_PRINT 8
#define TK_NUM 9
#define TK_ID 10
#define TK_EQ 11
#define TK_NEQ 12
#define TK_LT 13
#define TK_GT 14
#define TK_LE 15
#define TK_GE 16
#define TK_ASSIGN 17
#define TK_PLUS 18
#define TK_MINUS 19
#define TK_TIMES 20
#define TK_DIVIDE 21
#define TK_LPAREN 22
#define TK_RPAREN 23
#define TK_COLON 24
#define TK_INDENT 25
#define TK_DEDENT 26

// Declare yylval
typedef union {
    char* label;
} YYSTYPE;

YYSTYPE yylval;

%}

ESPACO [ \t]
DIGITO [0-9]
LETRA [A-Za-z_]

DELIM {ESPACO}+
NUM {DIGITO}+
ID {LETRA}({LETRA}|{DIGITO})*

%%

{DELIM}     { /* Ignore delimiters */ }

"def"       { printf("TK_DEF\n"); return TK_DEF; }
"return"    { printf("TK_RETURN\n"); return TK_RETURN; }
"if"        { printf("TK_IF\n"); return TK_IF; }
"else"      { printf("TK_ELSE\n"); return TK_ELSE; }
"while"     { printf("TK_WHILE\n"); return TK_WHILE; }
"for"       { printf("TK_FOR\n"); return TK_FOR; }
"in"        { printf("TK_IN\n"); return TK_IN; }
"print"     { printf("TK_PRINT\n"); return TK_PRINT; }

"=="        { printf("TK_EQ\n"); return TK_EQ; }
"!="        { printf("TK_NEQ\n"); return TK_NEQ; }
"<"         { printf("TK_LT\n"); return TK_LT; }
">"         { printf("TK_GT\n"); return TK_GT; }
"<="        { printf("TK_LE\n"); return TK_LE; }
">="        { printf("TK_GE\n"); return TK_GE; }
"="         { printf("TK_ASSIGN\n"); return TK_ASSIGN; }
"\+"        { printf("TK_PLUS\n"); return TK_PLUS; }
"-"         { printf("TK_MINUS\n"); return TK_MINUS; }
"\*"        { printf("TK_TIMES\n"); return TK_TIMES; }
"/"         { printf("TK_DIVIDE\n"); return TK_DIVIDE; }
"\("        { printf("TK_LPAREN\n"); return TK_LPAREN; }
"\)"        { printf("TK_RPAREN\n"); return TK_RPAREN; }
":"         { printf("TK_COLON\n"); return TK_COLON; }

{NUM}       { yylval.label = yytext; printf("TK_NUM: %s\n", yytext); return TK_NUM; }
{ID}        { yylval.label = yytext; printf("TK_ID: %s\n", yytext); return TK_ID; }

.           { printf("Unrecognized character: %c\n", *yytext); }

%%

int yywrap() {
    return 1;
}