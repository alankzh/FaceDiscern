#ifndef HEARTTHREADHELPER_H
#define HEARTTHREADHELPER_H

#include <QObject>
#include "heartbeat.h"
#include <QList>
#include "terran.h"
#include <QThread>
#include <QDebug>
#include "rapidjson/document.h"     //������Ҷ�����rapidjson���Ƚ����ã�����include��һ��
#include "rapidjson/prettywriter.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/stringbuffer.h"
#include <string>
#include <QString>
#include "DBC/sqldatabase.h"
#include "constant.h"

/**
 * @brief The HeartThreadHelper class
 * �����߳̿�����
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
    void beginThread();//�߳̿�ʼ
    void endThread();//�߳̽���

    void httpConnectionBreak();//����������ӶϿ�
    void httpServerError(QString errorHint);//�������һ��Ϊ���ǵķ���������

    void insertDBsynchronized(QList<Terran> insertList);//�������ϵ���������ͬ�����
    void deleteDBsynchronized(QList<Terran> deleteList);//�������ϵ�ɾ������ͬ�����
    void updateDBsynchronized(QList<Terran> updateList);//�������ϵĸ�������ͬ�����
private slots:
    void environmentInit();
    void receiveTerrans(QString &str);
    void httpErrorInHeart(int errorCode);
private:
    void synchronizationData(QList<Terran> &listFromServer);
    QThread *thread;
    HeartBeat *heartbeat=nullptr;
    QList<Terran> terranList;//�������Ա����
};

#endif // HEARTTHREADHELPER_H
