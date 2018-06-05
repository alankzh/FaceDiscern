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
    void endThread();//结束本线程
    void beginThread();//开始线程信号
    void askCapture();//请求截图
    void sendFaceImage(QImage image);//发送检测到的人脸截图
    void sendTerran(QList<Terran> terranlist);//发送检测到的人

    void error(QString errorHint);//发送错误提示，使用这个信号表示错误严重到必须退出程序

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
    QList<TerranFaceFeature> featureList={};//缓存的员工人脸特征列表
    QList<Terran> visitorTerranList={};//缓存访客，每一次检测如果检测不到，则清理一次
    QList<TerranFaceFeature> visitorFeatureList={};//缓存访客人脸特征

    void loadPicFeatureFromDb(QList<Terran> &terranList);
};

#endif // FACEDISCERNTHREADHELPER_H
