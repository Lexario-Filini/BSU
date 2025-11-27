import org.junit.jupiter.api.*;
import java.io.*;
import java.text.SimpleDateFormat;
import java.util.*;

import static org.junit.jupiter.api.Assertions.*;

public class ListMachineTest {

    private WashingMachine wm1;
    private WashingMachine wm2;
    private WashingMachine wm3;
    private ListMachine<WashingMachine> listMachine;

    @BeforeEach
    void setUp() throws Exception {
        Date d1 = new SimpleDateFormat("dd-MM-yyyy").parse("01-01-2020");
        Date d2 = new SimpleDateFormat("dd-MM-yyyy").parse("02-02-2021");
        Date d3 = new SimpleDateFormat("dd-MM-yyyy").parse("03-03-2022");

        wm1 = new WashingMachine(1, "TypeA", "Philips", "100VT", "1000rot/sec", d1, "300USD");
        wm2 = new WashingMachine(2, "TypeB", "Bosch", "200VT", "2000rot/sec", d2, "400USD");
        wm3 = new WashingMachine(3, "TypeC", "LG", "300VT", "3000rot/sec", d3, "500USD");

        listMachine = new ListMachine<>(new ArrayList<>());
    }

    @Test
    void testAddAndIterator() {
        listMachine.add(wm1);
        listMachine.add(wm2);
        Iterator<WashingMachine> it = listMachine.iterator();
        assertTrue(it.hasNext());
        assertEquals(wm1, it.next());
        assertEquals(wm2, it.next());
    }

    @Test
    void testUpdate() throws Exception {
        listMachine.add(wm1);
        Date d3 = new SimpleDateFormat("dd-MM-yyyy").parse("03-03-2022");
        WashingMachine wmUpdated = new WashingMachine(1, "TypeX", "LG", "300VT", "1500rot/sec", d3, "500USD");
        listMachine.update(1, wmUpdated);
        assertEquals("LG", listMachine.iterator().next().getModel());
    }

    @Test
    void testUpdateNonExistingIdDoesNothing() {
        listMachine.add(wm1);
        listMachine.update(99, wm2);
        assertEquals(1, countElements(listMachine));
        assertEquals("Philips", listMachine.iterator().next().getModel());
    }

    @Test
    void testDelete() {
        listMachine.add(wm1);
        listMachine.add(wm2);
        listMachine.delete(1);
        Iterator<WashingMachine> it = listMachine.iterator();
        assertTrue(it.hasNext());
        WashingMachine remaining = it.next();
        assertEquals(2, remaining.getId());
        assertFalse(it.hasNext());
    }

    @Test
    void testDeleteNonExistingIdDoesNothing() {
        listMachine.add(wm1);
        listMachine.delete(99);
        assertEquals(1, countElements(listMachine));
    }

    @Test
    void testSortById() {
        listMachine.add(wm2);
        listMachine.add(wm1);
        listMachine.sortById();
        Iterator<WashingMachine> it = listMachine.iterator();
        assertEquals(1, it.next().getId());
        assertEquals(2, it.next().getId());
    }

    @Test
    void testSortByIdWithMultipleElements() {
        listMachine.add(wm3);
        listMachine.add(wm1);
        listMachine.add(wm2);
        listMachine.sortById();
        Iterator<WashingMachine> it = listMachine.iterator();
        assertEquals(1, it.next().getId());
        assertEquals(2, it.next().getId());
        assertEquals(3, it.next().getId());
    }

    @Test
    void testSaveToFile() throws Exception {
        listMachine.add(wm1);
        String filename = "testSave.txt";
        listMachine.Save(filename);
        File f = new File(filename);
        assertTrue(f.exists());
        String content = new String(java.nio.file.Files.readAllBytes(f.toPath()));
        assertTrue(content.contains("Philips"));
        f.delete();
    }

    @Test
    void testSaveXML() throws Exception {
        listMachine.add(wm1);
        String filename = "testSave.xml";
        listMachine.SaveXML(filename);
        File f = new File(filename);
        assertTrue(f.exists());
        String content = new String(java.nio.file.Files.readAllBytes(f.toPath()));
        assertTrue(content.contains("<model>Philips</model>"));
        f.delete();
    }

    @Test
    void testSaveXMLWithMultipleObjects() throws Exception {
        listMachine.add(wm1);
        listMachine.add(wm2);
        String filename = "testMulti.xml";
        listMachine.SaveXML(filename);
        String content = new String(java.nio.file.Files.readAllBytes(new File(filename).toPath()));
        assertTrue(content.contains("<model>Philips</model>"));
        assertTrue(content.contains("<model>Bosch</model>"));
        new File(filename).delete();
    }

    @Test
    void testSaveJSON() throws Exception {
        listMachine.add(wm1);
        String filename = "testSave.json";
        listMachine.SaveJSON(filename);
        File f = new File(filename);
        assertTrue(f.exists());
        String content = new String(java.nio.file.Files.readAllBytes(f.toPath()));
        assertTrue(content.contains("\"model\": \"Philips\""));
        f.delete();
    }

    @Test
    void testSaveJSONWithMultipleObjects() throws Exception {
        listMachine.add(wm1);
        listMachine.add(wm2);
        listMachine.add(wm3);
        String filename = "testMulti.json";
        listMachine.SaveJSON(filename);
        String content = new String(java.nio.file.Files.readAllBytes(new File(filename).toPath()));
        assertTrue(content.contains("\"model\": \"Philips\""));
        assertTrue(content.contains("\"model\": \"Bosch\""));
        assertTrue(content.contains("\"model\": \"LG\""));
        new File(filename).delete();
    }

    @Test
    void testPrintAllEmpty() {
        ListMachine<WashingMachine> emptyList = new ListMachine<>(new ArrayList<>());
        emptyList.printAll();
    }

    @Test
    void testPrintAllWithElements() {
        listMachine.add(wm1);
        listMachine.add(wm2);
        listMachine.printAll();
    }

    private int countElements(ListMachine<WashingMachine> lm) {
        int count = 0;
        Iterator<WashingMachine> it = lm.iterator();
        while (it.hasNext()) {
            it.next();
            count++;
        }
        return count;
    }
}
