# ============================================================
# 学生ID: 2405111B08
# 策略: S2
# 修改说明: 见 changelog.csv
# ============================================================

"""
平面最近点对问题 —— 分治算法
时间复杂度: O(n log n)
空间复杂度: O(n)

设计要点:
  1. 点集预先按 x 排序，全程不再重排 -> Divide 是 O(1)
  2. 递归函数返回时, px[lo..hi] 已被原地整理成按 y 有序 -> Combine 是 O(n) 归并
     (避免了每层 sort(key=y) 导致的 O(n log^2 n) 退化)
  3. strip 内利用 "8 格装箱" 结论, 每个点最多向后检查 7 个点
"""

import math
from typing import List, Tuple

Point = Tuple[float, float]


def _dist(a: Point, b: Point) -> float:
    """欧几里得距离（用 hypot 避免中间量溢出/精度损失）"""
    return math.hypot(a[0] - b[0], a[1] - b[1])


class ClosestPairSolver:
    def __init__(self, points: List[Point]):
        if len(points) < 2:
            raise ValueError("至少需要 2 个点才能构成点对")
        # px: 按 x 升序（x 相同则按 y），递归过程中会被改写成按 y 有序
        self.px = sorted(points, key=lambda p: (p[0], p[1]))
        self.tmp = [None] * len(self.px)      # 归并用的辅助数组，只分配一次
        self.best_d = math.inf
        self.best_pair: Tuple[Point, Point] = None

    # ---------------- 核心分治 ----------------
    def solve(self) -> Tuple[float, Tuple[Point, Point]]:
        self._rec(0, len(self.px) - 1)
        return self.best_d, self.best_pair

    def _rec(self, lo: int, hi: int) -> None:
        """
        递归求解 px[lo..hi] 内部最近点对。
        前置条件: 调用前 px[lo..hi] 按 x 有序。
        后置条件: 返回时 px[lo..hi] 已变为按 y 有序（供上层归并使用）。
        """
        n = hi - lo + 1

        # ---- 递归边界: 规模 <= 3 直接暴力, 再手动按 y 排好 ----
        if n <= 3:
            for i in range(lo, hi + 1):
                for j in range(i + 1, hi + 1):
                    d = _dist(self.px[i], self.px[j])
                    if d < self.best_d:
                        self.best_d = d
                        self.best_pair = (self.px[i], self.px[j])
            # 插入排序, 规模常数, 不影响整体复杂度
            for i in range(lo + 1, hi + 1):
                k = i - 1
                v = self.px[i]
                while k >= lo and self.px[k][1] > v[1]:
                    self.px[k + 1] = self.px[k]
                    k -= 1
                self.px[k + 1] = v
            return

        mid = (lo + hi) // 2
        mid_x = self.px[mid][0]          # 必须在递归改写 px 之前记录分割线位置

        # ---- Conquer: 先递归左半, 再递归右半 ----
        # 注意: 左递归结束后 px[lo..mid] 变成按 y 有序, 但元素集合不变,
        #       仍然全部是 x <= mid_x 的那批点, 所以右递归区间 px[mid+1..hi] 依然合法。
        self._rec(lo, mid)
        self._rec(mid + 1, hi)

        # ---- Combine 第一步: 检查跨分界线的候选点 ----
        # 此时 best_d 已经是 min(dL, dR)（全局维护的当前最优下界）
        delta = self.best_d

        # 左右两半现在【已经按 y 有序】，直接双指针归并出整个区间的 y 有序序列
        i, j, k = lo, mid + 1, lo
        while i <= mid and j <= hi:
            if self.px[i][1] <= self.px[j][1]:
                self.tmp[k] = self.px[i]
                i += 1
            else:
                self.tmp[k] = self.px[j]
                j += 1
            k += 1
        while i <= mid:
            self.tmp[k] = self.px[i]
            i += 1
            k += 1
        while j <= hi:
            self.tmp[k] = self.px[j]
            j += 1
            k += 1

        # 把按 y 有序的结果写回 px[lo..hi]
        for t in range(lo, hi + 1):
            self.px[t] = self.tmp[t]

        # ---- 构造 strip（保持 y 有序）----
        strip = [p for p in self.px[lo:hi + 1] if abs(p[0] - mid_x) < delta]

        # ---- 每点最多向后检查 7 个点 ----
        m = len(strip)
        for a in range(m):
            # 内层最多走 7 步; 且一旦 y 差 >= delta 立刻 break（后面的点 y 更大，更远）
            for b in range(a + 1, min(a + 8, m)):
                if strip[b][1] - strip[a][1] >= delta:
                    break
                d = _dist(strip[a], strip[b])
                if d < self.best_d:
                    self.best_d = d
                    self.best_pair = (strip[a], strip[b])


def closest_pair(points: List[Point]) -> Tuple[float, Tuple[Point, Point]]:
    """对外接口：返回 (最近距离, (点A, 点B))"""
    return ClosestPairSolver(points).solve()


def format_result(points: List[Point]) -> str:
    """按题目要求的输出格式组织结果，点对内部先 x 后 y 升序。"""
    d, (a, b) = closest_pair(points)
    p1, p2 = (a, b) if (a[0], a[1]) <= (b[0], b[1]) else (b, a)
    return (
        f"最近距离: {d:.6f}\n"
        f"点1: ({int(p1[0])}, {int(p1[1])})\n"
        f"点2: ({int(p2[0])}, {int(p2[1])})"
    )


def brute_force(points: List[Point]) -> float:
    """O(n^2) 暴力参考实现，用于交叉校验分治结果"""
    best = math.inf
    n = len(points)
    for i in range(n):
        for j in range(i + 1, n):
            best = min(best, _dist(points[i], points[j]))
    return best


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
        # 用 O(n^2) 暴力法交叉校验距离正确性
        assert math.isclose(closest_pair(pts)[0], brute_force(pts), rel_tol=1e-12)
        print()

    print("[OK] 3 组测试用例全部通过，分治结果与暴力法一致")
