import org.junit.jupiter.api.Test;
import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.InputStreamReader;
import java.math.BigDecimal;

import static org.junit.jupiter.api.Assertions.*;

public class TeylorTest {

    @Test
    public void testValidInput() throws Exception {
        String input = "0.5\n5\n";
        BufferedReader reader = new BufferedReader(
                new InputStreamReader(new ByteArrayInputStream(input.getBytes()))
        );

        TeylorBig teylor = MainBig.runCalculation(reader);
        assertNotNull(teylor);
        assertTrue(teylor.getSum().compareTo(BigDecimal.ZERO) > 0);
    }

    @Test
    public void testInvalidX() {
        String input = "2.0\n5\n";
        BufferedReader reader = new BufferedReader(
                new InputStreamReader(new ByteArrayInputStream(input.getBytes()))
        );

        assertThrows(IllegalArgumentException.class, () -> {
            MainBig.runCalculation(reader);
        });
    }

    @Test
    public void testZeroX() throws Exception {
        String input = "0\n5\n";
        BufferedReader reader = new BufferedReader(
                new InputStreamReader(new ByteArrayInputStream(input.getBytes()))
        );

        TeylorBig teylor = MainBig.runCalculation(reader);
        assertEquals(BigDecimal.ZERO, teylor.getSum().setScale(0));
    }

    @Test
    public void testNegativeX() throws Exception {
        String input = "-0.5\n5\n";
        BufferedReader reader = new BufferedReader(
                new InputStreamReader(new ByteArrayInputStream(input.getBytes()))
        );

        TeylorBig teylor = MainBig.runCalculation(reader);
        assertTrue(teylor.getSum().compareTo(BigDecimal.ZERO) < 0);
    }

    @Test
    public void testSmallK() throws Exception {
        String input = "0.5\n1\n";
        BufferedReader reader = new BufferedReader(
                new InputStreamReader(new ByteArrayInputStream(input.getBytes()))
        );

        TeylorBig teylor = MainBig.runCalculation(reader);
        assertNotEquals(BigDecimal.ZERO, teylor.getSum());
    }

    @Test
    public void testLargeK() throws Exception {
        String input = "0.5\n20\n";
        BufferedReader reader = new BufferedReader(
                new InputStreamReader(new ByteArrayInputStream(input.getBytes()))
        );

        TeylorBig teylor = MainBig.runCalculation(reader);
        assertNotNull(teylor.getSum());
        assertTrue(teylor.getSum().compareTo(BigDecimal.ZERO) > 0);
    }

    @Test
    public void testBoundaryXOne() throws Exception {
        String input = "1\n5\n";
        BufferedReader reader = new BufferedReader(
                new InputStreamReader(new ByteArrayInputStream(input.getBytes()))
        );

        TeylorBig teylor = MainBig.runCalculation(reader);
        assertNotNull(teylor.getSum());
    }

    @Test
    public void testBoundaryXMinusOne() throws Exception {
        String input = "-1\n5\n";
        BufferedReader reader = new BufferedReader(
                new InputStreamReader(new ByteArrayInputStream(input.getBytes()))
        );

        TeylorBig teylor = MainBig.runCalculation(reader);
        assertNotNull(teylor.getSum());
    }
}
