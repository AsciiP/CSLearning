import csv
import json

with open('学生信息表.csv', encoding="utf-8") as f:
    reader = csv.reader(f)
    for row in reader:
        for item in row:
            print(item, end='\t')
        print()

data = ['17010002', '赵四', '女', '自动化1701']
with open('学生信息表.csv', 'a', encoding='utf-8', newline='') as f:
    writer = csv.writer(f)
    writer.writerow(data)

data_list = []
with open('学生信息表.csv', encoding="utf-8") as f:
    reader = csv.reader(f)
    headers = next(reader)
    for row in reader:
        data_list.append(dict(zip(headers, row)))

with open('学生信息表.json', 'w', encoding='utf-8') as json_file:
    json.dump(data_list, json_file, ensure_ascii=False, indent=4)