public class NextDay {
    private static boolean isLeapYear(int year) {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }

    public static String nextDay(int year, int month, int day) {
        if (month < 1 || month > 12 || day < 1 || day > 31) {
            return "Invalid date";
        }

        int[] daysInMonth = { 31, (isLeapYear(year) ? 29 : 28), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

        if (day > daysInMonth[month - 1]) {
            return "Invalid date";
        }

        day++;

        if (day > daysInMonth[month - 1]) {
            day = 1;
            month++;
            if (month > 12) {
                month = 1;
                year++;
            }
        }

        return String.format("%04d-%02d-%02d", year, month, day);
    }

}