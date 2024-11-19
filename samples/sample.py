def calculate(x, y):
    result = x + y
    if result > 10:
        print("Result is greater than 10")
        return result
    else:
        print("Result is small")
        return result

calculate(1, 0)

def loop_test():
    for i in range(5):
        if i == 3:
            print("Found three!")
        else:
            print(i)
    
    count = 0
    while count < 3:
        print("Counting:", count)
        count = count + 1

# Teste de expressões e operadores
a = 5
b = 3
c = a + b * 2
d = (a + b) * 2

# Teste de operadores lógicos
if a > b and c > d:
    print("Complex condition met")

if not (a < b) or c == 16:
    print("Another condition")