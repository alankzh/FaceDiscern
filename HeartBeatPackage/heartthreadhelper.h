#ifndef HEARTTHREADHELPER_H
#define HEARTTHREADHELPER_H

#include <QObject>
#include "heartbeat.h"
#include <QList>
#include "terran.h"
#include <QThread>
#include <QDebug>
#include "rapidjson/document.h"     //引用自叶劲峰的rapidjson，比较难用，所以include了一堆
#include "rapidjson/prettywriter.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/stringbuffer.h"
#include <string>
#include <QString>
#include "DBC/sqldatabase.h"
#include "constant.h"
#include "httputil.h"

/**
 * @brief The HeartThreadHelper class
 * 心跳线程开启类
 * @author alankzh
 */
class HeartThreadHelper:public QObject
{
    Q_OBJECT
public:
    HeartThreadHelper(QObject *obj=0);
    ~HeartThreadHelper();
    void startThread();
public slots:

signals:
    void beginThread();//线程开始
    void endThread();//线程结束

    void httpConnectionAlive(bool alive);//与服务器连接是断开还是连接上了
    void httpConnectionDelay(int ms);//与服务器连接延迟

    void httpServerError(QString errorHint);//服务错误，一般为我们的服务器关了

    void insertDBsynchronized(QList<Terran> insertList);//服务器上的新增数据同步完毕
    void deleteDBsynchronized(QList<Terran> deleteList);//服务器上的删除数据同步完毕
    void updateDBsynchronized(QList<Terran> updateList);//服务器上的更新数据同步完毕
private slots:
    void environmentInit();
    void receiveTerrans(QString &str);
    void httpErrorInHeart(int errorCode);

    void heartBeatDelay(int mesc);
private:
    void synchronizationData(QList<Terran> &listFromServer);

    void downAndInsertImageToDB(QList<Terran> &list);
    QThread *thread;
    HeartBeat *heartbeat=nullptr;
    QList<Terran> terranList;//缓存的人员数据
    HttpUtil *httpUtil;
};

#endif // HEARTTHREADHELPER_H
