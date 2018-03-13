#include "mainwindow.h"
#include <QApplication>
//#include <cstring>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //std::string text = "Ongoing";
    MainWindow w;
    w.show();

    return a.exec();
}
