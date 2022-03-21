#ifndef MYSOCKET_H
#define MYSOCKET_H

#include <QObject>
#include <QJsonDocument>
#include <QTcpSocket>
#include <QVector>
#include <QList>
#include <QThread>
#include <QTimer>
#include "Util/myapp.h"
#include "threadworker.h"

class MySocket : public QObject
{
    Q_OBJECT
private:
    /**
     * @brief MySocket
     * @param parent
     * 设置私有权限，防止被实例化
     */
    explicit MySocket(QObject *parent = nullptr);
    ~MySocket();
public:
    void initMySocket();
    void connectToHost(const QString &host, const int &port);
    void closeClientConnected();
    bool isConnected();
    void sendData(const QJsonDocument document);
    void handleData();
    //单例模式饿汉式
    static MySocket*& getTcpSocket(){
        return m_mySocket;
    }

    //为了销毁m_tcpSocket而创建的类
    class deleteMySocket
    {
    public:
        deleteMySocket(){}
        ~deleteMySocket(){
            if(m_mySocket != nullptr)
            {
                m_mySocket->deleteLater();
                m_mySocket = nullptr;
                qDebug()<<"m_mySocket delete";
            }
        }
    };
public:
    void handleServerMag(const QJsonValue &dataVal,int _type);
    /*这个实际上是服务器的tcp连接的keeplive机制，
     * 当服务器检测到一个socket端长时间不活动的时候，
     * 就会发送一个探测包检测client端是否还在，
     * 而当client端收到不回应的时候，会关闭连接，回收资源。*/
    //增加socket保活参数配置，覆盖系统配置,每隔18s发送一个心跳包过去
    void keepaliveConnetcted();
signals:
    void connectTCPServerSignalToUi(bool state);
signals:
    void loginMagSignal(bool state);
    void registerMagSignal(bool state);
    void pubBlogEditMagSignal(const QJsonValue& dataVal);
    void userAllBlogEditMagSignal(const QJsonValue& dataVal);
    void findBlogEditMagSignal(const QJsonValue& dataval);
    void recomBlogEditMsgSignal(const QJsonValue& datval);
    //void picMsgSignal(QJsonValue datval);
    //void downloadFileSignal(QJsonValue datval);
public slots:
    void serverTodisConnected();
    void autoConnectToHost();
    void readyrecvData();
private:
    QTimer *m_timer;
    QTcpSocket* m_tcpSocket;
    bool connectState;
    static MySocket *m_mySocket;
    static deleteMySocket m_deleteMySocket;
    //接收缓冲区要设置扩容
    recvMemory recvBuffer;

    int recvLength;
    int recvUseLen;

    QThread *workthread;
    ThreadWorker *threadWorker;
    //保活连接参数
    int keeplivetime;
};

#endif // MYSOCKET_H
