#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Token types
typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_OPERATOR,
    TOKEN_KEYWORD,
    TOKEN_DELIMITER,
    TOKEN_UNKNOWN,
    TOKEN_EOF,
    TOKEN_ASSIGN,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_SEMICOLON,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_COMMA,
    TOKEN_COLON,
    TOKEN_DEDENT
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    char value[256];
    int line;
    int column;
} Token;

// AST node types
typedef enum {
    NODE_PROGRAM,
    NODE_FUNCTION,
    NODE_IF,
    NODE_ELIF,
    NODE_ELSE,
    NODE_WHILE,
    NODE_ASSIGN,
    NODE_PRINT,
    NODE_EXPRESSION,
    NODE_IDENTIFIER,
    NODE_NUMBER,
    NODE_STRING,
    NODE_OPERATOR,
    NODE_UNKNOWN
} NodeType;

// AST node structure
typedef struct ASTNode {
    NodeType type;
    char value[256];
    struct ASTNode *left;
    struct ASTNode *right;
    struct ASTNode *next;  // For statement lists
} ASTNode;

// Function prototypes
Token* tokenize(const char *code);
ASTNode* parse(const char *code);
void print_ast(ASTNode *node, int indent);
void free_ast(ASTNode *node);

// Lexer implementation (same as before)
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
    if (!tokens) {
        perror("Error allocating memory for tokens");
        exit(1);
    }
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

// Parser implementation
Token *currentToken;
Token *tokens;

ASTNode* create_node(NodeType type, const char *value) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        perror("Error allocating memory for AST node");
        exit(1);
    }
    node->type = type;
    strcpy(node->value, value);
    node->left = NULL;
    node->right = NULL;
    node->next = NULL;
    return node;
}

void advance() {
    currentToken++;
}

int match(TokenType expected) {
    if (currentToken->type == expected) {
        advance();
        return 1;
    }
    return 0;
}

ASTNode* parse_expression();
ASTNode* parse_statement();
ASTNode* parse_block();

ASTNode* parse_function() {
    ASTNode *node = create_node(NODE_FUNCTION, currentToken->value);
    advance(); // Skip 'def'
    if (match(TOKEN_IDENTIFIER)) {
        strcpy(node->value, currentToken[-1].value);
        if (match(TOKEN_LPAREN)) {
            // Parse parameters (not implemented in this example)
            while (!match(TOKEN_RPAREN)) {
                advance();
            }
            if (match(TOKEN_COLON)) {
                node->left = parse_block();
            }
        }
    }
    return node;
}

ASTNode* parse_if_statement() {
    ASTNode *node = create_node(NODE_IF, "if");
    advance(); // Skip 'if'
    node->left = parse_expression();
    if (match(TOKEN_COLON)) {
        node->right = parse_block();
    }
    return node;
}

ASTNode* parse_while_statement() {
    ASTNode *node = create_node(NODE_WHILE, "while");
    advance(); // Skip 'while'
    node->left = parse_expression();
    if (match(TOKEN_COLON)) {
        node->right = parse_block();
    }
    return node;
}

ASTNode* parse_print_statement() {
    ASTNode *node = create_node(NODE_PRINT, "print");
    advance(); // Skip 'print'
    if (match(TOKEN_LPAREN)) {
        node->left = parse_expression();
        match(TOKEN_RPAREN);
    }
    return node;
}

ASTNode* parse_assignment() {
    ASTNode *node = create_node(NODE_ASSIGN, currentToken->value);
    advance(); // Skip identifier
    if (match(TOKEN_ASSIGN)) {
        node->left = create_node(NODE_IDENTIFIER, currentToken[-2].value);
        node->right = parse_expression();
    }
    return node;
}

ASTNode* parse_expression() {
    ASTNode *node = create_node(NODE_EXPRESSION, "expr");
    if (currentToken->type == TOKEN_NUMBER) {
        node->left = create_node(NODE_NUMBER, currentToken->value);
        advance();
    } else if (currentToken->type == TOKEN_IDENTIFIER) {
        node->left = create_node(NODE_IDENTIFIER, currentToken->value);
        advance();
    }
    if (currentToken->type == TOKEN_OPERATOR) {
        node->right = create_node(NODE_OPERATOR, currentToken->value);
        advance();
        node->right->left = parse_expression();
    }
    return node;
}

ASTNode* parse_statement() {
    if (currentToken->type == TOKEN_KEYWORD) {
        if (strcmp(currentToken->value, "def") == 0) {
            return parse_function();
        } else if (strcmp(currentToken->value, "if") == 0) {
            return parse_if_statement();
        } else if (strcmp(currentToken->value, "while") == 0) {
            return parse_while_statement();
        } else if (strcmp(currentToken->value, "print") == 0) {
            return parse_print_statement();
        }
    } else if (currentToken->type == TOKEN_IDENTIFIER) {
        return parse_assignment();
    }
    return create_node(NODE_UNKNOWN, "unknown");
}

ASTNode* parse_block() {
    ASTNode *node = create_node(NODE_PROGRAM, "block");
    ASTNode *current = node;
    while (currentToken->type != TOKEN_EOF && currentToken->type != TOKEN_DEDENT) {
        current->next = parse_statement();
        current = current->next;
    }
    return node;
}

ASTNode* parse(const char *code) {
    tokens = tokenize(code);
    currentToken = tokens;
    return parse_block();
}

void print_ast(ASTNode *node, int indent) {
    if (node == NULL) return;
    for (int i = 0; i < indent; i++) printf("  ");
    printf("Node: %s\n", node->value);
    print_ast(node->left, indent + 1);
    print_ast(node->right, indent + 1);
    print_ast(node->next, indent);
}

void free_ast(ASTNode *node) {
    if (node == NULL) return;
    free_ast(node->left);
    free_ast(node->right);
    free_ast(node->next);
    free(node);
}

int main() {
    FILE *file = fopen("SintaxAnalyserSample.py", "r");
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

    ASTNode *ast = parse(code);
    print_ast(ast, 0);
    free_ast(ast);
    free(code);
    return 0;
}
