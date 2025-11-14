import turtle

turtle.setup(1024, 768, 0, 0)
turtle.pencolor('red')
turtle.width(5)
turtle.speed(5)

results = []
with open('data.txt') as f:
    for line in f:
        results.append(list(map(float, line.split(','))))
print(results)

for i in results:
    turtle.pencolor(i[3], i[4], i[5])
    turtle.forward(i[0])
    if i[1] == 0:
        turtle.right(i[2])
    else:
        turtle.left(i[2])

turtle.done()
