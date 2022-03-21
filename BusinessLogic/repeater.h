#ifndef REPEATER_H
#define REPEATER_H

#include "json.h"
#include "NetSocket/mysocket.h"
#include "Util/Singleton.h"
class Repeater
{
private:
    Repeater() = default;
    ~Repeater() = default;
public:
    //转发登录信息至服务器
    void sendLoginJson(const QString& username,const QString& password);
    //转发创建笔记的信息至服务器
    void sendCreateNoteJson(const QString& content,const QString& title,const QString& time);
    //转发加载用户所有笔记的信息至服务器
    void sendLoadUserAllNoteJson();
    //转发保存后的笔记的信息至服务器
    void sendUpdateNoteJson(const int noteId,const QString& content,const QString& title,const QString& time);
    friend class Singleton<Repeater>;
};

#endif // REPEATER_H
