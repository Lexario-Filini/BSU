import java.util.List;

public abstract class SaverDecorator implements DataSaver {
    protected final DataSaver wrappee;

    public SaverDecorator(DataSaver wrappee) {
        this.wrappee = wrappee;
    }

    @Override
    public void save(List<WashingMachine> machines, String filename) {
        wrappee.save(machines, filename);
    }
}
