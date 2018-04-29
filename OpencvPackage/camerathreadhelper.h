#ifndef CAMERATHREADHELPER_H
#define CAMERATHREADHELPER_H

#include <QtWidgets>
#include <QtGui>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <imgproc/imgproc.hpp>
#include "cxcore.h"


class CameraThreadHelper:public QObject
{
    Q_OBJECT
public:
    CameraThreadHelper(QObject *parent=nullptr);
    ~CameraThreadHelper();
    void startThread();
public slots:
    void captureFrame();
signals:
    void beginThread();//线程开始
    void endThread();//线程结束
    void environmentComplete();
    void sendFrame(QImage image);//发送一帧图片

    void cameraError();//摄像机有误
private slots:
    void environmentInit();
private:
    QImage MatToQImage(cv::Mat mtx);
    cv::VideoCapture cap;
    cv::Mat frame;

    QThread *thread;
};

#endif // CAMERATHREADHELPER_H
