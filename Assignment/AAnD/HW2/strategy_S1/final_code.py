# ============================================================
# 学生ID: 2405111B08
# 策略: S1
# 修改说明: 见 changelog.csv
# ============================================================

import math


def closest_pair(points):
    pts = sorted((float(x), float(y)) for x, y in points)
    if len(pts) < 2:
        raise ValueError("至少需要两个点")

    def dist(a, b):
        return math.hypot(a[0] - b[0], a[1] - b[1])

    def rec(px, py):
        n = len(px)
        if n <= 3:
            best = None
            bd = float("inf")
            for i in range(n):
                for j in range(i + 1, n):
                    d = dist(px[i], px[j])
                    if d < bd:
                        bd = d
                        best = (px[i], px[j])
            return bd, best

        mid = n // 2
        lx = px[:mid]
        rx = px[mid:]
        lset = set(lx)
        ly = [p for p in py if p in lset]
        ry = [p for p in py if p not in lset]

        dl, bl = rec(lx, ly)
        dr, br = rec(rx, ry)

        if dl <= dr:
            d, best = dl, bl
        else:
            d, best = dr, br

        sx = px[mid][0]
        cand = [p for p in py if abs(p[0] - sx) < d]
        m = len(cand)
        for i in range(m):
            for j in range(i + 1, min(i + 8, m)):
                if cand[j][1] - cand[i][1] >= d:
                    break
                nd = dist(cand[i], cand[j])
                if nd < d:
                    d = nd
                    best = (cand[i], cand[j])
        return d, best

    px = pts
    py = sorted(pts, key=lambda p: (p[1], p[0]))
    d, (a, b) = rec(px, py)
    return d, a, b


def order_pair(a, b):
    return (a, b) if (a[0], a[1]) <= (b[0], b[1]) else (b, a)


def format_result(points):
    d, a, b = closest_pair(points)
    p1, p2 = order_pair(a, b)
    lines = [
        f"最近距离: {d:.6f}",
        f"点1: ({int(p1[0])}, {int(p1[1])})",
        f"点2: ({int(p2[0])}, {int(p2[1])})",
    ]
    return "\n".join(lines)


def read_points(data):
    n = int(data[0])
    return [(float(data[1 + 2 * k]), float(data[2 + 2 * k])) for k in range(n)]


if __name__ == "__main__":
    test_cases = [
        [(0, 0), (1, 1), (2, 2), (3, 3), (4, 4), (5, 5)],
        [(4, 7), (2, 1), (8, 3), (6, 9), (1, 4), (9, 8), (3, 5), (7, 2), (5, 6), (0, 0)],
        [(0, 0), (3, 4)],
    ]

    for idx, pts in enumerate(test_cases, 1):
        print(f"测试用例 {idx}：")
        print(format_result(pts))
        print()
