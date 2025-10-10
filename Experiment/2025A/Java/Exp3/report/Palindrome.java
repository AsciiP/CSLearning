import java.util.Scanner;

public class Palindrome {
    public static boolean isPalindrome(String str) {
        int left = 0;
        int right = str.length() - 1;

        while (left < right) {
            if (str.charAt(left) != str.charAt(right)) {
                return false;
            }
            left++;
            right--;
        }
        return true;
    }
    public static void main(String[] args) {
        try (Scanner scanner = new Scanner(System.in)) {
            System.out.print("Enter a number (1-9999): ");
            int number = scanner.nextInt();

            if (number < 1 || number > 9999) {
                System.out.println("Number out of range.");
            } else {
                String numStr = Integer.toString(number);
                if (isPalindrome(numStr)) {
                    System.out.println(number + " is a palindrome.");
                } else {
                    System.out.println(number + " is not a palindrome.");
                }
            }
        }
    }
}
