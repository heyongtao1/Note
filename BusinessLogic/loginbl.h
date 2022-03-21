#ifndef LOGINBL_H
#define LOGINBL_H
#include "json.h"
#include "NetSocket/mysocket.h"
class LoginBL
{
public:
    LoginBL();
public:
    void sendLoginJson(QString username,QString password);
};

#endif // LOGINBL_H
