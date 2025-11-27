public abstract class AbstractFunc<T> {
    public abstract void add(T obj);
    public abstract void update(int id, T obj);
    public abstract void delete (int id);
    public abstract void Save(String filename);
    public abstract int getId();
}
