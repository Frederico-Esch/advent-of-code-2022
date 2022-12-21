from functools import reduce

file = open("input", "r")

class Monkey:
    __slots__ = ("name", "items", "operation", "conditional", "inspections")
    def __init__(self, name, items, operation, conditional):
        self.name = name
        self.items = items
        match operation:
            case ("old", "*", "old"):
                self.operation = lambda x: x*x
            case ("old", "*", number):
                self.operation = lambda x: x*int(number)
            case ("old", "+", "old"):
                self.operation = lambda x: x+x
            case ("old", "+", number):
                self.operation = lambda x: x+int(number)
            case [*_]:
                print("Error")
                exit(0)
        self.conditional = conditional
        self.inspections = 0

    def inspect(self, monkeys):
        for item in self.items:
            #print(f"Monkey {self.name} item {item}")
            new = list(map(lambda x_mod: (self.operation(x_mod[0])%x_mod[1], x_mod[1]), item))
            #new = new//3
            if new[self.name][0] == 0:
                monkeys[self.conditional[1]].items.append(new)
            else:
                monkeys[self.conditional[2]].items.append(new)
            self.inspections+=1
        self.items = []

    def adapt_items(self, monkeys):
        self.items = list(map(lambda x: [(x%m.conditional[0], m.conditional[0]) for m in monkeys], self.items))

    def __repr__(self):
        return f"items: {self.items} | operation {self.operation} | val%{self.conditional[0]} == 0 ? monkey {self.conditional[1]} : monkey {self.conditional[2]}"

monkeys = []

lines = file.readlines()

for n in range(0, len(lines), 7):
    monkey_str = lines[n:n+6]
    name = int(monkey_str[0].split()[-1][:-1].strip())

    items = list(map(lambda x: int(x.strip()), monkey_str[1].split(": ")[1].split(",")))

    operation = list(map(lambda x: x.strip(), monkey_str[2].split("new = ")[1].split(" ")))

    conditional = int(monkey_str[3].split(" ")[-1].strip()), int(monkey_str[4].split(" ")[-1].strip()), int(monkey_str[5].split(" ")[-1].strip())

    monkeys.append(Monkey(name, items, operation, conditional))

for monkey in monkeys:
    monkey.adapt_items(monkeys)

for i in range(10000):
    for monkey in monkeys:
        monkey.inspect(monkeys)

result = reduce(lambda acc, x: acc*x, sorted(map(lambda x: x.inspections, monkeys))[-2:], 1)
print(result)

file.close()
