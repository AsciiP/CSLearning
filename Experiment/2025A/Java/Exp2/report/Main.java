public class Main {
    public static void main(String[] args) {
        // Test NextDay
        System.out.println(NextDay.nextDay(2024, 2, 28));
        System.out.println(NextDay.nextDay(2023, 2, 28));
        System.out.println(NextDay.nextDay(2023, 12, 31));
        System.out.println(NextDay.nextDay(2023, 4, 30));
        System.out.println(NextDay.nextDay(2023, 1, 15));
        System.out.println(NextDay.nextDay(2023, 2, 30));

        // Test CopyArray
        int[] array1D = {1, 2, 3, 4, 5};
        int[] copied1D = CopyArray.copy1DArray(array1D);
        System.out.print("Copied 1D Array: ");
        for (int num : copied1D) {
            System.out.print(num + " ");
        }
        System.out.println();

        int[][] array2D = {
            {1, 2, 3},
            {4, 5},
            {6}
        };
        int[][] copied2D = CopyArray.copy2DArray(array2D);
        System.out.println("Copied 2D Array:");
        for (int[] row : copied2D) {
            for (int num : row) {
                System.out.print(num + " ");
            }
            System.out.println();
        }
    }
}
