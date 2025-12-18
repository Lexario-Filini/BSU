import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

public class GUIApplication extends JFrame {
    private final ListMachine<WashingMachine> listMachine;
    private final MapMachine<WashingMachine> mapMachine;
    private final ExecutorService executorService;

    private JTable table;
    private DefaultTableModel tableModel;
    private JTextField idField, typeField, modelField, powerField, speedField, dateField, priceField;
    private JTextArea logArea;
    private JTextField searchField;
    private JTextArea searchResultArea;

    public GUIApplication(ListMachine<WashingMachine> listMachine, MapMachine<WashingMachine> mapMachine) {
        this.listMachine = listMachine;
        this.mapMachine = mapMachine;
        this.executorService = Executors.newFixedThreadPool(4);

        initializeUI();
        setupTable();
        loadInitialData();
    }

    private void initializeUI() {
        setTitle("Управление стиральными машинами");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new BorderLayout());

        JTabbedPane tabbedPane = new JTabbedPane();

        tabbedPane.addTab("Просмотр", createViewPanel());
        tabbedPane.addTab("Добавить/Изменить", createEditPanel());
        tabbedPane.addTab("Файловые операции", createFilePanel());
        tabbedPane.addTab("Поиск", createSearchPanel());
        tabbedPane.addTab("Логи", createLogPanel());

        add(tabbedPane, BorderLayout.CENTER);

        setSize(1000, 700);
        setLocationRelativeTo(null);
    }

    private JPanel createViewPanel() {
        JPanel panel = new JPanel(new BorderLayout());

        String[] columns = {"ID", "Тип", "Модель", "Мощность", "Скорость", "Дата", "Цена"};
        tableModel = new DefaultTableModel(columns, 0) {
            @Override
            public boolean isCellEditable(int row, int column) {
                return false;
            }
        };

        table = new JTable(tableModel);
        table.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        table.getSelectionModel().addListSelectionListener(e -> {
            if (!e.getValueIsAdjusting() && table.getSelectedRow() != -1) {
                int row = table.getSelectedRow();
                idField.setText(tableModel.getValueAt(row, 0).toString());
                typeField.setText(tableModel.getValueAt(row, 1).toString());
                modelField.setText(tableModel.getValueAt(row, 2).toString());
                powerField.setText(tableModel.getValueAt(row, 3).toString());
                speedField.setText(tableModel.getValueAt(row, 4).toString());
                dateField.setText(tableModel.getValueAt(row, 5).toString());
                priceField.setText(tableModel.getValueAt(row, 6).toString());
            }
        });

        JScrollPane scrollPane = new JScrollPane(table);

        JPanel buttonPanel = new JPanel(new FlowLayout());

        JButton refreshButton = new JButton("Обновить таблицу");
        refreshButton.addActionListener(e -> refreshTable());

        JButton sortButton = new JButton("Сортировать по ID");
        sortButton.addActionListener(e -> {
            listMachine.sortById();
            refreshTable();
            log("Таблица отсортирована по ID");
        });

        JButton deleteButton = new JButton("Удалить выбранное");
        deleteButton.addActionListener(e -> deleteSelected());

        JButton syncButton = new JButton("Синхронизировать с Map");
        syncButton.addActionListener(e -> {
            mapMachine.refreshMap();
            log("Карта синхронизирована. Элементов в карте: " + mapMachine.getMapSize());
            JOptionPane.showMessageDialog(this,
                    "Карта синхронизирована!\nЭлементов в карте: " + mapMachine.getMapSize(),
                    "Синхронизация", JOptionPane.INFORMATION_MESSAGE);
        });

        buttonPanel.add(refreshButton);
        buttonPanel.add(sortButton);
        buttonPanel.add(deleteButton);
        buttonPanel.add(syncButton);

        panel.add(scrollPane, BorderLayout.CENTER);
        panel.add(buttonPanel, BorderLayout.SOUTH);

        return panel;
    }

    private JPanel createSearchPanel() {
        JPanel panel = new JPanel(new BorderLayout(10, 10));
        panel.setBorder(BorderFactory.createTitledBorder("Поиск по ID"));

        JPanel searchPanel = new JPanel(new FlowLayout());
        searchPanel.add(new JLabel("Введите ID:"));

        searchField = new JTextField(15);
        searchPanel.add(searchField);

        JButton searchButton = new JButton("Найти");
        searchButton.addActionListener(e -> performSearch());
        searchPanel.add(searchButton);

        JButton advancedSearchButton = new JButton("Расширенный поиск");
        advancedSearchButton.addActionListener(e -> showAdvancedSearchDialog());
        searchPanel.add(advancedSearchButton);

        searchResultArea = new JTextArea(10, 40);
        searchResultArea.setEditable(false);
        searchResultArea.setFont(new Font("Monospaced", Font.PLAIN, 12));
        JScrollPane resultScrollPane = new JScrollPane(searchResultArea);

        JPanel infoPanel = new JPanel(new FlowLayout());
        infoPanel.add(new JLabel("Элементов в списке: " + listMachine.getList().size()));
        infoPanel.add(new JLabel(" | Элементов в карте: " + mapMachine.getMapSize()));

        panel.add(searchPanel, BorderLayout.NORTH);
        panel.add(resultScrollPane, BorderLayout.CENTER);
        panel.add(infoPanel, BorderLayout.SOUTH);

        return panel;
    }

    private void performSearch() {
        String searchText = searchField.getText().trim();
        if (searchText.isEmpty()) {
            searchResultArea.setText("Введите ID для поиска");
            return;
        }

        try {
            int id = Integer.parseInt(searchText);

            executorService.submit(() -> {
                WashingMachine wm = mapMachine.findById(id);

                SwingUtilities.invokeLater(() -> {
                    if (wm != null) {
                        searchResultArea.setText("НАЙДЕНО:\n\n" + wm.toString());
                        log("Найдена машина с ID: " + id);
                        highlightTableRow(id);
                    } else {
                        searchResultArea.setText("Машина с ID " + id + " не найдена в карте.\n\n");
                        log("Машина с ID " + id + " не найдена");
                    }
                });
            });
        } catch (NumberFormatException e) {
            searchResultArea.setText("Ошибка: ID должен быть числом");
        }
    }

    private void highlightTableRow(int id) {
        for (int i = 0; i < tableModel.getRowCount(); i++) {
            if (tableModel.getValueAt(i, 0).equals(id)) {
                table.setRowSelectionInterval(i, i);
                table.scrollRectToVisible(table.getCellRect(i, 0, true));
                return;
            }
        }
    }

    private void showAdvancedSearchDialog() {
        JDialog dialog = new JDialog(this, "Расширенный поиск", true);
        dialog.setLayout(new GridBagLayout());
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.insets = new Insets(5, 5, 5, 5);
        gbc.fill = GridBagConstraints.HORIZONTAL;

        JTextField modelField = new JTextField(20);
        JTextField typeField = new JTextField(20);
        JTextField minPriceField = new JTextField(10);
        JTextField maxPriceField = new JTextField(10);

        gbc.gridx = 0; gbc.gridy = 0;
        dialog.add(new JLabel("Модель:"), gbc);
        gbc.gridx = 1;
        dialog.add(modelField, gbc);

        gbc.gridx = 0; gbc.gridy = 1;
        dialog.add(new JLabel("Тип:"), gbc);
        gbc.gridx = 1;
        dialog.add(typeField, gbc);

        gbc.gridx = 0; gbc.gridy = 2;
        dialog.add(new JLabel("Цена от:"), gbc);
        gbc.gridx = 1;
        dialog.add(minPriceField, gbc);

        gbc.gridx = 0; gbc.gridy = 3;
        dialog.add(new JLabel("Цена до:"), gbc);
        gbc.gridx = 1;
        dialog.add(maxPriceField, gbc);

        gbc.gridx = 0; gbc.gridy = 4;
        gbc.gridwidth = 2;
        gbc.fill = GridBagConstraints.NONE;

        JPanel buttonPanel = new JPanel();
        JButton searchButton = new JButton("Искать");
        JButton cancelButton = new JButton("Отмена");

        searchButton.addActionListener(e -> {
            performAdvancedSearch(modelField.getText(), typeField.getText(),
                    minPriceField.getText(), maxPriceField.getText());
            dialog.dispose();
        });

        cancelButton.addActionListener(e -> dialog.dispose());

        buttonPanel.add(searchButton);
        buttonPanel.add(cancelButton);

        dialog.add(buttonPanel, gbc);

        dialog.pack();
        dialog.setLocationRelativeTo(this);
        dialog.setVisible(true);
    }

    private void performAdvancedSearch(String model, String type, String minPrice, String maxPrice) {
        executorService.submit(() -> {
            StringBuilder result = new StringBuilder("РЕЗУЛЬТАТЫ ПОИСКА:\n\n");
            int count = 0;

            for (WashingMachine wm : listMachine.getList()) {
                boolean match = true;

                if (!model.isEmpty() && !wm.getModel().toLowerCase().contains(model.toLowerCase())) {
                    match = false;
                }

                if (!type.isEmpty() && !wm.getType().toLowerCase().contains(type.toLowerCase())) {
                    match = false;
                }

                if (!minPrice.isEmpty()) {
                    try {
                        int price = extractPrice(wm.getPrice());
                        int min = extractPrice(minPrice);
                        if (price < min) match = false;
                    } catch (Exception e) {
                        match = false;
                    }
                }

                if (!maxPrice.isEmpty()) {
                    try {
                        int price = extractPrice(wm.getPrice());
                        int max = extractPrice(maxPrice);
                        if (price > max) match = false;
                    } catch (Exception e) {
                        match = false;
                    }
                }

                if (match) {
                    result.append(wm.toString()).append("---------------\n");
                    count++;
                }
            }

            int finalCount = count;
            SwingUtilities.invokeLater(() -> {
                if (finalCount > 0) {
                    searchResultArea.setText("Найдено " + finalCount + " машин:\n\n" + result.toString());
                    log("Найдено " + finalCount + " машин по расширенному поиску");
                } else {
                    searchResultArea.setText("Машины по заданным критериям не найдены");
                }
            });
        });
    }

    private int extractPrice(String priceStr) {
        try {
            String digits = priceStr.replaceAll("[^0-9]", "");
            return Integer.parseInt(digits);
        } catch (Exception e) {
            return 0;
        }
    }

    private JPanel createEditPanel() {
        JPanel panel = new JPanel(new GridBagLayout());
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.insets = new Insets(5, 5, 5, 5);
        gbc.fill = GridBagConstraints.HORIZONTAL;

        gbc.gridx = 0; gbc.gridy = 0;
        panel.add(new JLabel("ID:"), gbc);
        gbc.gridx = 1;
        idField = new JTextField(20);
        panel.add(idField, gbc);

        gbc.gridx = 0; gbc.gridy = 1;
        panel.add(new JLabel("Тип:"), gbc);
        gbc.gridx = 1;
        typeField = new JTextField(20);
        panel.add(typeField, gbc);

        gbc.gridx = 0; gbc.gridy = 2;
        panel.add(new JLabel("Модель:"), gbc);
        gbc.gridx = 1;
        modelField = new JTextField(20);
        panel.add(modelField, gbc);

        gbc.gridx = 0; gbc.gridy = 3;
        panel.add(new JLabel("Мощность:"), gbc);
        gbc.gridx = 1;
        powerField = new JTextField(20);
        panel.add(powerField, gbc);

        gbc.gridx = 0; gbc.gridy = 4;
        panel.add(new JLabel("Скорость:"), gbc);
        gbc.gridx = 1;
        speedField = new JTextField(20);
        panel.add(speedField, gbc);

        gbc.gridx = 0; gbc.gridy = 5;
        panel.add(new JLabel("Дата (dd-MM-yyyy):"), gbc);
        gbc.gridx = 1;
        dateField = new JTextField(20);
        panel.add(dateField, gbc);

        gbc.gridx = 0; gbc.gridy = 6;
        panel.add(new JLabel("Цена:"), gbc);
        gbc.gridx = 1;
        priceField = new JTextField(20);
        panel.add(priceField, gbc);

        gbc.gridx = 0; gbc.gridy = 7;
        gbc.gridwidth = 2;
        gbc.fill = GridBagConstraints.NONE;

        JPanel buttonPanel = new JPanel(new FlowLayout());

        JButton addButton = new JButton("Добавить");
        addButton.addActionListener(e -> addMachine());

        JButton updateButton = new JButton("Обновить");
        updateButton.addActionListener(e -> updateMachine());

        JButton clearButton = new JButton("Очистить");
        clearButton.addActionListener(e -> clearFields());

        buttonPanel.add(addButton);
        buttonPanel.add(updateButton);
        buttonPanel.add(clearButton);

        panel.add(buttonPanel, gbc);

        return panel;
    }

    private JPanel createFilePanel() {
        JPanel panel = new JPanel(new GridLayout(5, 2, 10, 10));
        panel.setBorder(BorderFactory.createEmptyBorder(20, 20, 20, 20));

        JButton loadTxtButton = new JButton("Загрузить из TXT");
        loadTxtButton.addActionListener(e -> loadFromFile("TXT"));

        JButton loadXmlButton = new JButton("Загрузить из XML");
        loadXmlButton.addActionListener(e -> loadFromFile("XML"));

        JButton loadJsonButton = new JButton("Загрузить из JSON");
        loadJsonButton.addActionListener(e -> loadFromFile("JSON"));

        JButton saveTxtButton = new JButton("Сохранить в TXT");
        saveTxtButton.addActionListener(e -> saveToFile("TXT"));

        JButton saveXmlButton = new JButton("Сохранить в XML");
        saveXmlButton.addActionListener(e -> saveToFile("XML"));

        JButton saveJsonButton = new JButton("Сохранить в JSON");
        saveJsonButton.addActionListener(e -> saveToFile("JSON"));

        JButton saveAllButton = new JButton("Сохранить во все форматы");
        saveAllButton.addActionListener(e -> saveToAllFormats());

        JButton encryptButton = new JButton("Зашифровать файл");
        encryptButton.addActionListener(e -> encryptFile());

        JButton decryptButton = new JButton("Расшифровать файл");
        decryptButton.addActionListener(e -> decryptFile());

        JButton archiveButton = new JButton("Архивировать");
        archiveButton.addActionListener(e -> archiveFiles());

        panel.add(loadTxtButton);
        panel.add(loadXmlButton);
        panel.add(loadJsonButton);
        panel.add(saveTxtButton);
        panel.add(saveXmlButton);
        panel.add(saveJsonButton);
        panel.add(saveAllButton);
        panel.add(encryptButton);
        panel.add(decryptButton);
        panel.add(archiveButton);

        return panel;
    }

    private JPanel createLogPanel() {
        JPanel panel = new JPanel(new BorderLayout());

        logArea = new JTextArea(15, 60);
        logArea.setEditable(false);
        logArea.setFont(new Font("Monospaced", Font.PLAIN, 12));

        JScrollPane scrollPane = new JScrollPane(logArea);

        JButton clearLogButton = new JButton("Очистить логи");
        clearLogButton.addActionListener(e -> logArea.setText(""));

        JButton exportLogButton = new JButton("Экспорт логов");
        exportLogButton.addActionListener(e -> exportLogs());

        JPanel buttonPanel = new JPanel(new FlowLayout());
        buttonPanel.add(clearLogButton);
        buttonPanel.add(exportLogButton);

        panel.add(new JLabel("Логи операций:"), BorderLayout.NORTH);
        panel.add(scrollPane, BorderLayout.CENTER);
        panel.add(buttonPanel, BorderLayout.SOUTH);

        return panel;
    }

    private void exportLogs() {
        JFileChooser fileChooser = new JFileChooser();
        fileChooser.setDialogTitle("Сохранить логи");
        if (fileChooser.showSaveDialog(this) == JFileChooser.APPROVE_OPTION) {
            try {
                java.io.FileWriter writer = new java.io.FileWriter(fileChooser.getSelectedFile());
                writer.write(logArea.getText());
                writer.close();
                log("Логи экспортированы в файл: " + fileChooser.getSelectedFile().getName());
                JOptionPane.showMessageDialog(this, "Логи успешно экспортированы",
                        "Экспорт", JOptionPane.INFORMATION_MESSAGE);
            } catch (Exception e) {
                JOptionPane.showMessageDialog(this, "Ошибка при экспорте: " + e.getMessage(),
                        "Ошибка", JOptionPane.ERROR_MESSAGE);
            }
        }
    }

    private void setupTable() {
        refreshTable();
    }

    private void refreshTable() {
        tableModel.setRowCount(0);
        for (WashingMachine wm : listMachine) {
            SimpleDateFormat format = new SimpleDateFormat("dd-MM-yyyy");
            tableModel.addRow(new Object[]{
                    wm.getId(),
                    wm.getType(),
                    wm.getModel(),
                    wm.getPower(),
                    wm.getSpeed(),
                    format.format(wm.getDate()),
                    wm.getPrice()
            });
        }
    }

    private void loadInitialData() {
        executorService.submit(() -> {
            List<WashingMachine> machines = Files.readFromFile();
            SwingUtilities.invokeLater(() -> {
                for (WashingMachine machine : machines) {
                    listMachine.add(machine);
                }
                mapMachine.refreshMap();
                refreshTable();
                log("Загружено " + machines.size() + " записей из файла");
                log("Элементов в списке: " + listMachine.getList().size());
                log("Элементов в карте: " + mapMachine.getMapSize());
            });
        });
    }

    private void addMachine() {
        try {
            int id = Integer.parseInt(idField.getText());
            String type = typeField.getText();
            String model = modelField.getText();
            String power = powerField.getText();
            String speed = speedField.getText();
            Date date = new SimpleDateFormat("dd-MM-yyyy").parse(dateField.getText());
            String price = priceField.getText();

            WashingMachine wm = new WashingMachine(id, type, model, power, speed, date, price);
            listMachine.add(wm);
            mapMachine.refreshMap();

            refreshTable();
            clearFields();
            log("Добавлена новая машина с ID: " + id);
            log("Элементов в списке: " + listMachine.getList().size());
            log("Элементов в карте: " + mapMachine.getMapSize());

            JOptionPane.showMessageDialog(this, "Машина успешно добавлена!",
                    "Успех", JOptionPane.INFORMATION_MESSAGE);
        } catch (Exception e) {
            JOptionPane.showMessageDialog(this, "Ошибка при добавлении: " + e.getMessage(),
                    "Ошибка", JOptionPane.ERROR_MESSAGE);
        }
    }

    private void updateMachine() {
        try {
            int id = Integer.parseInt(idField.getText());
            String type = typeField.getText();
            String model = modelField.getText();
            String power = powerField.getText();
            String speed = speedField.getText();
            Date date = new SimpleDateFormat("dd-MM-yyyy").parse(dateField.getText());
            String price = priceField.getText();

            WashingMachine wm = new WashingMachine(id, type, model, power, speed, date, price);
            listMachine.update(id, wm);
            mapMachine.refreshMap();

            refreshTable();
            log("Обновлена машина с ID: " + id);

            JOptionPane.showMessageDialog(this, "Машина успешно обновлена!",
                    "Успех", JOptionPane.INFORMATION_MESSAGE);
        } catch (Exception e) {
            JOptionPane.showMessageDialog(this, "Ошибка при обновлении: " + e.getMessage(),
                    "Ошибка", JOptionPane.ERROR_MESSAGE);
        }
    }

    private void deleteSelected() {
        int selectedRow = table.getSelectedRow();
        if (selectedRow != -1) {
            int id = (int) tableModel.getValueAt(selectedRow, 0);
            int confirm = JOptionPane.showConfirmDialog(this,
                    "Вы уверены, что хотите удалить запись с ID " + id + "?",
                    "Подтверждение удаления", JOptionPane.YES_NO_OPTION);

            if (confirm == JOptionPane.YES_OPTION) {
                listMachine.delete(id);
                mapMachine.refreshMap();
                refreshTable();
                clearFields();
                log("Удалена машина с ID: " + id);
                log("Элементов в списке: " + listMachine.getList().size());
                log("Элементов в карте: " + mapMachine.getMapSize());
            }
        } else {
            JOptionPane.showMessageDialog(this, "Выберите запись для удаления",
                    "Внимание", JOptionPane.WARNING_MESSAGE);
        }
    }

    private void clearFields() {
        idField.setText("");
        typeField.setText("");
        modelField.setText("");
        powerField.setText("");
        speedField.setText("");
        dateField.setText("");
        priceField.setText("");
    }

    private void loadFromFile(String format) {
        executorService.submit(() -> {
            try {
                List<WashingMachine> machines = null;

                switch (format) {
                    case "TXT" -> machines = Files.readFromFile();
                    case "XML" -> machines = XMLFile.readFromXML();
                    case "JSON" -> machines = JSONFile.readFromJSON();
                }

                if (machines != null && !machines.isEmpty()) {
                    List<WashingMachine> finalMachines = machines;
                    SwingUtilities.invokeLater(() -> {
                        for (WashingMachine machine : finalMachines) {
                            listMachine.add(machine);
                        }
                        mapMachine.refreshMap();
                        refreshTable();
                        log("Загружено " + finalMachines.size() + " записей из " + format);
                        log("Элементов в списке: " + listMachine.getList().size());
                        log("Элементов в карте: " + mapMachine.getMapSize());
                    });
                }
            } catch (Exception e) {
                SwingUtilities.invokeLater(() -> {
                    log("Ошибка при загрузке из " + format + ": " + e.getMessage());
                    JOptionPane.showMessageDialog(GUIApplication.this,
                            "Ошибка при загрузке файла: " + e.getMessage(),
                            "Ошибка", JOptionPane.ERROR_MESSAGE);
                });
            }
        });
    }

    private void saveToFile(String format) {
        executorService.submit(() -> {
            try {
                List<WashingMachine> machines = listMachine.getList();
                String filename = "output." + format.toLowerCase();

                switch (format) {
                    case "TXT" -> new BaseSaver().save(machines, filename);
                    case "XML" -> new XMLSaver(new BaseSaver()).save(machines, filename);
                    case "JSON" -> new JSONSaver(new BaseSaver()).save(machines, filename);
                }

                SwingUtilities.invokeLater(() -> {
                    log("Данные сохранены в " + format + " файл: " + filename);
                    JOptionPane.showMessageDialog(GUIApplication.this,
                            "Данные успешно сохранены в " + filename,
                            "Успех", JOptionPane.INFORMATION_MESSAGE);
                });
            } catch (Exception e) {
                SwingUtilities.invokeLater(() -> {
                    log("Ошибка при сохранении в " + format + ": " + e.getMessage());
                    JOptionPane.showMessageDialog(GUIApplication.this,
                            "Ошибка при сохранении: " + e.getMessage(),
                            "Ошибка", JOptionPane.ERROR_MESSAGE);
                });
            }
        });
    }

    private void saveToAllFormats() {
        List<WashingMachine> machines = listMachine.getList();

        Future<?> txtFuture = executorService.submit(() -> {
            new BaseSaver().save(machines, "output.txt");
            SwingUtilities.invokeLater(() -> log("Сохранено в TXT"));
        });

        Future<?> xmlFuture = executorService.submit(() -> {
            new XMLSaver(new BaseSaver()).save(machines, "output.xml");
            SwingUtilities.invokeLater(() -> log("Сохранено в XML"));
        });

        Future<?> jsonFuture = executorService.submit(() -> {
            new JSONSaver(new BaseSaver()).save(machines, "output.json");
            SwingUtilities.invokeLater(() -> log("Сохранено в JSON"));
        });

        executorService.submit(() -> {
            try {
                txtFuture.get();
                xmlFuture.get();
                jsonFuture.get();

                SwingUtilities.invokeLater(() -> {
                    log("Данные сохранены во все форматы параллельно");
                    JOptionPane.showMessageDialog(GUIApplication.this,
                            "Данные успешно сохранены во все форматы параллельно!",
                            "Успех", JOptionPane.INFORMATION_MESSAGE);
                });
            } catch (Exception e) {
                SwingUtilities.invokeLater(() -> {
                    log("Ошибка при параллельном сохранении: " + e.getMessage());
                    JOptionPane.showMessageDialog(GUIApplication.this,
                            "Ошибка при сохранении: " + e.getMessage(),
                            "Ошибка", JOptionPane.ERROR_MESSAGE);
                });
            }
        });
    }

    private void encryptFile() {
        executorService.submit(() -> {
            try {
                Menu menu = new Menu(listMachine, mapMachine);
                menu.encryptFile();

                SwingUtilities.invokeLater(() -> {
                    log("Файл успешно зашифрован");
                    JOptionPane.showMessageDialog(GUIApplication.this,
                            "Файл output.txt успешно зашифрован в encrypted.txt",
                            "Успех", JOptionPane.INFORMATION_MESSAGE);
                });
            } catch (Exception e) {
                SwingUtilities.invokeLater(() -> {
                    log("Ошибка при шифровании: " + e.getMessage());
                    JOptionPane.showMessageDialog(GUIApplication.this,
                            "Ошибка при шифровании: " + e.getMessage(),
                            "Ошибка", JOptionPane.ERROR_MESSAGE);
                });
            }
        });
    }

    private void decryptFile() {
        executorService.submit(() -> {
            try {
                Menu menu = new Menu(listMachine, mapMachine);
                menu.decryptFile();

                SwingUtilities.invokeLater(() -> {
                    log("Файл успешно расшифрован");
                    JOptionPane.showMessageDialog(GUIApplication.this,
                            "Файл encrypted.txt успешно расшифрован в decrypted.txt",
                            "Успех", JOptionPane.INFORMATION_MESSAGE);
                });
            } catch (Exception e) {
                SwingUtilities.invokeLater(() -> {
                    log("Ошибка при расшифровке: " + e.getMessage());
                    JOptionPane.showMessageDialog(GUIApplication.this,
                            "Ошибка при расшифровке: " + e.getMessage(),
                            "Ошибка", JOptionPane.ERROR_MESSAGE);
                });
            }
        });
    }

    private void archiveFiles() {
        executorService.submit(() -> {
            try {
                Menu menu = new Menu(listMachine, mapMachine);
                menu.archiveFiles();

                SwingUtilities.invokeLater(() -> {
                    log("Файлы заархивированы");
                    JOptionPane.showMessageDialog(GUIApplication.this,
                            "Файлы успешно заархивированы в archive.zip",
                            "Успех", JOptionPane.INFORMATION_MESSAGE);
                });
            } catch (Exception e) {
                SwingUtilities.invokeLater(() -> {
                    log("Ошибка при архивации: " + e.getMessage());
                    JOptionPane.showMessageDialog(GUIApplication.this,
                            "Ошибка при архивации: " + e.getMessage(),
                            "Ошибка", JOptionPane.ERROR_MESSAGE);
                });
            }
        });
    }

    private void log(String message) {
        SwingUtilities.invokeLater(() -> {
            SimpleDateFormat timeFormat = new SimpleDateFormat("HH:mm:ss");
            String timestamp = timeFormat.format(new Date());
            logArea.append("[" + timestamp + "] " + message + "\n");
            logArea.setCaretPosition(logArea.getDocument().getLength());
        });
    }

    @Override
    public void dispose() {
        executorService.shutdown();
        super.dispose();
    }
}