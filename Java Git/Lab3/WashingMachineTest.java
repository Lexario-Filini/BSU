import org.junit.jupiter.api.Test;
import java.text.SimpleDateFormat;
import java.util.Date;

import static org.junit.jupiter.api.Assertions.*;

public class WashingMachineTest {

    @Test
    void testNegativeId() throws Exception {
        Date date = new SimpleDateFormat("dd-MM-yyyy").parse("01-01-2020");
        WashingMachine wm = new WashingMachine(-1, "TypeNeg", "TestModel", "50VT", "500rot/sec", date, "50USD");

        assertEquals(-1, wm.getId(), "ID может быть отрицательным, проверяем сохранение значения");
    }

    @Test
    void testEmptyStrings() throws Exception {
        Date date = new SimpleDateFormat("dd-MM-yyyy").parse("02-02-2022");
        WashingMachine wm = new WashingMachine(0, "", "", "", "", date, "");

        assertEquals("", wm.getType());
        assertEquals("", wm.getModel());
        assertEquals("", wm.getPower());
        assertEquals("", wm.getSpeed());
        assertEquals("", wm.getPrice());
    }

    @Test
    void testToStringContainsAllFields() throws Exception {
        Date date = new SimpleDateFormat("dd-MM-yyyy").parse("15-03-2023");
        WashingMachine wm = new WashingMachine(5, "TypeX", "Bosch", "300VT", "1500rot/sec", date, "700USD");

        String output = wm.toString();

        assertTrue(output.contains("id = 5"));
        assertTrue(output.contains("type = TypeX"));
        assertTrue(output.contains("model = Bosch"));
        assertTrue(output.contains("power = 300VT"));
        assertTrue(output.contains("speed = 1500rot/sec"));
        assertTrue(output.contains("date = 15-03-2023"));
        assertTrue(output.contains("price = 700USD"));
    }

    @Test
    void testSetIdMultipleTimes() throws Exception {
        Date date = new SimpleDateFormat("dd-MM-yyyy").parse("01-01-2021");
        WashingMachine wm = new WashingMachine(1, "TypeA", "LG", "100VT", "1000rot/sec", date, "200USD");

        wm.setId(10);
        assertEquals(10, wm.getId());

        wm.setId(20);
        assertEquals(20, wm.getId());

        wm.setId(30);
        assertEquals(30, wm.getId());
    }

    @Test
    void testDifferentMachinesNotEqual() throws Exception {
        Date d1 = new SimpleDateFormat("dd-MM-yyyy").parse("01-01-2020");
        Date d2 = new SimpleDateFormat("dd-MM-yyyy").parse("02-02-2021");

        WashingMachine wm1 = new WashingMachine(1, "TypeA", "Philips", "100VT", "1000rot/sec", d1, "300USD");
        WashingMachine wm2 = new WashingMachine(2, "TypeB", "Bosch", "200VT", "2000rot/sec", d2, "400USD");

        assertNotEquals(wm1.getId(), wm2.getId());
        assertNotEquals(wm1.getModel(), wm2.getModel());
        assertNotEquals(wm1.getPower(), wm2.getPower());
        assertNotEquals(wm1.getSpeed(), wm2.getSpeed());
        assertNotEquals(wm1.getDate(), wm2.getDate());
        assertNotEquals(wm1.getPrice(), wm2.getPrice());
    }

    @Test
    void testDateFormattingEdgeCase() throws Exception {
        Date date = new SimpleDateFormat("dd-MM-yyyy").parse("01-01-1900");
        WashingMachine wm = new WashingMachine(99, "OldType", "Antique", "10VT", "100rot/sec", date, "5USD");

        String output = wm.toString();
        assertTrue(output.contains("date = 01-01-1900"), "Дата должна корректно форматироваться даже для старых значений");
    }
}
