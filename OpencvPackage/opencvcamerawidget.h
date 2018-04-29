#ifndef OPENCVCAMERAWIDGET_H
#define OPENCVCAMERAWIDGET_H

#include <QtWidgets>
#include <QtGui>
#include <QPixmap>
#include "terran.h"
#include "camerathreadhelper.h"
#include "httputil.h"
#include "DBC/sqldatabase.h"
#include "constant.h"
#include "opencvcameraviewwidget.h"

class OpencvCameraWidget:public QWidget
{
    Q_OBJECT
public:
    OpencvCameraWidget(QWidget *parent=nullptr);
    ~OpencvCameraWidget();
    const int width=1152;//640*1.8  (640,480)为opencv默认截图大小
    const int height=864;//480*1.8

    void needDiscernFromCamera();
public slots:
    void onTerranEnter(QList<Terran> listFromEngine);
    void receiveFrame(QImage image);
    void clearSignedTerranCache();
signals:
    void askCameraNextFrame();//向相机线程请求下一帧
    void sendCaptureImage(QImage image);

    void newTerranSign(Terran terran);
protected:
    virtual void paintEvent(QPaintEvent *event);
private slots:
    void nextFrame();
    void cameraErrorCatch();
private:
    void insertTerran(QList<Terran> &insertList);
    bool needDiscern;
    bool cameraErrored;
    QPixmap backgroundPix;
    CameraThreadHelper *cameraThread;
    OpencvCameraViewWidget *cameraViewWidget;

    QList<Terran> inCameraTerranList={};
    QList<Terran> signedList={};//已经签到的人，5分钟清空一次

    HttpUtil *httpUtil;
};

#endif // OPENCVCAMERAWIDGET_H
