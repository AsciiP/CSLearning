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


def calculate_vt(reverse=False):
    result = {nt: set() for nt in non_terminals}
    changed = True

    while changed:
        changed = False

        for A in grammar:
            for production in grammar[A]:
                symbols = production[::-1] if reverse else production
                X = symbols[0]
                if X in terminals and X != '$':
                    if X not in result[A]:
                        result[A].add(X)
                        changed = True
                elif X in non_terminals:
                    if len(symbols) >= 2:
                        Y = symbols[1]
                        if Y in terminals and Y != '$':
                            if Y not in result[A]:
                                result[A].add(Y)
                                changed = True
                    for symbol in result[X]:
                        if symbol not in result[A]:
                            result[A].add(symbol)
                            changed = True

    return result


def calculate_firstvt():
    return calculate_vt()


def calculate_lastvt():
    return calculate_vt(reverse=True)


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
                elif X in terminals and Y in non_terminals:
                    for b in firstvt[Y]:
                        add_relation(X, '<', b)
                elif X in non_terminals and Y in terminals:
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


def analyze(input_string, table):
    tokens = []
    i = 0

    try:
        while i < len(input_string):
            if input_string[i].isspace():
                i += 1
            elif input_string.startswith('id', i):
                tokens.append('id')
                i += 2
            elif input_string[i] in '+*()$':
                tokens.append(input_string[i])
                i += 1
            else:
                raise ValueError(f'存在非法字符：{input_string[i]}')
    except ValueError as error:
        print(f'分析失败：{error}')
        return False

    if not tokens:
        print('分析失败：输入串不能为空。')
        return False
    if tokens[-1] != '$':
        tokens.append('$')
    if tokens.count('$') > 1:
        print('分析失败：输入串只能包含一个结束符。')
        return False

    stack = ['$']
    pos = 0
    step = 1

    print('\n输入串：', ''.join(tokens))
    print('\n算符优先分析过程')
    print('-' * 82)
    print(
        f'{"步骤":<6}{"S栈":<20}'
        f'{"优先关系":<12}{"当前符号":<12}'
        f'{"输入流":<18}{"动作"}'
    )
    print('-' * 82)

    def show(relation, current, action):
        nonlocal step
        remaining = ''.join(tokens[pos:])
        print(
            f'{step:<6}{"".join(stack):<20}'
            f'{relation:<12}{current:<12}'
            f'{remaining:<18}{action}'
        )
        step += 1

    while True:
        current = tokens[pos]

        top_index = next(
            (i for i in range(len(stack) - 1, -1, -1)
             if stack[i] in terminals),
            None
        )

        if top_index is None:
            print('分析失败：栈中没有终结符。')
            return False

        top_terminal = stack[top_index]
        relation = table[top_terminal][current]

        if stack == ['$', 'N'] and current == '$':
            show('=', current, '结束')
            print('\n分析成功，输入串符合该文法。')
            return True

        if relation in ('<', '='):
            show(relation, current, '移进')
            stack.append(current)
            pos += 1

        elif relation == '>':
            terminal_positions = [
                i for i, symbol in enumerate(stack)
                if symbol in terminals
            ]

            boundary = None

            for k in range(len(terminal_positions) - 2, -1, -1):
                left = terminal_positions[k]
                right = terminal_positions[k + 1]

                rel = table[stack[left]][stack[right]]

                if rel == '<':
                    boundary = left
                    break

            if boundary is None:
                print('分析失败：无法确定句柄边界。')
                return False

            handle = stack[boundary + 1:]
            handle_text = ''.join(handle)
            reductions = {
                ('id',),
                ('N',),
                ('N', '+', 'N'),
                ('N', '*', 'N'),
                ('(', 'N', ')')
            }

            if tuple(handle) in reductions:
                show(relation, current, f'归约 {handle_text} 为 N')
                stack = stack[:boundary + 1]
                stack.append('N')
            else:
                print(f'分析失败：无法归约句柄 {handle_text}')
                return False

        else:
            show('无关系', current, '分析失败')
            return False


if __name__ == '__main__':
    firstvt = calculate_firstvt()
    lastvt = calculate_lastvt()

    print('FIRSTVT：')
    for nt in ['E', 'T', 'F']:
        print(f'{nt} = {sorted(firstvt[nt])}')

    print('\nLASTVT：')
    for nt in ['E', 'T', 'F']:
        print(f'{nt} = {sorted(lastvt[nt])}')

    table = build_precedence_table(firstvt, lastvt)
    print_table(table)

    analyze('id+id$', table)