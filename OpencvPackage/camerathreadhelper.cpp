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
    connect(thread,&QThread::finished,thread,&QObject::deleteLater);//�߳̽���ʱɾ���߳�
    connect(thread,&QThread::finished,this,&QObject::deleteLater);//�߳̽���ʱɾ���˶���
    connect(this,SIGNAL(endThread()),thread,SLOT(quit()));//endThread()�ź�ʹ�߳̽���

    connect(this,SIGNAL(beginThread()),this,SLOT(environmentInit()));//beginThread()�ź����߳���ʽ��ʼ
    thread->start();

    emit beginThread();//�����Է��źŵķ�ʽ����ʽ�ú������ý������߳��У�������ֱ�ӵ��ú���
}

void CameraThreadHelper::environmentInit(){
    qDebug()<<"CameraThreadHelper::environmentInit";
    cap.open(0);

    while(!cap.isOpened()) //if not successful then exit
    {
        qDebug() << "Cannot open webcam";
        emit cameraError();//û������ͷ
        QEventLoop loop;
        QTimer timer;

        QObject::connect(&timer,SIGNAL(timeout()),&loop,SLOT(quit()));
        timer.start(5000);
        loop.exec();//�¼�ѭ������������loop��quit()�����ã�����.exec()�Ժ�Ĵ��뽫��������

        //ʵ�ⷢ�֣���ʱδ��ʱ��timer����active��
        if (timer.isActive()){
            //δ��ʱ  ʵ���ʼ���������߼���
            timer.stop();
            cap.open(0);
        } else {
            //��ʱ
            cap.open(0);
        }
    }
    /*�ֱ���*/
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
 * ����һ֡���ͼ��
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
 * cv::mat ��ΪQImage
 * @param mtx
 * @return
 */
QImage CameraThreadHelper::MatToQImage(cv::Mat &dst){
    Mat mtx;//�ο�ϵ
    flip(dst, mtx,1);//1 ˮƽ��ת �����ң�
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
        closeCamera();//Matδ��ת��ΪQImage������Ӧ��
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
    ApplicationUtil::restartApplication();//����
}
