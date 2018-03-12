#ifndef HTTPUTIL_H
#define HTTPUTIL_H
#include <QObject>
#include <QtNetwork>
#include <QByteArray>
#include <QImage>
#include <QEventLoop>
#include <QTimer>
#include <QString>
#include <QUrl>

/**
 * @brief The HttpUtil class
 * 这是一个http模块
 * 后来觉得与其在信号与槽中传递浪费效率
 * 不如其他模块暂时先包含http模块
 */
class HttpUtil:public QObject
{
    Q_OBJECT
public:
    HttpUtil(QObject *obj=nullptr);
    ~HttpUtil();
    QImage* downLoadPic(QString url);
    QString* downMessage(QString url);
    void sendMessage(QString url);
signals:
    void httpFinished(QByteArray &byteArray);
    void httpError(int errorCode);
private slots:
    void asynHttpFinished();
private:
    QNetworkAccessManager *netManager;
    QNetworkReply* asynNetReply;//异步发送消息所需
};

#endif // HTTPUTIL_H
