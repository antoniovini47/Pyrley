%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
%token TK_LPAREN TK_RPAREN TK_COLON TK_COMMA
%token TK_AND TK_OR TK_NOT
%token TK_NEWLINE

/* Precedência de operadores (do menor para o maior) */
%right TK_ASSIGN
%left TK_OR
%left TK_AND
%right TK_NOT
%left TK_EQ TK_NEQ
%left TK_LT TK_GT TK_LE TK_GE
%left TK_PLUS TK_MINUS
%left TK_TIMES TK_DIVIDE
%nonassoc LOWER_THAN_ELSE
%nonassoc TK_ELSE

%start program

%%

program: statements
       ;

statements: statement
         | statements statement
         ;

statement: simple_statement TK_NEWLINE
        | compound_statement
        ;

simple_statement: assignment
                | print_stmt
                | return_stmt
                | function_call
                | /* empty */
                ;

compound_statement: function_def
                 | if_statement
                 | while_statement
                 | for_statement
                 ;

block: TK_COLON TK_NEWLINE statements
     ;

function_def: TK_DEF TK_ID TK_LPAREN param_list TK_RPAREN block
            {
                printf("Function definition: %s\n", $2);
            }
            ;

param_list: /* empty */
         | non_empty_param_list
         ;

non_empty_param_list: TK_ID
                   | non_empty_param_list TK_COMMA TK_ID
                   ;

if_statement: TK_IF expression block %prec LOWER_THAN_ELSE
            {
                printf("If statement\n");
            }
            | TK_IF expression block TK_ELSE block
            {
                printf("If-Else statement\n");
            }
            ;

while_statement: TK_WHILE expression block
               {
                   printf("While statement\n");
               }
               ;

for_statement: TK_FOR TK_ID TK_IN expression block
             {
                 printf("For statement with iterator: %s\n", $2);
             }
             ;

return_stmt: TK_RETURN expression
           {
               printf("Return statement\n");
           }
           ;

print_stmt: TK_PRINT TK_LPAREN expression TK_RPAREN
          {
              printf("Print statement\n");
          }
          ;

assignment: TK_ID TK_ASSIGN expression
          {
              printf("Assignment to: %s\n", $1);
          }
          ;

expression: TK_NUM
          {
              printf("Number: %s\n", $1);
          }
          | TK_STRING
          {
              printf("String: %s\n", $1);
          }
          | TK_ID
          {
              printf("Identifier: %s\n", $1);
          }
          | expression TK_PLUS expression
          {
              printf("Addition\n");
          }
          | expression TK_MINUS expression
          {
              printf("Subtraction\n");
          }
          | expression TK_TIMES expression
          {
              printf("Multiplication\n");
          }
          | expression TK_DIVIDE expression
          {
              printf("Division\n");
          }
          | expression TK_AND expression
          {
              printf("Logical AND\n");
          }
          | expression TK_OR expression
          {
              printf("Logical OR\n");
          }
          | TK_NOT expression
          {
              printf("Logical NOT\n");
          }
          | expression TK_EQ expression
          {
              printf("Equal comparison\n");
          }
          | expression TK_NEQ expression
          {
              printf("Not equal comparison\n");
          }
          | expression TK_LT expression
          {
              printf("Less than comparison\n");
          }
          | expression TK_GT expression
          {
              printf("Greater than comparison\n");
          }
          | expression TK_LE expression
          {
              printf("Less than or equal comparison\n");
          }
          | expression TK_GE expression
          {
              printf("Greater than or equal comparison\n");
          }
          | TK_LPAREN expression TK_RPAREN
          {
              printf("Parenthesized expression\n");
          }
          | function_call
          ;

function_call: TK_ID TK_LPAREN arg_list TK_RPAREN
             {
                 printf("Function call: %s\n", $1);
             }
             ;

arg_list: /* empty */
        | non_empty_arg_list
        ;

non_empty_arg_list: expression
                  | non_empty_arg_list TK_COMMA expression
                  ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Erro sintático: %s\n", s);
}

int main() {
    yyparse();
    return 0;
}