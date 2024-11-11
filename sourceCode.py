def saudar(nome):
    print("Olá, " + nome + "!")

def somar(a, b):
    return a + b

read = input("Digite seu nome: ")
saudar(read)
soma = input("Digite um número: ")
soma2 = input("Digite outro número: ")
print(somar(int(soma), int(soma2)))
