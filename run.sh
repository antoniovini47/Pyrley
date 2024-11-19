#!/bin/bash

COMPILER_DIR="Compiler"
OUTPUT_DIR="output"
SAMPLES_DIR="samples"

OS="$(uname)"

if [ "$#" -lt 1 ]; then
    echo "Uso: $0 <SAMPLE_NAME>"
    echo "Exemplo: $0 defAndGeneralSample.py"
    exit 1
fi

SAMPLE_NAME="$1"

if [ ! -d "$COMPILER_DIR" ]; then
    echo "Erro: Diretório $COMPILER_DIR não encontrado!"
    exit 1
fi

if [ ! -d "$SAMPLES_DIR" ]; then
    echo "Erro: Diretório $SAMPLES_DIR não encontrado!"
    exit 1
fi

if [ ! -f "$SAMPLES_DIR/$SAMPLE_NAME" ]; then
    echo "Erro: Arquivo de exemplo $SAMPLE_NAME não encontrado no diretório $SAMPLES_DIR!"
    exit 1
fi

mkdir -p "$OUTPUT_DIR"

rm -f $OUTPUT_DIR/*

echo "Navegando para o diretório de saída..."
cd "$OUTPUT_DIR" || exit 1

echo "Compilando o arquivo léxico..."
flex ../"$COMPILER_DIR"/grammar.l
if [ $? -ne 0 ]; then
    echo "Erro ao compilar o arquivo léxico!"
    exit 1
fi

echo "Compilando o arquivo sintático..."
bison -d ../"$COMPILER_DIR"/grammar.y
if [ $? -ne 0 ]; then
    echo "Erro ao compilar o arquivo sintático!"
    exit 1
fi

echo "Compilando os arquivos gerados pelo Flex e Bison..."

if [ "$OS" = "Darwin" ]; then
    # macOS
    gcc lex.yy.c grammar.tab.c -o parser -ll
elif [ "$OS" = "Linux" ]; then
    # Linux
    gcc lex.yy.c grammar.tab.c -o parser -lfl
else
    gcc lex.yy.c grammar.tab.c -o parser -ll
fi

if [ $? -ne 0 ]; then
    echo "Erro ao compilar o programa!"
    exit 1
fi

echo "Executando o exemplo $SAMPLE_NAME..."
./parser < ../"$SAMPLES_DIR"/"$SAMPLE_NAME"

echo "Processo concluído!"