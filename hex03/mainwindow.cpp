#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    bool flag = setPic(0, 0, 0);
    if (!flag)
    {
        QMessageBox::information(NULL, "Title", QString::number(flag), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
    redExe = "";
    blueExe = "";
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

void MainWindow::linkBoard(hexBoard* hexIn)
{
    // solution for logical problems
    hex = hexIn;
}

void MainWindow::on_buttonStart_clicked()
{
    // start a NEW game between AIs
    if (redExe == "" || blueExe == "")
        return;

    ui->labelStatus->setText("Ongoing");
    hex->clearBoard();
    clearPieces();
    ui->historyDisplay->setPlainText(QString::fromStdString(hex->getMoves()));
    QByteArray move;
    move.resize(5);
    std::string lastMove = "";

    QProcess p[2];  // 0: red .exe, 1: blue .exe
    p[0].start(QString::fromStdString(redExe));
    p[0].write("start\n");
    p[0].write("red\n");
    bool flag = p[0].waitForReadyRead(maxTime);
    if (flag)
    {
         move = p[0].readLine(3);
         qDebug()<<move[0]<<"|||||"<<move[1];
         short x = move[0] - 'A' + 1;
         short y = move[1] - 'A' + 1;
         hex->makeMove(x, y, 1);
         setPic(x, y, 1);
         ui->historyDisplay->setPlainText(QString::fromStdString(hex->getMoves()));
         lastMove = move[0] + move[1];
    }
    else
    {
        ui->labelStatus->setText("Blue win");
        p[0].write("exit");
        p[0].close();
        QMessageBox::information(NULL, "", "Blue time win!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }
    p[1].start(QString::fromStdString(blueExe));
    p[1].write("start\n");
    p[1].write("blue\n");
    hex->setTurn(1);
    while(1)
    {
        short turn = hex->getTurn();
        p[turn].write("move");
        p[turn].write(QString::fromStdString(lastMove).toLatin1().data());
        flag = p[turn].waitForReadyRead(maxTime);
        if (flag)
        {
             move = p[turn].readLine(3);
             qDebug()<<move[0]<<move[1];
             short x = move[0] - 'A' + 1;
             short y = move[1] - 'A' + 1;
             hex->makeMove(x, y, turn + 1);
             setPic(x, y, turn + 1);
             ui->historyDisplay->setPlainText(QString::fromStdString(hex->getMoves()));
             lastMove = move[0] + move[1];
             short currentStatus = hex->checkStatus();
             if (currentStatus == 1)
             {
                 ui->labelStatus->setText("Red win");
                 p[0].write("exit");
                 p[0].close();
                 p[1].write("exit");
                 p[1].close();
                 QMessageBox::information(NULL, "", "Red win", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
             }
             else if (currentStatus == 2)
             {
                 ui->labelStatus->setText("Blue win");
                 p[0].write("exit");
                 p[0].close();
                 p[1].write("exit");
                 p[1].close();
                 QMessageBox::information(NULL, "", "Blue win", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
             }
        }
        else
        {
            QString message = "Blue time win!";
            ui->labelStatus->setText("Blue win");
            if(turn)
            {
                ui->labelStatus->setText("Red win");
                message = "Red time win!";
            }
            p[0].write("exit");
            p[0].close();
            p[1].write("exit");
            p[1].close();
            QMessageBox::information(NULL, "", message, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            return;
        }
        hex->setTurn(!turn);
    }

}

bool MainWindow::setPic(short x, short y, short player)
{
    // set a pic on labels
    if (x == 0)
    {
        QPixmap boardPic;
        boardPic.load("img/board.jpg");
        ui->labelBoard->setPixmap(boardPic);
    }
    else if (x >= 1 && x <= 11 && y >=1 && y <= 11)
    {
        QPixmap tempPix;
        bool flag = tempPix.load("img/blue.png");
        if (player == 1)
            flag = tempPix.load("img/red.png");
        else if (player != 2)
            return false;

        if (!flag)
        {
            QMessageBox::information(NULL, "Title", QString::number(flag), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            return false;
        }
        pieces[x - 1][y - 1].setPixmap(tempPix);
        pieces[x - 1][y - 1].show();
    }

    return true;
}

bool MainWindow::clearPieces()
{
    // hide all pieces for real
    for (int i = 0; i <= 10; ++i)
        for (int j = 0; j <= 10; ++j)
            pieces[i][j].hide();

    return true;
}

void MainWindow::on_buttonLoadRed_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(NULL, "xian studio says hi!", ".", "*.exe");
    redExe = filePath.toStdString();
    ui->redFile->setPlainText(filePath);
}

void MainWindow::on_buttonLoadBlue_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(NULL, "xian studio says hi!", ".", "*.exe");
    blueExe = filePath.toStdString();
    ui->blueFile->setPlainText(filePath);
}

void MainWindow::on_buttonUnloadRed_clicked()
{
    ui->redFile->setPlainText("");
    redExe = "";
}

void MainWindow::on_buttonUnloadBlue_clicked()
{
    ui->blueFile->setPlainText("");
    blueExe = "";
}

void MainWindow::on_buttonExchange_clicked()
{
    QString temp = QString::fromStdString(redExe);
    redExe = blueExe;
    blueExe = temp.toStdString();
    ui->blueFile->setPlainText(QString::fromStdString(blueExe));
    ui->redFile->setPlainText(QString::fromStdString(redExe));
}

void MainWindow::on_buttonSave_clicked()
{
    QString filename = QFileDialog::getSaveFileName(NULL, "xian studio says hi!", ".", "*.txt");
    QFile file(filename);
    file.open(QIODevice::ReadWrite|QIODevice::Text);
    QTextStream in(&file);
    in<<QString::fromStdString(hex->getMoves());
    file.close();

}
