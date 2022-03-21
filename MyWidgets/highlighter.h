
#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

//! [0]
class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = 0);
    void SetText(QString text);
protected:
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

    QString word_text;
};
//! [0]

#endif // HIGHLIGHTER_H
