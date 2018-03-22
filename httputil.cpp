#include "httputil.h"

HttpUtil::HttpUtil(QObject *obj):QObject(obj)
{
    netManager = new QNetworkAccessManager(this);
}

HttpUtil::~HttpUtil(){
    if(netManager!=nullptr){
        netManager->deleteLater();
        netManager=nullptr;
    }
    if(asynNetReply!=nullptr){
        asynNetReply->deleteLater();
        asynNetReply=nullptr;
    }
}

/**
 * @brief HttpUtil::downLoadPic
 * 同步方式下载图片，超时时间4000s
 * 记得删除返回的QImage指针
 * @param url
 * @return
 */
QImage* HttpUtil::downLoadPic(QString url){
    QImage* image=nullptr;

    QNetworkRequest request;
    request.setUrl(url);

    QNetworkReply* netReply=netManager->get(request);
    //    connect(netReply, SIGNAL(readyRead()), this, SLOT(readFromResponse()));//准备读取，然后并不能读到全部数据

    QEventLoop loop;
    QTimer timer;

    QObject::connect(netReply,SIGNAL(finished()),&loop,SLOT(quit()));
    QObject::connect(&timer,SIGNAL(timeout()),&loop,SLOT(quit()));
    timer.start(10000);
    loop.exec();//事件循环阻塞，除非loop的quit()被调用，否则.exec()以后的代码将不被调用

    //实测发现，超时未超时，timer都是active的
    if (timer.isActive()){
        //未超时  实测会始终走这条逻辑线
        timer.stop();
    } else {
        //超时
        netReply->abort();
        netReply->deleteLater();
    }

    if (netReply->error() == QNetworkReply::NoError){
        //链接完毕后能读取到全部数据
        image=new QImage();
        image->loadFromData(netReply->readAll());
    }else{
        //        qDebug()<<"handle errors here";
        QVariant statusCodeV = netReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        //statusCodeV是HTTP服务器的相应码，reply->error()是Qt定义的错误码，可以参考QT的文档
        qDebug( "found error ....code: %d %d\n", statusCodeV.toInt(), (int)netReply->error());
        //statusCodeV.toInt()为0时表示网络故障
        qDebug()<<QString::fromLocal8Bit("下载图片错误,错误信息为：")<<netReply->errorString();
    }

    netReply->abort();
    netReply->deleteLater();
    return image;
}

/**
 * @brief HttpUtil::downMessage
 * 同步方式请求消息
 * 记得删除返回的消息指针
 * @param url
 * @return
 */
QString* HttpUtil::downMessage(QString url){
    QString* message=nullptr;

    QNetworkRequest request;
    request.setUrl(url);

    QNetworkReply* netReply=netManager->get(request);
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
    } else {
        //超时
        netReply->abort();
        netReply->deleteLater();
    }

    if (netReply->error() == QNetworkReply::NoError){
        //链接完毕后能读取到全部数据
        message=new QString(QString::fromUtf8(netReply->readAll()));
    }else{
        //        qDebug()<<"handle errors here";
        QVariant statusCodeV = netReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        //statusCodeV是HTTP服务器的相应码，reply->error()是Qt定义的错误码，可以参考QT的文档
        qDebug( "found error ....code: %d %d\n", statusCodeV.toInt(), (int)netReply->error());
        //statusCodeV.toInt()为0时表示网络故障
        qDebug()<<QString::fromLocal8Bit("下载消息错误,错误信息为：")<<netReply->errorString();

    }

    netReply->abort();
    netReply->deleteLater();

    return message;
}

/**
 * @brief HttpUtil::sendMessage
 * 异步方式发送消息
 */
void HttpUtil::sendMessage(QString url){
    QNetworkRequest request;
    request.setUrl(url);

    asynNetReply=netManager->get(request);
    QObject::connect(asynNetReply,SIGNAL(finished()),this,SLOT(asynHttpFinished()));
    //    connect(netReply, SIGNAL(readyRead()), this, SLOT(readFromResponse()));//准备读取，然后并不能读到全部数据
}

void HttpUtil::asynHttpFinished(){
//    qDebug()<<"HttpUtil::asynHttpFinished";
    if (asynNetReply->error() == QNetworkReply::NoError){
        //链接完毕后能读取到全部数据
        QByteArray byteArray=asynNetReply->readAll();
        QString readStr=QString::fromUtf8(byteArray);
    }
    else{
        //        qDebug()<<"handle errors here";
        QVariant statusCodeV = asynNetReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        //statusCodeV是HTTP服务器的相应码，reply->error()是Qt定义的错误码，可以参考QT的文档
        qDebug( "asyn http get error ....code: %d %d\n", statusCodeV.toInt(), (int)asynNetReply->error());
        //statusCodeV.toInt()为0时表示网络故障
        qDebug(qPrintable(asynNetReply->errorString()));
    }
    asynNetReply->abort();
    delete asynNetReply;
    asynNetReply=nullptr;
}
