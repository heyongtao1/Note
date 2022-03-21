#include "repeater.h"

void Repeater::sendLoginJson(const QString& username,const QString& password)
{
    MySocket::getTcpSocket()->sendData(Singleton<Json>::getInstance().createLoginJson(username,password));
}

void Repeater::sendCreateNoteJson(const QString& content,const QString& title,const QString& time)
{
    MySocket::getTcpSocket()->sendData(Singleton<Json>::getInstance().releaseNewNoteJson(content,title,time));
}

void Repeater::sendLoadUserAllNoteJson()
{
    MySocket::getTcpSocket()->sendData(Singleton<Json>::getInstance().loadUserAllNoteJson());
}

void Repeater::sendUpdateNoteJson(const int noteId,const QString& content,const QString& title,const QString& time)
{
    MySocket::getTcpSocket()->sendData(Singleton<Json>::getInstance().updateNoteJson(noteId,content,title,time));
}
