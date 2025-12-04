import java.io.BufferedWriter;
import java.io.FileWriter;
import java.util.List;

public class BaseSaver implements DataSaver {
    @Override
    public void save(List<WashingMachine> machines, String filename) {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(filename))) {
            for (WashingMachine wm : machines) {
                writer.write(wm.toString());
                writer.newLine();
                writer.write("---------------");
                writer.newLine();
            }
            System.out.println("Saved to TXT: " + filename);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
