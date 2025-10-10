public class CopyArray {
    public static int[] copy1DArray(int[] source) {
        if (source == null) return null;
        int[] dest = new int[source.length];
        for (int i = 0; i < source.length; i++) {
            dest[i] = source[i];
        }
        return dest;
    }

    public static int[][] copy2DArray(int[][] source) {
        if (source == null) return null;
        int[][] dest = new int[source.length][];
        for (int i = 0; i < source.length; i++) {
            if (source[i] != null) {
                dest[i] = new int[source[i].length];
                for (int j = 0; j < source[i].length; j++) {
                    dest[i][j] = source[i][j];
                }
            }
        }
        return dest;
    }
}