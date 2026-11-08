# ============================================================
# 学生ID: 2405111B08
# 策略: S3
# 修改说明: 见 changelog.csv
# ============================================================

"""
平面最近点对问题 —— 分治算法 O(n log n)
全程使用「距离平方」比较，最后再开方一次。
"""

from __future__ import annotations

import math
from typing import List, Sequence, Tuple

Point = Tuple[float, float]
Result = Tuple[float, Point, Point]


# ----------------------------------------------------------------------
# 工具函数
# ----------------------------------------------------------------------
def _dist_sq(a: Point, b: Point) -> float:
    """两点欧氏距离的平方（避免开方，减少误差与开销）。"""
    dx = a[0] - b[0]
    dy = a[1] - b[1]
    return dx * dx + dy * dy


def _brute_force(pts: Sequence[Point]) -> Result:
    """暴力枚举，仅用于递归出口和小规模正确性对拍。O(k^2)"""
    best = math.inf
    ba = bb = None
    k = len(pts)
    for i in range(k):
        for j in range(i + 1, k):
            d2 = _dist_sq(pts[i], pts[j])
            if d2 < best:
                best, ba, bb = d2, pts[i], pts[j]
    return (best, ba, bb)


# ----------------------------------------------------------------------
# 分治核心
# ----------------------------------------------------------------------
def _solve(px: List[Point], py: List[Point]) -> Result:
    """
    px: 当前子集按 x 升序（并列按 y 升序）
    py: 当前子集按 y 升序（并列按 x 升序）
    返回 (最小距离平方, 点A, 点B)
    """
    m = len(px)

    # ---------- 递归出口：<=3 个点直接暴力 ----------
    if m <= 3:
        return _brute_force(px)

    # ---------- Divide ----------
    mid = m >> 1
    qx = px[:mid]                 # 左半
    rx = px[mid:]                 # 右半
    mid_x = px[mid][0]            # 分割线 x = mid_x

    # ---------- 线性拆分 py，两侧各自保持 y 有序 ----------
    left_ids = {id(p) for p in qx}          # 用对象 id 做 O(1) 成员判定
    qy: List[Point] = []
    ry: List[Point] = []
    for p in py:
        (qy if id(p) in left_ids else ry).append(p)

    # ---------- Conquer ----------
    d1, a1, b1 = _solve(qx, qy)
    d2, a2, b2 = _solve(rx, ry)

    if d1 <= d2:
        delta_sq, best = d1, (a1, b1)
    else:
        delta_sq, best = d2, (a2, b2)

    # ---------- Merge：带状区域内每个点最多向后看 7 个点 ----------
    delta = math.sqrt(delta_sq)
    strip: List[Point] = [p for p in py if abs(p[0] - mid_x) < delta]

    n_strip = len(strip)
    for i in range(n_strip):
        pi = strip[i]
        # 显式限制最多检查后续 7 个点（索引 i+1 .. i+7），把 O(n) 上界写死；
        # 同时保留 y 差达到 delta 即 break 的剪枝。
        for j in range(i + 1, min(i + 8, n_strip)):
            pj = strip[j]
            if pj[1] - pi[1] >= delta:
                break
            d2_ = _dist_sq(pi, pj)
            if d2_ < delta_sq:
                delta_sq, best, delta = d2_, (pi, pj), math.sqrt(d2_)

    return (delta_sq, best[0], best[1])


# ----------------------------------------------------------------------
# 对外接口
# ----------------------------------------------------------------------
def closest_pair(points: Sequence[Point]) -> Tuple[float, Point, Point]:
    """
    求平面最近点对。返回 (最近距离, 点A, 点B)；点数 < 2 时返回 (inf, None, None)。
    时间复杂度 O(n log n)，空间复杂度 O(n)。
    """
    pts = list(points)
    n = len(pts)
    if n < 2:
        return (math.inf, None, None)

    # 去重优化：若存在完全重合的两点，最近距离必为 0，可直接返回
    seen = set()
    for p in pts:
        key = (p[0], p[1])
        if key in seen:
            return (0.0, key, key)
        seen.add(key)

    px = sorted(pts, key=lambda p: (p[0], p[1]))   # 预排序：O(n log n)
    py = sorted(pts, key=lambda p: (p[1], p[0]))   # 预排序：O(n log n)

    d_sq, a, b = _solve(px, py)
    return (math.sqrt(d_sq), a, b)


def format_result(points: Sequence[Point]) -> str:
    """按题目要求的输出格式组织结果，点对内部先 x 后 y 升序。"""
    d, a, b = closest_pair(points)
    p1, p2 = (a, b) if (a[0], a[1]) <= (b[0], b[1]) else (b, a)
    return (
        f"最近距离: {d:.6f}\n"
        f"点1: ({int(p1[0])}, {int(p1[1])})\n"
        f"点2: ({int(p2[0])}, {int(p2[1])})"
    )


def brute_closest_pair(points: Sequence[Point]) -> Tuple[float, Point, Point]:
    """O(n^2) 参考实现，用于验证分治结果。"""
    d_sq, a, b = _brute_force(list(points))
    return (math.sqrt(d_sq) if math.isfinite(d_sq) else math.inf, a, b)


if __name__ == "__main__":
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
        assert math.isclose(closest_pair(pts)[0], brute_closest_pair(pts)[0], rel_tol=1e-12)
        print()

    print("[OK] 3 组测试用例全部通过，分治结果与暴力法一致")
