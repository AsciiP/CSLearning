import random

def int_to_bits(value, length):
    """将整数转为长度为 length 的比特列表，下标 0 为最高位"""
    return [int(bit) for bit in format(value, f"0{length}b")]

def bits_to_string(bits):
    """将比特列表转为 0/1 字符串"""
    return "".join(str(b) for b in bits)

def bits_to_int(bits):
    """将比特列表转为整数"""
    return int(bits_to_string(bits), 2)

def poly_to_bits(poly):
    """将多项式整数转为比特列表。"""
    if poly == 0:
        return [0]

    length = poly.bit_length()
    return int_to_bits(poly, length)


def compute_crc(data_bits, poly_bits, r):
    """
    计算 CRC 校验码。
    data_bits: 待校验数据比特列表
    poly_bits: 生成多项式比特列表
    r: 校验位长度
    返回 r 位 CRC 校验码的比特列表
    """
    work = data_bits.copy()
    work += [0] * r

    poly_len = len(poly_bits)
    n = len(work)

    for i in range(n - poly_len + 1):
        if work[i] == 1:
            for j in range(poly_len):
                work[i + j] ^= poly_bits[j]

    return work[-r:]


def verify_crc(code_bits, poly_bits, r):
    """
    对码字做 CRC 校验。
    若余数全为 0，返回 True；否则返回 False。
    """
    work = code_bits.copy()
    poly_len = len(poly_bits)
    n = len(work)

    for i in range(n - poly_len + 1):
        if work[i] == 1:
            for j in range(poly_len):
                work[i + j] ^= poly_bits[j]

    return all(b == 0 for b in work[-r:])


def flip_random_bits(bits, count):
    """随机翻转 count 个不同位置的比特"""
    positions = random.sample(range(len(bits)), count)
    for pos in positions:
        bits[pos] ^= 1


def flip_consecutive_bits(bits, count):
    """随机翻转连续 count 个比特"""
    start = random.randint(0, len(bits) - count)
    for i in range(count):
        bits[start + i] ^= 1


def run_test(title, original_code, poly_bits, r, error_bits, consecutive, groups=10):
    print(title)
    print(
        f"多项式二进制: {bits_to_string(poly_bits)}, "
        f"校验位长度 r = {r}, "
        f"错误比特数 = {error_bits}, "
        f"{'连续错误' if consecutive else '随机错误'}"
    )

    for g in range(groups):
        test_code = original_code[:]

        if consecutive:
            flip_consecutive_bits(test_code, error_bits)
        else:
            flip_random_bits(test_code, error_bits)

        ok = verify_crc(test_code, poly_bits, r)
        print(f"第 {g + 1:2d} 组: 校验结果 = {'通过' if ok else '未通过'}")

    print()


if __name__ == "__main__":
    A_value = random.getrandbits(128)
    A = int_to_bits(A_value, 128)


    poly_crc8 = poly_to_bits(0b1010111)
    r_crc8 = 6

    poly_ff = poly_to_bits(0xFF)
    r_ff = 8

    A_shift8 = A + [0] * 8
    crc_bits = compute_crc(A_shift8, poly_crc8, r_crc8)
    B_value = bits_to_int(crc_bits)

    C = A_shift8 + crc_bits

    print("原始数据 A:      ", bits_to_string(A))
    print("A << 8:          ", bits_to_string(A_shift8))
    print(f"CRC 校验码 B:    {B_value:08b} (十进制 {B_value})")
    print("码字 C:          ", bits_to_string(C))
    print()

    for k in [1, 3, 5]:
        run_test(
            f"(1) 随机修改 {k} 个比特 (CRC-8)",
            C, poly_crc8, r_crc8, k, False
        )
        run_test(
            f"(1) 随机修改 {k} 个比特 (0xFF)",
            C, poly_ff, r_ff, k, False
        )

    run_test("(2) 随机修改 2 个比特 (CRC-8)", C, poly_crc8, r_crc8, 2, False)
    run_test("(2) 随机修改 2 个比特 (0xFF)", C, poly_ff, r_ff, 2, False)

    # (3) 随机修改 6 个连续比特
    run_test("(3) 随机修改 6 个连续比特 (CRC-8)", C, poly_crc8, r_crc8, 6, True)
    run_test("(3) 随机修改 6 个连续比特 (0xFF)", C, poly_ff, r_ff, 6, True)

    # (4) 随机修改 10 个连续比特
    run_test("(4) 随机修改 10 个连续比特 (CRC-8)", C, poly_crc8, r_crc8, 10, True)
    run_test("(4) 随机修改 10 个连续比特 (0xFF)", C, poly_ff, r_ff, 10, True)

