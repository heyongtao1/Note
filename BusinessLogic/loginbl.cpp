#include "loginbl.h"

LoginBL::LoginBL()
{

}

void LoginBL::sendLoginJson(QString username,QString password)
{
    MySocket::getTcpSocket()->sendData(Singleton<Json>::getInstance().createLoginJson(username,password));
}
