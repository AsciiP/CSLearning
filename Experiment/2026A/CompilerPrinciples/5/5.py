grammar = {
    'E': [
        ['E', '+', 'T'],
        ['T']
    ],
    'T': [
        ['T', '*', 'F'],
        ['F']
    ],
    'F': [
        ['(', 'E', ')'],
        ['id']
    ]
}
non_terminals = set(grammar.keys())
terminals = {'+', '*', 'id', '(', ')', '$'}


def calculate_firstvt():
    firstvt = {nt: set() for nt in non_terminals}
    changed = True
    while changed:
        changed = False
        for A in grammar:
            for production in grammar[A]:
                if len(production) >= 1:
                    X = production[0]
                    if X in terminals and X != '$':
                        if X not in firstvt[A]:
                            firstvt[A].add(X)
                            changed = True
                    elif X in non_terminals:
                        if len(production) >= 2:
                            Y = production[1]
                            if Y in terminals and Y != '$':
                                if Y not in firstvt[A]:
                                    firstvt[A].add(Y)
                                    changed = True
                        for x in firstvt[X]:
                            if x not in firstvt[A]:
                                firstvt[A].add(x)
                                changed = True
    return firstvt


def calculate_lastvt():
    lastvt = {nt: set() for nt in non_terminals}
    changed = True
    while changed:
        changed = False
        for A in grammar:
            for production in grammar[A]:
                if len(production) >= 1:
                    X = production[-1]
                    if X in terminals and X != '$':
                        if X not in lastvt[A]:
                            lastvt[A].add(X)
                            changed = True
                    elif X in non_terminals:
                        if len(production) >= 2:
                            Y = production[-2]
                            if Y in terminals and Y != '$':
                                if Y not in lastvt[A]:
                                    lastvt[A].add(Y)
                                    changed = True
                        for x in lastvt[X]:
                            if x not in lastvt[A]:
                                lastvt[A].add(x)
                                changed = True
    return lastvt


def build_precedence_table(firstvt, lastvt):
    table = {
        row: {col: '' for col in terminals}
        for row in terminals
    }

    def add_relation(a, relation, b):
        if table[a][b] != '' and table[a][b] != relation:
            print(
                f"警告：发现冲突 {a} {table[a][b]} {b} "
                f"与 {a} {relation} {b}"
            )
        table[a][b] = relation
    for A in grammar:
        for production in grammar[A]:
            for i in range(len(production) - 1):
                X = production[i]
                Y = production[i + 1]
                if X in terminals and Y in terminals:
                    add_relation(X, '=', Y)
                if X in terminals and Y in non_terminals:
                    for b in firstvt[Y]:
                        add_relation(X, '<', b)
                if X in non_terminals and Y in terminals:
                    for a in lastvt[X]:
                        add_relation(a, '>', Y)
            for i in range(len(production) - 2):
                X = production[i]
                Y = production[i + 1]
                Z = production[i + 2]
                if X in terminals and Y in non_terminals and Z in terminals:
                    add_relation(X, '=', Z)
    for x in firstvt['E']:
        add_relation('$', '<', x)
    for x in lastvt['E']:
        add_relation(x, '>', '$')
    add_relation('$', '=', '$')
    return table


def print_sets(firstvt, lastvt):
    print("=" * 60)
    print("FIRSTVT 和 LASTVT")
    print("=" * 60)
    print(f"{'非终结符':<12}{'FIRSTVT':<30}{'LASTVT'}")
    print("-" * 60)
    order = ['E', 'T', 'F']
    for nt in order:
        first = ', '.join(sorted(firstvt[nt]))
        last = ', '.join(sorted(lastvt[nt]))
        print(f"{nt:<12}{{ {first} }}".ljust(42) +
              f"{{ {last} }}")


def print_table(table):
    symbols = ['+', '*', 'id', '(', ')', '$']
    print("\n")
    print("=" * 70)
    print("算符优先关系表")
    print("=" * 70)
    print(f"{'':>8}", end='')
    for symbol in symbols:
        print(f"{symbol:>8}", end='')
    print()
    print("-" * 56)
    for row in symbols:
        print(f"{row:>8}", end='')
        for col in symbols:
            relation = table[row][col]
            if relation == '':
                relation = ' '
            print(f"{relation:>8}", end='')
        print()


if __name__ == '__main__':
    firstvt = calculate_firstvt()
    lastvt = calculate_lastvt()
    print_sets(firstvt, lastvt)
    table = build_precedence_table(firstvt, lastvt)
    print_table(table)
