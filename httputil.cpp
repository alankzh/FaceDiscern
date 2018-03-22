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
 * ͬ����ʽ����ͼƬ����ʱʱ��4000s
 * �ǵ�ɾ�����ص�QImageָ��
 * @param url
 * @return
 */
QImage* HttpUtil::downLoadPic(QString url){
    QImage* image=nullptr;

    QNetworkRequest request;
    request.setUrl(url);

    QNetworkReply* netReply=netManager->get(request);
    //    connect(netReply, SIGNAL(readyRead()), this, SLOT(readFromResponse()));//׼����ȡ��Ȼ�󲢲��ܶ���ȫ������

    QEventLoop loop;
    QTimer timer;

    QObject::connect(netReply,SIGNAL(finished()),&loop,SLOT(quit()));
    QObject::connect(&timer,SIGNAL(timeout()),&loop,SLOT(quit()));
    timer.start(10000);
    loop.exec();//�¼�ѭ������������loop��quit()�����ã�����.exec()�Ժ�Ĵ��뽫��������

    //ʵ�ⷢ�֣���ʱδ��ʱ��timer����active��
    if (timer.isActive()){
        //δ��ʱ  ʵ���ʼ���������߼���
        timer.stop();
    } else {
        //��ʱ
        netReply->abort();
        netReply->deleteLater();
    }

    if (netReply->error() == QNetworkReply::NoError){
        //������Ϻ��ܶ�ȡ��ȫ������
        image=new QImage();
        image->loadFromData(netReply->readAll());
    }else{
        //        qDebug()<<"handle errors here";
        QVariant statusCodeV = netReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        //statusCodeV��HTTP����������Ӧ�룬reply->error()��Qt����Ĵ����룬���Բο�QT���ĵ�
        qDebug( "found error ....code: %d %d\n", statusCodeV.toInt(), (int)netReply->error());
        //statusCodeV.toInt()Ϊ0ʱ��ʾ�������
        qDebug()<<QString::fromLocal8Bit("����ͼƬ����,������ϢΪ��")<<netReply->errorString();
    }

    netReply->abort();
    netReply->deleteLater();
    return image;
}

/**
 * @brief HttpUtil::downMessage
 * ͬ����ʽ������Ϣ
 * �ǵ�ɾ�����ص���Ϣָ��
 * @param url
 * @return
 */
QString* HttpUtil::downMessage(QString url){
    QString* message=nullptr;

    QNetworkRequest request;
    request.setUrl(url);

    QNetworkReply* netReply=netManager->get(request);
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
    } else {
        //��ʱ
        netReply->abort();
        netReply->deleteLater();
    }

    if (netReply->error() == QNetworkReply::NoError){
        //������Ϻ��ܶ�ȡ��ȫ������
        message=new QString(QString::fromUtf8(netReply->readAll()));
    }else{
        //        qDebug()<<"handle errors here";
        QVariant statusCodeV = netReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        //statusCodeV��HTTP����������Ӧ�룬reply->error()��Qt����Ĵ����룬���Բο�QT���ĵ�
        qDebug( "found error ....code: %d %d\n", statusCodeV.toInt(), (int)netReply->error());
        //statusCodeV.toInt()Ϊ0ʱ��ʾ�������
        qDebug()<<QString::fromLocal8Bit("������Ϣ����,������ϢΪ��")<<netReply->errorString();

    }

    netReply->abort();
    netReply->deleteLater();

    return message;
}

/**
 * @brief HttpUtil::sendMessage
 * �첽��ʽ������Ϣ
 */
void HttpUtil::sendMessage(QString url){
    QNetworkRequest request;
    request.setUrl(url);

    asynNetReply=netManager->get(request);
    QObject::connect(asynNetReply,SIGNAL(finished()),this,SLOT(asynHttpFinished()));
    //    connect(netReply, SIGNAL(readyRead()), this, SLOT(readFromResponse()));//׼����ȡ��Ȼ�󲢲��ܶ���ȫ������
}

void HttpUtil::asynHttpFinished(){
//    qDebug()<<"HttpUtil::asynHttpFinished";
    if (asynNetReply->error() == QNetworkReply::NoError){
        //������Ϻ��ܶ�ȡ��ȫ������
        QByteArray byteArray=asynNetReply->readAll();
        QString readStr=QString::fromUtf8(byteArray);
    }
    else{
        //        qDebug()<<"handle errors here";
        QVariant statusCodeV = asynNetReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        //statusCodeV��HTTP����������Ӧ�룬reply->error()��Qt����Ĵ����룬���Բο�QT���ĵ�
        qDebug( "asyn http get error ....code: %d %d\n", statusCodeV.toInt(), (int)asynNetReply->error());
        //statusCodeV.toInt()Ϊ0ʱ��ʾ�������
        qDebug(qPrintable(asynNetReply->errorString()));
    }
    asynNetReply->abort();
    delete asynNetReply;
    asynNetReply=nullptr;
}
