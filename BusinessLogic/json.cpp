#include "json.h"

const QJsonDocument Json::createLoginJson(const QString& username,const QString& password)
{
    QJsonObject object;
    QJsonObject data_obj;
    data_obj.insert("username",username);
    data_obj.insert("password",password);

    object.insert("data",data_obj);
    object.insert("type",LoginMagType::LOGIN_MAG);

    QJsonDocument document;
    document.setObject(object);
    return document;
}

const QJsonDocument Json::releaseNewNoteJson(const QString &content, const QString &title,const QString &time)
{
    QJsonObject object;
    QJsonObject data_obj;
    data_obj.insert("userid",MyApp::userid);
    data_obj.insert("blogtext",content);
    data_obj.insert("bloghead",title);
    data_obj.insert("pubtime",time);

    object.insert("data",data_obj);
    object.insert("type",BlogMagType::BLOG_MAG);

    QJsonDocument document;
    document.setObject(object);
    return document;
}


const QJsonDocument Json::loadUserAllNoteJson()
{
    QJsonObject object;
    QJsonObject data_obj;
    data_obj.insert("userid",MyApp::userid);

    object.insert("data",data_obj);
    object.insert("type",LoadBlogMagType::LOADBLOG_MAG);

    QJsonDocument document;
    document.setObject(object);
    return document;
}

const QJsonDocument Json::updateNoteJson(const int noteId,const QString& content,const QString& title,const QString& time)
{
    QJsonObject object;
    QJsonObject data_obj;
    data_obj.insert("userid",MyApp::userid);
    data_obj.insert("noteid",noteId);
    data_obj.insert("blogtext",content);
    data_obj.insert("bloghead",title);
    data_obj.insert("pubtime",time);

    object.insert("data",data_obj);
    object.insert("type",UpdateNoteType::UPDATE_NOTE_MAG);

    QJsonDocument document;
    document.setObject(object);
    return document;
}
