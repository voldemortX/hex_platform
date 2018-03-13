#include "mainwindow.h"
#include "hexboard.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    hexBoard hex(&w);
    hex.makeMove("AA", 1, &w);
    hex.makeMove("CD", 2, &w);
    hex.makeMove("AB", 2, &w);
    hex.makeMove("KK", 1, &w);
    return a.exec();
}
