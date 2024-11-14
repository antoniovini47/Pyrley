%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declare yylex to be used by Bison
int yylex();
void yyerror(const char *s);
%}

%union {
    char* label;
}

%token <label> TK_DEF TK_RETURN TK_IF TK_ELSE TK_WHILE TK_FOR TK_IN TK_PRINT
%token <label> TK_NUM TK_ID TK_STRING
%token TK_EQ TK_NEQ TK_LT TK_GT TK_LE TK_GE TK_ASSIGN
%token TK_PLUS TK_MINUS TK_TIMES TK_DIVIDE
%token TK_LPAREN TK_RPAREN TK_COLON

%start program

%%

program:
    | statement
    | program statement
    ;

statement:
      function_def
    | return_stmt
    | if_stmt
    | while_stmt
    | for_stmt
    | print_stmt
    | assignment
    ;

function_def:
    TK_DEF TK_ID TK_LPAREN TK_RPAREN TK_COLON statement
    {
        printf("Function definition: %s\n", $2);
    }
    ;

return_stmt:
    TK_RETURN expression
    {
        printf("Return statement\n");
    }
    ;

if_stmt:
    TK_IF expression TK_COLON statement
    {
        printf("If statement\n");
    }
    | TK_IF expression TK_COLON statement TK_ELSE TK_COLON statement
    {
        printf("If-Else statement\n");
    }
    ;

while_stmt:
    TK_WHILE expression TK_COLON statement
    {
        printf("While statement\n");
    }
    ;

for_stmt:
    TK_FOR TK_ID TK_IN expression TK_COLON statement
    {
        printf("For statement\n");
    }
    ;

print_stmt:
    TK_PRINT TK_LPAREN expression TK_RPAREN
    {
        printf("Print statement\n");
    }
    ;

assignment:
    TK_ID TK_ASSIGN expression
    {
        printf("Assignment: %s\n", $1);
    }
    ;

expression:
      TK_NUM
    | TK_STRING
    | TK_ID
    | expression TK_PLUS expression
    | expression TK_MINUS expression
    | expression TK_TIMES expression
    | expression TK_DIVIDE expression
    | TK_LPAREN expression TK_RPAREN
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "errinho: %s\n", s);
}

int main() {
    while (1) {
        yyparse();
    }
}