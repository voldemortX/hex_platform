#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>

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

private slots:
    void on_buttonStart_clicked();

private:
    Ui::MainWindow *ui;
    short xOffset;
    short yOffset;
};

#endif // MAINWINDOW_H
