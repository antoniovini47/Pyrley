#!/bin/bash

COMPILER_DIR="compiler"
OUTPUT_DIR="output"
SAMPLES_DIR="samples"

OS="$(uname)"

# Função para exibir o menu animado
animated_menu() {
    TITLE="Pyrley"
    TITLE2="The final Python Compiler"
    SUBTITLE="Alunos: Pedro.h CarlinJS, Toin, Korea"

    # Array de cores para animação
    COLORS=("$RED" "$GREEN" "$YELLOW" "$BLUE" "$CYAN")

    # Função para centralizar texto
    center_text() {
        local text="$1"
        local term_width=$(tput cols)
        local text_width=${#text}
        local padding=$(( (term_width - text_width) / 2 ))
        printf "%${padding}s%s\n" "" "$text"
    }

    # Animação do título
    for ((i = 0; i < 10; i++)); do
        clear
        color="${COLORS[$((i % ${#COLORS[@]}))]}"
        echo -e "${color}"
        figlet "$TITLE" | while IFS= read -r line; do center_text "$line"; done
        figlet "$TITLE2" | while IFS= read -r line; do center_text "$line"; done
        sleep 0.2
    done

    # Exibe o subtítulo
    echo -e "${CYAN}"
    figlet "$SUBTITLE" | while IFS= read -r line; do center_text "$line"; done
    echo -e "${NC}"
    sleep 2
}

# Verifica se os diretórios necessários existem
if [ ! -d "$COMPILER_DIR" ]; then
    echo "Erro: Diretório $COMPILER_DIR não encontrado!"
    exit 1
fi

if [ ! -d "$SAMPLES_DIR" ]; then
    echo "Erro: Diretório $SAMPLES_DIR não encontrado!"
    exit 1
fi

# Cores para animação
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # Sem cor

# Flag para controlar a exibição da animação inicial
first_execution=true

# Salva o diretório inicial
BASE_DIR=$(pwd)

while true; do
    # Exibe o menu animado apenas na primeira execução
    if $first_execution; then
        animated_menu
        first_execution=false
    else
        clear
    fi

    # Retorna ao diretório base antes de listar os exemplos
    cd "$BASE_DIR" || exit 1

    # Lista os exemplos disponíveis
    echo "Arquivos disponíveis no diretório $SAMPLES_DIR:"
    samples=($(ls "$SAMPLES_DIR"))
    for i in "${!samples[@]}"; do
        echo "$i) ${samples[$i]}"
    done
    echo "q) Sair"

    # Solicita que o usuário escolha um exemplo
    read -p "Digite o número correspondente ao arquivo que deseja executar ou 'q' para sair: " choice
    if [[ "$choice" == "q" ]]; then
        echo "Saindo... Obrigado por usar o Pyrley!"
        exit 0
    fi

    if [[ ! "$choice" =~ ^[0-9]+$ ]] || [ "$choice" -lt 0 ] || [ "$choice" -ge "${#samples[@]}" ]; then
        echo "Seleção inválida!"
        continue
    fi

    SAMPLE_NAME="${samples[$choice]}"

    # Cria o diretório de saída
    mkdir -p "$OUTPUT_DIR"

    # Remove arquivos antigos no diretório de saída
    rm -f "$OUTPUT_DIR"/*

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
    read -p "Pressione Enter para retornar ao menu..."
done