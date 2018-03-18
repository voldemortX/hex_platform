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


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool setPic(short x, short y, short player);
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

private:
    Ui::MainWindow *ui;
    hexBoard* hex;
    const short xOffset = 116 - 20;
    const short yOffset = 83 - 10;
    const short xDelta = 44;
    const short yDelta = 38;
    const short pieceWidth = 47;
    const short pieceHeight = 53;
    const int maxTime = 30000;  // step time
    QLabel pieces[11][11];
    std::string redExe;
    std::string blueExe;
};

#endif // MAINWINDOW_H
