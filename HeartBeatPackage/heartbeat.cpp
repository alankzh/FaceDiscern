#include "heartbeat.h"

HeartBeat::HeartBeat(QObject *obj):QObject(obj)
{
    netManager = new QNetworkAccessManager(this);
    QObject::connect(netManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(httpFinishedSlot(QNetworkReply*)));
}

HeartBeat::~HeartBeat(){
    if(netReply!=nullptr){
        netReply->deleteLater();
        netReply=nullptr;
    }
    if(netManager!=nullptr){
        netManager->deleteLater();
        netManager=nullptr;
    }
}

/**
 * @brief HeartBeat::beat
 * ������һ����ʼ��������5s���ٴε���
 * ������ֹ��������Ϣ��
 */
void HeartBeat::beat(){
    QNetworkRequest request;
    request.setUrl(HEART_BEAT_URL);

    netReply=netManager->get(request);
//    connect(netReply, SIGNAL(readyRead()), this, SLOT(readFromResponse()));//׼����ȡ��Ȼ�󲢲��ܶ���ȫ������

    QEventLoop loop;
    QTimer timer;

    QObject::connect(netReply,SIGNAL(finished()),&loop,SLOT(quit()));
    QObject::connect(&timer,SIGNAL(timeout()),&loop,SLOT(quit()));
    timer.start(5000);
    loop.exec();//�¼�ѭ������������loop��quit()�����ã�����.exec()�Ժ�Ĵ��뽫��������

    //ʵ�ⷢ�֣���ʱδ��ʱ��timer����active��
    if (timer.isActive()){
        //δ��ʱ  ʵ���ʼ���������߼���
        timer.stop();
        netReply->abort();
        netReply->deleteLater();

    } else {
        //��ʱ
        netReply->abort();
        netReply->deleteLater();

    }
}

/**
 * @brief HeartBeat::httpFinishedSlot
 * һ��http���ӽ����ĵ���
 * @param reply
 */
void HeartBeat::httpFinishedSlot(QNetworkReply *reply)
{
    qDebug()<<"HeartBeat::httpFinishedSlot";
    if (reply->error() == QNetworkReply::NoError){
        //������Ϻ��ܶ�ȡ��ȫ������
        QByteArray byteArray=netReply->readAll();
        QString readStr=QString::fromUtf8(byteArray);

        emit receiveResponseStr(readStr);
    }
    else{
//        qDebug()<<"handle errors here";
        QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        //statusCodeV��HTTP����������Ӧ�룬reply->error()��Qt����Ĵ����룬���Բο�QT���ĵ�
        qDebug( "found error ....code: %d %d\n", statusCodeV.toInt(), (int)reply->error());
        //statusCodeV.toInt()Ϊ0ʱ��ʾ�������
        qDebug(qPrintable(reply->errorString()));

        //֪ͨ�������
        errorCatch(statusCodeV.toInt());
    }
    reply->deleteLater();
    QTimer::singleShot(BEAT_CIRCLE,this,SLOT(beat()));
}


/**
 * @brief HeartBeat::errorCatch
 * ������
 * Ŀǰֻ�ǽ�http������ֱ���׳�
 */
void HeartBeat::errorCatch(int errorCode){
    emit httpError(errorCode);
}
