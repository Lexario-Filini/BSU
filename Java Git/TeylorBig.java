import java.math.BigDecimal;
import java.math.MathContext;
import java.math.RoundingMode;

public class TeylorBig {
    private BigDecimal sum = BigDecimal.ZERO;

    public void calculate(BigDecimal x, int k) {
        MathContext mc = new MathContext(k + 5, RoundingMode.HALF_UP);
        BigDecimal e = BigDecimal.TEN.pow(-k, mc);

        BigDecimal slug = x; // первый член: x^(2*0+1)/1
        int n = 0;

        while (slug.abs().compareTo(e) > 0) {
            sum = sum.add(slug, mc);
            n++;
            slug = slug.multiply(x, mc).multiply(x, mc);
            BigDecimal numerator = BigDecimal.valueOf(2 * n - 1);
            BigDecimal denominator = BigDecimal.valueOf(2 * n + 1);
            slug = slug.multiply(numerator, mc).divide(denominator, mc);
        }

        sum = sum.multiply(BigDecimal.valueOf(2), mc);
    }

    public BigDecimal getSum() {
        return sum;
    }
}
