#include "mainwindow.h"
#include "hexboard.h"
#include <QApplication>
#include <synchapi.h>

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
/*
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
