#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QMenu>
#include "NetSocket/mysocket.h"
#include "MyWidgets/customwidget.h"
#include "BusinessLogic/repeater.h"
namespace Ui {
class Login;
}

class Login : public QMainWindow
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();
protected:
    QPoint mousePoint;
    bool m_mousePressed;
    void mouseMoveEvent(QMouseEvent *e);//鼠标移动事件
    void mousePressEvent(QMouseEvent *e);//鼠标按下事件
    void mouseReleaseEvent(QMouseEvent *);//鼠标松开事件
private:
    void readLoginWidgetQss();
    void initLogin();
    void initSignal();
    void initNetSocket();
private slots:
    void on_closeBt_clicked();
    void passwordIsShow(bool isTriggered);
    void on_LoginBt_clicked();
    void loginMag(bool isSuccess);
private:
    Ui::Login *ui;
    QAction *passwordAt;
    bool isNoShowPassword;
};

#endif // LOGIN_H
