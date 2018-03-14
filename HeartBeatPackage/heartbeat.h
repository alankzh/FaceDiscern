#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include <QObject>
#include <QString>
#include <QtNetwork>
#include "constant.h"
#include <QTimer>
#include <QEventLoop>
#include <QUrl>

/**
 * @brief The HeartBeat class
 * 心跳实现类
 * 每过5秒向服务器征询，是否需要更新人脸数据库
 */
class HeartBeat:public QObject
{
    Q_OBJECT
public:
    HeartBeat(QObject *obj=0);
    ~HeartBeat();
public slots:
    void beat();
signals:
    void httpError(int httpErrorCode);
    void receiveResponseStr(QString &str);
    void httpConnectionDelay(int msec);
private slots:
    void httpFinishedSlot(QNetworkReply *reply);
private:
    void errorCatch(int errorCode);
    QNetworkAccessManager *netManager;
    QNetworkReply *netReply;

    int httpRequestMsecToStartTime;//从一天开始时间 到最近一次http请求的毫秒数  每天的00：00 可能会显示有问题
};

#endif // HEARTBEAT_H
