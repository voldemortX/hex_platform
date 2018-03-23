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
    exchangeflag = false;
    iothread[0] = 0;
    iothread[1] = 0;
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
    terminateThread(RED);
    terminateThread(BLUE);
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
    if(exchangeflag)
        who = !who;
    p[who] = tp;
    p[who]->write("name?\n");
}

void MainWindow::receive_message(QByteArray response, int who)
{
    // comm
    if(exchangeflag)
        who = !who;

    if (response[0] == 'n' && response[1] == 'a')
    {
        setName(response, who);
    }
    else if (response[0] == 'm' && response[1] == 'o')
    {
        if(isStop==1)
        {
             isStop = 2;
             moveRecover = response;
             ui->labelStatus->setText("Stoped");
             return;
        }
        if(isStop!=0) return;
        QString lastMove = handleMove(response, who);
        if (lastMove == "")
        {
            hex->setStatus(-1);
            return;
        }
        QString tempQ = "move " + lastMove + "\n";
        p[!who]->write(tempQ.toLatin1());
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
    ui->historyDisplay->setPlainText(hex->getMoves());
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
    qDebug() << "Start";

    if((!p[RED-1] || !p[BLUE-1])||(redExe == "" || blueExe == ""))
    {
        return;
    }
    if(ui->labelStatus->text() == "Ongoing")
    {
        return;
    }
    startGame();
    // careful with time
    p[BLUE-1]->write("start blue\n");
    qDebug() << "/red start()" << "\n";
    p[RED-1]->write("start red\n");
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
    ui->historyDisplay->setPlainText(hex->getMoves());
    time[0] = time[1] = 0;
    isStop = 0;
    //refreshThreads();
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
    if(iothread[0]) return;
    QString filePath = QFileDialog::getOpenFileName(NULL, "xian studio says hi!", ".", "*.exe");
    if(filePath == "")return;
    filePath = "\""+filePath+"\"";
    ui->redName->setText(filePath);
    redExe = filePath;
    //refreshThreads();
    //if(iothread[0])
    //terminateThread(RED);

    iothread[0] = new myThread(redExe, exchangeflag?1:0);
    connect(iothread[0],SIGNAL(set_process(QProcess*,int)),this,SLOT(receive_process(QProcess*,int)));
    connect(iothread[0],SIGNAL(send_message(QByteArray,int)),this,SLOT(receive_message(QByteArray,int)));
    iothread[0]->start();
}

void MainWindow::on_buttonLoadBlue_clicked()
{
    if(iothread[1]) return;
    QString filePath = QFileDialog::getOpenFileName(NULL, "xian studio says hi!", ".", "*.exe");
    if(filePath == "")return;
    filePath = "\""+filePath+"\"";
    blueExe = filePath;
    ui->blueName->setText(filePath);
    //refreshThreads();
    //terminateThread(BLUE);
    //qDebug()<<"blue exchange"<<exchangeflag;
    iothread[1] = new myThread(blueExe, exchangeflag?0:1);
    connect(iothread[1],SIGNAL(set_process(QProcess*,int)),this,SLOT(receive_process(QProcess*,int)));
    connect(iothread[1],SIGNAL(send_message(QByteArray,int)),this,SLOT(receive_message(QByteArray,int)));
    iothread[1]->start();
}

void MainWindow::on_buttonUnloadRed_clicked()
{
    //if(ui->labelStatus->text()=="Ongoing") return;
    ui->redName->setText("Player 1");
    redExe = "";
    terminateThread(RED);
}

void MainWindow::on_buttonUnloadBlue_clicked()
{
    //if(ui->labelStatus->text()=="Ongoing") return;
    ui->blueName->setText("Player 2");
    blueExe = "";
    terminateThread(BLUE);
}

void MainWindow::on_StopButton_clicked()
{
    qDebug()<<isStop;
    if(isStop == 0)
    {
        isStop = 1;
        ui->labelStatus->setText("Stopping");
        return;
    }
    if(isStop == 1) return;
    if(isStop == 2)
    {
        ui->labelStatus->setText("Ongoing");
        isStop = 0;
        receive_message(moveRecover,(int)hex->getWho());
        return;
    }

}
void MainWindow::terminateThread(short x)
{
    x -= 1;
    if (iothread[x])
    {
        iothread[x]->terminate();
        //iothread[x]->wait();
        delete iothread[x];
        iothread[x] = 0;
        if (p[x])
        {
         /*   p[x]->write("exit\n");
            p[x]->terminate();
            delete p[x];*/
            p[x] = 0;
        }

    }
}

void MainWindow::on_buttonExchange_clicked()
{
    if(ui->labelStatus->text()=="Ongoing") return;
    //if(ui->labelStatus->text())
    isStop = 0;
    QString temp = redExe;
    redExe = blueExe;
    blueExe = temp;
    //qDebug()<<"<"<<redExe<<">";
    temp = ui->blueName->text();
    ui->blueName->setText(ui->redName->text());
    ui->redName->setText(temp);

    QProcess* tp = p[0];
    p[0] = p[1];
    p[1] = tp;
    if(p[0])
        p[0]->write("name?\n");
    if(p[1])
        p[1]->write("name?\n");
    myThread* tt = iothread[0];
    iothread[0] = iothread[1];
    iothread[1] = tt;
    exchangeflag = !exchangeflag;
}

void MainWindow::setName(QByteArray name, short x)
{
    if (x)
    {
        ui->blueName->setText(QString(name).mid(5));
        ui->blueName->setAlignment(Qt::AlignHCenter);
    }
    else
    {
        ui->redName->setText(QString(name).mid(5));
        ui->redName->setAlignment(Qt::AlignHCenter);
    }
    qDebug() << "/setName()";
}

void MainWindow::on_buttonSave_clicked()
{
    QString filename = QFileDialog::getSaveFileName(NULL, "xian studio says hi!", ".", "*.txt");
    QFile file(filename);
    file.open(QIODevice::ReadWrite|QIODevice::Text);
    QTextStream in(&file);
    in<<hex->getMoves();
    file.close();
}
void MainWindow::resetTimer()
{

    //if(timer != -1)
   //     this->killTimer(timer);
    timer_thread = new QThread(this);
    timer = new QTimer();
    timer ->moveToThread(timer_thread);
    timer->setInterval(10);
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

    if(hex->getStatus() == 0 && isStop!=2)
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
    int minute = time[0]/100 / 60;
    int second = time[0]/100 % 60;
    QString time_string;
    time_string = QString("%1:%2").arg(minute,2,10,QChar('0')).arg(second,2,10,QChar('0'));
    ui->timerRed->setText(time_string);
    minute = time[1] /100 / 60;
    second = time[1] /100 % 60;
    time_string = QString("%1:%2").arg(minute,2,10,QChar('0')).arg(second,2,10,QChar('0'));
    ui->timerBlue->setText(time_string);
    //qDebug() << "board status: >>>>>" << hex->getStatus();
    // fucking inconsistent
    // QCoreApplication::processEvents();
}
// MainWindow::on_pushButton_clicked(){}
