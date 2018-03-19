#include "iothread.h"

myThread::myThread(QProcess* p,int id)
{
   // if(p=NULL)
    this->proc = p;
    this->id = id;
}

void myThread::run()
{
    while(1)
    {
        emit send_message("abcd",id);
       bool flag = this->proc->waitForReadyRead(6);
        //qDebug()<<id<<id<<'\n';
       // sleep(3);
//        emit send_message("abc2",1);
        if(flag)
        {
            QByteArray move;
            move = proc->readLine(10);
            emit send_message(move,id);
        }
    }
}
