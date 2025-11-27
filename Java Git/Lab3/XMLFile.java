import org.w3c.dom.*;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import java.io.File;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

public class XMLFile {
    public static List<WashingMachine> readFromXML() {
        List<WashingMachine> machines = new ArrayList<>();
        String inputFile = "input.xml";
        SimpleDateFormat format = new SimpleDateFormat("dd-MM-yyyy");

        try {
            DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
            factory.setIgnoringComments(true);
            factory.setIgnoringElementContentWhitespace(true);
            DocumentBuilder builder = factory.newDocumentBuilder();

            Document doc = builder.parse(new File(inputFile));
            doc.getDocumentElement().normalize();

            NodeList machineNodes = doc.getElementsByTagName("machine");

            for (int i = 0; i < machineNodes.getLength(); i++) {
                Element machineElement = (Element) machineNodes.item(i);

                int id = Integer.parseInt(getTagValue(machineElement, "id"));
                String type = getTagValue(machineElement, "type");
                if (type.matches(".*\\d.*")) {
                    type = "0000";
                }
                String model = getTagValue(machineElement, "model");
                String power = getTagValue(machineElement, "power");
                String speed = getTagValue(machineElement, "speed");
                String dateRelease = getTagValue(machineElement, "dateRelease");
                String price = getTagValue(machineElement, "price");

                Date date = format.parse(dateRelease);

                WashingMachine machine = new WashingMachine(id, type, model, power, speed, date, price);
                machines.add(machine);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return machines;
    }

    private static String getTagValue(Element parent, String tagName) {
        NodeList list = parent.getElementsByTagName(tagName);
        if (list.getLength() > 0) {
            return list.item(0).getTextContent().trim();
        }
        return "";
    }
}
