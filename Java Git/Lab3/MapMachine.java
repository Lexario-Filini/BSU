import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.List;
import java.util.Map;
import java.util.HashMap;

public class MapMachine<T extends Identifiable> extends AbstractFunc<T> {
    private final List<T> list;
    private final Map<Integer, T> map;

    public MapMachine(List<T> list) {
        this.list = list;
        this.map = new HashMap<>();
        syncMap();
    }

    private void syncMap() {
        map.clear();
        for (T obj : list) {
            map.put(obj.getId(), obj);
        }
    }

    @Override
    public void add(T obj) {
        list.add(obj);
        map.put(obj.getId(), obj);
    }

    @Override
    public void update(int id, T obj) {
        for (int i = 0; i < list.size(); i++) {
            if (list.get(i).getId() == id) {
                list.set(i, obj);
                map.put(id, obj);
                return;
            }
        }
    }

    @Override
    public void delete(int id) {
        list.removeIf(o -> o.getId() == id);
        map.remove(id);
    }

    @Override
    public void Save(String filename) {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(filename))) {
            for (T obj : map.values()) {
                writer.write(obj.toString());
                writer.newLine();
                writer.write("---------------");
                writer.newLine();
            }
            System.out.println("Map save to file: " + filename);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public T findById(int id) {
        return map.get(id);
    }

    public void printAll() {
        if (map.isEmpty()) {
            System.out.println("Map empty");
        } else {
            for (T obj : map.values()) {
                System.out.println(obj);
                System.out.println("---------------");
            }
        }
    }

    @Override
    public int getId() {
        return 0;
    }
}
