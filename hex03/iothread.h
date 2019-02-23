#ifndef IOTHREAD_H
#define IOTHREAD_H

#include<QThread>
#include<QProcess>
#include<QDebug>
#include <synchapi.h>


class myThread: public QThread
{
   Q_OBJECT
public:
    myThread(QString path, int);
    virtual void run();
    ~myThread();
private:
    QProcess* proc=NULL;
    int id = 0;//0 for red 1 for blue
    void new_process();
    bool test_message(QByteArray);

signals:
    void send_message(QByteArray,int);
    void set_process(QProcess*,int);
};

#endif
