#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum { TOKEN_ID, TOKEN_NUM, TOKEN_ASSIGN, TOKEN_PLUS, TOKEN_MINUS, TOKEN_STAR, TOKEN_SLASH, TOKEN_SEMICOLON, TOKEN_EOF, TOKEN_ERROR } TokenType;

typedef struct {
    TokenType type;
    char lexeme[64];
    int line;
    int column;
} Token;

typedef struct {
    char name[64];
    int value;
} Symbol;

Symbol symbolTable[256];
int symbolCount = 0;
int currentLine = 1;
int currentColumn = 1;
const char *source;
Token currentToken;

// Helper function to add a symbol to the table
void addSymbol(const char *name, int value) {
    strcpy(symbolTable[symbolCount].name, name);
    symbolTable[symbolCount].value = value;
    symbolCount++;
}

// Check if a symbol is in the table
int getSymbolValue(const char *name) {
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i].name, name) == 0) {
            return symbolTable[i].value;
        }
    }
    return -1;
}

// Lexical analysis: retrieves the next token from the source
Token getNextToken() {
    Token token = { .type = TOKEN_ERROR, .line = currentLine, .column = currentColumn };

    while (*source != '\0') {
        char ch = *source;
        
        if (isspace(ch)) {
            if (ch == '\n') {
                currentLine++;
                currentColumn = 1;
            } else {
                currentColumn++;
            }
            source++;
            continue;
        }
        if (isdigit(ch)) {
            int startCol = currentColumn;
            int i = 0;
            token.type = TOKEN_NUM;
            while (isdigit(*source)) {
                token.lexeme[i++] = *source++;
                currentColumn++;
            }
            token.lexeme[i] = '\0';
            token.line = currentLine;
            token.column = startCol;
            return token;
        }
        if (isalpha(ch)) {
            int startCol = currentColumn;
            int i = 0;
            token.type = TOKEN_ID;
            while (isalnum(*source)) {
                token.lexeme[i++] = *source++;
                currentColumn++;
            }
            token.lexeme[i] = '\0';
            token.line = currentLine;
            token.column = startCol;
            return token;
        }
        switch (ch) {
            case '=':
                token.type = TOKEN_ASSIGN;
                strcpy(token.lexeme, "=");
                token.line = currentLine;
                token.column = currentColumn++;
                source++;
                return token;
            case '+': case '-': case '*': case '/':
                token.type = (ch == '+') ? TOKEN_PLUS : (ch == '-') ? TOKEN_MINUS : (ch == '*') ? TOKEN_STAR : TOKEN_SLASH;
                token.lexeme[0] = ch;
                token.lexeme[1] = '\0';
                token.line = currentLine;
                token.column = currentColumn++;
                source++;
                return token;
            case ';':
                token.type = TOKEN_SEMICOLON;
                strcpy(token.lexeme, ";");
                token.line = currentLine;
                token.column = currentColumn++;
                source++;
                return token;
            default:
                token.type = TOKEN_ERROR;
                token.lexeme[0] = ch;
                token.lexeme[1] = '\0';
                token.line = currentLine;
                token.column = currentColumn++;
                source++;
                return token;
        }
    }
    token.type = TOKEN_EOF;
    return token;
}

// Match current token with expected type
int match(TokenType expected) {
    if (currentToken.type == expected) {
        currentToken = getNextToken();
        return 1;
    }
    return 0;
}

// Parsing and Semantic Analysis: Parse assignment statements and expressions
int expression();

void statement() {
    Token idToken = currentToken;
    if (match(TOKEN_ID)) {
        if (match(TOKEN_ASSIGN)) {
            int value = expression();
            if (match(TOKEN_SEMICOLON)) {
                int index = getSymbolValue(idToken.lexeme);
                if (index == -1) {
                    addSymbol(idToken.lexeme, value);
                } else {
                    symbolTable[index].value = value;
                }
                printf("TAC: %s = %d\n", idToken.lexeme, value);
            } else {
                printf("Error: Missing ';' at line %d, character %d\n", currentToken.line, currentToken.column);
            }
        } else {
            printf("Error: Missing '=' after variable '%s' at line %d, character %d\n", idToken.lexeme, idToken.line, idToken.column);
        }
    } else {
        printf("Error: Invalid statement at line %d, character %d\n", currentToken.line, currentToken.column);
    }
}

int term() {
    if (currentToken.type == TOKEN_NUM) {
        int value = atoi(currentToken.lexeme);
        currentToken = getNextToken();
        return value;
    } else if (currentToken.type == TOKEN_ID) {
        int value = getSymbolValue(currentToken.lexeme);
        if (value == -1) {
            printf("Error: Undefined variable '%s' at line %d, column %d\n", currentToken.lexeme, currentToken.line, currentToken.column);
            return 0;
        }
        currentToken = getNextToken();
        return value;
    }
    printf("Error: Unexpected token '%s' at line %d, column %d\n", currentToken.lexeme, currentToken.line, currentToken.column);
    return 0;
}

int expression() {
    int result = term();
    while (currentToken.type == TOKEN_PLUS || currentToken.type == TOKEN_MINUS) {
        TokenType op = currentToken.type;
        currentToken = getNextToken();
        int rhs = term();
        if (op == TOKEN_PLUS) {
            result += rhs;
        } else {
            result -= rhs;
        }
    }
    return result;
}

// Symbol Table Display
void printSymbolTable() {
    printf("\nSymbol Table:\n");
    for (int i = 0; i < symbolCount; i++) {
        printf("Variable %s = %d\n", symbolTable[i].name, symbolTable[i].value);
    }
}

int main() {
    FILE *file = fopen("sourceCode.py", "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *sourceCode = malloc(fileSize + 1);
    if (!sourceCode) {
        perror("Error allocating memory");
        fclose(file);
        return 1;
    }

    fread(sourceCode, 1, fileSize, file);
    sourceCode[fileSize] = '\0';
    fclose(file);

    source = sourceCode;

    printf("Source Code:\n%s\n", sourceCode);
    currentToken = getNextToken();
    while (currentToken.type != TOKEN_EOF) {
        statement();
    }

    printSymbolTable();
    return 0;
}
