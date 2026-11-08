# ============================================================
# 学生ID: 2405111B08
# 策略: S4
# 修改说明: 见 changelog.csv
# ============================================================

from typing import List, Optional


class Stirling2:
    def __init__(self, mod: Optional[int] = None):
        if mod is not None and mod <= 0:
            raise ValueError("mod must be a positive integer")
        self.mod = mod
        self._rows: List[List[int]] = [[1]]

    def _reduce(self, x: int) -> int:
        return x % self.mod if self.mod else x

    def row(self, n: int) -> List[int]:
        if not isinstance(n, int) or isinstance(n, bool):
            raise TypeError("n must be an integer")
        if n < 0:
            raise ValueError("n must be non-negative")
        while len(self._rows) <= n:
            i = len(self._rows)
            prev = self._rows[-1]
            cur = [0] * (i + 1)
            cur[0] = 0
            for k in range(1, i + 1):
                if k == i:
                    cur[k] = self._reduce(1)
                else:
                    cur[k] = self._reduce(k * prev[k] + prev[k - 1])
            self._rows.append(cur)
        return self._rows[n]

    def __call__(self, n: int, k: int) -> int:
        if not isinstance(n, int) or isinstance(n, bool) or not isinstance(k, int) or isinstance(k, bool):
            raise TypeError("n and k must be integers")
        if n < 0 or k < 0:
            raise ValueError("n and k must be non-negative")
        if k > n:
            return 0
        return self.row(n)[k]

    def triangle(self, n: int) -> List[List[int]]:
        self.row(n)
        return self._rows[:n + 1]


if __name__ == "__main__":
    test_cases = {
        (3, 2): 3,
        (4, 2): 7,
        (5, 3): 25,
        (6, 3): 90,
        (10, 4): 34105,
    }
    S = Stirling2()
    for (n, k), expected in test_cases.items():
        result = S(n, k)
        assert result == expected, f"S({n}, {k}) = {result}, expected {expected}"
        print(f"S({n}, {k}) = {result}")

    mod_one = Stirling2(mod=1)
    assert mod_one(3, 2) == 0