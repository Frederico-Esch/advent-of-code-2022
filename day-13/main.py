from itertools import zip_longest

file = open("input.txt", "r")
lines = list(map( lambda x: x.strip(), filter(lambda x: len(x) > 1, file.readlines())))

def compare(left, right):
    for l, r in zip_longest(left, right, fillvalue=-1):
        if l == -1: return True
        if r == -1: return False

        if type(l) is int and type(r) is int:
            if l < r: return True
            if r < l: return False
        else:
            if type(l) is int: l = [l]
            if type(r) is int: r = [r]

            ret = compare(l, r)
            if type(ret) is bool:
                return ret

pair = 0
soma = 0
saida = open("python_saida.txt", "+w")
for left, right in zip(lines[::2], lines[1::2]):
    pair += 1
    print(left)
    print(right)

    comparison = compare(eval(left), eval(right))
    print(comparison)
    if comparison:
        soma += pair
    print()

    saida.write(f"pair {pair} RESULT {int(comparison)}\n")

print(f"{soma=}")
saida.close()
file.close()