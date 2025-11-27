import java.io.BufferedReader;
import java.io.FileReader;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

public class Files {
    public static List<WashingMachine> readFromFile() {
        List<WashingMachine> machines = new ArrayList<>();
        String inputFile = "input.txt";
        try (BufferedReader reader = new BufferedReader(new FileReader(inputFile))) {
            String line;
            SimpleDateFormat format = new SimpleDateFormat("dd-MM-yyyy");

            while ((line = reader.readLine()) != null) {
                String[] word = line.split(" ");
                int id = Integer.parseInt(word[0]);
                String type = word[1];
                if (type.matches(".*\\d.*")) {
                    type = "0000";
                }
                String model = word[2];
                String power = word[3];
                String speed = word[4];
                String dateRelease = word[5];
                String price = word[6];

                Date date = format.parse(dateRelease);
                WashingMachine machine = new WashingMachine(id, type, model, power, speed, date, price);
                machines.add(machine);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return machines;
    }
}
