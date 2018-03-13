#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    xOffset = 120;
    yOffset = 118;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonStart_clicked()
{
    // QMessageBox::information(this, "!", "buttonStart");
    ui->labelStatus->setText("Ongoing");
}

bool MainWindow::setPic(int x, int y, QPixmap& pic)
{
    ui->labelStatus->setText(QString::number(x) + " " + QString::number(y));
    // set a pic on labels
    if (x == 0)
    {
        ui->labelBoard->setPixmap(pic);
    }
    else
    {
        QLabel* tempLable = new QLabel("", this);
        tempLable->setText("hahaha");
        tempLable->setGeometry((y-1) * 44 + (x - 1) * 22 + yOffset, (x-1) * 50 + xOffset, 47, 54);
        tempLable->setPixmap(pic);
        tempLable->show();
    }

    return true;
}
