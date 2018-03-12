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
 * 颤动，一旦开始，将会间隔5s后再次调用
 * 心跳不止，生无所息！
 */
void HeartBeat::beat(){
    QNetworkRequest request;
    request.setUrl(HEART_BEAT_URL);

    netReply=netManager->get(request);
//    connect(netReply, SIGNAL(readyRead()), this, SLOT(readFromResponse()));//准备读取，然后并不能读到全部数据

    QEventLoop loop;
    QTimer timer;

    QObject::connect(netReply,SIGNAL(finished()),&loop,SLOT(quit()));
    QObject::connect(&timer,SIGNAL(timeout()),&loop,SLOT(quit()));
    timer.start(5000);
    loop.exec();//事件循环阻塞，除非loop的quit()被调用，否则.exec()以后的代码将不被调用

    //实测发现，超时未超时，timer都是active的
    if (timer.isActive()){
        //未超时  实测会始终走这条逻辑线
        timer.stop();
        netReply->abort();
        netReply->deleteLater();

    } else {
        //超时
        netReply->abort();
        netReply->deleteLater();

    }
}

/**
 * @brief HeartBeat::httpFinishedSlot
 * 一次http连接结束的调用
 * @param reply
 */
void HeartBeat::httpFinishedSlot(QNetworkReply *reply)
{
    qDebug()<<"HeartBeat::httpFinishedSlot";
    if (reply->error() == QNetworkReply::NoError){
        //链接完毕后能读取到全部数据
        QByteArray byteArray=netReply->readAll();
        QString readStr=QString::fromUtf8(byteArray);

        emit receiveResponseStr(readStr);
    }
    else{
//        qDebug()<<"handle errors here";
        QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        //statusCodeV是HTTP服务器的相应码，reply->error()是Qt定义的错误码，可以参考QT的文档
        qDebug( "found error ....code: %d %d\n", statusCodeV.toInt(), (int)reply->error());
        //statusCodeV.toInt()为0时表示网络故障
        qDebug(qPrintable(reply->errorString()));

        //通知网络错误
        errorCatch(statusCodeV.toInt());
    }
    reply->deleteLater();
    QTimer::singleShot(BEAT_CIRCLE,this,SLOT(beat()));
}


/**
 * @brief HeartBeat::errorCatch
 * 错误处理
 * 目前只是将http错误码直接抛出
 */
void HeartBeat::errorCatch(int errorCode){
    emit httpError(errorCode);
}
