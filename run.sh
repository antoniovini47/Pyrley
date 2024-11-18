#!/bin/bash

if [ ! -f "lexico.l" ]; then
  echo "Erro: Arquivo léxico (lexico.l) não encontrado!"
  exit 1
fi

if [ ! -f "sintatico.y" ]; then
  echo "Erro: Arquivo sintático (sintatico.y) não encontrado!"
  exit 1
fi

if [ ! -f "pythonCodeSample.py" ]; then
  echo "Erro: Arquivo de exemplo pythonCodeSample.py não encontrado!"
  exit 1
fi

echo "Compilando o arquivo léxico..."
flex lexico.l
if [ $? -ne 0 ]; then
  echo "Erro ao compilar o arquivo léxico!"
  exit 1
fi

echo "Compilando o arquivo sintático..."
bison -d sintatico.y
if [ $? -ne 0 ]; then
  echo "Erro ao compilar o arquivo sintático!"
  exit 1
fi

echo "Compilando os arquivos gerados pelo Flex e Bison..."
gcc lex.yy.c sintatico.tab.c -o parser -ll
if [ $? -ne 0 ]; then
  echo "Erro ao compilar o programa!"
  exit 1
fi

echo "Executando o exemplo..."
./parser < pythonCodeSample.py
if [ $? -ne 0 ]; then
  echo "Erro ao executar o compilador!"
  exit 1
fi

echo "Processo concluído com sucesso!"