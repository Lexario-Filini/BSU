import org.junit.jupiter.api.*;
import java.io.*;
import java.text.SimpleDateFormat;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

public class FileTest {

    private final String testFile = "input.txt";

    @AfterEach
    void tearDown() {
        new java.io.File(testFile).delete();
    }

    @Test
    void testValidDataParsedCorrectly() throws Exception {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(testFile))) {
            writer.write("12 TypeA Philips 60VT 1000rot/sec 12-01-2025 1000USD\n");
        }

        List<WashingMachine> machines = Files.readFromFile();
        assertEquals(1, machines.size());
        WashingMachine m = machines.get(0);

        assertEquals(12, m.getId());
        assertEquals("TypeA", m.getType());
        assertEquals("Philips", m.getModel());
        assertEquals("60VT", m.getPower());
        assertEquals("1000rot/sec", m.getSpeed());
        assertEquals(new SimpleDateFormat("dd-MM-yyyy").parse("12-01-2025"), m.getDate());
        assertEquals("1000USD", m.getPrice());
    }

    @Test
    void testTypeWithDigitsReplacedBy0000() throws Exception {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(testFile))) {
            writer.write("9 Type123 LG1 400VT 2500rot/sec 05-06-2012 3500USD\n");
        }

        List<WashingMachine> machines = Files.readFromFile();
        assertEquals("0000", machines.get(0).getType());
    }

    @Test
    void testEmptyFileReturnsEmptyList() throws Exception {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(testFile))) {
        }

        List<WashingMachine> machines = Files.readFromFile();
        assertTrue(machines.isEmpty());
    }

    @Test
    void testLineWithExtraFieldsStillParses() throws Exception {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(testFile))) {
            writer.write("1 TypeA ModelX 100VT 500rot/sec 12-01-2025 200USD EXTRA\n");
        }

        List<WashingMachine> machines = Files.readFromFile();
        assertEquals(1, machines.size());
        assertEquals("ModelX", machines.get(0).getModel());
    }

    @Test
    void testInvalidDateFormatSkipped() throws Exception {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(testFile))) {
            writer.write("3 TypeD ModelD 200VT 800rot/sec 2025-01-12 500USD\n");
        }

        List<WashingMachine> machines = Files.readFromFile();
        assertFalse(machines.isEmpty(), "Неверный формат даты должен приводить к пропуску строки");
    }

    @Test
    void testMultipleValidLines() throws Exception {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(testFile))) {
            writer.write("4 TypeA ModelA 150VT 600rot/sec 10-10-2010 100USD\n");
            writer.write("5 TypeB ModelB 250VT 1200rot/sec 11-11-2011 200USD\n");
        }

        List<WashingMachine> machines = Files.readFromFile();
        assertEquals(2, machines.size());
        assertEquals(4, machines.get(0).getId());
        assertEquals(5, machines.get(1).getId());
    }
}
