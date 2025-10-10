public class PriceGuessing {
    public static void main(String[] args) {
        try (java.util.Scanner scanner = new java.util.Scanner(System.in)) {
            int target = (int) (Math.random() * 1001);
            int guess;
            System.out.println("Guess the price (between 0 and 1000):");
            while (true) {
                System.out.print("Your guess: ");
                guess = scanner.nextInt();
                if (guess < target) {
                    System.out.println("Too low!");
                } else if (guess > target) {
                    System.out.println("Too high!");
                } else {
                    System.out.println("Congratulations! You guessed the correct price.");
                    break;
                }
            }
        }
    }
}
