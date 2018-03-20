#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QLabel>
#include <string>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <synchapi.h>
#include <QProcess>
#include <QFile>
#include <QTextCodec>
#include "hexboard.h"
#include <QTimer>
#include <QThread>
#include <iothread.h>
namespace Ui {
class MainWindow;
}

extern QProcess* p[2];

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool setPic(short x, short y, short player);  // uses 12 when "who" uses 01
    bool clearPieces();
    void linkBoard(hexBoard* hexIn);

private slots:
    void on_buttonStart_clicked();

    void on_buttonLoadRed_clicked();

    void on_buttonLoadBlue_clicked();

    void on_buttonUnloadRed_clicked();

    void on_buttonUnloadBlue_clicked();

    void on_buttonExchange_clicked();

    void on_buttonSave_clicked();

    void receive_message(QByteArray response, int who);
    void receive_process(QProcess* tp, int who);
    void refreshTimer();
    void refreshTimerLabel();

private:
    Ui::MainWindow *ui;
    hexBoard* hex;
    myThread* iothread[2];
    const short RED=1;
    const short BLUE=2;
    const short xOffset = 116 - 20;
    const short yOffset = 83 - 10;
    const short xDelta = 44;
    const short yDelta = 38;
    const short pieceWidth = 47;
    const short pieceHeight = 53;
    const int maxTime = -1;  // step time
    //int timer = -1;
    QThread *timer_thread = NULL;
    QTimer *timer = NULL;
    int time[2] = {0};
    QLabel pieces[11][11];
    QString redExe;
    QString blueExe;
    QString lastMove = "";
    bool exchangeflag;

    //void timeWin();
    void startGame();
    void resetTimer();
    void refreshProcesses();
    void setName(QByteArray name, short x);
    void terminateThread(short x);
    void refreshThreads();
    QString handleMove(QByteArray response, int who);
}; 

#endif // MAINWINDOW_H
