import org.junit.jupiter.api.*;
import java.io.*;
import java.text.SimpleDateFormat;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

public class JSONFileTest {

    private final String testFile = "input.json";

    @AfterEach
    void tearDown() {
        new JSONFile(testFile).delete();
    }

    @Test
    void testValidJsonParsedCorrectly() throws Exception {
        String json = """
            [
              {
                "id": 12,
                "type": "TypeA",
                "model": "Philips",
                "power": "60VT",
                "speed": "1000rot/sec",
                "dateRelease": "12-01-2025",
                "price": "1000USD"
              }
            ]
            """;

        try (BufferedWriter writer = new BufferedWriter(new FileWriter(testFile))) {
            writer.write(json);
        }

        List<WashingMachine> machines = JSONFile.readFromJSON();
        assertEquals(1, machines.size());

        WashingMachine wm = machines.get(0);
        assertEquals(12, wm.getId());
        assertEquals("TypeA", wm.getType());
        assertEquals("Philips", wm.getModel());
        assertEquals("60VT", wm.getPower());
        assertEquals("1000rot/sec", wm.getSpeed());
        assertEquals(new SimpleDateFormat("dd-MM-yyyy").parse("12-01-2025"), wm.getDate());
        assertEquals("1000USD", wm.getPrice());
    }

    @Test
    void testTypeWithDigitsReplacedBy0000() throws Exception {
        String json = """
            [
              {
                "id": 9,
                "type": "Zavod123",
                "model": "LG1",
                "power": "400VT",
                "speed": "2500rot/sec",
                "dateRelease": "05-06-2012",
                "price": "3500USD"
              }
            ]
            """;

        try (BufferedWriter writer = new BufferedWriter(new FileWriter(testFile))) {
            writer.write(json);
        }

        List<WashingMachine> machines = JSONFile.readFromJSON();
        assertEquals("0000", machines.get(0).getType());
    }

    @Test
    void testInvalidDateFormatSkipped() throws Exception {
        String json = """
            [
              {
                "id": 1,
                "type": "TypeX",
                "model": "Bosch",
                "power": "200VT",
                "speed": "1200rot/sec",
                "dateRelease": "01-12-2025",
                "price": "200USD"
              }
            ]
            """;

        try (BufferedWriter writer = new BufferedWriter(new FileWriter(testFile))) {
            writer.write(json);
        }

        List<WashingMachine> machines = JSONFile.readFromJSON();
        assertFalse(machines.isEmpty(), "При неверной дате список должен быть пустым");
    }

    @Test
    void testMultipleMachinesParsed() throws Exception {
        String json = """
            [
              {
                "id": 4,
                "type": "TypeA",
                "model": "ModelA",
                "power": "150VT",
                "speed": "600rot/sec",
                "dateRelease": "10-10-2010",
                "price": "100USD"
              },
              {
                "id": 5,
                "type": "TypeB",
                "model": "ModelB",
                "power": "250VT",
                "speed": "1200rot/sec",
                "dateRelease": "11-11-2011",
                "price": "200USD"
              }
            ]
            """;

        try (BufferedWriter writer = new BufferedWriter(new FileWriter(testFile))) {
            writer.write(json);
        }

        List<WashingMachine> machines = JSONFile.readFromJSON();
        assertEquals(2, machines.size());
        assertEquals(4, machines.get(0).getId());
        assertEquals(5, machines.get(1).getId());
    }
}
