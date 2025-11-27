import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.util.jar.JarEntry;
import java.util.jar.JarOutputStream;

public class JarClass {

    public JarClass(String jarFile) {
    }

    public static void archiveToJar(String[] inputFiles, String jarFile) {
        try (FileOutputStream fos = new FileOutputStream(jarFile);
             JarOutputStream jos = new JarOutputStream(fos)) {

            for (String file : inputFiles) {
                try (FileInputStream fis = new FileInputStream(file)) {
                    JarEntry entry = new JarEntry(file);
                    jos.putNextEntry(entry);

                    byte[] buffer = new byte[1024];
                    int length;
                    while ((length = fis.read(buffer)) != -1) {
                        jos.write(buffer, 0, length);
                    }

                    jos.closeEntry();
                    System.out.println("Файл " + file + " добавлен в JAR.");
                }
            }

            System.out.println("Все файлы успешно заархивированы в " + jarFile);

        } catch (Exception e) {
            System.out.println("Ошибка при архивации: " + e.getMessage());
        }
    }
}
