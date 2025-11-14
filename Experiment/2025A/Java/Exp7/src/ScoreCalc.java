import java.util.Arrays;

interface ScoreInterface {

    double calculateScore();

    String getDetails();
}

class SingingContestScore implements ScoreInterface {
    private final double[] judgeScores;
    private final double penalty; // 惩罚分

    public SingingContestScore(double[] judgeScores) {
        this(judgeScores, 0.0);
    }

    public SingingContestScore(double[] judgeScores, double penalty) {
        if (judgeScores == null)
            throw new IllegalArgumentException("judgeScores cannot be null");
        this.judgeScores = Arrays.copyOf(judgeScores, judgeScores.length);
        this.penalty = penalty;
    }

    @Override
    public double calculateScore() {
        if (judgeScores.length == 0)
            return 0.0;
        double[] s = Arrays.copyOf(judgeScores, judgeScores.length);
        Arrays.sort(s);
        double avg;
        if (s.length > 2) {
            double sum = 0;
            for (int i = 1; i < s.length - 1; i++)
                sum += s[i];
            avg = sum / (s.length - 2);
        } else {
            double sum = 0;
            for (double v : s)
                sum += v;
            avg = sum / s.length;
        }
        return clamp(avg - penalty);
    }

    @Override
    public String getDetails() {
        return String.format("SingingContestScore{score=%.2f, penalty=%.2f}", calculateScore(), penalty);
    }

    private static double clamp(double v) {
        if (Double.isNaN(v))
            return 0.0;
        if (v < 0)
            return 0.0;
        if (v > 100)
            return 100.0;
        return v;
    }
}

class StudentGrade implements ScoreInterface {
    private final double[] scores;
    private final double[] weights; // 可为 null（表示等权）

    public StudentGrade(double[] scores, double[] weights) {
        if (scores == null)
            throw new IllegalArgumentException("scores cannot be null");
        this.scores = Arrays.copyOf(scores, scores.length);
        if (weights == null || weights.length != scores.length) {
            this.weights = null;
        } else {
            this.weights = Arrays.copyOf(weights, weights.length);
        }
    }

    @Override
    public double calculateScore() {
        if (scores.length == 0)
            return 0.0;
        double total;
        if (weights == null) {
            total = 0;
            for (double v : scores)
                total += v;
            return clamp(total / scores.length);
        } else {
            double sumW = 0;
            for (double w : weights)
                sumW += w;
            if (sumW == 0)
                return 0.0;
            double weighted = 0;
            for (int i = 0; i < scores.length; i++)
                weighted += scores[i] * (weights[i] / sumW);
            return clamp(weighted);
        }
    }

    @Override
    public String getDetails() {
        double s = calculateScore();
        String grade;
        if (s >= 90)
            grade = "A";
        else if (s >= 80)
            grade = "B";
        else if (s >= 70)
            grade = "C";
        else if (s >= 60)
            grade = "D";
        else
            grade = "F";
        return String.format("StudentGrade{score=%.2f, grade=%s}", s, grade);
    }

    private static double clamp(double v) {
        if (Double.isNaN(v))
            return 0.0;
        if (v < 0)
            return 0.0;
        if (v > 100)
            return 100.0;
        return v;
    }
}

public class ScoreCalc {
    public static void main(String[] args) {

        double[] judges = { 88.5, 92.0, 79.0, 85.5, 91.0 };
        ScoreInterface singing = new SingingContestScore(judges, 1.0); // 惩罚1分
        System.out.println(singing.getDetails());

        double[] comps = { 85.0, 78.0, 92.0 };
        double[] weights = { 0.4, 0.2, 0.4 };
        ScoreInterface student = new StudentGrade(comps, weights);
        System.out.println(student.getDetails());

        double[] comps2 = { 70, 80, 90 };
        ScoreInterface student2 = new StudentGrade(comps2, null);
        System.out.println(student2.getDetails());
    }
}