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
   //emit set_process(proc,id);
   qDebug()<<"set "<<id<<" "<<proc;
}

void myThread::run()
{
    emit set_process(proc,id);
    QByteArray message;
    while(1)
    {
        bool flag = 1;
        do
        {
            Sleep(12);
            message = proc->readLine(20);
          //  qDebug()<<"read " <<id;
        }while(!test_message(message));
        if(flag)
        {
           // QByteArray message;
            //message = proc->readLine();
            qDebug()<<"get "<<message <<" "<<id;

            if(message[0] == 'm' && message [1] == 'o'&& message [2] == 'v'&& message [3] == 'e')
                emit send_message(message,id);
            if(message[0] == 'n' && message [1] == 'a'&& message [2] == 'm'&& message [3] == 'e')
                emit send_message(message,id);
        }
        else
        {
            qDebug()<<"no "<<id;
        }
    }
}

myThread::~myThread()
{
    proc->terminate();
    delete proc;
}
bool myThread::test_message(QByteArray message)
{
    if(message[0] == 'm' && message [1] == 'o'&& message [2] == 'v'&& message [3] == 'e')
        return true;
    if(message[0] == 'n' && message [1] == 'a'&& message [2] == 'm'&& message [3] == 'e')
        return true;
    return false;
}
