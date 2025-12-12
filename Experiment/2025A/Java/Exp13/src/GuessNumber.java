public class GuessNumber {
    static class Game {
        private int target = -1;

        public synchronized void setTarget(int t) {
            this.target = t;
            notifyAll();
        }

        public synchronized int checkGuess(int guess) {
            while (target == -1) {
                try {
                    wait();
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                    return 0;
                }
            }
            if (guess < target) return -1;
            if (guess > target) return 1;
            return 0;
        }
    }

    // 可视化当前区间和猜测位置：1..100 显示为一行字符，已排除位置用 '.', 当前可能区间用 '=', 猜测位置用 'G'
    private static String visualizeRange(int low, int high, int guess) {
        StringBuilder sb = new StringBuilder(100);
        for (int i = 1; i <= 100; i++) {
            if (i == guess) sb.append('G');
            else if (i >= low && i <= high) sb.append('=');
            else sb.append('.');
        }
        return sb.toString();
    }

    public static void main(String[] args) {
        Game game = new Game();

        Thread generator = new Thread(() -> {
            int number = (int) (Math.random() * 100) + 1;
            System.out.println("生成线程：生成了一个 1 到 100 的随机数（已隐藏）。");
            game.setTarget(number);
            System.out.println("生成线程：随机数已设置。");
        }, "Generator");

        Thread guesser = new Thread(() -> {
            int low = 1, high = 100;
            while (low <= high) {
                int guess = (low + high) / 2;
                // 打印可视化进度
                System.out.println("[" + visualizeRange(low, high, guess) + "]  区间: " + low + "-" + high + " 猜测: " + guess);
                int res = game.checkGuess(guess);
                if (res == 0) {
                    System.out.println("猜测线程：猜对了！答案是 " + guess);
                    break;
                } else if (res < 0) { // 猜小了
                    System.out.println("提示：太小");
                    low = guess + 1;
                } else { // 猜大了
                    System.out.println("提示：太大");
                    high = guess - 1;
                }
            }
        }, "Guesser");

        guesser.start();
        generator.start();

        try {
            generator.join();
            guesser.join();
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}