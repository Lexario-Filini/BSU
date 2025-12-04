import java.io.FileWriter;
import java.io.PrintWriter;
import java.text.SimpleDateFormat;
import java.util.List;
import org.json.JSONArray;
import org.json.JSONObject;

public class JSONSaver extends SaverDecorator {
    public JSONSaver(DataSaver wrappee) {
        super(wrappee);
    }

    @Override
    public void save(List<WashingMachine> machines, String filename) {
        try (PrintWriter pw = new PrintWriter(new FileWriter(filename))) {
            JSONArray jsonArray = new JSONArray();
            SimpleDateFormat format = new SimpleDateFormat("dd-MM-yyyy");

            for (WashingMachine wm : machines) {
                JSONObject jsonObject = new JSONObject();
                jsonObject.put("id", wm.getId());
                jsonObject.put("type", wm.getType());
                jsonObject.put("model", wm.getModel());
                jsonObject.put("power", wm.getPower());
                jsonObject.put("speed", wm.getSpeed());
                jsonObject.put("date", format.format(wm.getDate()));
                jsonObject.put("price", wm.getPrice());
                jsonArray.put(jsonObject);
            }

            pw.print(jsonArray.toString(4));
            System.out.println("Saved to JSON: " + filename);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
