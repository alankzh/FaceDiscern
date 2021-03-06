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
#include "OpencvPackage/opencvcamerawidget.h"
#include "CustomWidget/popwidget.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
private slots:
//    void captureFrameImage(int,QImage);
    void receiveAskCapture();
    //FIXed  传回截图不再使用引用方式
    void receiveCaptureImage(QImage image);//摄像机传回的截图
    void receiveDetectTerran(QList<Terran> terranList);//引擎传回的检测员工

    void errorDispose(QString str);

    void quitApplication(bool);

    void showHintWindow();//提示弹窗
    void closeHintWindow();//关闭提示弹窗
signals:
    void sendImageToFaceEngine(QImage &image);//发送截图到人脸识别引擎
protected:
    virtual void paintEvent(QPaintEvent* event);
private:
    void init();
    QPixmap backgroundPix;

    TitleWidget *titleWidget;
    SystemLogoWidget *systemLogoWidget;
//    CameraShowWidget *cameraShowWidget;
    OpencvCameraWidget *opencvCameraWidget;
    SignBottomWidget *signNumWidget;
    SignGatherWidget *signMessageWidget;

    PopWidget *popWidget;
    bool isNeedPop=false;

    FaceDiscernThreadHelper *faceDiscernThreadHelper;
    HeartThreadHelper *heartBeatThreadHelper;
};

#endif // WIDGET_H
