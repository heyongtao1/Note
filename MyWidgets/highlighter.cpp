
#include "highlighter.h"

//! [0]
Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{

}


void Highlighter::highlightBlock(const QString &text)
{
    if(word_text.isEmpty())return;

    QTextCharFormat myClassFormat;
    myClassFormat.setFontWeight(QFont::Bold);
    myClassFormat.setForeground(Qt::darkMagenta);
    QString pattern = word_text;

    QRegExp expression(pattern);
    int index = text.indexOf(expression);
    while (index >= 0) {
        int length = expression.matchedLength();
        setFormat(index, length, myClassFormat);
        index = text.indexOf(expression, index + length);
    }
}


/*
工程: QTextEdit_Highlight_test
日期: 2021-10-30
作者: DS小龙哥
环境: win10 QT5.12.6 MinGW32
功能: 高亮的文本设置
*/
void Highlighter::SetText(QString text)
{
    word_text=text;
}

