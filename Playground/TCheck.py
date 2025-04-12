k, n = map(int, input().split(' '))

def foo(k, n):
    seen = {}
    sequence = []
    step = 0
    while step < n:
        if k in seen:
            T_start = seen[k]
            T_length = step - T_start
            remaining_steps = (n - step) % T_length
            return sequence[T_start + remaining_steps]
        seen[k] = step
        sequence.append(k)
        if k % 2 == 0:
            k = (k + 4) // 3
        else:
            k *= 2
        step += 1
    return k
print(foo(k,n))