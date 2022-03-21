#ifndef NOTEWIDGET_H
#define NOTEWIDGET_H

#include <QWidget>
#include "NetSocket/mysocket.h"
#include <QList>
#include <QTextCharFormat>
namespace Ui {
class NoteWidget;
}

enum FormatLevel{
    Bold = 0,
    bold,
    Nobold,

    Italics = 5,
    italics ,
    Noitalics,

    Underline = 10,
    underline,
    Nounderline
};

struct TextFormat{
    int startPos;
    int endPos;
    FormatLevel fmtlevel;
    bool operator ==(const TextFormat fmt){
          if(fmtlevel == fmt.fmtlevel) return true;
          return false;
   }

};

class NoteWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NoteWidget(QWidget *parent = 0);
    ~NoteWidget();
public:
    void setNote(QString mtitle,QString mcontent,QString mtime,QString msavePath,int mnoteSize);
    int  getNoteId();
    void setNoteId(int mnoteId);
    QString getNoteTitle();
    void setNoteTitle(QString mtitle);
    QString getNoteContent();
    void setNoteContent(QString mcontent);
    QString getNoteTime();
    void setNoteTime(QString mtime);
    QString getNoteSavePath();
    void setNoteSavePath(QString msavePath);
    int  getNoteSize();
    void setNoteSize(int mnoteSize);
    QList<TextFormat> getFormatList();
    void formatListAppend(TextFormat fmt);
    bool isExistence(TextFormat fmt,bool& isNormal);
private:
    void readLoginWidgetQss();

private:
    Ui::NoteWidget *ui;
    QString title;
    QString content;
    QString time;
    QString savePath;
    int     noteSize;
    int     noteId;
    QList<TextFormat> fmtList;
};

#endif // NOTEWIDGET_H
