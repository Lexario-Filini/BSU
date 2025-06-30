#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void digits_arc();
    void on_pushButton_AC_arc_clicked();
    void on_pushButton_ravno_arc_clicked();
    void on_pushButton_dot_clicked();
    void on_pushButton_arc_min_clicked();

    void on_label_linkActivated(const QString &link);

    void on_result_show_cursorPositionChanged(int arg1, int arg2);

    void on_action_triggered();
    void on_action_2_triggered();
};
#endif // MAINWINDOW_H
