#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "flight.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QPixmap>
#include <QMenu>
#include <QKeyEvent>
#include <QStandardItemModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    connect(ui->btnLoad, &QPushButton::clicked, this, &MainWindow::loadFromFile);
    connect(ui->btnSortAsc, &QPushButton::clicked, this, &MainWindow::sortAscending);
    connect(ui->btnSortDesc, &QPushButton::clicked, this, &MainWindow::sortDescending);
    connect(ui->btnAddRow, &QPushButton::clicked, this, &MainWindow::addRow);
    connect(ui->btnRemoveRow, &QPushButton::clicked, this, &MainWindow::removeRow);
    connect(ui->btnAddList, &QPushButton::clicked, this, &MainWindow::addListItem);
    connect(ui->btnRemoveList, &QPushButton::clicked, this, &MainWindow::removeListItem);
    connect(ui->btnSortListCustom, &QPushButton::clicked, this, &MainWindow::sortListCustom);
    connect(ui->btnLoad, &QPushButton::clicked, this, &MainWindow::updateList_2);
    connect(ui->comboBox, &QComboBox::currentTextChanged, this, &MainWindow::flightsCompany);


    ui->lineEdit->text().toInt();
    ui->comboBox->addItems({"Belavia", "Lufthansa", "Turkish"});

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::loadFromFile() {
    bool ok;
    int userSize = ui->lineEdit->text().toInt(&ok);
    if (!ok || userSize <= 0) {
        QMessageBox::warning(this, "Ошибка", "Введите корректное положительное число");
        return;
    }

    QFile file("input.txt");
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл");
        return;
    }

    QTextStream in(&file);
    flights.clear();

    for (int i = 0; i < userSize && !in.atEnd(); ++i) {
        Flight f;
        in >> f.number >> f.name >> f.date;
        if (in.status() != QTextStream::Ok) break;
        flights.append(f);
    }

    updateTable();
    updateList();
}

void MainWindow::sortAscending() {
    countingSort(flights, true);
    updateTable();
    updateList();
}

void MainWindow::sortDescending() {
    countingSort(flights, false);
    updateTable();
    updateList();
}

void MainWindow::updateTable() {
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(flights.size());

    for (int i = 0; i < flights.size(); ++i) {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(flights[i].number)));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(flights[i].name));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(flights[i].date));
    }
}

void MainWindow::updateList() {
    ui->listWidget->clear();
    for (const auto& f : flights)
        ui->listWidget->addItem(QString::number(f.number));
}

void MainWindow::addRow() {
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);
}

void MainWindow::removeRow() {
    int row = ui->tableWidget->currentRow();
    if (row >= 0)
        ui->tableWidget->removeRow(row);
}

void MainWindow::addListItem() {
    ui->listWidget->addItem("Новый элемент");
}

void MainWindow::removeListItem() {
    delete ui->listWidget->takeItem(ui->listWidget->currentRow());
}

void MainWindow::sortListCustom() {
    QList<QListWidgetItem*> items;
    for (int i = ui->listWidget->count() - 1; i >= 0; --i)
        items.append(ui->listWidget->takeItem(i));

    std::sort(items.begin(), items.end(), [](QListWidgetItem* a, QListWidgetItem* b) {
        return a->text().toInt() > b->text().toInt();
    });

    for (QListWidgetItem* item : items)
        ui->listWidget->addItem(item);
}


void MainWindow::updateList_2() {
    ui->listWidget_2->clear();
    ui->listWidget_2->setIconSize(QSize(720, 720));

    QPixmap pix("D:/C++/Praktica/Task1/Sam.bmp");

    QListWidgetItem* item = new QListWidgetItem(" ");
    item->setIcon(QIcon(pix));
    ui->listWidget_2->addItem(item);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
     if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        loadFromFile();
        updateList_2();
    }
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event) {
    QMenu menu(this);
    menu.addAction("Сортировать ↑", this, SLOT(sortAscending()));
    menu.addAction("Сортировать ↓", this, SLOT(sortDescending()));
    menu.exec(event->globalPos());
}

void MainWindow::flightsCompany(const QString& companyName) {
    QFile file("input.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ui->label->setText("Не удалось открыть файл");
        return;
    }

    QTextStream in(&file);
    QString result;
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.contains(companyName, Qt::CaseInsensitive)) {
            result += line + "\n";
        }
    }

    file.close();

    if (result.isEmpty()) {
        ui->label->setText("Нет данных по выбранной компании");
    } else {
        ui->label->setText(result.trimmed());
    }
}

void MainWindow::on_btnLoad_clicked()
{

}
