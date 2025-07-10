#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QKeyEvent>
#include <QContextMenuEvent>

#include "flight.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

private slots:
    void loadFromFile();
    void sortAscending();
    void sortDescending();
    void addRow();
    void removeRow();
    void addListItem();
    void removeListItem();
    void sortListCustom();
    void flightsCompany(const QString& name);

    void on_btnLoad_clicked();

private:
    Ui::MainWindow *ui;
    QVector<Flight> flights;

    void updateTable();
    void updateList();
    void loadImage();
};

#endif // MAINWINDOW_H
