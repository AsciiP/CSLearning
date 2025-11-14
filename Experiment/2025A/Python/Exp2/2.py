from random import randint

guess = 0
secret = randint(0,100)
times = 1

print('---欢迎加入猜数字游戏，请开始---')
while guess != secret:
    guess = int(input('@数字区间0-100，请输入你猜的数字:'))
    print('你输入的数字是:', guess)
    if guess == secret:
        print('你猜了{}次，猜对了，真厉害！'.format(times))
    else:
        if guess < secret:
            print('你猜的数字小于正确答案')
        else:
            print('你猜的数字大于正确答案')
    times += 1
print('游戏结束')