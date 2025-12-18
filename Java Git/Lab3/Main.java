import java.util.ArrayList;
import java.util.List;
import javax.swing.SwingUtilities;

public class Main {
    public static void main(String[] args) throws Exception {
        List<WashingMachine> sharedList = new ArrayList<>();

        MapMachine<WashingMachine> mapMachine = new MapMachine<>(sharedList);
        ListMachine<WashingMachine> listMachine = new ListMachine<>(sharedList);

        SwingUtilities.invokeLater(() -> {
            GUIApplication gui = new GUIApplication(listMachine, mapMachine);
            gui.setVisible(true);
        });
    }
}