#include "camerashowwidget.h"

CameraShowWidget::CameraShowWidget(QWidget *parent):QWidget(parent){
    init();
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

    connect(cameraImageCapture, SIGNAL(imageCaptured(int,QImage)), this, SLOT(receiveCapturedImage(int,QImage)));

    mainLayout->addWidget(customViewFinder);
    this->setLayout(mainLayout);
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


    for(Terran terran:listFromEngine){
        qDebug()<<"engine terran id:"<<terran.id<<"left:"<<terran.frFaceInput.rcFace.left<<"top:"<<terran.frFaceInput.rcFace.top<<"right:"<<terran.frFaceInput.rcFace.right<<"bottom:"<<terran.frFaceInput.rcFace.bottom;
        //        qDebug()<<inCameraTerranList.size();
        //        qDebug()<<listFromEngine.at(0).id;
    }

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
        for(Terran terran:insertList){
            qDebug()<<"insert";
            if(terran.id>0){
             emit newTerranSign(terran);
            }
            customViewFinder->addTerranRect(terran);//����ʱ�Զ���ʼ����
        }
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
    qDebug()<<"CameraShowWidget::captureImageFromCamera";
    emit sendCaptureImage(image);
}

void CameraShowWidget::captureImageFromCamera(){
    qDebug()<<"CameraShowWidget::captureImageFromCamera";
    cameraImageCapture->capture();
}
