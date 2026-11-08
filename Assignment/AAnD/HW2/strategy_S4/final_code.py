# ============================================================
# 学生ID: 2405111B08
# 策略: S4
# 修改说明: 见 changelog.csv
# ============================================================

"""平面最近点对 · 分治 O(n log n) 严格实现（基于 S4 AI 第三步改进版）。

设计要点:
1. 不做去重: 存在重复点时最近距离就是 0, 必须如实报告。
2. px / py 全程用索引而非坐标元组传递, 避免哈希集合分流导致的划分错位。
3. 合并步固定 delta = min(dL, dR), 严格按 "后继 7 邻居" 定理实现。
4. 距离全程用平方距离比较, 只在最终输出时开方一次。
5. 子问题规模 <= 32 时切换到暴力, 降低递归常数。
"""

from __future__ import annotations

import math
from typing import Sequence

INF = float("inf")
BRUTE_THRESHOLD = 32


class Result:
    __slots__ = ("d2", "i", "j")

    def __init__(self, d2: float, i: int, j: int) -> None:
        self.d2 = d2
        self.i = i
        self.j = j

    def __repr__(self) -> str:
        return f"Result(d={math.sqrt(self.d2)!r}, i={self.i}, j={self.j})"


def _sq_dist(ax: float, ay: float, bx: float, by: float) -> float:
    dx = ax - bx
    dy = ay - by
    return dx * dx + dy * dy


def _brute(xs: Sequence[float], ys: Sequence[float], idx: Sequence[int]) -> Result:
    """小规模暴力求解, 用于递归叶子, 同时天然覆盖 len(idx) < 2 的情况。"""
    best = INF
    bi = bj = -1
    k = len(idx)
    for a in range(k):
        ia = idx[a]
        for b in range(a + 1, k):
            ib = idx[b]
            d2 = _sq_dist(xs[ia], ys[ia], xs[ib], ys[ib])
            if d2 < best:
                best = d2
                bi, bj = ia, ib
                if best == 0.0:
                    return Result(0.0, bi, bj)
    return Result(best, bi, bj)


def _solve(
    xs: Sequence[float],
    ys: Sequence[float],
    px: list[int],
    py: list[int],
    tmp: list[int],
    lo: int,
    hi: int,
) -> Result:
    """
    递归求解 px[lo:hi] (按 x 有序) 的最近点对。
    前置不变量: py[lo:hi] 恰好是同一批点的索引, 且按 (y, x) 有序。
    后置副作用: py[lo:hi] 被原地归并排序回 (y, x) 序, 供上层直接使用,
    因此不需要每层重新构造 py —— 这是把复杂度锁死在 O(n log n) 的关键。
    """
    cnt = hi - lo

    if cnt <= BRUTE_THRESHOLD:
        res = _brute(xs, ys, px[lo:hi])
        py[lo:hi] = sorted(px[lo:hi], key=lambda i: (ys[i], xs[i], i))
        return res

    mid = (lo + hi) // 2
    midx = xs[px[mid]]

    dl = _solve(xs, ys, px, py, tmp, lo, mid)
    dr = _solve(xs, ys, px, py, tmp, mid, hi)

    cur = dl if dl.d2 <= dr.d2 else dr

    # ---- 原地归并: 让 py[lo:hi] 恢复 (y,x) 序, 代价 O(cnt) ----
    # 此时 py[lo:mid] 与 py[mid:hi] 各自已按 (y,x) 有序, 双指针合并即可。
    l, r, k = lo, mid, lo
    while l < mid and r < hi:
        a, b = py[l], py[r]
        if (ys[a], xs[a], a) <= (ys[b], xs[b], b):
            tmp[k] = a
            l += 1
        else:
            tmp[k] = b
            r += 1
        k += 1
    while l < mid:
        tmp[k] = py[l]
        l += 1
        k += 1
    while r < hi:
        tmp[k] = py[r]
        r += 1
        k += 1
    py[lo:hi] = tmp[lo:hi]

    if cur.d2 == 0.0:
        return cur

    delta = math.sqrt(cur.d2)   # 冻结为进入合并步时的 min(dL, dR)

    # ---- 构造 strip: py 已按 y 有序, 故 strip 自然按 y 有序 ----
    m = lo
    for t in range(lo, hi):
        idx = py[t]
        if abs(xs[idx] - midx) < delta:
            tmp[m] = idx
            m += 1

    # ---- 每个点最多检查随后 7 个点 ----
    for i in range(lo, m):
        ii = tmp[i]
        yi = ys[ii]
        limit = min(i + 7, m - 1)     # j - i <= 7, 且不越界
        for j in range(i + 1, limit + 1):
            jj = tmp[j]
            if ys[jj] - yi >= delta:
                break
            d2 = _sq_dist(xs[ii], ys[ii], xs[jj], ys[jj])
            if d2 < cur.d2:
                cur = Result(d2, ii, jj)
                if d2 == 0.0:
                    return cur

    return cur


def closest_pair(points: Sequence[Sequence[float]]):
    """
    主入口。points 为 n 个 (x, y)。返回 (最近距离, 点A, 点B)。
    时间 O(n log n), 空间 O(n)。
    """
    n = len(points)
    if n < 2:
        raise ValueError("最近点对至少需要 2 个点")

    xs = [float(p[0]) for p in points]
    ys = [float(p[1]) for p in points]

    order_x = sorted(range(n), key=lambda i: (xs[i], ys[i], i))
    order_y = sorted(range(n), key=lambda i: (ys[i], xs[i], i))

    px = order_x[:]
    py = order_y[:]
    tmp = [0] * n

    res = _solve(xs, ys, px, py, tmp, 0, n)

    a = (xs[res.i], ys[res.i])
    b = (xs[res.j], ys[res.j])
    return math.sqrt(res.d2), a, b


def format_result(points: Sequence[Sequence[float]]) -> str:
    """按题目要求的输出格式组织结果，点对内部先 x 后 y 升序。"""
    d, a, b = closest_pair(points)
    p1, p2 = (a, b) if (a[0], a[1]) <= (b[0], b[1]) else (b, a)
    return (
        f"最近距离: {d:.6f}\n"
        f"点1: ({int(p1[0])}, {int(p1[1])})\n"
        f"点2: ({int(p2[0])}, {int(p2[1])})"
    )


def _reference_bruteforce(points: Sequence[Sequence[float]]) -> float:
    best = INF
    n = len(points)
    for i in range(n):
        for j in range(i + 1, n):
            d = math.hypot(points[i][0] - points[j][0], points[i][1] - points[j][1])
            if d < best:
                best = d
    return best


if __name__ == "__main__":
    import sys

    sys.setrecursionlimit(10000)

    # 题目给定的 3 组测试用例
    test_cases = [
        ("测试用例 1（基础）", [(0, 0), (1, 1), (2, 2), (3, 3), (4, 4), (5, 5)]),
        ("测试用例 2（中等随机）",
         [(4, 7), (2, 1), (8, 3), (6, 9), (1, 4), (9, 8), (3, 5), (7, 2), (5, 6), (0, 0)]),
        ("测试用例 3（边界：n=2）", [(0, 0), (3, 4)]),
    ]
    for title, pts in test_cases:
        print(title)
        print(format_result(pts))
        assert math.isclose(closest_pair(pts)[0], _reference_bruteforce(pts), rel_tol=1e-9)
        print()

    # 退化与边界场景
    assert math.isclose(closest_pair([(0.0, float(i)) for i in range(500)])[0], 1.0)
    assert math.isclose(closest_pair([(float(i), 7.5) for i in range(500)])[0], 1.0)
    assert closest_pair([(1, 2), (5, 6), (1, 2)])[0] == 0.0
    assert math.isclose(
        closest_pair([(float(x), float(y)) for x in range(60) for y in range(60)])[0], 1.0)

    print("[OK] 3 组测试用例与退化场景（同 x / 同 y / 重复点 / 网格）全部通过")
