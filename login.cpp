#include "login.h"
#include "ui_login.h"

#include <QFile>
#include <QMouseEvent>
#include <QDebug>
#include <QMenu>
#include "uniblog.h"

Login::Login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    //设置无边框
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    //读取qss文件
    readLoginWidgetQss();
    //初始化界面
    initLogin();
    //信号处理
    initSignal();
    //初始化网络TCP
    initNetSocket();
}
Login::~Login()
{
    passwordAt->deleteLater();
    passwordAt = nullptr;
    qDebug() << "~Login";
    delete ui;
}
void Login::initSignal()
{
    connect(MySocket::getTcpSocket(),SIGNAL(loginMagSignal(bool)),this,SLOT(loginMag(bool)));
}

void Login::initNetSocket()
{
    MySocket::getTcpSocket()->initMySocket();
}

void Login::readLoginWidgetQss()
{
    //这是在Qt的资源下的文件,可以不用在资源下
    QFile file(":/Qss/login/default.qss");
    //只读方式打开文件
    file.open(QFile::ReadOnly);
    //读取文件的所有内容，并转换成QString类型
    QString styleSheet = tr(file.readAll());
    //当前窗口设置样式表
    this->setStyleSheet(styleSheet);
    file.close(); 
}

void Login::initLogin()
{
    passwordAt = new QAction(this);
    passwordAt->setIcon(QIcon(":/Image/Login/inshow.png"));
    ui->passWordlineEdit->addAction(passwordAt,QLineEdit::TrailingPosition);
    connect(passwordAt, SIGNAL(triggered(bool)), this, SLOT(passwordIsShow(bool)));
}

void Login::on_closeBt_clicked()
{
    qApp->quit();
}

void Login::passwordIsShow(bool isTriggered)
{
    if(!isNoShowPassword)
    {
        passwordAt->setIcon(QIcon(":/Image/Login/show.png"));
        ui->passWordlineEdit->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        passwordAt->setIcon(QIcon(":/Image/Login/inshow.png"));
        ui->passWordlineEdit->setEchoMode(QLineEdit::Password);
    }
    isNoShowPassword = isNoShowPassword ? false : true;
}
//------------------------------------------------------------------
/*                          窗口移动重写                             */
void Login::mouseMoveEvent(QMouseEvent *e)
{
    if (m_mousePressed && (e->buttons() && Qt::LeftButton)) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}
void Login::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        m_mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}
void Login::mouseReleaseEvent(QMouseEvent *)
{
    m_mousePressed = false;
}
//------------------------------------------------------------------

void Login::on_LoginBt_clicked()
{
    Singleton<Repeater>::getInstance().sendLoginJson(ui->userNamelineEdit->text(),ui->passWordlineEdit->text());
}

void Login::loginMag(bool isSuccess)
{
    if(isSuccess){
        //清空输入框
        ui->userNamelineEdit->setText("");
        ui->passWordlineEdit->setText("");
        //主界面
        UniBlog* uniblog = new UniBlog();
        uniblog->show();
        this->hide();
        connect(uniblog,&UniBlog::exitLoginSiganl,this,[=](){
            this->show();
            uniblog->close();
            uniblog->deleteLater();
        });
    }else{
        CMessageBox::Infomation(this, QStringLiteral("登录提示"), QStringLiteral("登录失败，输入用户或密码错误"));
    }
}
