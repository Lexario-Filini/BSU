import org.junit.jupiter.api.*;
import java.io.*;
import java.text.SimpleDateFormat;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

public class XMLFileTest {

    private final String testFile = "input.xml";

    @AfterEach
    void tearDown() {
        new File(testFile).delete();
    }

    @Test
    void testValidXmlParsedCorrectly() throws Exception {
        String xml = """
            <items>
              <machine>
                <id>1</id>
                <type>TypeA</type>
                <model>Philips</model>
                <power>100VT</power>
                <speed>1000rot/sec</speed>
                <dateRelease>01-01-2020</dateRelease>
                <price>300USD</price>
              </machine>
            </items>
            """;
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(testFile))) {
            writer.write(xml);
        }
        List<WashingMachine> machines = XMLFile.readFromXML();
        assertEquals(1, machines.size());
        WashingMachine wm = machines.get(0);
        assertEquals(1, wm.getId());
        assertEquals("TypeA", wm.getType());
        assertEquals("Philips", wm.getModel());
        assertEquals("100VT", wm.getPower());
        assertEquals("1000rot/sec", wm.getSpeed());
        assertEquals(new SimpleDateFormat("dd-MM-yyyy").parse("01-01-2020"), wm.getDate());
        assertEquals("300USD", wm.getPrice());
    }

    @Test
    void testTypeWithDigitsReplacedBy0000() throws Exception {
        String xml = """
            <items>
              <machine>
                <id>2</id>
                <type>Type123</type>
                <model>LG</model>
                <power>200VT</power>
                <speed>1200rot/sec</speed>
                <dateRelease>02-02-2021</dateRelease>
                <price>400USD</price>
              </machine>
            </items>
            """;
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(testFile))) {
            writer.write(xml);
        }
        List<WashingMachine> machines = XMLFile.readFromXML();
        assertEquals("0000", machines.get(0).getType());
    }

    @Test
    void testMultipleMachinesParsed() throws Exception {
        String xml = """
            <items>
              <machine>
                <id>3</id>
                <type>TypeA</type>
                <model>Samsung</model>
                <power>300VT</power>
                <speed>1500rot/sec</speed>
                <dateRelease>03-03-2022</dateRelease>
                <price>500USD</price>
              </machine>
              <machine>
                <id>4</id>
                <type>TypeB</type>
                <model>Bosch</model>
                <power>400VT</power>
                <speed>2000rot/sec</speed>
                <dateRelease>04-04-2023</dateRelease>
                <price>600USD</price>
              </machine>
            </items>
            """;
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(testFile))) {
            writer.write(xml);
        }
        List<WashingMachine> machines = XMLFile.readFromXML();
        assertEquals(2, machines.size());
        assertEquals("Samsung", machines.get(0).getModel());
        assertEquals("Bosch", machines.get(1).getModel());
    }

    @Test
    void testEmptyFileReturnsEmptyList() throws Exception {
        String xml = "<items></items>";
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(testFile))) {
            writer.write(xml);
        }
        List<WashingMachine> machines = XMLFile.readFromXML();
        assertTrue(machines.isEmpty());
    }

    @Test
    void testInvalidDateFormatSkipped() throws Exception {
        String xml = """
            <items>
              <machine>
                <id>5</id>
                <type>TypeX</type>
                <model>Whirlpool</model>
                <power>500VT</power>
                <speed>2500rot/sec</speed>
                <dateRelease>12-01-2025</dateRelease>
                <price>700USD</price>
              </machine>
            </items>
            """;
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(testFile))) {
            writer.write(xml);
        }
        List<WashingMachine> machines = XMLFile.readFromXML();
        assertFalse(machines.isEmpty());
    }
}
