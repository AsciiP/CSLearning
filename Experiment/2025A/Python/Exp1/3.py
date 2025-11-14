import turtle

turtle.speed('fastest')
turtle.pensize(20)

colors = ['purple', 'red', 'yellow', 'pink', 'blue', 'green']

turtle.penup()
turtle.backward(200)
turtle.right(40)
turtle.pendown()

for x in range(4):
    turtle.pencolor(colors[x])
    for y in range(10):
        turtle.forward(5)
        turtle.left(8)
    for y in range(10):
        turtle.forward(5)
        turtle.right(8)

turtle.pencolor(colors[4])
for x in range(5):
    turtle.forward(5)
    turtle.left(8)
turtle.forward(40)

turtle.pencolor(colors[5])
for x in range(10):
    turtle.forward(5)
    turtle.left(18)
turtle.forward(30)

turtle.done()
