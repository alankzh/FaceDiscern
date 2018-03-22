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
    void receiveCaptureImage(QImage &image);//��������صĽ�ͼ
    void receiveDetectTerran(QList<Terran> terranList);//���洫�صļ��Ա��

    void errorDispose(QString str);

    void quitApplication(bool);
signals:
    void sendImageToFaceEngine(QImage &image);//���ͽ�ͼ������ʶ������
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
