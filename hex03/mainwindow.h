#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QLabel>
#include <string>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool setPic(int x, int y, QPixmap& pic);
    bool clearPieces();

private slots:
    void on_buttonStart_clicked();

    void on_buttonLoadRed_clicked();

    void on_buttonLoadBlue_clicked();

private:
    Ui::MainWindow *ui;
    const short xOffset = 116 - 20;
    const short yOffset = 83 - 10;
    const short xDelta = 44;
    const short yDelta = 38;
    const short pieceWidth = 47;
    const short pieceHeight = 53;
    QLabel pieces[11][11];
    std::string redExe;
    std::string blueExe;
};

#endif // MAINWINDOW_H
