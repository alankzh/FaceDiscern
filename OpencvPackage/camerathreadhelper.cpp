#include "camerathreadhelper.h"

using namespace cv;
using namespace std;
CameraThreadHelper::CameraThreadHelper(QObject *parent):QObject(parent){
}

CameraThreadHelper::~CameraThreadHelper(){
    emit endThread();
    if(cap.isOpened()){
        qDebug()<<"capture is release";
        cap.release();
    }
}

void CameraThreadHelper::startThread(){
    thread=new QThread();
    this->moveToThread(thread);
    connect(thread,&QThread::finished,thread,&QObject::deleteLater);//线程结束时删除线程
    connect(thread,&QThread::finished,this,&QObject::deleteLater);//线程结束时删除此对象
    connect(this,SIGNAL(endThread()),thread,SLOT(quit()));//endThread()信号使线程结束

    connect(this,SIGNAL(beginThread()),this,SLOT(environmentInit()));//beginThread()信号让线程正式开始
    thread->start();

    emit beginThread();//必须以发信号的方式来正式让函数调用进入子线程中，而不是直接调用函数
}

void CameraThreadHelper::environmentInit(){
    qDebug()<<"CameraThreadHelper::environmentInit";
    cap.open(0);

    while(!cap.isOpened()) //if not successful then exit
    {
        qDebug() << "Cannot open webcam";
        emit cameraError();//没有摄像头
        QEventLoop loop;
        QTimer timer;

        QObject::connect(&timer,SIGNAL(timeout()),&loop,SLOT(quit()));
        timer.start(5000);
        loop.exec();//事件循环阻塞，除非loop的quit()被调用，否则.exec()以后的代码将不被调用

        //实测发现，超时未超时，timer都是active的
        if (timer.isActive()){
            //未超时  实测会始终走这条逻辑线
            timer.stop();
            cap.open(0);
        } else {
            //超时
            cap.open(0);
        }
    }
    /*分辨率*/
    //    cap.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
    //    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
    cap.set(CV_CAP_PROP_FOURCC, CV_FOURCC('M', 'J', 'P', 'G'));
    cap.set(CV_CAP_PROP_FPS, 30);
    double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get width of frames of video
    double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get height of frames of video

    qDebug() << "Frame size = " << dWidth << "x" << dHeight << endl;

    emit environmentComplete();
}

/**
 * @brief CameraThreadHelper::captureFrame
 * 捕获一帧相机图像
 */
void CameraThreadHelper::captureFrame(){
    bool success=cap.read(frame);
    if(!success){
        qDebug() << "Cannot read frame from video file" << endl;
        emit cameraError();
        return;
    }
    emit sendFrame(MatToQImage(frame));
}

/**
 * @brief CameraThreadHelper::MatToQImage
 * cv::mat 换为QImage
 * @param mtx
 * @return
 */
QImage CameraThreadHelper::MatToQImage(cv::Mat &dst){
    Mat mtx;//参考系
    flip(dst, mtx,1);//1 水平反转 （左右）
    switch (mtx.type()){
    case CV_8UC1:{
        QImage img((const unsigned char *)(mtx.data), mtx.cols, mtx.rows, mtx.cols, QImage::Format_Grayscale8);
        dst.release();
//        mtx.release();
        return img;
    }
        break;
    case CV_8UC3:{
        QImage img((const unsigned char *)(mtx.data), mtx.cols, mtx.rows, mtx.cols * 3, QImage::Format_RGB888);
        dst.release();
//        mtx.release();
        return img.rgbSwapped();
    }
        break;
    case CV_8UC4:{
        QImage img((const unsigned char *)(mtx.data), mtx.cols, mtx.rows, mtx.cols * 4, QImage::Format_ARGB32);
        dst.release();
//        mtx.release();
        return img;
    }
        break;
    default:{
        QImage img;
        dst.release();
        closeCamera();//Mat未能转换为QImage，重启应用
//        mtx.release();
        return img;
    }
        break;
    }
}

void CameraThreadHelper::closeCamera(){
    qDebug()<<"CameraThreadHelper::closeCamera";
    if(cap.isOpened()){
        cap.release();
    }
    emit endThread();
    ApplicationUtil::restartApplication();//重启
}
