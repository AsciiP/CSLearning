import random
import json

def generate_all_sequences(n=8):
    """生成所有2^n个长度为n的{-1,+1}序列"""
    sequences = []
    for i in range(2**n):
        seq = []
        for j in range(n):
            seq.append(1 if (i >> j) & 1 else -1)
        sequences.append(seq)
    return sequences

def dot_product(a, b):
    """计算两个序列的点积"""
    return sum(x * y for x, y in zip(a, b))

def is_orthogonal(a, b):
    """判断两个序列是否正交"""
    return dot_product(a, b) == 0

def get_orthogonal(seq, all_seqs):
    """获取与给定序列正交的所有序列"""
    return [s for s in all_seqs if is_orthogonal(seq, s)]

def are_mutually_non_orthogonal(seqs):
    """判断序列集合中是否两两不正交"""
    for i in range(len(seqs)):
        for j in range(i + 1, len(seqs)):
            if is_orthogonal(seqs[i], seqs[j]):
                return False
    return True

def get_unique_orthogonal(orth_seqs):
    """去除反码对，每对反码只保留一个"""
    unique = []
    seen = set()
    for seq in orth_seqs:
        seq_tuple = tuple(seq)
        neg_tuple = tuple(-x for x in seq)
        canonical = min(seq_tuple, neg_tuple)
        if canonical not in seen:
            seen.add(canonical)
            unique.append(list(canonical))
    return unique

# 主程序
all_seqs = generate_all_sequences()
print(f"总共 {len(all_seqs)} 个8位码片序列")

# 随机生成5个互不正交的码片
random.seed(42)
random.shuffle(all_seqs)

selected = []
for seq in all_seqs:
    if len(selected) >= 5:
        break
    if are_mutually_non_orthogonal(selected + [seq]):
        selected.append(seq)

M = selected[:5]

# 验证互不正交
print("\n验证互不正交性:")
for i in range(5):
    for j in range(i + 1, 5):
        dp = dot_product(M[i], M[j])
        print(f"  M{i+1}·M{j+1} = {dp} (正交? {'是' if dp == 0 else '否'})")

# 对每个M求正交码片
results = []
for i, m in enumerate(M):
    orth_seqs = get_orthogonal(m, all_seqs)
    unique_orth = get_unique_orthogonal(orth_seqs)
    print(f"\n与M{i+1}正交的码片:")
    print(f"  总数(含反码对): {len(orth_seqs)}")
    print(f"  去重后(反码对计1个): {len(unique_orth)}")
    print(f"  前10个: {unique_orth[:10]}")
    results.append({
        'M': m,
        'total': len(orth_seqs),
        'unique': len(unique_orth),
        'examples': unique_orth[:10]
    })

# 比较分析
print("\n=== 比较分析 ===")
counts = [r['unique'] for r in results]
print(f"与M1~M5正交的码片数(去重): {counts}")
print(f"是否全部相等: {len(set(counts)) == 1}")
print("\n分析:")
print("所有码片的正交码片数相同(均为35个去重)，因为:")
print("对于任意n位{-1,+1}序列M，正交序列数=C(n,n/2)，与M具体取值无关。")
print("n=8时C(8,4)=70，去重(反码对计1个)后为35个。")
