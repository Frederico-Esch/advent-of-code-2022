lines = None
stacks = [[] for i in range(9)]

class Operation:

    __slots__ = ("amount", "from_", "to")

    def __init__(self, initializer:list):
        self.amount, self.from_, self.to = initializer

    def __repr__(self):
        return f"amount: {self.amount} from: {self.from_} to: {self.to}"

with open("input", "r") as file:
    lines = file.readlines()

crate_lines = lines[:8]
for crates in crate_lines[::-1]:
    crates = list(map(lambda x: "[-]" if x.strip() == "" else x.strip(), crates.replace("\n", "").replace("]", "],").replace("    ", " ,").split(",")[:-1]))
    for idx, crate in enumerate(crates):
        if crate != "[-]":
            stacks[idx].append(crate)
    #print(crates, len(crates))

instructions = lines[10:]
for op in instructions:
    op = list(map(int, op.replace("move ", "").replace("from ", "").replace("to ", "").strip().split(" ")))
    op = Operation(op)
    crane = []
    for i in range(op.amount):
        crate = stacks[op.from_-1].pop()
        crane.append(crate)
    while crane:
        stacks[op.to-1].append(crane.pop())

answer = []

for stack in stacks:
    answer.append(stack.pop()[1:-1])

answer = "".join(answer)

print(answer)
