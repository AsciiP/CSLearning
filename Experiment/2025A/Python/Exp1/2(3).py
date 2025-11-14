import turtle

turtle.speed('fastest')
turtle.pensize(2)

colors = ['red', 'yellow', 'purple', 'blue']

for x in range(100):
    turtle.pencolor(colors[x % 4])
    turtle.forward(2 * x)
    turtle.left(91)

turtle.done()