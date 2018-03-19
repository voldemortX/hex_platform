#ifndef IOTHREAD_H
#endif // IOTHREAD_H
#include<QThread>
#include<QProcess>
#include<QDebug>
class myThread: public QThread
{
   Q_OBJECT
public:
    myThread(QProcess*,int);
    virtual void run();
private:
    QProcess* proc=NULL;
    int id = 0;//1 for red 2 for blue
signals:
    void send_message(QByteArray,int);
};
