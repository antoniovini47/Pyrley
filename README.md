# Pyrley - The Varley Python Compiler

## Como Executar o Projeto

Este projeto utiliza o Flex para análise léxica e o Bison para análise sintática. Siga os passos abaixo para gerar os arquivos necessários e compilar o projeto.

### Pré-requisitos

Certifique-se de ter o Flex e o Bison instalados em seu sistema. Você pode instalá-los usando os seguintes comandos:

```sh
sudo apt-get install flex bison

1. Gere o arquivo lex.yy.c a partir do arquivo lexico.l usando o Flex:
flex [lexico.l](http://_vscodecontentref_/1)

2. Gere o arquivo y.tab.c a partir do arquivo sintatico.y usando o Bison:
bison -d [sintatico.y](http://_vscodecontentref_/2)

3. Compile os arquivos gerados (lex.yy.c e syntacticName.tab.c) junto com o arquivo syntacticName.tab.h:
gcc lex.yy.c syntacticName.tab.c -o parser -lfl

4. Execute o parser:
./parser < pythonCodeSample.py


