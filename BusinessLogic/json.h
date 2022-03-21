#ifndef JSON_H
#define JSON_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <memory>
#include "Util/myapp.h"
#include "Util/Singleton.h"
class Json
{
private:
    Json() = default;
    ~Json() = default;
public:
    const QJsonDocument createLoginJson(const QString& username,const QString& password);
    const QJsonDocument releaseNewNoteJson(const QString& content,const QString& title,const QString& time);
    const QJsonDocument loadUserAllNoteJson();
    const QJsonDocument updateNoteJson(const int noteId,const QString& content,const QString& title,const QString& time);
    friend class Singleton<Json>;
};

#endif // JSON_H
