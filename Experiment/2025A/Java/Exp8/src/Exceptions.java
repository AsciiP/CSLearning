import java.util.InputMismatchException;
import java.util.Scanner;

public class Exceptions {
    private static final String[] NAMES = { "张三", "李四", "王五", "赵六" };
    private static final int[] SCORES = { 85, 59, 73, 48 };

    public static class FailException extends Exception {
        public FailException(String message) {
            super(message);
        }
    }

    public static int queryScore(int index) throws FailException {
        if (index < 0 || index >= SCORES.length) {
            throw new IndexOutOfBoundsException(
                    "索引值越界：有效范围为 0 到 " + (SCORES.length - 1));
        }
        int score = SCORES[index];
        if (score < 60) {
            throw new FailException("学生 " + NAMES[index] + " 成绩为 " + score + "，不及格");
        }
        return score;
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int idx;
        System.out.println("学生序号对应表：");
        for (int i = 0; i < NAMES.length; i++) {
            System.out.println(i + " -> " + NAMES[i]);
        }
        System.out.print("请输入要查询的学生序号：");
        try {
            idx = sc.nextInt();
            int score = queryScore(idx);
            System.out.println("查询结果：学生 " + NAMES[idx] + " 的成绩为 " + score);
        } catch (FailException fe) {
            System.out.println("FailException: " + fe.getMessage());
        } catch (IndexOutOfBoundsException ie) {
            System.out.println("IndexOutOfBoundsException: " + ie.getMessage());
        } catch (InputMismatchException ime) {
            System.out.println("输入错误：请输入整数序号");
        } finally {
            sc.close();
        }
    }
}