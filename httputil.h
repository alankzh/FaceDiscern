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
 * ����һ��httpģ��
 * ���������������ź�����д����˷�Ч��
 * ��������ģ����ʱ�Ȱ���httpģ��
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
    QNetworkReply* asynNetReply;//�첽������Ϣ����
};

#endif // HTTPUTIL_H
