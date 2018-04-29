#include "opencvcamerawidget.h"

OpencvCameraWidget::OpencvCameraWidget(QWidget *parent):QWidget(parent){
    resize(width,height);
    setFixedSize(width,height);
    cameraViewWidget=new OpencvCameraViewWidget(this);
    QVBoxLayout *mainLayout=new QVBoxLayout();
    mainLayout->addWidget(cameraViewWidget);
    this->setLayout(mainLayout);

    connect(cameraViewWidget,SIGNAL(viewShowEnd()),this,SLOT(nextFrame()));//һ֡��ʾ��Ϻ�����������һ֡

    httpUtil=new HttpUtil();

    cameraThread=new CameraThreadHelper();

    connect(cameraThread,SIGNAL(environmentComplete()),this,SLOT(nextFrame()));//������ʼ����ϣ�����ʼ������һ֡
    connect(cameraThread,SIGNAL(cameraError()),this,SLOT(cameraErrorCatch()));//�����������
    connect(this,SIGNAL(askCameraNextFrame()),cameraThread,SLOT(captureFrame()));//������һ֡����󣬶�ȡ��һ֡
    connect(cameraThread,SIGNAL(sendFrame(QImage)),this,SLOT(receiveFrame(QImage)));
    cameraThread->startThread();
    needDiscern=false;
    cameraErrored=false;

    qDebug()<<"OpencvCameraWidget::OpencvCameraWidget";
}

OpencvCameraWidget::~OpencvCameraWidget(){
    if(httpUtil!=nullptr){
        delete httpUtil;
        httpUtil=nullptr;
    }
    if(cameraThread!=nullptr){
        delete cameraThread;
        cameraThread=nullptr;
    }
    if(cameraViewWidget!=nullptr){
        cameraViewWidget->deleteLater();
        cameraViewWidget=nullptr;
    }
}

void OpencvCameraWidget::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    if(cameraErrored){
        QPainter painter(this);
        painter.drawPixmap(event->rect(),backgroundPix,event->rect());
        painter.setPen(QColor(255,255,255));
        painter.drawText(QRect(400,300,840,67),QString::fromLocal8Bit("����ͷ�ѶϿ�������Ϻ����´򿪳���"));
    }
}

//�����������һ֡ͼƬ
void OpencvCameraWidget::nextFrame(){
    emit askCameraNextFrame();
}

/**
 * @brief OpencvCameraWidget::receiveFrame
 * �յ��������һ֡ͼƬ
 * @param image
 */
void OpencvCameraWidget::receiveFrame(QImage image){
    cameraViewWidget->updateView(image);
    if(needDiscern){
        emit sendCaptureImage(image);//���ͽ�ͼȥʶ��
        needDiscern=false;
    }
    update();
}

/**
 * @brief OpencvCameraWidget::needDiscernFromCamera
 * ��Ҫ��ȡһ֡ͼƬȥʶ��
 */
void OpencvCameraWidget::needDiscernFromCamera(){
    if(!needDiscern){
        needDiscern=true;
    }else{
        qDebug()<<QString::fromLocal8Bit("��һ֡��δ�����ʾ");
    }
}

void OpencvCameraWidget::onTerranEnter(QList<Terran> listFromEngine){
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
        for(Terran terran:deleteList){
            cameraViewWidget->startDisappearAnimator(terran.id);
        }
    }

    //���������ƶ�
    if(updateList.size()>0){
        for(Terran terran:updateList){
            cameraViewWidget->updateFaceRect(terran);
        }
    }

    inCameraTerranList.clear();
    inCameraTerranList=listFromEngine;
}

/**
 * @brief OpencvCameraWidget::insertTerran
 * �������γ��ֵĴ���
 * @param insertList
 */
void OpencvCameraWidget::insertTerran(QList<Terran> &insertList){
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
                QString url=Constant::SEND_SIGN_IN_MESSAGE_URL;
                url.append(QString::fromLocal8Bit("&Name="));
                url.append(terran.name);

                url.append(QString::fromLocal8Bit("&UserId="));
                url.append(QString::number(terran.id));

                url.append(QString::fromLocal8Bit("&Type="));
                url.append(QString::number(terran.uglyType));

                url.append(QString::fromLocal8Bit("&Department="));
                url.append(QString::number(terran.departmentId));

                url.append(QString::fromLocal8Bit("&Work="));
                url.append(QString::fromLocal8Bit("true"));

                httpUtil->sendMessage(url.toUtf8());

                if(terran.uglyType==1){
                    terran.uglyType=2;
                    QList<Terran> list={};
                    list.append(terran);
                    SQLDataBase::instance()->operationDB(UI_DB_CONNECTION_NAME,SQLDataBase::OperationWay::UpdateDB,list);
                }
            }
        }

        cameraViewWidget->addTerranRect(terran);//����ʱ�Զ���ʼ����
    }
}

void OpencvCameraWidget::clearSignedTerranCache(){
    qDebug()<<"OpencvCameraWidget::clearSignedTerranCache";
    signedList.clear();
    signedList={};
}

/**
 * @brief OpencvCameraWidget::cameraErrorCatch
 * �����������ʱ
 */
void OpencvCameraWidget::cameraErrorCatch(){
    backgroundPix=QPixmap(width,height);
    backgroundPix.fill(QColor(56,58,76));
    cameraErrored=true;
}
