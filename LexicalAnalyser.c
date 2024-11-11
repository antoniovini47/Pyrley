#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_OPERATOR,
    TOKEN_KEYWORD,
    TOKEN_DELIMITER,
    TOKEN_UNKNOWN,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[256];
    int line;
    int column;
} Token;

const char *keywords[] = {
    "False", "None", "True", "and", "break", "def",
    "elif", "else", "finally", "for", "if", "import",
    "or", "return", "try", "except", "while", NULL
};

int is_keyword(const char *str) {
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

Token* tokenize(const char *code) {
    Token *tokens = malloc(1024 * sizeof(Token));
    int token_count = 0;
    int line = 1;
    int column = 1;
    const char *p = code;
    
    while (*p != '\0') {
        if (isspace(*p)) {
            if (*p == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            p++;
            continue;
        }

        if (isalpha(*p) || *p == '_') {
            int len = 0;
            while (isalnum(*p) || *p == '_') {
                tokens[token_count].value[len++] = *p++;
                column++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = is_keyword(tokens[token_count].value) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
            tokens[token_count].line = line;
            tokens[token_count].column = column - len;
            token_count++;
            continue;
        }

        if (isdigit(*p)) {
            int len = 0;
            while (isdigit(*p)) {
                tokens[token_count].value[len++] = *p++;
                column++;
            }
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            tokens[token_count].line = line;
            tokens[token_count].column = column - len;
            token_count++;
            continue;
        }

        if (*p == '"' || *p == '\'') {
            char quote = *p++;
            int len = 0;
            while (*p != quote && *p != '\0') {
                tokens[token_count].value[len++] = *p++;
                column++;
            }
            if (*p == quote) p++;
            tokens[token_count].value[len] = '\0';
            tokens[token_count].type = TOKEN_STRING;
            tokens[token_count].line = line;
            tokens[token_count].column = column - len;
            token_count++;
            continue;
        }

        if (strchr("+-*/=<>!", *p)) {
            tokens[token_count].value[0] = *p++;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = TOKEN_OPERATOR;
            tokens[token_count].line = line;
            tokens[token_count].column = column++;
            token_count++;
            continue;
        }

        if (strchr("()[]{}:,;", *p)) {
            tokens[token_count].value[0] = *p++;
            tokens[token_count].value[1] = '\0';
            tokens[token_count].type = TOKEN_DELIMITER;
            tokens[token_count].line = line;
            tokens[token_count].column = column++;
            token_count++;
            continue;
        }

        tokens[token_count].value[0] = *p++;
        tokens[token_count].value[1] = '\0';
        tokens[token_count].type = TOKEN_UNKNOWN;
        tokens[token_count].line = line;
        tokens[token_count].column = column++;
        token_count++;
    }

    tokens[token_count].type = TOKEN_EOF;
    tokens[token_count].line = line;
    tokens[token_count].column = column;
    return tokens;
}

int main() {
    FILE *file = fopen("LexicalAnaylerSample.py", "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *code = malloc(fileSize + 1);
    if (!code) {
        perror("Error allocating memory");
        fclose(file);
        return 1;
    }

    fread(code, 1, fileSize, file);
    code[fileSize] = '\0';
    fclose(file);

    Token *tokens = tokenize(code);

    for (int i = 0; tokens[i].type != TOKEN_EOF; i++) {
        printf("Token: %s, Type: %d, Line: %d, Column: %d\n", tokens[i].value, tokens[i].type, tokens[i].line, tokens[i].column);
    }

    free(tokens);
    free(code);
    return 0;
}