import java.io.File;
import java.text.SimpleDateFormat;
import java.util.List;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import org.w3c.dom.Document;
import org.w3c.dom.Element;

public class XMLSaver extends SaverDecorator {
    public XMLSaver(DataSaver wrappee) {
        super(wrappee);
    }

    @Override
    public void save(List<WashingMachine> machines, String filename) {
        try {
            DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
            DocumentBuilder builder = factory.newDocumentBuilder();
            Document doc = builder.newDocument();
            Element root = doc.createElement("items");
            doc.appendChild(root);

            SimpleDateFormat format = new SimpleDateFormat("dd-MM-yyyy");

            for (WashingMachine wm : machines) {
                Element item = doc.createElement("item");
                root.appendChild(item);

                Element id = doc.createElement("id");
                id.appendChild(doc.createTextNode(String.valueOf(wm.getId())));
                item.appendChild(id);

                Element type = doc.createElement("type");
                type.appendChild(doc.createTextNode(wm.getType()));
                item.appendChild(type);

                Element model = doc.createElement("model");
                model.appendChild(doc.createTextNode(wm.getModel()));
                item.appendChild(model);

                Element power = doc.createElement("power");
                power.appendChild(doc.createTextNode(wm.getPower()));
                item.appendChild(power);

                Element speed = doc.createElement("speed");
                speed.appendChild(doc.createTextNode(wm.getSpeed()));
                item.appendChild(speed);

                Element date = doc.createElement("date");
                date.appendChild(doc.createTextNode(format.format(wm.getDate())));
                item.appendChild(date);

                Element price = doc.createElement("price");
                price.appendChild(doc.createTextNode(wm.getPrice()));
                item.appendChild(price);
            }

            TransformerFactory tf = TransformerFactory.newInstance();
            Transformer transformer = tf.newTransformer();
            transformer.setOutputProperty(OutputKeys.INDENT, "yes");
            transformer.setOutputProperty(OutputKeys.METHOD, "xml");
            transformer.setOutputProperty(OutputKeys.ENCODING, "UTF-8");
            transformer.setOutputProperty("{http://xml.apache.org/xslt}indent-amount", "2");
            transformer.transform(new DOMSource(doc), new StreamResult(new File(filename)));

            System.out.println("Saved to XML: " + filename);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
