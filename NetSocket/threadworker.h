#ifndef THREADWORKER_H
#define THREADWORKER_H

#include <QObject>
#include "Util/myapp.h"
class ThreadWorker : public QObject
{
    Q_OBJECT
public:
    ThreadWorker();

private slots:
    void doTcpWorker();
public slots:

signals:

};

#endif // THREADWORKER_H
