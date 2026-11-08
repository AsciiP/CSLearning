# ============================================================
# 学生ID: 2405111B08
# 策略: S5
# 修改说明: 见 changelog.csv
# ============================================================

# =====================================================================
#                    算法思想概览（请先读这段再往下看）
# =====================================================================
# 【问题】平面上 n 个点，求欧几里得距离最小的一对点。要求 O(n log n)。
#
# 【分治三阶段】
#   ┌──────── 分解 (Divide) ───────┐
#   │ 把所有点按 x 坐标排序，取中位数 x=midx 画一条竖直分割线，       │
#   │ 平面被劈成「左半 S_L」和「右半 S_R」，各约 n/2 个点。          │
#   └──────────────────────────────┘
#   ┌──────── 解决 (Conquer) ──────┐
#   │ 递归地在 S_L 内求最近距离 dL，在 S_R 内求最近距离 dR。         │
#   │ 点数 <= 3 时不再递归，直接两两算（暴力），这是递归基。         │
#   └──────────────────────────────┘
#   ┌──────── 合并 (Combine) ──────┐
#   │ delta = min(dL, dR) 是「目前已知最好成绩」的一个上界。        │
#   │ 唯一可能打败它的，是【一个点在左、一个点在右】的跨线点对。     │
#   │ 而这种点对必须落在以中线为中心、半宽 = sqrt(delta) 的竖直      │
#   │ 「带状区域」里 —— 因为若某点离中线超过 sqrt(delta)，它到对面   │
#   │ 任何点的距离都已经 > delta，不可能更优。                       │
#   │ 在带内按 y 排序后，每个点只需向后看至多 7 个点（几何证明）。   │
#   └──────────────────────────────┘
#
# 【复杂度推导】
#   分解 O(1)（排序只做一次） + 递归 2T(n/2) + 合并 O(n)（线性筛带 + 常数次比较）
#   => 主定理 case 2 => T(n) = O(n log n)
#
# 【本版本相对 AI 教学版的唯一实质改动（重要！）】
#   AI 的教学版为了"好读"，在每层递归里都对带状区域重新 sort 一次，
#   它自己在注释中承认这样复杂度退化为 O(n log^2 n)，不满足题目要求。
#   本版本按它文末给出的「进阶提示」把 py（按 y 有序的列表）一路传下去：
#   划分时只做 O(n) 的过滤，带状区域直接从 py 里筛出来（天然 y 有序，
#   无需再排序），合并阶段回到 O(n)，整体严格 O(n log n)。
#   算法骨架与 7 点引理和教学版完全一致，只是去掉了那次多余的排序。
#
# 【单位约定】内部所有比较一律使用「距离的平方」，只在最外层开一次平方根。
#   好处：避免反复开方带来的性能损失与浮点误差。
# =====================================================================

import math
from typing import List, Tuple

# 类型别名：一个点就是一个 (x, y) 二元组
Point = Tuple[float, float]


def dist2(a: Point, b: Point) -> float:
    """计算两点【欧氏距离的平方】。

    做什么：(x1-x2)^2 + (y1-y2)^2
    为什么：平方函数在非负数上单调递增，所以「谁的平方小」等价于「谁的距离小」，
           可以全程不开方，既快又稳。
    """
    dx = a[0] - b[0]          # x 方向的差
    dy = a[1] - b[1]          # y 方向的差
    return dx * dx + dy * dy  # 返回平方距离（注意：不是真实距离！）


def brute_force(pts: List[Point]) -> Tuple[float, Point, Point]:
    """递归基：点数很少（<=3）时，直接两两比较，全部枚举。

    做什么：双重循环枚举所有点对，记录最小的平方距离及其对应的两个点。
    为什么：n<=3 时点对数最多 3 个，暴力比递归更快也更简单；
           同时它天然规避了「单点区间无法构成点对」的边界问题。
    """
    best = math.inf           # 初始上界设为正无穷，保证第一对一定能替换它
    pa, pb = pts[0], pts[1]   # 当前最优点对（先用前两个点占位）

    m = len(pts)              # 本区间的点数
    for i in range(m - 1):            # 固定第一个点
        for j in range(i + 1, m):     # 第二个点从 i+1 开始，避免自比和重复比
            d = dist2(pts[i], pts[j])  # 这一对的平方距离
            if d < best:              # 只有严格更小才更新（相等时保留先找到的）
                best = d
                pa, pb = pts[i], pts[j]
    return best, pa, pb


def closest(pts: List[Point]) -> Tuple[float, Point, Point]:
    """分治主函数：返回 (最小距离, 点A, 点B)。真实距离，非平方。

    做什么：整体流程 = 排序预处理 -> 递归求解（平方单位）-> 出口开方。
    为什么：把「排序」放在外面只做一次，递归内部只负责分与合，职责清晰。
    """
    if len(pts) < 2:
        # 少于两个点根本构不成「一对」，提前给出明确报错而不是 IndexError
        raise ValueError("最近点对问题至少需要 2 个点")

    # 若存在完全重合的两点，最近距离必为 0，可直接返回。
    # 这一步同时保证了下面用 set(left) 做归属判定时不会因重复点而分错边。
    seen = set()
    for p in pts:
        if p in seen:
            return (0.0, p, p)
        seen.add(p)

    by_x = sorted(pts)                                  # 按 x（x 相同则按 y）升序，用于「分解」
    by_y = sorted(pts, key=lambda p: (p[1], p[0]))      # 按 y（y 相同则按 x）升序，用于「扫描」
    d_square, a, b = _solve(by_x, by_y)                 # 递归求解，拿到的是【平方距离】
    return math.sqrt(d_square), a, b                    # ⭐ 全流程唯一一次开方


def _solve(px: List[Point], py: List[Point]) -> Tuple[float, Point, Point]:
    """递归内核：px 按 x 有序、py 按 y 有序，且两者是同一批点。

    做什么：严格执行 分解 -> 解决 -> 合并 三步。
    为什么：把 py 一路传下来，是为了在合并阶段拿到「已经按 y 排好」的带状区域，
           从而省掉每层的 sort，把复杂度锁在 O(n log n) 而不是 O(n log^2 n)。
    """
    n = len(px)

    # ---------- ① 递归基：小规模直接暴力 ----------
    if n <= 3:
        # 3 个点以内没有继续划分的意义（再分会出现单边只剩 1 点、无法成对）
        return brute_force(px)

    # ---------- ② 分解 (Divide)：沿中位线把点集一分为二 ----------
    mid = n // 2                 # 中点下标（整数除法）
    left = px[:mid]              # 左半区：x 较小的那一半点
    right = px[mid:]             # 右半区：x 较大的那一半点
    mid_x = px[mid][0]           # 中位线的 x 坐标 = 右半第一个点的 x
    # 注意：left 与 right 各自仍然是「按 x 有序」的，因为它们是从有序数组里切的连续段

    # ---------- ②-b 把 py 也线性拆成左右两半（关键：拆完仍然按 y 有序）----------
    # 做什么：扫一遍 py，用「是否属于左半区」把点分到 py_left / py_right。
    # 为什么：过滤操作保持原有相对顺序，所以两个子列表天然仍按 y 升序，
    #         递归下去就不需要再排序 —— 这正是省掉 O(n log n) 的地方。
    # 代价：O(n) 一趟扫描 + 一次 set 构造，整体递推仍是 T(n)=2T(n/2)+O(n)。
    left_set = set(left)                                   # 左半区点集，O(1) 判归属
    py_left = [p for p in py if p in left_set]
    py_right = [p for p in py if p not in left_set]

    # ---------- ③ 解决 (Conquer)：递归求两侧各自的最近点对 ----------
    dl, la1, lb1 = _solve(left, py_left)    # 左半区内部的最好成绩（平方）
    dr, ra1, rb1 = _solve(right, py_right)  # 右半区内部的最好成绩（平方）

    # ---------- ④ 合并 (Combine) 第 1 步：取两侧较优者作为当前上界 ----------
    if dl <= dr:
        delta, ca, cb = dl, la1, lb1   # 左边更好（或持平）=> 继承左边的答案
    else:
        delta, ca, cb = dr, ra1, rb1   # 右边更好 => 继承右边的答案
    # delta 的含义：目前已知的最小【平方距离】，它是后续剪枝的唯一依据。
    # 它不是最终答案，而是「想赢我必须比你小」的门槛。

    # ---------- ⑤ 合并 第 2 步：筛出带状区域内的候选点 ----------
    # ⚠️ 注意（带状区域）：
    #   这里筛的条件是 dx*dx < delta，即 |x - mid_x| < sqrt(delta)。
    #   千万别写成 abs(x - mid_x) < delta —— 那是把「平方距离」当成了「真实距离」，
    #   相当于把带宽放大到 sqrt(delta) 倍，会捞进大量无用点，最坏退化为 O(n^2)。
    #   另外，只有【跨线点对】才需要检查：同侧点对的最短距离已经被 dl/dr 覆盖了，
    #   所以这里的 strip 混合了左右两侧的点，我们要的就是「一左一右」的组合。
    #
    # ⚠️ 注意（不要再排序）：
    #   py 本身已按 y 有序，顺序过滤出来的 strip 自然也按 y 有序，
    #   所以这里【不需要】strip.sort()。加上那次 sort 就会退化成 O(n log^2 n)。
    strip = []                   # 存放落在带状区域内的点（已按 y 有序）
    for p in py:                 # 遍历 py 而不是 px：这样 strip 直接就是 y 序
        dx = p[0] - mid_x        # 该点到中位线的水平距离
        if dx * dx < delta:      # 水平距离的平方 < 当前最优平方距离 => 有可能跨线取胜
            strip.append(p)      # 收入候选带

    # ---------- ⑥ 合并 第 3 步：带内扫描，每个点最多比较 7 次 ----------
    m = len(strip)
    # ⚠️ 注意（7 个点）：
    #   为什么上限是 7 而不是 m-i-1？
    #   取带内一点 p，只需考虑 y 坐标在 [p.y, p.y + sqrt(delta)) 内的点。
    #   这些点全部落在一个「宽 2*sqrt(delta)、高 sqrt(delta)」的矩形里。
    #   把它切成 8 个边长 sqrt(delta)/2 的小方格：
    #     · 小方格的对角线 = sqrt(delta)/sqrt(2) < sqrt(delta)
    #     · 若同一格里有两个点，它们距离 < sqrt(delta)，即平方距离 < delta，
    #       这与「delta = min(dL, dR) 已是同侧最小」矛盾（鸽巢原理）
    #     · 所以每格至多 1 个点 => 整个矩形至多 8 个点 => 除去 p 自己，后继至多 7 个
    #   少了这层限制，j 循环就是 O(n)，总复杂度退化为 O(n^2)。
    #   写成 i + 8 是因为 range 的右端开区间，正好覆盖 j = i+1 ... i+7 共 7 个后继。
    for i in range(m):                     # 带内第 i 个点（按 y 从小到大）
        hi = min(i + 8, m)                 # 只向后看 7 个位置，且不越界
        for j in range(i + 1, hi):         # j 从 i+1 起：i 自己不用比，前面也比过了
            d = dist2(strip[i], strip[j])  # 这对跨线候选的平方距离
            if d < delta:                  # 严格小于才更新（等于时无需替换）
                delta = d                  # ⭐ 门槛立刻收紧
                ca, cb = strip[i], strip[j]
                # 为什么必须同步更新 delta：带状区域的宽度、以及后续每个点的
                # 比较范围都依赖 delta。不更新就会漏掉更优解，也会浪费比较次数。
    return delta, ca, cb


def format_result(pts: List[Point]) -> str:
    """按题目要求的输出格式组织结果，点对内部先 x 后 y 升序。"""
    d, a, b = closest(pts)
    p1, p2 = (a, b) if (a[0], a[1]) <= (b[0], b[1]) else (b, a)
    return (
        f"最近距离: {d:.6f}\n"
        f"点1: ({int(p1[0])}, {int(p1[1])})\n"
        f"点2: ({int(p2[0])}, {int(p2[1])})"
    )


if __name__ == "__main__":
    import random

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
        print()

    # 随机自检：与 O(n^2) 暴力对照，确保分治实现正确
    random.seed(2026)
    for trial in range(300):
        k = random.randint(2, 120)
        rnd = [(random.uniform(-100, 100), random.uniform(-100, 100)) for _ in range(k)]
        fast_d, _, _ = closest(rnd)
        slow_d = min(dist2(rnd[a], rnd[b]) for a in range(k) for b in range(a + 1, k))
        assert abs(fast_d - math.sqrt(slow_d)) < 1e-9, f"第 {trial} 轮不一致"

    # 退化场景自检：全同 x / 全同 y / 网格 / 重复点
    assert math.isclose(closest([(0, i) for i in range(300)])[0], 1.0)
    assert math.isclose(closest([(i, 7) for i in range(300)])[0], 1.0)
    assert math.isclose(closest([(x, y) for x in range(40) for y in range(40)])[0], 1.0)
    assert closest([(1, 2), (5, 6), (1, 2)])[0] == 0.0

    print("随机自检    : 300 轮全部通过")
    print("退化场景    : 全同 x / 全同 y / 网格 / 重复点 全部通过")
