#include "notewidget.h"
#include "ui_notewidget.h"

#include <QFile>
#include <QJsonObject>

NoteWidget::NoteWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NoteWidget)
{
    ui->setupUi(this);
    setNoteId(-1);
    readLoginWidgetQss();
}

NoteWidget::~NoteWidget()
{
    fmtList.clear();
    delete ui;
}

void NoteWidget::setNote(QString mtitle, QString mcontent, QString mtime, QString msavePath, int mnoteSize)
{
    title = mtitle;
    content = mcontent;
    time = mtime;
    savePath = msavePath;
    noteSize = mnoteSize;
    ui->headLb->setText(title);
    ui->savePathBt->setText(savePath);
    ui->timeLb->setText(time);
    ui->fileSizeLb->setText(QString("%1").arg(noteSize));
}

int NoteWidget::getNoteId()
{
    return noteId;
}

void NoteWidget::setNoteId(int mnoteId)
{
    noteId = mnoteId;
}

void NoteWidget::setNoteTitle(QString mtitle)
{
    title = mtitle;
    ui->headLb->setText(title);
}

QString NoteWidget::getNoteContent()
{
    return content;
}

void NoteWidget::setNoteContent(QString mcontent)
{
    content = mcontent;
}

QString NoteWidget::getNoteTime()
{
    return time;
}

void NoteWidget::setNoteTime(QString mtime)
{
    time = mtime;
    ui->timeLb->setText(time);
}

QString NoteWidget::getNoteSavePath()
{
    return savePath;
}

void NoteWidget::setNoteSavePath(QString msavePath)
{
    savePath = msavePath;
    ui->savePathBt->setText(savePath);
}

int NoteWidget::getNoteSize()
{
    return noteSize;
}

void NoteWidget::setNoteSize(int mnoteSize)
{
    noteSize = mnoteSize;
    ui->fileSizeLb->setText(QString("%1").arg(noteSize));
}

QList<TextFormat> NoteWidget::getFormatList()
{
    return fmtList;
}

void NoteWidget::formatListAppend(TextFormat fmt)
{
    fmtList.append(fmt);
}

bool NoteWidget::isExistence(TextFormat fmt,bool& isNormal)
{
    isNormal = true;
    auto iter = fmtList.begin();
    while(iter != fmtList.end())
    {
       if(iter->startPos == fmt.startPos && iter->endPos == fmt.endPos)
       {
           if(fmt.fmtlevel == FormatLevel::Bold && (iter->fmtlevel == FormatLevel::bold || iter->fmtlevel == FormatLevel::Nobold))
           {
               isNormal = iter->fmtlevel == FormatLevel::bold ? false : true;
               iter->fmtlevel = isNormal ? FormatLevel::bold : FormatLevel::Nobold;
               return true;
           }
           else if(fmt.fmtlevel == FormatLevel::Italics && (iter->fmtlevel == FormatLevel::italics || iter->fmtlevel == FormatLevel::Noitalics))
           {
               isNormal = iter->fmtlevel == FormatLevel::italics ? false : true;
               iter->fmtlevel = isNormal ? FormatLevel::italics : FormatLevel::Noitalics;
               return true;
           }
           else if(fmt.fmtlevel == FormatLevel::Underline && (iter->fmtlevel == FormatLevel::underline || iter->fmtlevel == FormatLevel::Nounderline))
           {
               isNormal = iter->fmtlevel == FormatLevel::underline ? false : true;
               iter->fmtlevel = isNormal ? FormatLevel::underline : FormatLevel::Nounderline;
               return true;
           }
       }
       iter++;
    }
    return false;
}

QString NoteWidget::getNoteTitle()
{
    return title;
}

void NoteWidget::readLoginWidgetQss()
{
    QFile file(":/Qss/note/default.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = tr(file.readAll());
    this->setStyleSheet(styleSheet);
    file.close();
}
