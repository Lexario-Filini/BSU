import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Comparator;
import java.util.Iterator;
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

import org.json.JSONArray;
import org.json.JSONObject;

import java.io.PrintWriter;

public class ListMachine<T extends Identifiable> extends AbstractFunc<T> implements Iterable<T> {
    private final List<T> list;

    public ListMachine(List<T> list) {
        this.list = list;
    }

    @Override
    public void add(T obj) {
        list.add(obj);
    }

    @Override
    public void update(int id, T obj) {
        for (int i = 0; i < list.size(); i++) {
            if (list.get(i).getId() == id) {
                list.set(i, obj);
                return;
            }
        }
    }

    @Override
    public void delete(int id) {
        list.removeIf(o -> o.getId() == id);
    }

    @Override
    public void Save(String filename) {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(filename))) {
            for (T obj : list) {
                writer.write(obj.toString());
                writer.newLine();
                writer.write("---------------");
                writer.newLine();
            }
            System.out.println("List save to file: " + filename);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void SaveXML(String filename) {
        try {
            DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
            DocumentBuilder builder = factory.newDocumentBuilder();
            Document doc = builder.newDocument();
            Element root = doc.createElement("items");
            doc.appendChild(root);
            SimpleDateFormat format = new SimpleDateFormat("dd-MM-yyyy");
            for (T obj : list) {
                if (obj instanceof WashingMachine) {
                    WashingMachine wm = (WashingMachine) obj;
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
            }
            TransformerFactory tf = TransformerFactory.newInstance();
            Transformer transformer = tf.newTransformer();
            transformer.setOutputProperty(OutputKeys.INDENT, "yes");
            transformer.setOutputProperty(OutputKeys.METHOD, "xml");
            transformer.setOutputProperty(OutputKeys.ENCODING, "UTF-8");
            transformer.setOutputProperty("{http://xml.apache.org/xslt}indent-amount", "2");
            transformer.transform(new DOMSource(doc), new StreamResult(new File(filename)));
            System.out.println("List saved to XML file: " + filename);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void SaveJSON(String filename) {
        try (PrintWriter pw = new PrintWriter(new FileWriter(filename))) {
            JSONArray jsonArray = new JSONArray();
            SimpleDateFormat format = new SimpleDateFormat("dd-MM-yyyy");

            for (T obj : list) {
                if (obj instanceof WashingMachine) {
                    WashingMachine wm = (WashingMachine) obj;
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
            }

            pw.print(jsonArray.toString(4));
            System.out.println("Data successfully saved to JSON file: " + filename);
        } catch (IOException e) {
            System.err.println("Error writing JSON file: " + filename + " - " + e.getMessage());
        }
    }

    public void printAll() {
        if (list.isEmpty()) {
            System.out.println("List empty");
        } else {
            Iterator<T> it = list.iterator();
            while (it.hasNext()) {
                T obj = it.next();
                System.out.println(obj);
                System.out.println("---------------");
            }
        }
    }

    public void sortById() {
        list.sort(Comparator.comparingInt(Identifiable::getId));
    }

    @Override
    public Iterator<T> iterator() {
        return list.iterator();
    }

    @Override
    public int getId() {
        return 0;
    }
}
