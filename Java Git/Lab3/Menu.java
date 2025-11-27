import java.nio.file.Path;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;
import javax.crypto.Cipher;
import javax.crypto.KeyGenerator;
import javax.crypto.SecretKey;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.util.Scanner;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;
import javax.crypto.Cipher;
import javax.crypto.KeyGenerator;
import javax.crypto.SecretKey;
import javax.crypto.spec.SecretKeySpec;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.ByteArrayOutputStream;

public class Menu {
    private final Scanner sc = new Scanner(System.in);
    private final ListMachine<WashingMachine> listMachine;
    private final MapMachine<WashingMachine> mapMachine;
    private SecretKey secretKey;

    public Menu(ListMachine<WashingMachine> listMachine, MapMachine<WashingMachine> mapMachine) throws Exception {
        this.listMachine = listMachine;
        this.mapMachine = mapMachine;

        KeyGenerator keyGen = KeyGenerator.getInstance("AES");
        keyGen.init(128);
        secretKey = keyGen.generateKey();
    }

    public void start() {
        while (true) {
            System.out.println("""
                1. Load from file 
                2. Load from XML-file
                3. Load from JSON-file
                4. Add 
                5. Update 
                6. Delete 
                7. Save to file 
                8. Save to XML-file
                9. Save to JSON-file
                10. Print list 
                11. Sort by ID 
                12. Find by ID 
                13. Encrypt file
                14. Decrypt file
                15. Archiving file
                16. JarArchiving file
                17. Exit
                """);

            int choice = sc.nextInt();
            sc.nextLine();

            switch (choice) {
                case 1 -> loadFromFile();
                case 2 -> loadFromXMLFile();
                case 3 -> loadFromJSONFile();
                case 4 -> addMachine();
                case 5 -> updateList();
                case 6 -> deleteMachine();
                case 7 -> listMachine.Save("output.txt");
                case 8 -> listMachine.SaveXML("output.xml");
                case 9 -> listMachine.SaveJSON("output.json");
                case 10 -> listMachine.printAll();
                case 11 -> { listMachine.sortById(); listMachine.printAll(); }
                case 12 -> findId();
                case 13 -> encryptFile();
                case 14 -> decryptFile();
                case 15 -> archiveFiles();
                case 16 -> JarClass.archiveToJar(new String[]{"output.txt", "output.xml", "output.json", "input.txt", "input.xml", "input.json"},
                        "archive.jar");
                case 17 -> { return; }
                default -> System.out.println("Wrong choice");
            }
        }
    }

    private void archiveFiles() {
        String[] inputFiles = { "output.txt", "output.xml", "output.json" };
        String zipFile = "archive.zip";

        try (FileOutputStream fos = new FileOutputStream(zipFile);
             ZipOutputStream zos = new ZipOutputStream(fos)) {

            for (String file : inputFiles) {
                try (FileInputStream fis = new FileInputStream(file)) {
                    ZipEntry entry = new ZipEntry(file);
                    zos.putNextEntry(entry);

                    byte[] buffer = new byte[1024];
                    int length;
                    while ((length = fis.read(buffer)) >= 0) {
                        zos.write(buffer, 0, length);
                    }

                    zos.closeEntry();
                    System.out.println("Файл " + file + " добавлен в архив.");
                }
            }
            System.out.println("Все файлы успешно заархивированы в " + zipFile);
        } catch (Exception e) {
            System.out.println("Ошибка при архивации: " + e.getMessage());
        }
    }

    public void encryptFile() {
        String inputFile = "output.txt";
        String outputFile = "encrypted.txt";
        String keyFile = "key.txt";

        try {
            KeyGenerator kg = KeyGenerator.getInstance("AES");
            kg.init(128);
            SecretKey secretKey = kg.generateKey();

            try (FileOutputStream keyOut = new FileOutputStream(keyFile)) {
                keyOut.write(secretKey.getEncoded());
            }

            Cipher cipher = Cipher.getInstance("AES");
            cipher.init(Cipher.ENCRYPT_MODE, secretKey);

            try (FileInputStream fis = new FileInputStream(inputFile);
                 FileOutputStream fos = new FileOutputStream(outputFile)) {

                byte[] buffer = new byte[1024];
                int bytesRead;
                while ((bytesRead = fis.read(buffer)) != -1) {
                    byte[] output = cipher.update(buffer, 0, bytesRead);
                    if (output != null) {
                        fos.write(output);
                    }
                }

                byte[] finalBytes = cipher.doFinal();
                if (finalBytes != null) {
                    fos.write(finalBytes);
                }
            }

            System.out.println("Файл " + inputFile + " успешно зашифрован в " + outputFile);

        } catch (Exception e) {
            System.out.println("Ошибка при шифровании: " + e.getMessage());
        }
    }

    public void decryptFile() {
        String inputFile = "encrypted.txt";
        String outputFile = "decrypted.txt";
        String keyFile = "key.txt";

        try {
            byte[] raw;
            try (FileInputStream fis = new FileInputStream(keyFile);
                 ByteArrayOutputStream bos = new ByteArrayOutputStream()) {
                byte[] buffer = new byte[1024];
                int bytesRead;
                while ((bytesRead = fis.read(buffer)) != -1) {
                    bos.write(buffer, 0, bytesRead);
                }
                raw = bos.toByteArray();
            }
            SecretKey secretKey = new SecretKeySpec(raw, "AES");

            Cipher cipher = Cipher.getInstance("AES");
            cipher.init(Cipher.DECRYPT_MODE, secretKey);

            try (FileInputStream fis = new FileInputStream(inputFile);
                 FileOutputStream fos = new FileOutputStream(outputFile)) {

                byte[] buffer = new byte[1024];
                int bytesRead;
                while ((bytesRead = fis.read(buffer)) != -1) {
                    byte[] output = cipher.update(buffer, 0, bytesRead);
                    if (output != null) {
                        fos.write(output);
                    }
                }

                byte[] finalBytes = cipher.doFinal();
                if (finalBytes != null) {
                    fos.write(finalBytes);
                }
            }

            System.out.println("Файл " + inputFile + " успешно расшифрован в " + outputFile);

        } catch (Exception e) {
            System.out.println("Ошибка при расшифровке: " + e.getMessage());
        }
    }

    private void loadFromJSONFile() {
        List<WashingMachine> machines = JSONFile.readFromJSON();
        if (machines.isEmpty()) {
            System.out.println("Error file");
        } else {
            for (WashingMachine machine : machines) {
                mapMachine.add(machine);
            }
            System.out.println("Loaded to mape:");
            mapMachine.printAll();
        }
    }

    private void loadFromXMLFile() {
        List<WashingMachine> machines = XMLFile.readFromXML();
        if (machines.isEmpty()) {
            System.out.println("Error file");
        } else {
            for (WashingMachine machine : machines) {
                mapMachine.add(machine);
            }
            System.out.println("Loaded to mape:");
            mapMachine.printAll();
        }
    }

    private Date readDate() {
        while (true) {
            System.out.println("Enter date (dd-MM-yyyy):");
            String strdate = sc.nextLine();
            try {
                SimpleDateFormat format = new SimpleDateFormat("dd-MM-yyyy");
                return format.parse(strdate);
            } catch (Exception e) {
                System.out.println("Error date.");
            }
        }
    }

    private void loadFromFile() {
        List<WashingMachine> machines = Files.readFromFile();
        if (machines.isEmpty()) {
            System.out.println("Error file");
        } else {
            for (WashingMachine machine : machines) {
                mapMachine.add(machine);
            }
            System.out.println("Loaded to mape:");
            mapMachine.printAll();
        }
    }

    private void addMachine() {
        System.out.println("Enter id");
        int id = sc.nextInt();
        sc.nextLine();

        System.out.println("Enter type");
        String type = sc.nextLine();

        System.out.println("Enter model");
        String model = sc.nextLine();

        System.out.println("Enter power");
        String power = sc.nextLine();

        System.out.println("Enter speed");
        String speed = sc.nextLine();

        Date date = readDate();

        System.out.println("Enter price");
        String price = sc.nextLine();

        listMachine.add(new WashingMachine(id, type, model, power, speed, date, price));
    }

    private void updateList() {
        System.out.println("Enter id to update");
        int id = sc.nextInt();
        sc.nextLine();

        System.out.println("Enter new type");
        String type = sc.nextLine();

        System.out.println("Enter new model");
        String model = sc.nextLine();

        System.out.println("Enter new power");
        String power = sc.nextLine();

        System.out.println("Enter new speed");
        String speed = sc.nextLine();

        Date date = readDate();

        System.out.println("Enter new price");
        String price = sc.nextLine();

        listMachine.update(id, new WashingMachine(id, type, model, power, speed, date, price));
    }

    private void deleteMachine() {
        System.out.println("Enter id for delete");
        int id = sc.nextInt();
        sc.nextLine();
        listMachine.delete(id);
    }

    private void findId() {
        System.out.println("Enter id:");
        int id = sc.nextInt();
        sc.nextLine();
        WashingMachine wm = mapMachine.findById(id);
        System.out.println(wm != null ? wm : "No found");
    }
}
