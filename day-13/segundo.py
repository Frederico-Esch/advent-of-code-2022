from queue import PriorityQueue
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

class CustomList:
    __slots__ = ("list")
    def __init__(self, list):
        self.list = list

    def __eq__(self, other: 'CustomList') -> bool:
        return self.list == other.list

    def __lt__(self, other: 'CustomList') -> bool:
        return compare(self.list, other.list)
    def __gt__(self, other: 'CustomList') -> bool:
        return compare(other.list, self.list)
    def __ne__(self, other: 'CustomList') -> bool:
        return self.list != other.list
    def __le__(self, other: 'CustomList') -> bool:
        return compare(self.list, other.list)
    def __ge__(self, other: 'CustomList') -> bool:
        return compare(other.list, self.list)

listas = PriorityQueue()
for left, right in zip(lines[::2], lines[1::2]):
    pair += 1
    print(left)
    print(right)

    comparison = compare(eval(left), eval(right))
    print(comparison)
    if comparison:
        soma += pair
    print()
    listas.put(CustomList(eval(left)))
    listas.put(CustomList(eval(right)))

listas.put(CustomList([[2]]))
listas.put(CustomList([[6]]))


indexes = [0, 0]
i = 1
while indexes[0] == 0 or indexes[1] == 0:

    lista = listas.get()
    if lista == CustomList([[2]]):
        indexes[0] = i
    elif lista == CustomList([[6]]):
        indexes[1] = i
    i += 1

valor = indexes[0] * indexes[1]

print(f"{soma=}")
print(f"{valor=}")
file.close()