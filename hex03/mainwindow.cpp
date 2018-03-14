#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // setup pieces
    int x = xOffset, y = yOffset;
    for (int i = 0; i <= 10; ++i)
    {
        for (int j = 0; j <= 10; ++j)
        {
            pieces[i][j].setGeometry(x, y, pieceWidth, pieceHeight);
            pieces[i][j].setText("");
            pieces[i][j].setParent(this);
            x += xDelta;
        }
        x = xOffset + (xDelta / 2) * (i + 1);
        y += yDelta;
    }
    // nxp!
    pieces[10][0].setGeometry(336, 500, pieceWidth, pieceHeight);

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
    // set a pic on labels
    if (x == 0)
    {
        ui->labelBoard->setPixmap(pic);
    }
    else if (x >= 1 && x <= 11 && y >=1 && y <= 11)
    {
        pieces[x - 1][y - 1].setPixmap(pic);
        pieces[x - 1][y - 1].show();
    }

    return true;
}

bool MainWindow::clearPieces()
{
    for (int i = 0; i <= 10; ++i)
        for (int j = 0; j <= 10; ++j)
            pieces[i][j].hide();

    return true;
}
