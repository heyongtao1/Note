#ifndef MYAPP_H
#define MYAPP_H
#include <QObject>

typedef struct _Package{
    char confrim[3] = {0};
    int total_len;//包的总长度
    char *data;//包的数据
}Package;

typedef struct RecvMemory{
    char *recvData = NULL;
    int totalLen = 51200;//总容量
    int usedLen = 0;//已使用容量
    int expandSeq = 1;//扩容序号
}recvMemory;

typedef struct blogData{
    int userid;
    int blogEditid;
    QString blogText;
    QString blogHead;
    QString publtime;
}blogSqlData;

class MyApp
{
public:
    MyApp();

public:
    static QString m_strHostAddr;
    static qint32  m_nTcpMsgPort;
    static qint32  m_nFtpMsgPort;
    static qint32  userid;
    static QString userHead;
};
typedef enum {
    KEEPLIVE_MAG = 0x00,
    KEEPLIVE_SUCCESS = 0x01,
    KEEPLIVE_FAIL = 0X02
}KeepLiveType;

typedef enum {
    LOGIN_MAG = 0x10,
    LOGIN_SUCCESS = 0x11,
    LOGIN_FAIL = 0X12
}LoginMagType;

typedef enum {
    REGIST_MAG = 0x20,
    REGIST_SUCCESS = 0x21,
    REGIST_FAIL = 0X22
}RegistMagType;

typedef enum {
    BLOG_MAG = 0x30,
    BLOG_SUCCESS = 0x31,
    BLOG_FAIL = 0x32
}BlogMagType;

typedef enum {
    LOADBLOG_MAG = 0x40,
    LOADBLOG_SUCCESS = 0x41,
    LOADBLOG_FAIL = 0x42
}LoadBlogMagType;

typedef enum{
    FINDBLOG_MAG = 0X50,
    FINDBLOG_SUCCESS = 0X51,
    FINDBLOG_FAIL = 0X52
}FindBlogMagType;

typedef enum{
    RECOMBLOG_MAG = 0X60,
    RECOMBLOG_SUCCESS = 0X61,
    RECOMBLOG_FAIL = 0X62
}RecomBlogMagType;

typedef enum{
    FILE_MAG = 0X70,
    FILE_SUCCESS = 0X71,
    FILE_FAIL = 0X72
}FileMsgTyPe;
//资源类型
typedef enum{
    FILE_TYPE_PICTURE = 0X80,
    FILE_TYPE_TXT = 0X81,
    FILE_TYPE_MUSIC = 0X82,
    FILE_TYPE_VIDEO = 0X83,
    FILE_TYPE_OTHERFILE = 0X84
}FileType;

typedef enum{
    FILE_DOWNLOAD_MAG = 0X90,
    FILE_DOWNLOAD_SUCCESS = 0X91,
    FILE_DOWNLOAD_FAIL = 0X92
}FILEDownLoadType;

typedef enum{
    UPDATE_NOTE_MAG = 0XA0,
    UPDATE_NOTE_SUCCESS = 0XA1,
    UPDATE_NOTE_FAIL = 0XA2
}UpdateNoteType;
#endif // MYAPP_H
