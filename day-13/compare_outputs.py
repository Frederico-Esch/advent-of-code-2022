cout = []
pyout = []

with open("saida.txt", "r") as c:
    with open("python_saida.txt", "r") as py:
        cout = c.readlines()
        pyout = py.readlines()

print(f"LINHAS: c = {len(cout)} | py = {len(pyout)}")
for c, p in zip(cout, pyout):
    if c != p:
        print(f"c  = {c.strip()}", sep='')
        print(f"py = {p.strip()}", sep='')
        print()