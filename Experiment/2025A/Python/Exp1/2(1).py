import turtle

turtle.speed('fastest')
turtle.pensize(2)
for x in range(100):
    turtle.forward(2 * x)
    turtle.left(90)

turtle.done()