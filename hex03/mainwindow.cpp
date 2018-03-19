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
    //start a timer
    resetTimer();
    // nxp!
    //pieces[10][0].setGeometry(336, 500, pieceWidth, pieceHeight);

}

MainWindow::~MainWindow()
{
    delete ui;
    terminateThread(0);
    terminateThread(1);
    if(timer)
    {
        timer->stop();
        delete timer;
    }
    if(timer_thread)
    {
        timer_thread->terminate();
        delete timer_thread;
    }
}

void MainWindow::linkBoard(hexBoard* hexIn)
{
    // solution for logical problems
    hex = hexIn;
}

void MainWindow::receive_process(QProcess* tp, int who)
{
    p[who] = tp;
}

void MainWindow::receive_message(QByteArray response, int who)
{
    // comm
    if (response[0] == 'n' && response[1] == 'a')
    {
        setName(response, who);
    }
    else if (response[0] == 'm' && response[1] == 'o')
    {

        QString lastMove = handleMove(response, who);
        if (lastMove == "")
        {
            hex->setStatus(-1);
            return;
        }
        p[!who]->write("move\n");
        p[!who]->write((lastMove + "\n").toLatin1());
        hex->setWho(!who);
    }
}

QString MainWindow::handleMove(QByteArray response, int who)
{
    short x = response[5] - 'A' + 1;
    short y = response[6] - 'A' + 1;
    qDebug() << x << y;
    if(!hex->makeMove(x, y, who + 1))
    {
        p[who]->write("wrong\n");
        return "";
    }
    setPic(x, y, who + 1);
   // Sleep(3000);
    ui->historyDisplay->setPlainText(QString::fromStdString(hex->getMoves()));
    short currentStatus = hex->checkStatus();
    if (currentStatus == 1)
    {
        ui->labelStatus->setText("Red win");
        QMessageBox::information(NULL, "", "Red win", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return "";
    }
    else if (currentStatus == 2)
    {
        ui->labelStatus->setText("Blue win");
        QMessageBox::information(NULL, "", "Blue win", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return "";
    }
    QString temp = "";
    temp = temp + response[5] + response[6];
    return temp;
}

void MainWindow::on_buttonStart_clicked()
{
    // start a NEW game between AIs
    qDebug() << QString::fromStdString(redExe) << "555\n";
    if((!p[RED-1] || !p[BLUE-1])||(redExe == "" || blueExe == ""))
    {
        return;
    }
    startGame();
    // careful with time
    p[BLUE-1]->write("start\n");
    p[BLUE-1]->write("blue\n");
    qDebug() << "/red start()" << "\n";
    p[RED-1]->write("start\n");
    p[RED-1]->write("red\n");
    hex->setStatus(0);
    hex->setWho(0);
    hex->setTurn(1);
}

/*
void MainWindow::timeWin()
{
    QString message = "Blue time win!";
    ui->labelStatus->setText("Blue win");
    if(hex->getTurn())
    {
        ui->labelStatus->setText("Red win");
        message = "Red time win!";
    }
    hex->setStatus(!hex->getTurn() + 1);
    QMessageBox::information(NULL, "", message, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    hex->setStatus(-1);

}
*/

void MainWindow::startGame()
{
    qDebug() << "startGame()" << "\n";
    hex->clearBoard();
    clearPieces();
    ui->labelStatus->setText("Ongoing");
    ui->historyDisplay->setPlainText(QString::fromStdString(hex->getMoves()));
    refreshThreads();
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
    refreshThreads();
}

void MainWindow::on_buttonLoadBlue_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(NULL, "xian studio says hi!", ".", "*.exe");
    blueExe = filePath.toStdString();
    ui->blueFile->setPlainText(filePath);
    refreshThreads();
}

void MainWindow::on_buttonUnloadRed_clicked()
{
    ui->redFile->setPlainText("");
    redExe = "";
    terminateThread(RED);
}
void MainWindow::on_buttonUnloadBlue_clicked()
{
    ui->blueFile->setPlainText("");
    blueExe = "";
    terminateThread(BLUE);
}

void MainWindow::terminateThread(short x)
{
    x -= 1;
    if (iothread[x])
    {
        if (p[x])
            p[x]->write("exit");

        iothread[x]->terminate();
        delete iothread[x];
    }
}

void MainWindow::on_buttonExchange_clicked()
{
    QString temp = QString::fromStdString(redExe);
    redExe = blueExe;
    blueExe = temp.toStdString();
    ui->blueFile->setPlainText(QString::fromStdString(blueExe));
    ui->redFile->setPlainText(QString::fromStdString(redExe));
}
void MainWindow::refreshThreads()
{
    // logically concluded
    qDebug() << "refreshProcess()";
    for (int i = 0; i <= 1; i++)
    {
        if (iothread[i])
        {
            if (p[i])
                p[i]->write("exit");

            iothread[i]->terminate();
            delete iothread[i];
        }
        iothread[i] = new myThread(redExe, i);
        connect(iothread[i],SIGNAL(set_process(QProcess*,int)),this,SLOT(receive_process(QProcess*,int)));
        connect(iothread[i],SIGNAL(send_message(QByteArray,int)),this,SLOT(receive_message(QByteArray,int)));
        iothread[i]->start();
    }
}

void MainWindow::setName(QByteArray name, short x)
{
    if (x)
    {
        ui->blueFile->setPlainText(QString(name));
    }
    else
    {
        ui->redFile->setPlainText(QString(name));
    }
    qDebug() << "/setName()";
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

    //if(timer != -1)
   //     this->killTimer(timer);
    timer_thread = new QThread(this);
    timer = new QTimer();
    timer ->moveToThread(timer_thread);
    timer->setInterval(100);
    connect(timer_thread,SIGNAL(started()),timer,SLOT(start()));
    //connect(timer,&QTimer::timeout,this,SLOT(refreshTimer()),Qt::DirectConnection);
    connect(timer,&QTimer::timeout,this,&MainWindow::refreshTimer,Qt::DirectConnection);
    time[0] = 0;
    time[1] = 0;
    timer_thread->start();
//    timer = this->startTimer(1000);

}

void MainWindow::refreshTimer()
{

    if(hex->getStatus() == 0)
    {
        //unit of time is 0.1s
        time[hex->getWho()]++;
        /*
        if (time > threshold)
        {
            timeWin();
        }
        */
        refreshTimerLabel();
    }
    else if (hex->getStatus() == -1)
    {
        time[0] = time[1] = 0;
    }
    else
    {
        // just freeze time
    }
}
void MainWindow::refreshTimerLabel()
{
    int minute = time[0]/10 / 60;
    int second = time[0]/10 % 60;
    QString time_string;
    time_string = QString("%1:%2").arg(minute,2,10,QChar('0')).arg(second,2,10,QChar('0'));
    ui->timerRed->setText(time_string);
    minute = time[1] /10 / 60;
    second = time[1] /10 % 60;
    time_string = QString("%1:%2").arg(minute,2,10,QChar('0')).arg(second,2,10,QChar('0'));
    ui->timerBlue->setText(time_string);
    //qDebug() << "board status: >>>>>" << hex->getStatus();
    // fucking inconsistent
    // QCoreApplication::processEvents();
}


