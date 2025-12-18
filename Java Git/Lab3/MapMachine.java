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
        refreshMap();
    }

    @Override
    public void add(T obj) {
        map.put(obj.getId(), obj);
    }

    @Override
    public void update(int id, T obj) {
        map.put(id, obj);
    }

    @Override
    public void delete(int id) {
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

    public void refreshMap() {
        map.clear();
        for (T obj : list) {
            map.put(obj.getId(), obj);
        }
    }

    public int getMapSize() {
        return map.size();
    }
}