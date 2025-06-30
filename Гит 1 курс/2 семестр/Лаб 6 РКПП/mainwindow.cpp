#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRegularExpression>
#include <QFile>


qreal num_first;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton_sin, SIGNAL(clicked()), this, SLOT(digits_arc()));
    connect(ui->pushButton_cos, SIGNAL(clicked()), this, SLOT(digits_arc()));
    connect(ui->pushButton_tg, SIGNAL(clicked()), this, SLOT(digits_arc()));
    connect(ui->pushButton_ctg, SIGNAL(clicked()), this, SLOT(digits_arc()));

    connect(ui->pushButton_a1, SIGNAL(clicked()), this, SLOT(digits_arc()));
    connect(ui->pushButton_a2, SIGNAL(clicked()), this, SLOT(digits_arc()));
    connect(ui->pushButton_a3, SIGNAL(clicked()), this, SLOT(digits_arc()));
    connect(ui->pushButton_a4, SIGNAL(clicked()), this, SLOT(digits_arc()));
    connect(ui->pushButton_a5, SIGNAL(clicked()), this, SLOT(digits_arc()));
    connect(ui->pushButton_a6, SIGNAL(clicked()), this, SLOT(digits_arc()));
    connect(ui->pushButton_a7, SIGNAL(clicked()), this, SLOT(digits_arc()));
    connect(ui->pushButton_a8, SIGNAL(clicked()), this, SLOT(digits_arc()));
    connect(ui->pushButton_a9, SIGNAL(clicked()), this, SLOT(digits_arc()));
    connect(ui->pushButton_a0, SIGNAL(clicked()), this, SLOT(digits_arc()));

     connect(ui->pushButton_arc_min, SIGNAL(clicked()), this, SLOT(arc_math_operation()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::digits_arc() {
    QPushButton *button = (QPushButton *)(sender());

    QString inputText = ui->pole_vvoda->text();
    QString buttonText = button->text();

    QStringList arcFunctions = {"arcsin", "arccos", "arctg", "arcctg"};

    for (const QString &funс : arcFunctions) {
        if (inputText.contains(funс) && arcFunctions.contains(buttonText)) {
            return;
        }
    }

    bool isNumber;
    double num = buttonText.toDouble(&isNumber);

    if (isNumber) {
        if (inputText.back().isDigit()) {
            ui->pole_vvoda->setText(inputText + buttonText);
        } else {
            ui->pole_vvoda->setText(inputText + buttonText);
        }
    } else {
        ui->pole_vvoda->setText(inputText + buttonText);
    }

}



void MainWindow::on_pushButton_dot_clicked()
{
        QString inputText = ui->pole_vvoda->text();

        if (inputText.isEmpty()) {
            return;
        }
        if (inputText.contains('.')) {
            return;
        }
        if (!inputText.back().isDigit()) {
            return;
        }
        ui->pole_vvoda->setText(inputText + ".");
    }



void MainWindow::on_pushButton_arc_min_clicked()
{
    if(!(ui->pole_vvoda->text().contains('-')))
        ui->pole_vvoda->setText(ui->pole_vvoda->text() + "-");
}



void MainWindow::on_pushButton_AC_arc_clicked() {
   ui->pole_vvoda->setText(" ");
    ui->pole_vvoda->clear();
}



void MainWindow::on_pushButton_ravno_arc_clicked() {
    QString resultText;
    QString inputText = ui->pole_vvoda->text();

    QRegularExpression regexs("^arcsin\\s*(-?[0-9]*\\.?[0-9]+)$");
    QRegularExpression regexc("^arccos\\s*(-?[0-9]*\\.?[0-9]+)$");
    QRegularExpression regext("^arctg\\s*(-?[0-9]*\\.?[0-9]+)$");
    QRegularExpression regexct("^arcctg\\s*(-?[0-9]*\\.?[0-9]+)$");

    QRegularExpressionMatch matchs = regexs.match(inputText);
    QRegularExpressionMatch matchc = regexc.match(inputText);
    QRegularExpressionMatch matcht = regext.match(inputText);
    QRegularExpressionMatch matchct = regexct.match(inputText);

    if (ui->radioButton_rad->isChecked()) {

    if (matchs.hasMatch()) {
        qreal num = matchs.captured(1).toDouble();

        if (num >= -1 && num <= 1) {
           qreal arcNumber = qAsin(num);
            QString new_label = QString::number(arcNumber, 'g', 15);
            ui->pole_vvoda->setText(new_label);
        } else {
            ui->pole_vvoda->setText("Ошибка: число вне диапазона [-1, 1]");
        }
    } else if (matchc.hasMatch()) {
        qreal num = matchc.captured(1).toDouble();

        if (num >= -1 && num <= 1) {
            qreal arcNumber = qAcos(num);
            QString new_label = QString::number(arcNumber, 'g', 15);
            ui->pole_vvoda->setText(new_label);
        } else {
            ui->pole_vvoda->setText("Ошибка: число вне диапазона [-1, 1]");
        }
    }

    else if (matcht.hasMatch()) {
        qreal num = matcht.captured(1).toDouble();
            qreal arcNumber = qAtan(num);
            QString new_label = QString::number(arcNumber, 'g', 15);
            ui->pole_vvoda->setText(new_label);
    }
    else if (matchct.hasMatch()) {
        qreal num = matchct.captured(1).toDouble();
        qreal arcNumber = M_PI_2 - qAtan(num);

        QString new_label = QString::number(arcNumber, 'g', 15);
        ui->pole_vvoda->setText(new_label);
    }
    else {
        ui->pole_vvoda->setText("Ошибка: неверный формат ввода");
    }

    ui->pushButton_sin->setChecked(false);
   }


   if (ui->radioButton_grad->isChecked()) {

       if (matchs.hasMatch()) {
           qreal num = matchs.captured(1).toDouble();

           if (num >= -1 && num <= 1) {
               qreal arcNumber = qAsin(num)* 180.0 / M_PI;
               QString new_label = QString::number(arcNumber, 'g', 15);
               ui->pole_vvoda->setText(new_label);
           } else {
               ui->pole_vvoda->setText("Ошибка: число вне диапазона [-1, 1]");
           }
       } else if (matchc.hasMatch()) {
          qreal num = matchc.captured(1).toDouble();

           if (num >= -1 && num <= 1) {
               qreal arcNumber = qAcos(num)* 180.0 / M_PI;
               QString new_label = QString::number(arcNumber, 'g', 15);
               ui->pole_vvoda->setText(new_label);
           } else {
               ui->pole_vvoda->setText("Ошибка: число вне диапазона [-1, 1]");
           }
       }

       else if (matcht.hasMatch()) {
           qreal num = matcht.captured(1).toDouble();
           qreal arcNumber = qAtan(num)* 180.0 / M_PI;
           QString new_label = QString::number(arcNumber, 'g', 15);
           ui->pole_vvoda->setText(new_label);
       }
       else if (matchct.hasMatch()) {
           qreal num = matchct.captured(1).toDouble();
           qreal arcNumber = (M_PI_2 - qAtan(num))* 180.0 / M_PI;

           QString new_label = QString::number(arcNumber, 'g', 15);
           ui->pole_vvoda->setText(new_label);
       }
       else {
           ui->pole_vvoda->setText("Ошибка: неверный формат ввода");
       }

       ui->pushButton_sin->setChecked(false);       
   }
}



void MainWindow::on_action_triggered()
{
    ui->statusBar->showMessage("Проект открыт");
}

void MainWindow::on_action_2_triggered()
{
    QApplication::quit();
}



void MainWindow::on_label_linkActivated(const QString &link)
{

}



void MainWindow::on_result_show_cursorPositionChanged(int arg1, int arg2)
{

}






