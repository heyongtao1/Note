#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <synchapi.h>
#include <QJsonValue>
#include "mysocket.h"
//实例化唯一Socket对象和销毁Socket对象的对象
MySocket* MySocket::m_mySocket = new MySocket();
MySocket::deleteMySocket MySocket::m_deleteMySocket;
MySocket::MySocket(QObject *parent) : QObject(parent)
{
    qDebug()<<"MySocket";
    //在不同的线程中使用同一对象，必须在构造函数中注册
    qRegisterMetaType<QJsonValue >("QJsonValue ");
    keeplivetime = 0;
    recvLength = -1;
    recvUseLen = 0;
}
/**
 * @brief MySocket::~MySocket
 */
MySocket::~MySocket()
{
    qDebug()<<"~MySocket() start";
    workthread->requestInterruption();
    workthread->quit();
    workthread->wait();
    if(workthread != nullptr){
        workthread->deleteLater();
        workthread = nullptr;
    }
    m_timer->stop();
    threadWorker->deleteLater();
    threadWorker = nullptr;
    closeClientConnected();
    m_tcpSocket->deleteLater();
    m_tcpSocket = nullptr;
    m_timer->deleteLater();
    m_timer = nullptr;
    qDebug()<<"~MySocket() end";
}
/**
 * @brief MySocket::initMySocket
 */
void MySocket::initMySocket()
{
    m_timer = new QTimer();
    m_tcpSocket = new QTcpSocket();
    connect(m_timer,&QTimer::timeout,this,&MySocket::autoConnectToHost);
    connect(m_tcpSocket,&QTcpSocket::disconnected,this,&MySocket::serverTodisConnected);
    workthread = new QThread();
    threadWorker = new ThreadWorker();

    connect(m_tcpSocket,SIGNAL(readyRead()),threadWorker,SLOT(doTcpWorker()));
    threadWorker->moveToThread(workthread);
    workthread->start();
    m_timer->start(200);
}
/**
 * @brief MySocket::connectToHost
 * @param host
 * @param port
 * 连接服务器
 */
void MySocket::connectToHost(const QString &host, const int &port)
{
    if (m_tcpSocket->isOpen()) m_tcpSocket->abort();
    m_tcpSocket->connectToHost(host, port);
}
/**
 * @brief MySocket::closeClientConnected
 * 客户端主动断开连接
 */
void MySocket::closeClientConnected()
{
    if (m_tcpSocket->isOpen())
    {
        m_tcpSocket->disconnected();
        m_timer->stop();
    }
}
/**
 * @brief MySocket::isConnected
 * 是否已经连接状态
 */
bool MySocket::isConnected()
{
    if(m_tcpSocket->state() != QTcpSocket::ConnectedState)
    {
        qDebug()<< "connected fail";
        return false;
    }
    //qDebug()<< "connected success";
    return true;
}


/**
 * @brief MySocket::serverTodisConnected
 * 服务器主动断开连接
 */
void MySocket::serverTodisConnected()
{
    if (m_tcpSocket->isOpen())
    {
        m_tcpSocket->abort();
        //m_timer->start(2000);
        connectState = true;
    }
}
/**
 * @brief MySocket::autoConnectToHost
 * @param host
 * @param port
 * 定时器心跳包检测连接服务器
 */
void MySocket::autoConnectToHost()
{
    if(!isConnected()){
        emit connectTCPServerSignalToUi(false);
        m_timer->setInterval(200);
        connectToHost(MyApp::m_strHostAddr,MyApp::m_nTcpMsgPort);
    }else{
        connectState = true;
        emit connectTCPServerSignalToUi(true);
        m_timer->setInterval(50);//1s 20
        keeplivetime += 50;
        if(keeplivetime == 18000){
            //keepaliveConnetcted();
        }
    }
}
/**
 * @brief MySocket::sendData
 */
void MySocket::sendData(const QJsonDocument document)
{
    qDebug()<<"void MySocket::sendData(QJsonDocument document)";
    //发送JSON文档
    if(isConnected()){
         QByteArray bufarray= QString(document.toJson(QJsonDocument::Compact)).toUtf8();
         Package _package;
         _package.data = bufarray.data();
         _package.total_len = bufarray.count();
         qDebug() << "len = " << _package.total_len;
         //发送确认值
         strcpy(_package.confrim,"HYT");
         m_tcpSocket->write(_package.confrim,sizeof(_package.confrim));
         //发送包头信息
         int recvsize = m_tcpSocket->write((char *)&_package.total_len,sizeof(_package.total_len));
         //发送包体消息
         recvsize = m_tcpSocket->write(_package.data,_package.total_len);
         keeplivetime = 0;
    }
}

void MySocket::handleData()
{
    QString recvDataString = QString(recvBuffer.recvData).toUtf8();
    qDebug() << recvDataString;
    QJsonParseError jsonerror;
    QJsonDocument document = QJsonDocument::fromJson(recvDataString.toUtf8(),&jsonerror);
    if(!document.isNull() && (jsonerror.error == QJsonParseError::NoError))
    {
        if(document.isObject())
        {
              QJsonObject jsonObj = document.object();
              QJsonValue dataVal = jsonObj.value("data");
              int _type = jsonObj.value("type").toInt();
              qDebug() << "type = " << _type;
              handleServerMag(dataVal,_type);
        }
    }else{
        qDebug()<<jsonerror.errorString();
    }
    memset(recvBuffer.recvData,'\0',recvLength);
    recvLength = -1;
    recvUseLen = 0;
    delete recvBuffer.recvData;
    recvBuffer.recvData = nullptr;
}

/**
 * @brief MySocket::readyrecvData
 */
void MySocket::readyrecvData()
{
    qDebug()<<"readyrecvData thread id = " << QThread::currentThreadId();
    //接收包头信息
    if(recvLength == -1){
        if(m_tcpSocket->read((char *)&recvLength,4) <= 0){
            return ;
        }
        qDebug()<<"recv len" << recvLength;
        recvBuffer.recvData = nullptr;
        recvBuffer.recvData = (char*)malloc((recvLength+1)*sizeof(char));
        memset(recvBuffer.recvData,'\0',recvLength+1);
    }
    //接收消息体
    recvUseLen += m_tcpSocket->read(recvBuffer.recvData+recvUseLen,recvLength-recvUseLen);
    qDebug()<<"recvUseLen len" << recvUseLen;
    //已接收完整数据包
    if(recvUseLen >= recvLength){
        handleData();
    }
}
/**
 * @brief MySocket::handleServerMag
 * @param dataVal
 * @param _type
 * 处理服务器各种类型信息
 */
void MySocket::handleServerMag(const QJsonValue &dataVal,int _type)
{
    qDebug()<<"handleServerMag(QJsonValue &dataVal,int _type)";
    if(dataVal.isObject() || dataVal.isArray()){
        QJsonObject dataobj = dataVal.toObject();
        switch(_type){
              case LoginMagType::LOGIN_SUCCESS   : MyApp::userid = dataobj.value("userid").toInt();
                                                   emit loginMagSignal(true);break;
              case LoginMagType::LOGIN_FAIL      : emit loginMagSignal(false);break;
              case RegistMagType::REGIST_SUCCESS : MyApp::userid = dataobj.value("userid").toInt();
                                                   emit registerMagSignal(true);break;
              case RegistMagType::REGIST_FAIL    : emit registerMagSignal(false);break;
              case BlogMagType::BLOG_SUCCESS    : emit pubBlogEditMagSignal(dataVal);qDebug()<<"emit";break;
              case BlogMagType::BLOG_FAIL    : emit pubBlogEditMagSignal(dataVal);break;

              case LoadBlogMagType::LOADBLOG_SUCCESS : emit userAllBlogEditMagSignal(dataVal);break;

              case FindBlogMagType::FINDBLOG_SUCCESS : emit findBlogEditMagSignal(dataVal);break;

              case RecomBlogMagType::RECOMBLOG_SUCCESS : emit recomBlogEditMsgSignal(dataVal);break;

              //case FileMsgTyPe::FILE_SUCCESS: emit picMsgSignal(dataVal);break;

              //case FILEDownLoadType::FILE_DOWNLOAD_SUCCESS : emit downloadFileSignal(dataVal);break;
        }
    }
}

void MySocket::keepaliveConnetcted()
{
    qDebug()<<"void MySocket::keepaliveConnetcted()";
    QJsonObject object;
    QJsonObject data_obj;

    object.insert("data",data_obj);
    object.insert("type",KeepLiveType::KEEPLIVE_MAG);

    QJsonDocument document;
    document.setObject(object);
    sendData(document);
}
