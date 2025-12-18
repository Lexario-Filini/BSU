import java.util.Comparator;
import java.util.Iterator;
import java.util.List;

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
        list.add(obj);
    }

    @Override
    public void delete(int id) {
        list.removeIf(o -> o.getId() == id);
    }

    @Override
    public void Save(String filename) {
    }

    public void printAll() {
        if (list.isEmpty()) {
            System.out.println("List empty");
        } else {
            for (T obj : list) {
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

    public List<T> getList() {
        return list;
    }
}