import turtle


def koch(t: turtle.Turtle, length: float, level: int) -> None:
    if level == 0:
        t.forward(length)
    else:
        length /= 3.0
        koch(t, length, level - 1)
        t.left(60)
        koch(t, length, level - 1)
        t.right(120)
        koch(t, length, level - 1)
        t.left(60)
        koch(t, length, level - 1)


def koch_snowflake(t: turtle.Turtle, length: float, level: int) -> None:
    for i in range(3):
        koch(t, length, level)
        t.right(120)

t = turtle.Turtle()
t.speed('fastest')
t.penup()
t.goto(-200, 100)
t.pendown()

koch_snowflake(t, 400, 4)

turtle.done()