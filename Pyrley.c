#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Define AST structures
typedef enum {
    NODE_PRINT,
    NODE_STRING,
    NODE_IF,
    NODE_CONDITION,
    NODE_UNKNOWN
} NodeType;

typedef struct ASTNode {
    NodeType type;
    char value[256];
    struct ASTNode *left;
    struct ASTNode *right;
} ASTNode;

ASTNode* create_node(NodeType type, const char *value) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    strcpy(node->value, value);
    node->left = NULL;
    node->right = NULL;
    return node;
}

void print_ast(ASTNode *node, int indent) {
    if (node == NULL) return;
    for (int i = 0; i < indent; i++) printf("  ");
    printf("Node: %s\n", node->value);
    print_ast(node->left, indent + 1);
    print_ast(node->right, indent + 1);
}

ASTNode* parse_print_statement(const char **p) {
    if (strncmp(*p, "print", 5) == 0) {
        *p += 5;
        while (isspace(**p)) (*p)++;
        if (**p == '(') {
            (*p)++;
            while (isspace(**p)) (*p)++;
            if (**p == '\'') {
                (*p)++;
                const char *start = *p;
                while (**p != '\'' && **p != '\0') (*p)++;
                if (**p == '\'') {
                    ASTNode *print_node = create_node(NODE_PRINT, "print");
                    ASTNode *string_node = create_node(NODE_STRING, "");
                    strncpy(string_node->value, start, *p - start);
                    string_node->value[*p - start] = '\0';
                    print_node->left = string_node;
                    (*p)++;
                    while (isspace(**p)) (*p)++;
                    if (**p == ')') {
                        (*p)++;
                        return print_node;
                    }
                }
            }
        }
    }
    return create_node(NODE_UNKNOWN, "unknown");
}

ASTNode* parse_if_statement(const char **p) {
    if (strncmp(*p, "if", 2) == 0) {
        *p += 2;
        while (isspace(**p)) (*p)++;
        const char *start = *p;
        while (**p != ':' && **p != '\0') (*p)++;
        if (**p == ':') {
            ASTNode *if_node = create_node(NODE_IF, "if");
            ASTNode *condition_node = create_node(NODE_CONDITION, "");
            strncpy(condition_node->value, start, *p - start);
            condition_node->value[*p - start] = '\0';
            if_node->left = condition_node;
            (*p)++;
            while (isspace(**p)) (*p)++;
            if (strncmp(*p, "print", 5) == 0) {
                if_node->right = parse_print_statement(p);
            }
            return if_node;
        }
    }
    return create_node(NODE_UNKNOWN, "unknown");
}

ASTNode* parse(const char *code) {
    const char *p = code;
    if (strncmp(p, "if", 2) == 0) {
        return parse_if_statement(&p);
    } else if (strncmp(p, "print", 5) == 0) {
        return parse_print_statement(&p);
    }
    return create_node(NODE_UNKNOWN, "unknown");
}

// Define bytecode instructions
typedef enum {
    BC_PRINT_STRING,
    BC_UNKNOWN
} Bytecode;

typedef struct {
    Bytecode instruction;
    char operand[256];
} BytecodeInstruction;

BytecodeInstruction* generate_bytecode(ASTNode *node, int *size) {
    BytecodeInstruction *bytecode = (BytecodeInstruction*)malloc(sizeof(BytecodeInstruction) * 10);
    int index = 0;

    if (node->type == NODE_PRINT && node->left && node->left->type == NODE_STRING) {
        bytecode[index].instruction = BC_PRINT_STRING;
        strcpy(bytecode[index].operand, node->left->value);
        index++;
    } else if (node->type == NODE_IF && node->left && node->left->type == NODE_CONDITION) {
        // Simplified: Assume condition is always true for this example
        if (node->right && node->right->type == NODE_PRINT) {
            bytecode[index].instruction = BC_PRINT_STRING;
            strcpy(bytecode[index].operand, node->right->left->value);
            index++;
        }
    } else {
        bytecode[index].instruction = BC_UNKNOWN;
        strcpy(bytecode[index].operand, "unknown");
        index++;
    }

    *size = index;
    return bytecode;
}

void execute_bytecode(BytecodeInstruction *bytecode, int size) {
    for (int i = 0; i < size; i++) {
        if (bytecode[i].instruction == BC_PRINT_STRING) {
            printf("%s\n", bytecode[i].operand);
        } else {
            printf("Unknown bytecode instruction\n");
        }
    }
}

// The main function
int main() {
    const char *code = "if(2<3): print('2 é menor que 3!')";
    ASTNode *ast = parse(code);
    print_ast(ast, 0);

    int bytecode_size;
    BytecodeInstruction *bytecode = generate_bytecode(ast, &bytecode_size);
    execute_bytecode(bytecode, bytecode_size);

    free(bytecode);
    return 0;
}