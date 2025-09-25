public class Teylor {
    private double sum = 0;

    public void calculate(double x, int k) {
        double e = Math.pow(10, -k);
        double slug = x;
        int st = 0;
        sum = 0;

        while (Math.abs(slug) > e) {
            sum += slug;
            st++;
            slug *= x * x;
            slug *= 1.0 * (2 * st - 1) / (2 * st + 1);
        }
        sum *= 2;
    }
    public double getSum() {
        return sum;
    }
}
