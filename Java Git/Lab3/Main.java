import java.util.ArrayList;
import java.util.List;

public class Main {
    public static void main(String[] args) throws Exception {
        List<WashingMachine> sharedList = new ArrayList<>();

        ListMachine<WashingMachine> listMachine = new ListMachine<>(sharedList);
        MapMachine<WashingMachine> mapMachine = new MapMachine<>(sharedList);

        Menu menu = new Menu(listMachine, mapMachine);
        menu.start();
    }
}