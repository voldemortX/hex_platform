#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QLabel>

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

private:
    Ui::MainWindow *ui;
    const short xOffset = 116 + 0;
    const short yOffset = 83 + 35;
    const short xDelta = 44;
    const short yDelta = 38;
    const short pieceWidth = 47;
    const short pieceHeight = 53;
    QLabel pieces[11][11];
};

#endif // MAINWINDOW_H
