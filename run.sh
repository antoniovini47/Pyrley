#!/bin/bash

COMPILER_DIR="compiler"
OUTPUT_DIR="output"
SAMPLES_DIR="samples"

OS="$(uname)"

if [ ! -d "$COMPILER_DIR" ]; then
    echo "Erro: Diretório $COMPILER_DIR não encontrado!"
    exit 1
fi

if [ ! -d "$SAMPLES_DIR" ]; then
    echo "Erro: Diretório $SAMPLES_DIR não encontrado!"
    exit 1
fi

mkdir -p "$OUTPUT_DIR"

rm -f $OUTPUT_DIR/*

cd $OUTPUT_DIR

echo "Compilando o arquivo léxico..."
flex ../$COMPILER_DIR/lexico.l
if [ $? -ne 0 ]; then
    echo "Erro ao compilar o arquivo léxico!"
    exit 1
fi

echo "Compilando o arquivo sintático..."
bison -d ../$COMPILER_DIR/sintatico.y
if [ $? -ne 0 ]; then
    echo "Erro ao compilar o arquivo sintático!"
    exit 1
fi

echo "Compilando os arquivos gerados pelo Flex e Bison..."

if [ "$OS" = "Darwin" ]; then
    # macOS
    gcc lex.yy.c sintatico.tab.c -o parser -ll
elif [ "$OS" = "Linux" ]; then
    # Linux
    gcc lex.yy.c sintatico.tab.c -o parser -lfl
else
    gcc lex.yy.c sintatico.tab.c -o parser -ll
fi

if [ $? -ne 0 ]; then
    echo "Erro ao compilar o programa!"
    exit 1
fi

echo "Executando o exemplo..."
./parser < ../$SAMPLES_DIR/pythonCodeSample.py

echo "Processo concluído!"