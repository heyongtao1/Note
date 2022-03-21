#include "threadworker.h"
#include "mysocket.h"
#include <QDebug>
ThreadWorker::ThreadWorker()
{

}

void ThreadWorker::doTcpWorker()
{
    qDebug()<<"TCP thread id = " << QThread::currentThreadId();
    if(MySocket::getTcpSocket()->isConnected())
        MySocket::getTcpSocket()->readyrecvData();
}
