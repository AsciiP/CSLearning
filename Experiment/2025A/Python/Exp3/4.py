import jieba
from collections import Counter

name_alias_map = {
    "贾宝玉": ["宝玉", "宝二爷", "宝兄弟", "怡红公子", "绛洞花主"],
    "林黛玉": ["黛玉", "林姑娘", "林妹妹", "潇湘妃子"],
    "薛宝钗": ["宝钗", "宝姐姐", "蘅芜君", "薛姑娘"],
    "王熙凤": ["熙凤", "凤辣子", "凤姐", "王二奶奶", "琏二奶奶"],
    "贾母": ["老太太", "老祖宗", "史太君", "贾母老太太"],
    "袭人": ["袭人", "花袭人", "袭姑娘", "花大姑娘"],
    "晴雯": ["晴雯", "晴姑娘", "晴大姑娘"],
    "探春": ["探春", "三姑娘", "贾探春", "蕉下客"],
    "迎春": ["迎春", "二姑娘", "贾迎春", "菱洲"],
    "惜春": ["惜春", "四姑娘", "贾惜春", "藕榭"],
    "妙玉": ["妙玉", "妙姑娘", "槛外人", "畸人"],
    "史湘云": ["湘云", "云姑娘", "史大姑娘", "枕霞旧友"],
    "李纨": ["李纨", "李宫裁", "大奶奶", "稻香老农"],
    "贾政": ["贾政", "贾政老爷", "贾大人", "政老爹"],
    "王夫人": ["王夫人", "王太太", "太太"]
}

txt = open('红楼梦.txt', 'r', encoding='utf-8').read()
words = jieba.lcut(txt)
counts = Counter()
for word in words:
    if len(word) == 1:
        continue
    for name, aliases in name_alias_map.items():
        if word == name or word in aliases:
            counts[name] += 1
            break
items = list(counts.items())
items.sort(key=lambda x: x[1], reverse=True)
for item in items:
    word, count = item
    print("{0:<10}{1:>5}".format(word, count))