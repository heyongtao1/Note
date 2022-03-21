#ifndef UNIBLOG_H
#define UNIBLOG_H

#include <QListWidget>
#include <QMainWindow>
#include <QMouseEvent>
#include <QTextCharFormat>
#include <QSystemTrayIcon>
#include <QSharedPointer>
#include "BusinessLogic/repeater.h"
#include "MyWidgets/NoteWidget/notewidget.h"
#include "MyWidgets/highlighter.h"
#include "MyWidgets/customwidget.h"
namespace Ui {
class UniBlog;
}

class UniBlog : public QMainWindow
{
    Q_OBJECT

public:
    explicit UniBlog(QWidget *parent = 0);
    ~UniBlog();
protected:
    QPoint mousePoint;
    bool m_mousePressed;
    void mouseMoveEvent(QMouseEvent *e);//鼠标移动事件
    void mousePressEvent(QMouseEvent *e);//鼠标按下事件
    void mouseReleaseEvent(QMouseEvent *);//鼠标松开事件
private slots:
    void on_needDealBt_clicked();

    void on_newBuildBt_clicked();

    void on_closeBt_clicked();

    void on_zoomBt_clicked();

    void on_minimizeBt_clicked();

    void on_notelistWidget_itemClicked(QListWidgetItem *item);

    void on_saveNoteBt_clicked();

    void on_boldBt_clicked();

    void on_ItalicsBt_clicked();

    void on_underlineBt_clicked();

    void on_highlightBt_clicked();

    void on_fontComboBox_activated(const QString &arg1);
private slots:
    void recvServerUserAllNoteForResult(const QJsonValue& dataVal);
    void activeTray(QSystemTrayIcon::ActivationReason reason);
    void recvNewCreateNoteForResult(const QJsonValue& dataVal);
signals:
    void exitLoginSiganl();
private:
    void readUniBlogWidgetQss();
    void initUniBlog();
    void initSignal();
    void initMenu();
    void initFormat(const QList<TextFormat>& fmtList);
    void resetTextFormat();
    void changeTextAttribute(const FormatLevel fmtlevel);
    void setTextFormat(int startPos,int endPos,const FormatLevel fmtlevel);
    void initSystemTray();
    void showSystemMenu();
private:
    Ui::UniBlog *ui;
    //窗口缩放
    bool widgetChange;
    //当前正在编辑的note
    NoteWidget* currNote;
    //当前新建的note
    NoteWidget* newNote;
    //系统托盘
    QSystemTrayIcon* systemTray;
private:
    bool isBold;
};

#endif // UNIBLOG_H
