import java.io.File;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicLong;
import java.util.stream.Collectors;

public class FileSystemScanner {

    private static final AtomicLong fileCount = new AtomicLong();
    private static final AtomicLong totalSize = new AtomicLong();
    private static final Map<String, AtomicLong> extensionCount = new ConcurrentHashMap<>();

    public static void main(String[] args) throws InterruptedException {
        if (args.length != 1) {
            System.out.println("Usage: java FileSystemScanner <directory>");
            return;
        }

        File root = new File(args[0]);
        if (!root.exists() || !root.isDirectory()) {
            System.out.println("Invalid directory");
            return;
        }

        ExecutorService executor = Executors.newFixedThreadPool(Runtime.getRuntime().availableProcessors());
        executor.submit(() -> scanDirectory(root, executor));
        executor.shutdown();
        executor.awaitTermination(Long.MAX_VALUE, TimeUnit.MILLISECONDS);

        System.out.println("\n=== Scan Results ===");
        System.out.printf("Total files: %d%n", fileCount.get());
        System.out.printf("Total size: %,d bytes%n", totalSize.get());
        System.out.println("\nFiles by extension:");

        extensionCount.entrySet().stream()
                .sorted((e1, e2) -> Long.compare(e2.getValue().get(), e1.getValue().get()))
                .forEach(entry -> System.out.printf("  %-15s : %,10d%n", entry.getKey(), entry.getValue().get()));

        System.out.println("===================\n");
    }

    private static void scanDirectory(File dir, ExecutorService executor) {
        File[] files = dir.listFiles();
        if (files == null) return;

        for (File file : files) {
            if (file.isDirectory()) {
                executor.submit(() -> scanDirectory(file, executor));
            } else {
                fileCount.incrementAndGet();
                totalSize.addAndGet(file.length());

                String ext = getExtension(file.getName());
                extensionCount
                        .computeIfAbsent(ext, k -> new AtomicLong())
                        .incrementAndGet();
            }
        }
    }

    private static String getExtension(String name) {
        int index = name.lastIndexOf('.');
        if (index == -1 || index == name.length() - 1) return "no_extension";
        return name.substring(index + 1).toLowerCase();
    }
}
