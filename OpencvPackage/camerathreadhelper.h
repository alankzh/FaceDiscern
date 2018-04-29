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
    void beginThread();//�߳̿�ʼ
    void endThread();//�߳̽���
    void environmentComplete();
    void sendFrame(QImage image);//����һ֡ͼƬ

    void cameraError();//���������
private slots:
    void environmentInit();
private:
    QImage MatToQImage(cv::Mat mtx);
    cv::VideoCapture cap;
    cv::Mat frame;

    QThread *thread;
};

#endif // CAMERATHREADHELPER_H
