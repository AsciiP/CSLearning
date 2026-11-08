from collections import defaultdict


grammar = {
    "S": [["a"], ["ε"], ["(", "T", ")"]],
    "T": [["S", "T'"]],
    "T'": [[",", "S", "T'"], ["ε"]],
}

start_symbol = "S"
non_terminals = set(grammar)
terminals = {
    symbol
    for productions in grammar.values()
    for production in productions
    for symbol in production
    if symbol not in non_terminals and symbol != "ε"
}
terminals.add("$")

FIRST = defaultdict(set)
FOLLOW = defaultdict(set)
FOLLOW[start_symbol].add("$")


def first_of_string(symbols):
    result = set()
    for symbol in symbols:
        if symbol == "ε":
            result.add("ε")
            return result
        if symbol not in non_terminals:
            result.add(symbol)
            return result
        result.update(FIRST[symbol] - {"ε"})
        if "ε" not in FIRST[symbol]:
            return result
    result.add("ε")
    return result


def calculate_first():
    changed = True
    while changed:
        changed = False
        for left, productions in grammar.items():
            for production in productions:
                before = len(FIRST[left])
                FIRST[left].update(first_of_string(production))
                changed |= len(FIRST[left]) != before


def calculate_follow():
    changed = True
    while changed:
        changed = False
        for left, productions in grammar.items():
            for production in productions:
                for index, symbol in enumerate(production):
                    if symbol not in non_terminals:
                        continue
                    beta = production[index + 1:]
                    first_beta = first_of_string(beta)
                    before = len(FOLLOW[symbol])
                    FOLLOW[symbol].update(first_beta - {"ε"})
                    if not beta or "ε" in first_beta:
                        FOLLOW[symbol].update(FOLLOW[left])
                    changed |= len(FOLLOW[symbol]) != before


def build_table():
    table = defaultdict(dict)
    for left, productions in grammar.items():
        for production in productions:
            first = first_of_string(production)
            lookaheads = (first - {"ε"}) | (
                FOLLOW[left] if "ε" in first else set()
            )
            for terminal in lookaheads:
                if terminal in table[left]:
                    print(f"警告：M[{left}, {terminal}] 存在冲突")
                table[left][terminal] = production
    return table


def production_text(left, production):
    return f"{left} -> {' '.join(production)}"


def print_sets():
    print("\n========== FIRST 集 ==========")
    for symbol in grammar:
        print(f"FIRST({symbol}) = {{ {', '.join(sorted(FIRST[symbol]))} }}")
    print("\n========== FOLLOW 集 ==========")
    for symbol in grammar:
        print(f"FOLLOW({symbol}) = {{ {', '.join(sorted(FOLLOW[symbol]))} }}")


def print_table(table):
    print("\n========== 预测分析表 ==========\n")
    columns = ["a", "ε", "(", ")", ",", "$"]
    print(f"{'':8}" + "".join(f"{column:^18}" for column in columns))
    print("-" * (8 + 18 * len(columns)))
    for left in grammar:
        nullable = next(
            (p for p in grammar[left] if "ε" in first_of_string(p)), None
        )
        cells = []
        for column in columns:
            production = nullable if column == "ε" else table[left].get(column)
            cells.append(production_text(left, production) if production else "")
        print(f"{left:<8}" + "".join(f"{cell:^18}" for cell in cells))


def analyze_input(table, input_tokens):
    print("\n========== 输入串分析 ==========")
    print("输入串：", " ".join(input_tokens))
    print()
    print(f"{'分析栈':<24}{'输入串':<24}所用规则/动作")
    print("-" * 72)

    stack = ["$", start_symbol]
    position = 0
    while True:
        stack_text = " ".join(stack)
        remaining = " ".join(input_tokens[position:])
        top = stack[-1]
        current = input_tokens[position]
        if top == current == "$":
            print(f"{stack_text:<24}{remaining:<24}接受")
            return
        if top not in non_terminals:
            if top != current:
                raise ValueError(f"分析失败：{top!r} 与 {current!r} 不匹配")
            stack.pop()
            position += 1
            action = f"匹配 {current}"
        else:
            production = table[top].get(current)
            if production is None:
                raise ValueError(f"分析失败：M[{top}, {current}] 为空")
            stack.pop()
            if production != ["ε"]:
                stack.extend(reversed(production))
            action = production_text(top, production)
        print(f"{stack_text:<24}{remaining:<24}{action}")

if __name__ == "__main__":
    calculate_first()
    calculate_follow()
    print_sets()
    table = build_table()
    print_table(table)
    analyze_input(table, ["(", "a", ",", "a", ")", "$"])