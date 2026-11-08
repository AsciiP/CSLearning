# ============================================================
# 学生ID: 2405111B08
# 策略: S1
# 修改说明: 见 changelog.csv
# ============================================================

def stirling(n, k):
    if n == k or k == 1:
        return 1
    elif n == 0 or k == 0:
        return 0
    return k * stirling(n - 1, k) + stirling(n - 1, k - 1)


if __name__ == "__main__":
    test_cases = [(3, 2), (4, 2), (5, 3), (6, 3), (10, 4)]
    for n, k in test_cases:
        print(f"S({n}, {k}) = {stirling(n, k)}")
