#ifndef FACEDISCERNTHREADHELPER_H
#define FACEDISCERNTHREADHELPER_H

#include <QObject>
#include "facediscern.h"
#include <QThread>
#include <QTimer>
#include <QImage>
#include "terran.h"
#include "DBC/sqldatabase.h"
#include "constant.h"
#include "terranfacefeature.h"
#include <QList>
#include "httputil.h"
#include <QUrl>


class FaceDiscernThreadHelper:public QObject
{
    Q_OBJECT
public:
    FaceDiscernThreadHelper(QObject *obj=nullptr);
    ~FaceDiscernThreadHelper();
    void startThread();
signals:
    void endThread();//�������߳�
    void beginThread();//��ʼ�߳��ź�
    void askCapture();//�����ͼ
    void sendFaceImage(QImage image);//���ͼ�⵽��������ͼ
    void sendTerran(QList<Terran> terranlist);//���ͼ�⵽����

    void error(QString errorHint);//���ʹ�����ʾ��ʹ������źű�ʾ�������ص������˳�����

    void loadDate();
    void loadDateDown();
public slots:
    void receiveCaptureImage(QImage &image);

    void receiveUpdateFaceData(QList<Terran> list);
    void receiveDeleteFaceData(QList<Terran> list);
    void receiveInsertFaceData(QList<Terran> list);
private slots:
    void environmentInit();
    void errorCatch(int errorCode,FaceDiscern::FailedType failedType);
    void requestScreenShot();
private :
    QThread *thread;
    FaceDiscern *faceDiscern;
    QList<TerranFaceFeature> featureList={};//�����Ա�����������б�
    QList<Terran> visitorTerranList={};//����ÿͣ�ÿһ�μ�������ⲻ����������һ��
    QList<TerranFaceFeature> visitorFeatureList={};//����ÿ���������

    void loadPicFeatureFromDb(QList<Terran> &terranList);
};

#endif // FACEDISCERNTHREADHELPER_H
