match input().split(" "):
    case ("old", op, "old"):
        print(op)
    case ("old", op, num):
        print(op, int(num))
    case [*_]:
        print("NONE")
