import org.json.JSONArray;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.FileReader;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

public class JSONFile {
    public JSONFile(String testFile) {
    }

    public static List<WashingMachine> readFromJSON() {
        List<WashingMachine> machines = new ArrayList<>();
        String inputFile = "input.json";
        SimpleDateFormat format = new SimpleDateFormat("dd-MM-yyyy" );

        try (BufferedReader reader = new BufferedReader(new FileReader(inputFile))) {
            StringBuilder sb = new StringBuilder();
            String line;
            while ((line = reader.readLine()) != null) {
                sb.append(line);
            }

            JSONArray arr = new JSONArray(sb.toString());
            for (int i = 0; i < arr.length(); i++) {
                JSONObject obj = arr.getJSONObject(i);

                int id = obj.getInt("id");
                String type = obj.getString("type");
                if (type.matches(".*\\d.*")) {
                    type = "0000";
                }
                String model = obj.getString("model");
                String power = obj.getString("power");
                String speed = obj.getString("speed");
                String dateRelease = obj.getString("dateRelease");
                String price = obj.getString("price");

                Date date = format.parse(dateRelease);

                WashingMachine machine = new WashingMachine(id, type, model, power, speed, date, price);
                machines.add(machine);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return machines;
    }

    public void delete() {
    }
}
