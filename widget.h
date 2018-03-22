#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QDebug>
#include "ArcFacePackage/facediscernthreadhelper.h"
#include "HeartBeatPackage/heartthreadhelper.h"
#include "CustomWidget/titlewidget.h"
#include "CustomWidget/systemlogowidget.h"
#include "CustomWidget/camerashowwidget.h"
#include "CustomWidget/signbottomwidget.h"
#include "CustomWidget/signgatherwidget.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
private slots:
//    void captureFrameImage(int,QImage);
    void receiveAskCapture();
    void receiveCaptureImage(QImage &image);//摄像机传回的截图
    void receiveDetectTerran(QList<Terran> terranList);//引擎传回的检测员工

    void errorDispose(QString str);

    void quitApplication(bool);
signals:
    void sendImageToFaceEngine(QImage &image);//发送截图到人脸识别引擎
protected:
    virtual void paintEvent(QPaintEvent* event);
private:
    void init();
    QPixmap backgroundPix;

    TitleWidget *titleWidget;
    SystemLogoWidget *systemLogoWidget;
    CameraShowWidget *cameraShowWidget;
    SignBottomWidget *signNumWidget;
    SignGatherWidget *signMessageWidget;

    FaceDiscernThreadHelper *faceDiscernThreadHelper;
    HeartThreadHelper *heartBeatThreadHelper;
};

#endif // WIDGET_H
