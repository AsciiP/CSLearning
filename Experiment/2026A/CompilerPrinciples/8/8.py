import re

productions = {
    1: ('S', ['T', 'S']), 2: ('S', ['A']), 3: ('A', ['i', '=', 'E']),
    4: ('T', ['C', 'S', 'else']), 5: ('C', ['if', "E'", 'then']),
    6: ("E'", ['i', '<', 'i']), 7: ('E', ['E', '+', 'E']), 8: ('E', ['i'])
}
symbols = ['i', '=', 'if', 'then', 'else', '<', '+', '$']
ACTION = {
    0: {'i': 'S6', 'if': 'S4'}, 1: {'$': 'ACC'},
    2: {'i': 'S6', 'if': 'S4'}, 3: {'i': 'S6', 'if': 'S4'},
    4: {'i': 'S10'}, 5: {x: 'R2' for x in symbols}, 6: {'=': 'S11'},
    7: {x: 'R1' for x in symbols}, 8: {'else': 'S12'}, 9: {'then': 'S13'},
    10: {'<': 'S14'}, 11: {'i': 'S16'}, 12: {x: 'R4' for x in symbols},
    13: {x: 'R5' for x in symbols}, 14: {'i': 'S17'},
    15: {'else': 'R3', '+': 'S18', '$': 'R3'},
    16: {x: 'R8' for x in symbols}, 17: {x: 'R6' for x in symbols},
    18: {'i': 'S16'}, 19: {'else': 'R7', '+': 'R7', '$': 'R7'}
}
GOTO = {
    0: {'S': 1, 'A': 5, 'T': 2, 'C': 3},
    2: {'S': 7, 'A': 5, 'T': 2, 'C': 3},
    3: {'S': 8, 'A': 5, 'T': 2, 'C': 3},
    4: {"E'": 9}, 11: {'E': 15}, 18: {'E': 19}
}
DEFAULT_SOURCE = 'if x < 6 then x=x+2 else x=x+1'
START_LABEL = 100
nextq, quads, labels, symbol_table, temp_count = START_LABEL, [], {}, {}, 0


class Quad:
    def __init__(self, label, kind, fields, target=0):
        self.label, self.kind, self.fields, self.target = label, kind, fields, target

    def text(self):
        if self.kind == 'cond':
            left, op, right = self.fields
            return f'if {left} {op} {right} goto {self.target}'
        if self.kind == 'goto':
            return f'goto {self.target}'
        if self.kind == 'bin':
            result, left, op, right = self.fields
            return f'{result}={left}{op}{right}'
        if self.kind == 'copy':
            result, source = self.fields
            return f'{result}={source}'
        raise ValueError(f'未知的四元式类型：{self.kind}')


def reset():
    global nextq, quads, labels, symbol_table, temp_count
    nextq, quads, labels, symbol_table, temp_count = START_LABEL, [], {}, {}, 0


def newtemp():
    global temp_count
    temp_count += 1
    return f'T{temp_count}'


def emit(kind, fields):
    global nextq
    quad = Quad(nextq, kind, fields)
    quads.append(quad)
    labels[nextq] = quad
    nextq += 1
    return quad.label


def lookup(name):
    return symbol_table.get(name)


def entry(name):
    symbol_table.setdefault(name, len(symbol_table) + 1)
    return symbol_table[name]


def backpatch(p, t):
    while p:
        quad = labels[p]
        p, quad.target = quad.target, t


def merge(p1, p2):
    if not p1:
        return p2
    quad = labels[p1]
    while quad.target:
        quad = labels[quad.target]
    quad.target = p2
    return p1


def tokenize(source):
    source = source.replace('\ufeff', '').replace('\r', '')
    pattern = r'if|then|else|[A-Za-z_]\w*|\d+|[=<>+]'
    raw = re.findall(pattern, source)
    remainder = re.sub(pattern, '', source)
    if remainder.strip():
        raise ValueError(f'非法字符：{remainder}')
    tokens = []
    for word in raw:
        if word in ('if', 'then', 'else', '=', '<', '+'):
            category = word
        elif re.fullmatch(r'[A-Za-z_]\w*|\d+', word):
            category = 'i'
        else:
            raise ValueError(f'无法识别的词素：{word}')
        tokens.append((category, word))
    tokens.append(('$', '$'))
    return tokens


def semantic_action(number, values):
    if number == 1:
        t, s = values
        return {'chain': merge(t['chain'], s['chain'])}
    if number == 2:
        return {'chain': 0}
    if number == 3:
        name, _, expression = values
        if lookup(name) is None:
            entry(name)
        emit('copy', (name, expression['place']))
        return {'chain': 0}
    if number == 4:
        c, s, _ = values
        q = emit('goto', ())
        backpatch(c['chain'], nextq)
        return {'chain': merge(s['chain'], q)}
    if number == 5:
        _, condition, _ = values
        backpatch(condition['true'], nextq)
        return {'chain': condition['false']}
    if number == 6:
        left, _, right = values
        true, false = nextq, nextq + 1
        emit('cond', (left, '<', right))
        emit('goto', ())
        return {'true': true, 'false': false}
    if number == 7:
        e1, _, e2 = values
        place = newtemp()
        emit('bin', (place, e1['place'], '+', e2['place']))
        return {'place': place}
    if number == 8:
        name = values[0]
        if lookup(name) is None:
            entry(name)
        return {'place': name}
    raise ValueError(f'未知产生式：{number}')


def intermediate_code():
    return [f'{q.label}  {q.text()}' for q in quads] + [str(nextq)]


def lr_parse(source):
    reset()
    tokens, states, semantic, pos = tokenize(source), [0], [], 0
    while True:
        state, current, lexeme = states[-1], tokens[pos][0], tokens[pos][1]
        action = ACTION.get(state, {}).get(current)
        if action is None:
            raise ValueError(f'语法错误：状态 {state} 遇到符号 {lexeme}')
        if action == 'ACC':
            if len(semantic) != 1:
                raise ValueError('语义栈状态异常')
            backpatch(semantic[0]['chain'], nextq)
            return intermediate_code()
        if action.startswith('S'):
            semantic.append(tokens[pos][1])
            states.append(int(action[1:]))
            pos += 1
            continue
        number = int(action[1:])
        lhs, rhs = productions[number]
        length = len(rhs)
        values = semantic[-length:]
        del semantic[-length:], states[-length:]
        result = semantic_action(number, values)
        next_state = GOTO.get(states[-1], {}).get(lhs)
        if next_state is None:
            raise ValueError(f'GOTO表缺少状态 {states[-1]} 对应的 {lhs}')
        semantic.append(result)
        states.append(next_state)


if __name__ == '__main__':
    try:
        try:
            source = input()
        except EOFError:
            source = ''
        if not source.strip():
            source = DEFAULT_SOURCE
        code = lr_parse(source)
        for line in code:
            print(line)
    except (ValueError, IndexError) as error:
        print(f'语法分析失败：{error}')
