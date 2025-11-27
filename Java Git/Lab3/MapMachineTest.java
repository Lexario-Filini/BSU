import org.junit.jupiter.api.*;
import java.io.*;
import java.text.SimpleDateFormat;
import java.util.*;

import static org.junit.jupiter.api.Assertions.*;

public class MapMachineTest {

    private WashingMachine wm1;
    private WashingMachine wm2;
    private WashingMachine wm3;
    private MapMachine<WashingMachine> mapMachine;

    @BeforeEach
    void setUp() throws Exception {
        Date d1 = new SimpleDateFormat("dd-MM-yyyy").parse("01-01-2020");
        Date d2 = new SimpleDateFormat("dd-MM-yyyy").parse("02-02-2021");
        Date d3 = new SimpleDateFormat("dd-MM-yyyy").parse("03-03-2022");

        wm1 = new WashingMachine(1, "TypeA", "Philips", "100VT", "1000rot/sec", d1, "300USD");
        wm2 = new WashingMachine(2, "TypeB", "Bosch", "200VT", "2000rot/sec", d2, "400USD");
        wm3 = new WashingMachine(1, "TypeC", "LG", "300VT", "3000rot/sec", d3, "500USD");

        mapMachine = new MapMachine<>(new ArrayList<>());
    }

    @Test
    void testAddAndFindById() {
        mapMachine.add(wm1);
        mapMachine.add(wm2);
        assertEquals(wm1, mapMachine.findById(1));
        assertEquals(wm2, mapMachine.findById(2));
    }

    @Test
    void testUpdateExistingId() throws Exception {
        mapMachine.add(wm1);
        Date dNew = new SimpleDateFormat("dd-MM-yyyy").parse("10-10-2025");
        WashingMachine wmUpdated = new WashingMachine(1, "TypeX", "LG", "500VT", "2500rot/sec", dNew, "700USD");
        mapMachine.update(1, wmUpdated);
        assertEquals("LG", mapMachine.findById(1).getModel());
        assertEquals("500VT", mapMachine.findById(1).getPower());
    }

    @Test
    void testUpdateNonExistingIdDoesNothing() {
        mapMachine.add(wm1);
        mapMachine.update(99, wm2);
        assertEquals("Philips", mapMachine.findById(1).getModel());
        assertNull(mapMachine.findById(99));
    }

    @Test
    void testDeleteExistingId() {
        mapMachine.add(wm1);
        mapMachine.add(wm2);
        mapMachine.delete(1);
        assertNull(mapMachine.findById(1));
        assertEquals(wm2, mapMachine.findById(2));
    }

    @Test
    void testDeleteNonExistingIdDoesNothing() {
        mapMachine.add(wm1);
        mapMachine.delete(99);
        assertEquals(wm1, mapMachine.findById(1));
    }

    @Test
    void testSaveToFile() throws Exception {
        mapMachine.add(wm1);
        mapMachine.add(wm2);
        String filename = "mapSave.txt";
        mapMachine.Save(filename);
        File f = new File(filename);
        assertTrue(f.exists());
        String content = new String(java.nio.file.Files.readAllBytes(f.toPath()));
        assertTrue(content.contains("Philips"));
        assertTrue(content.contains("Bosch"));
        f.delete();
    }

    @Test
    void testPrintAllEmpty() {
        MapMachine<WashingMachine> emptyMap = new MapMachine<>(new ArrayList<>());
        emptyMap.printAll();
    }

    @Test
    void testPrintAllWithElements() {
        mapMachine.add(wm1);
        mapMachine.add(wm2);
        mapMachine.printAll();
    }

    @Test
    void testConstructorSyncsMap() {
        List<WashingMachine> initialList = new ArrayList<>();
        initialList.add(wm1);
        initialList.add(wm2);
        MapMachine<WashingMachine> machine = new MapMachine<>(initialList);
        assertEquals(wm1, machine.findById(1));
        assertEquals(wm2, machine.findById(2));
    }

    @Test
    void testAddDuplicateIdOverrides() {
        mapMachine.add(wm1);
        mapMachine.add(wm3);
        assertEquals("LG", mapMachine.findById(1).getModel());
        assertEquals(1, countElements(mapMachine));
    }

    @Test
    void testSaveEmptyMap() throws Exception {
        MapMachine<WashingMachine> machine = new MapMachine<>(new ArrayList<>());
        String filename = "emptyMap.txt";
        machine.Save(filename);
        File f = new File(filename);
        assertTrue(f.exists());
        String content = new String(java.nio.file.Files.readAllBytes(f.toPath()));
        assertTrue(content.isEmpty());
        f.delete();
    }

    @Test
    void testFindByIdAfterDelete() {
        mapMachine.add(wm1);
        mapMachine.add(wm2);
        mapMachine.delete(2);
        assertNull(mapMachine.findById(2));
        assertEquals(wm1, mapMachine.findById(1));
    }

    @Test
    void testListAndMapConsistency() {
        mapMachine.add(wm1);
        mapMachine.add(wm2);
        mapMachine.delete(1);
        assertNull(mapMachine.findById(1));
        assertEquals(1, countElements(mapMachine));
    }

    private int countElements(MapMachine<WashingMachine> mm) {
        int count = 0;
        for (int id : Arrays.asList(1, 2, 3)) {
            if (mm.findById(id) != null) count++;
        }
        return count;
    }
}
