import javax.swing.SwingWorker;
import java.util.List;
import java.util.function.Consumer;

public class FileWorker extends SwingWorker<Void, String> {
    private final Runnable task;
    private final Consumer<String> onProgress;
    private final Consumer<Exception> onError;

    public FileWorker(Runnable task, Consumer<String> onProgress, Consumer<Exception> onError) {
        this.task = task;
        this.onProgress = onProgress;
        this.onError = onError;
    }

    @Override
    protected Void doInBackground() throws Exception {
        task.run();
        return null;
    }

    @Override
    protected void process(List<String> chunks) {
        chunks.forEach(onProgress::accept);
    }

    @Override
    protected void done() {
        try {
            get();
        } catch (Exception e) {
            onError.accept(e);
        }
    }

    public static void executeAsync(Runnable task, Consumer<String> onProgress, Consumer<Exception> onError) {
        new FileWorker(task, onProgress, onError).execute();
    }
}