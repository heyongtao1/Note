#pragma execution_character_set("utf-8")
#include "uniblog.h"
#include "ui_uniblog.h"

#include <QFile>
#include <QGridLayout>
#include <QDebug>
#include <QLineEdit>
#include <QSplitter>
#include <QScrollBar>
#include <QTime>
#include <QDateTime>
#include <QJsonArray>
#include <QMenu>
#include "MyWidgets/NoteWidget/notewidget.h"
UniBlog::UniBlog(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UniBlog)
{
    ui->setupUi(this);
    readUniBlogWidgetQss();
    //初始化托盘
    initSystemTray();
    initSignal();
    initMenu();
    initUniBlog();
    //highlight = new Highlighter(ui->noteTextEdit->document());
}

UniBlog::~UniBlog()
{
    systemTray->deleteLater();
    systemTray = nullptr;
    for (int i = 0; i < ui->notelistWidget->count(); ++i)
    {
        QListWidgetItem *item = ui->notelistWidget->takeItem(0);
        if(item)
            delete item;  item = nullptr;
    }
    ui->notelistWidget->clear();
    qDebug() << "~UniBlog";
    delete ui;
}
//------------------------------------------------------------------
/*                          窗口移动重写                             */
void UniBlog::mouseMoveEvent(QMouseEvent *e)
{
    if (m_mousePressed && (e->buttons() && Qt::LeftButton)) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}
void UniBlog::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        m_mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}
void UniBlog::mouseReleaseEvent(QMouseEvent *)
{
    m_mousePressed = false;
}
//------------------------------------------------------------------
void UniBlog::on_needDealBt_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
void UniBlog::initSystemTray()
{
    systemTray = new QSystemTrayIcon(this);
    systemTray->setIcon(QIcon(":/Image/Login/笔记_s.png"));
    systemTray->setToolTip("UniBlog Note");
    systemTray->show();
}
void UniBlog::showSystemMenu()
{
    QMenu* m_menu = new QMenu(this);
    QAction* exitAction = new QAction(m_menu);
    QAction* showMainAction = new QAction(m_menu);
    QAction* setAction = new QAction(m_menu);
    QAction* closeAction = new QAction(m_menu);

    closeAction->setText("关闭UniBlog");
    setAction->setText("设置");
    exitAction->setText("退出登录");
    showMainAction->setText("显示主界面");

    m_menu->addAction(showMainAction);
    m_menu->addAction(setAction);
    m_menu->addAction(exitAction);
    m_menu->addAction(closeAction);

    connect(exitAction, &QAction::triggered, this, [=](){
        on_closeBt_clicked();
    });
    connect(showMainAction, &QAction::triggered, this, [=](){
            this->showNormal();
    });
    systemTray->setContextMenu(m_menu);
}
void UniBlog::activeTray(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Context:
        showSystemMenu();
        break;
    case QSystemTrayIcon::DoubleClick:
        qDebug()<<"DoubleClick";
        break;
    case QSystemTrayIcon::Trigger:
        this->showNormal();
        break;
    }
}

void UniBlog::recvNewCreateNoteForResult(const QJsonValue& dataVal)
{
    if(dataVal.isObject())
    {
        QJsonObject obj = dataVal.toObject();
        newNote->setNoteId(obj.value("blogEditid").toInt());
        CMessageBox::Infomation(this, "新建提示", "新建成功");
    }
}
void UniBlog::readUniBlogWidgetQss()
{
    //这是在Qt的资源下的文件,可以不用在资源下
    QFile file(":/Qss/Uniblog/default.qss");
    //只读方式打开文件
    file.open(QFile::ReadOnly);
    //读取文件的所有内容，并转换成QString类型
    QString styleSheet = tr(file.readAll());
    //当前窗口设置样式表
    this->setStyleSheet(styleSheet);
    file.close();

    QFile file1(":/Qss/util/QScrollBar.qss");
    file1.open(QFile::ReadOnly);
    QString styleSheet1 = tr(file1.readAll());
    ui->notelistWidget->verticalScrollBar()->setStyleSheet(styleSheet1);
    ui->noteTextEdit->verticalScrollBar()->setStyleSheet(styleSheet1);
    file1.close();
}

void UniBlog::initUniBlog()
{
    //设置无边框
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    QAction *searchAction = new QAction(ui->searchlineEdit);
    searchAction->setIcon(QIcon(":/Image/Uniblog/搜索.png"));
    ui->searchlineEdit->addAction(searchAction,QLineEdit::LeadingPosition);

    //加载资源
    Singleton<Repeater>::getInstance().sendLoadUserAllNoteJson();

    //初始化变量
    currNote = nullptr;
}

void UniBlog::initSignal()
{
    connect(MySocket::getTcpSocket(),SIGNAL(userAllBlogEditMagSignal(const QJsonValue&)),this,SLOT(recvServerUserAllNoteForResult(const QJsonValue&)));
    connect(systemTray, &QSystemTrayIcon::activated, this, &UniBlog::activeTray);//点击托盘，执行相应的动作
    connect(MySocket::getTcpSocket(),SIGNAL(pubBlogEditMagSignal(const QJsonValue&)),this,SLOT(recvNewCreateNoteForResult(const QJsonValue&)));
}

void UniBlog::initMenu()
{
    QMenu* MyMenu = new QMenu(this);
    MyMenu->setFixedWidth(200);
    //这是在Qt的资源下的文件,可以不用在资源下
    QFile file(":/Qss/util/Menu.qss");
    //只读方式打开文件
    file.open(QFile::ReadOnly);
    //读取文件的所有内容，并转换成QString类型
    QString styleSheet = tr(file.readAll());
    //当前窗口设置样式表
    MyMenu->setStyleSheet(styleSheet);
    file.close();
    QAction* signIn = new QAction(MyMenu);
    QAction* userInfo = new QAction(MyMenu);
    QAction* accountInfo = new QAction(MyMenu);
    QAction* set = new QAction(MyMenu);
    QAction* member = new QAction(MyMenu);
    //QAction* help = new QAction(MyMenu);

    QMenu* help_menu = new QMenu(this);
    help_menu->setStyleSheet(styleSheet);
    help_menu->setFixedWidth(200);
    QAction* help_1 = new QAction(help_menu);
    QAction* help_2 = new QAction(help_menu);
    QAction* help_3 = new QAction(help_menu);
    help_1->setText("联系客服");
    help_2->setText("意见反馈");
    help_3->setText("问卷调查");
    help_1->setIcon(QIcon(":/Image/Uniblog/联系客服.png"));
    help_2->setIcon(QIcon(":/Image/Uniblog/意见反馈.png"));
    help_3->setIcon(QIcon(":/Image/Uniblog/问卷调查.png"));
    help_menu->addAction(help_1);
    help_menu->addAction(help_2);
    help_menu->addAction(help_3);
    help_menu->setTitle("帮助与反馈");
    help_menu->setIcon(QIcon(":/Image/Uniblog/帮助.png"));

    QAction* exit = new QAction(MyMenu);
    connect(exit,&QAction::triggered,this,[=](){
        emit exitLoginSiganl();
    });

    QAction* about = new QAction(MyMenu);
    QAction* update = new QAction(MyMenu);
    signIn->setText("  签到");
    userInfo->setText("个人信息");
    userInfo->setIcon(QIcon(":/Image/Uniblog/个人信息.png"));
    accountInfo->setText("账号信息");
    accountInfo->setIcon(QIcon(":/Image/Uniblog/账号信息.png"));
    set->setText("设置");
    set->setIcon(QIcon(":/Image/Uniblog/设置.png"));
    member->setText("会员");
    member->setIcon(QIcon(":/Image/Uniblog/3.1 会员.png"));
    exit->setText("退出登录");
    exit->setIcon(QIcon(":/Image/Uniblog/退出登录.png"));
    about->setText("关于我们");
    about->setIcon(QIcon(":/Image/Uniblog/关于我们.png"));
    update->setText("更新版本");
    update->setIcon(QIcon(":/Image/Uniblog/版本更新.png"));
    MyMenu->addAction(signIn);
    MyMenu->addAction(userInfo);
    MyMenu->addAction(accountInfo);
    MyMenu->addSeparator();
    MyMenu->addAction(set);
    MyMenu->addAction(member);
    MyMenu->addMenu(help_menu);
    MyMenu->addSeparator();
    MyMenu->addAction(exit);
    MyMenu->addAction(about);
    MyMenu->addAction(update);
    ui->headBt->setMenu(MyMenu);
}
void UniBlog::resetTextFormat()
{
    int end = ui->noteTextEdit->toPlainText().size();
    QTextCharFormat initfmt;
    initfmt.setFontWeight(QFont::Normal);
    initfmt.setFontItalic(false);
    initfmt.setFontUnderline(false);
    QTextCursor cursor = ui->noteTextEdit->textCursor();
    cursor.setPosition(0, QTextCursor::MoveAnchor);
    cursor.setPosition(end, QTextCursor::KeepAnchor);
    cursor.mergeCharFormat(initfmt);
    ui->noteTextEdit->mergeCurrentCharFormat(initfmt);
    ui->noteTextEdit->update();
}

void UniBlog::initFormat(const QList<TextFormat>& fmtList)
{
    //重置文本属性
    resetTextFormat();
    if(fmtList.isEmpty()) return ;
    for(auto fmt : fmtList)
    {
        setTextFormat(fmt.startPos,fmt.endPos,fmt.fmtlevel);
    }
}

void UniBlog::setTextFormat(int startPos, int endPos,const FormatLevel fmtlevel)
{
    QTextCharFormat fmt;
    switch (fmtlevel) {
    case FormatLevel::bold :fmt.setFontWeight(QFont::Bold);break;

    case FormatLevel::Nobold :fmt.setFontWeight(QFont::Normal);break;

    case FormatLevel::italics : fmt.setFontItalic(true);break;

    case FormatLevel::Noitalics : fmt.setFontItalic(false);break;

    case FormatLevel::underline : fmt.setFontUnderline(true);break;

    case FormatLevel::Nounderline : fmt.setFontUnderline(false);break;
    default:
        break;
    }
    QTextCursor cursor = ui->noteTextEdit->textCursor();
    cursor.setPosition(startPos, QTextCursor::MoveAnchor);
    cursor.setPosition(endPos, QTextCursor::KeepAnchor);
    cursor.mergeCharFormat(fmt);
    ui->noteTextEdit->mergeCurrentCharFormat(fmt);
    ui->noteTextEdit->update();
}

void UniBlog::on_newBuildBt_clicked()
{
    QString currTime = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm");

    NoteWidget* note = new NoteWidget();
    note->setNoteTitle("无标题笔记");
    note->setNoteContent("欢迎使用Uniblog笔记");
    note->setNoteTime(currTime);

    QListWidgetItem *item = new QListWidgetItem();
    item->setSizeHint(QSize(note->width(),note->height()));
    ui->notelistWidget->addItem(item);
    ui->notelistWidget->setItemWidget(item,note);
    ui->notelistWidget->update();

    newNote = note;

    //转发至服务器
    Singleton<Repeater>::getInstance().sendCreateNoteJson(QString("欢迎使用Uniblog笔记").toUtf8(),QString("无标题笔记").toUtf8(),currTime);
}

void UniBlog::on_closeBt_clicked()
{
    emit exitLoginSiganl();
}

void UniBlog::on_zoomBt_clicked()
{
    if(widgetChange)
    {
        this->setWindowState(Qt::WindowMaximized);
        ui->zoomBt->setStyleSheet("border-image: url(:/Image/Uniblog/最大化.png);");
    }
    else
    {
        this->showNormal();
        ui->zoomBt->setStyleSheet("border-image: url(:/Image/Uniblog/全屏.png);");
    }
    widgetChange = widgetChange ? false : true;
}

void UniBlog::on_minimizeBt_clicked()
{
    this->showMinimized();
}
//=========================================================================
void UniBlog::on_notelistWidget_itemClicked(QListWidgetItem *item)
{
    /*
    2.  从得到的widget上的找到需要的子控件
    方法一:    QList<QLabel*> labelList = pwig->findChildren<QLabel*>();
    方法二:    QLabel * label = pwig->findChild<QLabel *>("label2");
    */
    auto note = (NoteWidget*)ui->notelistWidget->itemWidget(item);
    ui->noteHeadlineEdit->setText(note->getNoteTitle());
    ui->noteTextEdit->setText(note->getNoteContent());
    ui->describeLb->setText(note->getNoteTime());
    currNote = note;

    //初始化文本属性格式
    initFormat(note->getFormatList());
}

void UniBlog::on_saveNoteBt_clicked()
{
    if(!currNote) return ;
    if(currNote->getNoteId() == -1)
    {
        CMessageBox::Infomation(this, QStringLiteral("保存提示"), QStringLiteral("保存出错"));
        return ;
    }
    currNote->setNoteTitle(ui->noteHeadlineEdit->text());
    currNote->setNoteContent(ui->noteTextEdit->toPlainText());
    currNote->setNoteTime(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm"));
    //将修改后的信息发送到服务器
    Singleton<Repeater>::getInstance().sendUpdateNoteJson(currNote->getNoteId(),currNote->getNoteContent(),currNote->getNoteTitle(),currNote->getNoteTime());
}

void UniBlog::recvServerUserAllNoteForResult(const QJsonValue& dataVal)
{
    if(dataVal.isArray()){
        QJsonArray noteArray = dataVal.toArray();
        for(int i=0;i<noteArray.count();i++){
            int noteId = noteArray.at(i).toObject().value("blogeditid").toInt();
            QString mcontent = noteArray.at(i).toObject().value("blogtext").toString();
            QString mtitle = noteArray.at(i).toObject().value("bloghead").toString();
            QString time = noteArray.at(i).toObject().value("pubtime").toString();

            NoteWidget* note = new NoteWidget();
            note->setNoteTitle(mtitle);
            note->setNoteContent(mcontent);
            note->setNoteTime(time);
            note->setNoteId(noteId);

            QListWidgetItem *item = new QListWidgetItem();
            item->setSizeHint(QSize(note->width(),note->height()));
            ui->notelistWidget->addItem(item);
            ui->notelistWidget->setItemWidget(item,note);
            ui->notelistWidget->update();
        }
    }
}

void UniBlog::changeTextAttribute(const FormatLevel fmtlevel)
{
    QTextCharFormat fmt;
    QTextCursor cursor = ui->noteTextEdit->textCursor();
    if (!cursor.hasSelection()) {
        cursor.select(QTextCursor::WordUnderCursor);
    }
    TextFormat txtFmt;
    txtFmt.startPos = cursor.selectionStart();
    txtFmt.endPos   = cursor.selectionEnd();
    txtFmt.fmtlevel = fmtlevel;
    bool isNormal;
    if(currNote->isExistence(txtFmt,isNormal))
    {
        if(isNormal)
        {
            switch(fmtlevel)
            {
                case FormatLevel::Bold :fmt.setFontWeight(QFont::Bold);break;
                case FormatLevel::Italics : fmt.setFontItalic(true);break;
                case FormatLevel::Underline :fmt.setFontUnderline(true);break;
            }
        }
        else
        {
            switch(fmtlevel)
            {
                case FormatLevel::Bold :fmt.setFontWeight(QFont::Normal);break;
                case FormatLevel::Italics : fmt.setFontItalic(false);break;
                case FormatLevel::Underline :fmt.setFontUnderline(false);break;
            }
        }
    }
    else
    {
        switch(fmtlevel)
        {
            case FormatLevel::Bold :txtFmt.fmtlevel = FormatLevel::bold;break;
            case FormatLevel::Italics : txtFmt.fmtlevel = FormatLevel::italics;break;
            case FormatLevel::Underline :txtFmt.fmtlevel = FormatLevel::underline;break;
        }
        currNote->formatListAppend(txtFmt);
        switch(fmtlevel)
        {
            case FormatLevel::Bold :fmt.setFontWeight(QFont::Bold);break;
            case FormatLevel::Italics : fmt.setFontItalic(true);break;
            case FormatLevel::Underline : fmt.setFontUnderline(true);break;
        }
    }
    cursor.mergeCharFormat(fmt);
    ui->noteTextEdit->mergeCurrentCharFormat(fmt);
    ui->noteTextEdit->update();
}
//加粗
void UniBlog::on_boldBt_clicked()
{
    if(currNote)
    changeTextAttribute(FormatLevel::Bold);
}
//斜体
void UniBlog::on_ItalicsBt_clicked()
{
    if(currNote)
    changeTextAttribute(FormatLevel::Italics);
}
//下划线
void UniBlog::on_underlineBt_clicked()
{
    if(currNote)
    changeTextAttribute(FormatLevel::Underline);
}

void UniBlog::on_highlightBt_clicked()
{

}

void UniBlog::on_fontComboBox_activated(const QString &arg1)
{
    ui->noteTextEdit->setFont(QFont(arg1));
}
