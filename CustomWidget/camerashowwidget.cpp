#include "camerashowwidget.h"

CameraShowWidget::CameraShowWidget(QWidget *parent):QWidget(parent){
    init();
}

CameraShowWidget::~CameraShowWidget(){
    if(httpUtil!=nullptr){
        delete httpUtil;
        httpUtil=nullptr;
    }
}

void CameraShowWidget::init(){
    resize(width,height);
    setFixedSize(width,height);
    noCameraInputBackgroundPix=QPixmap(width,height);
    noCameraInputBackgroundPix.fill(QColor(56,58,76));

    QVBoxLayout *mainLayout=new QVBoxLayout();

    camera=new QCamera(this);

    cameraImageCapture=new QCameraImageCapture(camera);

    customViewFinder=new CustomCameraViewFinder(this);
    camera->setViewfinder(customViewFinder);

    camera->start();
    camera->setCaptureMode(QCamera::CaptureStillImage);

    cameraImageCapture->setBufferFormat(QVideoFrame::Format_RGB32);
    cameraImageCapture->setCaptureDestination(QCameraImageCapture::CaptureToBuffer);
    qDebug()<< cameraImageCapture->isCaptureDestinationSupported(QCameraImageCapture :: CaptureToBuffer);
    qDebug()<<cameraImageCapture->isReadyForCapture();
    //    cameraImageCapture->capture();//�Ƚ�ͼ������  qt���������ͼ�Ῠ��һ��
    connect(cameraImageCapture, SIGNAL(imageCaptured(int,QImage)), this, SLOT(receiveCapturedImage(int,QImage)));

    mainLayout->addWidget(customViewFinder);
    this->setLayout(mainLayout);

    httpUtil=new HttpUtil();
}


void CameraShowWidget::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    if(camera->availability()!=QMultimedia::AvailabilityStatus::Available){
        QPainter painter(this);
        painter.drawPixmap(event->rect(),noCameraInputBackgroundPix,event->rect());
        painter.setPen(QColor(255,255,255));
        QFont font;
        font.setPixelSize(50);
        painter.drawText(QRect(400,300,840,67),QString::fromLocal8Bit("����ͷ�ѶϿ�������Ϻ����´򿪳���"));
    }
}

/**
 * @brief CameraShowWidget::onTerranEnter
 * ����ʶ�������⵽���˽���
 * ��terran��idΪ����ʱ����ʾΪ�ο�
 * @param terran
 */
void CameraShowWidget::onTerranEnter(QList<Terran> listFromEngine){
    qDebug()<<"CameraShowWidget::onTerranEnter";


//    for(Terran terran:listFromEngine){
//        qDebug()<<"engine terran id:"<<terran.id<<"left:"<<terran.frFaceInput.rcFace.left<<"top:"<<terran.frFaceInput.rcFace.top<<"right:"<<terran.frFaceInput.rcFace.right<<"bottom:"<<terran.frFaceInput.rcFace.bottom;
//        //        qDebug()<<inCameraTerranList.size();
//        //        qDebug()<<listFromEngine.at(0).id;
//    }

    QList<Terran> insertList={};
    QList<Terran> deleteList={};
    QList<Terran> updateList={};

    for(Terran terranFromEngine:listFromEngine){
        bool hasCache=false;
        for(Terran terranFromCache:inCameraTerranList){
            if(terranFromEngine.id==terranFromCache.id){
                updateList.append(terranFromEngine);
                hasCache=true;
            }
        }
        if(!hasCache){
            insertList.append(terranFromEngine);
        }
    }
    for(Terran terranFromCache:inCameraTerranList){
        bool needretain=false;
        for(Terran terranUpdate:updateList){
            if(terranFromCache.id==terranUpdate.id){
                needretain=true;
            }
        }
        if(!needretain){
            deleteList.append(terranFromCache);
        }
    }

    //�������γ���
    if(insertList.size()>0){
        insertTerran(insertList);
    }

    //ɾ����������
    if(deleteList.size()>0){
        qDebug()<<"delete";
        for(Terran terran:deleteList){
            customViewFinder->startDisappearAnimator(terran.id);
        }
    }

    //���������ƶ�
    if(updateList.size()>0){
        qDebug()<<"update";
        for(Terran terran:updateList){
            customViewFinder->updateFaceRect(terran);
        }
    }

    inCameraTerranList.clear();
    inCameraTerranList=listFromEngine;
}


void CameraShowWidget::receiveCapturedImage(int, QImage image){
    qDebug()<<"CameraShowWidget::receiveCapturedImage";
    emit sendCaptureImage(image);
}

void CameraShowWidget::captureImageFromCamera(){
    qDebug()<<"CameraShowWidget::captureImageFromCamera";
    cameraImageCapture->capture();
}

/**
 * @brief CameraShowWidget::insertTerran
 * �������γ��ֵĴ���
 * @param insertList
 */
void CameraShowWidget::insertTerran(QList<Terran> &insertList){
//    for(Terran terran:insertList){
//        qDebug()<<"insert";
//        if(terran.id>0){
//            emit newTerranSign(terran);
//        }
//        customViewFinder->addTerranRect(terran);//����ʱ�Զ���ʼ����
//    }
        bool hasSigned=false;
        for(Terran terran:insertList){
            qDebug()<<"insert";

            if(terran.id>0){
                hasSigned=false;
                for(Terran signedTerran:signedList){
                    if(signedTerran.id==terran.id){
                        hasSigned=true;
                        break;
                    }
                }
                if(!hasSigned){
                    signedList.append(terran);
                    /*��չʾ�Կؼ�֪ͨ������ǩ��*/
                    emit newTerranSign(terran);

                    /*�����������ǩ����Ϣ*/
                    QString url=SEND_SIGN_IN_MESSAGE_URL;
                    url.append(QString::fromLocal8Bit("&Name="));
                    url.append(terran.name);

                    url.append(QString::fromLocal8Bit("&UserId="));
                    url.append(QString::number(terran.id));

                    url.append(QString::fromLocal8Bit("&Type="));
                    url.append(QString::fromLocal8Bit("true"));

                    url.append(QString::fromLocal8Bit("&Department="));
                    url.append(QString::number(terran.departmentId));

                    url.append(QString::fromLocal8Bit("&Work="));
                    url.append(QString::fromLocal8Bit("true"));

                    httpUtil->sendMessage(url.toUtf8());
                }
            }

            customViewFinder->addTerranRect(terran);//����ʱ�Զ���ʼ����
        }
}

void CameraShowWidget::clearSignedTerranCache(){
    signedList.clear();
    signedList={};
}
