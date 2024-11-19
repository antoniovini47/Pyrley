#!/bin/bash

COMPILER_DIR="Compiler"
OUTPUT_DIR="output"
SAMPLES_DIR="samples"

OS="$(uname)"

animated_menu() {
    TITLE="Pyrley"
    TITLE2="The final Python Compiler"
    SUBTITLE="Alunos: Pedro.h CarlinJS, Toin, Korea"

    # Colors
    RED='\033[0;31m'
    GREEN='\033[0;32m'
    YELLOW='\033[0;33m'
    BLUE='\033[0;34m'
    CYAN='\033[0;36m'
    NC='\033[0m' # No Color

    # Array of colors for animation
    COLORS=("$RED" "$GREEN" "$YELLOW" "$BLUE" "$CYAN")

    # Function to center text
    center_text() {
        local text="$1"
        local term_width=$(tput cols)
        local text_width=${#text}
        local padding=$(( (term_width - text_width) / 2 ))
        printf "%${padding}s%s\n" "" "$text"
    }

    # Animate Title
    for ((i = 0; i < 10; i++)); do
        clear
        color="${COLORS[$((i % ${#COLORS[@]}))]}"
        echo -e "${color}"
        figlet "$TITLE" | while IFS= read -r line; do center_text "$line"; done
        figlet "$TITLE2" | while IFS= read -r line; do center_text "$line"; done
        sleep 0.2
    done

    # Display Subtitle
    echo -e "${CYAN}"
    figlet "$SUBTITLE" | while IFS= read -r line; do center_text "$line"; done
    echo -e "${NC}"
    sleep 2
}

# Check if required directories exist
if [ ! -d "$COMPILER_DIR" ]; then
    echo "Erro: Diretório $COMPILER_DIR não encontrado!"
    exit 1
fi

if [ ! -d "$SAMPLES_DIR" ]; then
    echo "Erro: Diretório $SAMPLES_DIR não encontrado!"
    exit 1
fi

# Display animated menu
animated_menu

# List available samples
echo "Arquivos disponíveis no diretório $SAMPLES_DIR:"
samples=($(ls "$SAMPLES_DIR"))
for i in "${!samples[@]}"; do
    echo "$i) ${samples[$i]}"
done

# Prompt user to select a sample
read -p "Digite o número correspondente ao arquivo que deseja executar: " choice
if [[ ! "$choice" =~ ^[0-9]+$ ]] || [ "$choice" -lt 0 ] || [ "$choice" -ge "${#samples[@]}" ]; then
    echo "Seleção inválida!"
    exit 1
fi

SAMPLE_NAME="${samples[$choice]}"

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