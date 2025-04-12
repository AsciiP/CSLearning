# 原码转为8位反码

N = 8
def Real2OnesComplement(real):
    if real[0] not in ['+' , '-']:
        real = '+' + real
    one = '0' if real[0] == '+' else '1'
    one = one + '0' * (N - len(real))
    one = one + real[1:len(real)]
    return one

def Reversal(one):
    reversal = one[0]
    for x in one[1:len(one)]:
        if x == '1':
            reversal = reversal + '0'
        else:
            reversal = reversal + '1'
    return reversal

def Real2TwosComplement(real):
    one = Real2OnesComplement(real)
    if one[0] == '0':
        return one
    else:
        return Reversal(one)
    
if __name__ == '__main__':
    real = input()
    two = Real2TwosComplement(real)
    print(f'{real} -> {two}')