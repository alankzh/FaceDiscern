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
 * ����ʵ����
 * ÿ��5�����������ѯ���Ƿ���Ҫ�����������ݿ�
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
private slots:
    void httpFinishedSlot(QNetworkReply *reply);
private:
    void errorCatch(int errorCode);
    QNetworkAccessManager *netManager;
    QNetworkReply *netReply;
};

#endif // HEARTBEAT_H
