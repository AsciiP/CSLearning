public class AreaSolver {
    public static void main(String[] args) {
        Shape2D[] shapes = new Shape2D[] {
                new Circle(2.5),
                new Triangle(3, 4, 5),
                new Rectangle(4, 5),
                new Triangle(2, 2, 3)
        };

        double totalArea = 0;
        double totalPerimeter = 0;

        System.out.println("Shapes summary:");
        for (Shape2D s : shapes) {
            double a = s.area();
            double p = s.perimeter();
            totalArea += a;
            totalPerimeter += p;
            System.out.printf("%-10s Area = %.4f\tPerimeter = %.4f%n", s.getName(), a, p);
        }

        System.out.printf("Total area = %.4f\tTotal perimeter = %.4f%n", totalArea, totalPerimeter);
    }
}

abstract class Shape2D {
    private final String name;

    protected Shape2D(String name) {
        this.name = name;
    }

    public String getName() {
        return name;
    }

    public abstract double area();

    public abstract double perimeter();
}

class Circle extends Shape2D {
    private final double radius;

    public Circle(double radius) {
        super("Circle");
        if (radius <= 0)
            throw new IllegalArgumentException("radius must be > 0");
        this.radius = radius;
    }

    @Override
    public double area() {
        return Math.PI * radius * radius;
    }

    @Override
    public double perimeter() {
        return 2 * Math.PI * radius;
    }

    public double getRadius() {
        return radius;
    }
}

class Rectangle extends Shape2D {
    private final double width;
    private final double height;

    public Rectangle(double width, double height) {
        super("Rectangle");
        if (width <= 0 || height <= 0)
            throw new IllegalArgumentException("width/height must be > 0");
        this.width = width;
        this.height = height;
    }

    @Override
    public double area() {
        return width * height;
    }

    @Override
    public double perimeter() {
        return 2 * (width + height);
    }

    public double getWidth() {
        return width;
    }

    public double getHeight() {
        return height;
    }
}

class Triangle extends Shape2D {
    private final double a, b, c;

    public Triangle(double a, double b, double c) {
        super("Triangle");
        if (!isValidSides(a, b, c))
            throw new IllegalArgumentException("Invalid triangle side lengths");
        this.a = a;
        this.b = b;
        this.c = c;
    }

    private static boolean isValidSides(double a, double b, double c) {
        if (a <= 0 || b <= 0 || c <= 0)
            return false;
        return a + b > c && a + c > b && b + c > a;
    }

    @Override
    public double area() {
        double s = (a + b + c) / 2.0;
        return Math.sqrt(Math.max(0.0, s * (s - a) * (s - b) * (s - c))); // Heron
    }

    @Override
    public double perimeter() {
        return a + b + c;
    }

    public double getA() {
        return a;
    }

    public double getB() {
        return b;
    }

    public double getC() {
        return c;
    }
}