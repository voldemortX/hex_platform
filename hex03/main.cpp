#include "mainwindow.h"
#include "hexboard.h"
#include <QApplication>
#include <synchapi.h>
QProcess* p[2] = {NULL, NULL};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    hexBoard hex;
    MainWindow w;
    w.linkBoard(&hex);
    w.show();

/*
    hex.makeMove("AA", 1, &w);
    hex.makeMove("CD", 2, &w);
    hex.makeMove("AB", 2, &w);
    hex.makeMove("KK", 1, &w);

    std::string tempString = "";
    for (char i = 'K'; i <= 'K'; ++i)
        for (char j = 'B'; j <= 'B'; ++j)
        {

            hex.makeMove(tempString + i + j, 1, &w);
            //Sleep(300);
        }
*/
    return a.exec();
}
