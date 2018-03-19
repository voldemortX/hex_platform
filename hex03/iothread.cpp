#include "iothread.h"

myThread::myThread(std::string p,int id)
{
   // if(p=NULL)
   QString path = QString::fromStdString(p);
   qDebug() << "Path:" << path;
   proc = new QProcess;
   proc->start(path);
   proc->waitForStarted();
   this->id = id;
   emit set_process(proc,id);
}

void myThread::run()
{
    while(1)
    {
        bool flag = proc->waitForReadyRead();
        if(flag)
        {
            QByteArray message;
            message = proc->readLine();
            if(message[0] == 'm' && message [1] == 'o'&& message [2] == 'v'&& message [3] == 'e')
                emit send_message(message,id);
            if(message[0] == 'n' && message [1] == 'a'&& message [2] == 'm'&& message [3] == 'e')
                emit send_message(message,id);
        }
    }
        /* while(1)
    {
       // emit send_message("abcd",id);
       //proc->write("start blue\n");
       // proc->write("move BB\n\0");
       bool flag = proc->waitForReadyRead(1000);

        //qDebug()<<id<<id<<'\n';
       // sleep(3);
//        emit send_message("abc2",1);
        if(flag)
        {
            QByteArray move;
           // move.resize(10);
            move = proc->readLine();
           // qDebug()<<"mm"<<move;
            emit send_message(move,id);
        }
        else
        {
            QByteArray move;
           // move.resize(10);
            move = proc->readLine();
            qDebug()<<"false";
            emit send_message(move,id);
        }
        break;
    }*/
}

myThread::~myThread()
{
    proc->terminate();
    delete proc;
}
