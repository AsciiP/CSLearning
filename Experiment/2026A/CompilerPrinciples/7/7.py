
import re

productions = {
    1: ('S', ['T', 'S']),
    2: ('S', ['A']),
    3: ('A', ['i', '=', 'E']),
    4: ('T', ['C', 'S', 'else']),
    5: ('C', ['if', "E'", 'then']),
    6: ("E'", ['i', '<', 'i']),
    7: ('E', ['E', '+', 'E']),
    8: ('E', ['i'])
}

ACTION = {
    0: {'i': 'S6', 'if': 'S4'},
    1: {'$': 'ACC'},
    2: {'i': 'S6', 'if': 'S4'},
    3: {'i': 'S6', 'if': 'S4'},
    4: {'i': 'S10'},
    5: {x: 'R2' for x in
        ['i', '=', 'if', 'then', 'else', '<', '+', '$']},
    6: {'=': 'S11'},
    7: {x: 'R1' for x in
        ['i', '=', 'if', 'then', 'else', '<', '+', '$']},
    8: {'else': 'S12'},
    9: {'then': 'S13'},
    10: {'<': 'S14'},
    11: {'i': 'S16'},
    12: {x: 'R4' for x in
         ['i', '=', 'if', 'then', 'else', '<', '+', '$']},
    13: {x: 'R5' for x in
         ['i', '=', 'if', 'then', 'else', '<', '+', '$']},
    14: {'i': 'S17'},
    15: {'else': 'R3', '+': 'S18', '$': 'R3'},
    16: {x: 'R8' for x in
         ['i', '=', 'if', 'then', 'else', '<', '+', '$']},
    17: {x: 'R6' for x in
         ['i', '=', 'if', 'then', 'else', '<', '+', '$']},
    18: {'i': 'S16'},
    19: {'else': 'R7', '+': 'R7', '$': 'R7'}
}

GOTO = {
    0: {'S': 1, 'A': 5, 'T': 2, 'C': 3},
    2: {'S': 7, 'A': 5, 'T': 2, 'C': 3},
    3: {'S': 8, 'A': 5, 'T': 2, 'C': 3},
    4: {"E'": 9},
    11: {'E': 15},
    18: {'E': 19}
}

def tokenize(source):
    pattern = r'if|then|else|[A-Za-z_]\w*|\d+|[=<>+]'
    raw_tokens = re.findall(pattern, source)

    remainder = re.sub(pattern, '', source)
    if remainder.strip():
        raise ValueError(f'存在非法字符：{remainder}')

    tokens = []
    for token in raw_tokens:
        if token in ('if', 'then', 'else'):
            tokens.append(token)
        elif token in ('=', '<', '>', '+'):
            tokens.append(token)
        else:
            tokens.append('i')

    tokens.append('$')
    return tokens


def lr_parse(source):
    tokens = tokenize(source)
    state_stack = [0]
    symbol_stack = []

    pos = 0

    while True:
        state = state_stack[-1]
        current = tokens[pos]

        action = ACTION.get(state, {}).get(current)

        if action is None:
            return False

        if action == 'ACC':
            return True

        if action.startswith('S'):
            next_state = int(action[1:])
            symbol_stack.append(current)
            state_stack.append(next_state)
            pos += 1

        elif action.startswith('R'):
            number = int(action[1:])
            lhs, rhs = productions[number]
            length = len(rhs)

            if length:
                del symbol_stack[-length:]
                del state_stack[-length:]

            next_state = GOTO.get(
                state_stack[-1], {}
            ).get(lhs)

            if next_state is None:
                return False

            symbol_stack.append(lhs)
            state_stack.append(next_state)

        else:
            return False


if __name__ == '__main__':
    try:
        print('success' if lr_parse(input()) else 'fail')
    except (ValueError, IndexError, KeyError):
        print('fail')
