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
    if(p[0])
        delete p[0];
    if(p[1])
        delete p[1];
}

void MainWindow::linkBoard(hexBoard* hexIn)
{
    // solution for logical problems
    hex = hexIn;
}

void MainWindow::on_buttonStart_clicked()
{
    // start a NEW game between AIs
    startGame();
    QString lastMove = "";
    //start a timer
    resetTimer();
    qDebug() << QString::fromStdString(redExe) << "555\n";
    if((!p[RED-1] || !p[BLUE-1])||(redExe == "" || blueExe == ""))
    {
        return;
    }

    p[RED-1]->start(QString::fromStdString(redExe));
    qDebug() << "/red start()" << "\n";
    p[RED-1]->write("start\n");
    p[RED-1]->write("red\n");
    lastMove = penddingMove();
    if (lastMove == "")
        return;
    p[BLUE-1]->start(QString::fromStdString(blueExe));
    p[BLUE-1]->write("start\n");
    p[BLUE-1]->write("blue\n");
    hex->setTurn(1);

    while(1)
    {
        short turn = hex->getTurn();
        p[turn]->write("move ");
        qDebug() << lastMove.toLatin1().data();
        p[turn]->write(lastMove.toLatin1().data());
        p[turn]->write("\n");
        lastMove = penddingMove();
        if (lastMove == "")
            return;
        hex->setTurn(!turn);
    }

}

QString MainWindow::penddingMove()
{
    qDebug() << "penddingMove()" << "\n";
    short turn = hex->getTurn();
    QByteArray move;
    move.resize(8);
    bool flag = p[turn]->waitForReadyRead(maxTime);
    if (flag)
    {
         move = p[turn]->readLine(8);
         short x = move[5] - 'A' + 1;
         short y = move[6] - 'A' + 1;
         qDebug() << x << y;
         if(!hex->makeMove(x, y, turn + 1))
         {
             p[turn]->write("wrong\n");
             return penddingMove();
         }
         setPic(x, y, turn + 1);
         ui->historyDisplay->setPlainText(QString::fromStdString(hex->getMoves()));
         short currentStatus = hex->checkStatus();
         if (currentStatus == 1)
         {
             ui->labelStatus->setText("Red win");
             QMessageBox::information(NULL, "", "Red win", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
         }
         else if (currentStatus == 2)
         {
             ui->labelStatus->setText("Blue win");
             QMessageBox::information(NULL, "", "Blue win", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
         }
         QString temp = "";
         temp = temp + move[5] + move[6];
         qDebug() << temp << "\n";
         return temp;
    }
    else
    {
        timeWin();
        return "";
    }
}

void MainWindow::timeWin()
{
    QString message = "Blue time win!";
    ui->labelStatus->setText("Blue win");
    if(hex->getTurn())
    {
        ui->labelStatus->setText("Red win");
        message = "Red time win!";
    }

    QMessageBox::information(NULL, "", message, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

}

void MainWindow::startGame()
{
    qDebug() << "startGame()" << "\n";
    ui->labelStatus->setText("Ongoing");
    hex->clearBoard();
    clearPieces();
    ui->historyDisplay->setPlainText(QString::fromStdString(hex->getMoves()));
    qDebug() << "/startGame()" << "\n";
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
        QCoreApplication::processEvents();
    }

    return true;
}

bool MainWindow::clearPieces()
{
    // hide all pieces for real
    for (int i = 0; i <= 10; ++i)
        for (int j = 0; j <= 10; ++j)
            pieces[i][j].hide();

    QCoreApplication::processEvents();
    return true;
}

void MainWindow::on_buttonLoadRed_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(NULL, "xian studio says hi!", ".", "*.exe");
    redExe = filePath.toStdString();
    ui->redFile->setPlainText(filePath);
    if (p[RED-1])
    {
        p[RED-1]->write("exit");
        delete p[RED-1];
    }
    p[RED-1] = new QProcess;
}

void MainWindow::on_buttonLoadBlue_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(NULL, "xian studio says hi!", ".", "*.exe");
    blueExe = filePath.toStdString();
    ui->blueFile->setPlainText(filePath);
    if (p[BLUE-1])
    {
        p[BLUE-1]->write("exit");
        delete p[BLUE-1];
    }
    p[BLUE-1] = new QProcess;
}

void MainWindow::on_buttonUnloadRed_clicked()
{
    ui->redFile->setPlainText("");
    redExe = "";
    if(p[0])
        delete p[0];
}

void MainWindow::on_buttonUnloadBlue_clicked()
{
    ui->blueFile->setPlainText("");
    blueExe = "";
    if(p[1])
        delete p[1];
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
void MainWindow::resetTimer()
{

    if(timer != -1)
        this->killTimer(timer);
    time[0] = time[1] = 0;
    timer = this->startTimer(1000);

}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if(!hex->checkStatus())
    {
        time[hex->getTurn()-1]++;
        refreshTimerLabel();
    }
}
void MainWindow::refreshTimerLabel()
{
    int minute = time[0] / 60;
    int second = time[0] % 60;
    QString time_string;
    time_string = QString("%1:%2").arg(minute,2,10,QChar('0')).arg(second,2,10,QChar('0'));
    ui->timerRed->setText(time_string);
    minute = time[1] / 60;
    second = time[1] % 60;
    time_string = QString("%1:%2").arg(minute,2,10,QChar('0')).arg(second,2,10,QChar('0'));
    ui->timerBlue->setText(time_string);
}


