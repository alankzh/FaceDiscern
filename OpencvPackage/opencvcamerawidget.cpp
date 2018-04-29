#include "opencvcamerawidget.h"

OpencvCameraWidget::OpencvCameraWidget(QWidget *parent):QWidget(parent){
    resize(width,height);
    setFixedSize(width,height);
    cameraViewWidget=new OpencvCameraViewWidget(this);
    QVBoxLayout *mainLayout=new QVBoxLayout();
    mainLayout->addWidget(cameraViewWidget);
    this->setLayout(mainLayout);

    connect(cameraViewWidget,SIGNAL(viewShowEnd()),this,SLOT(nextFrame()));//一帧显示完毕后，立即呼唤下一帧

    httpUtil=new HttpUtil();

    cameraThread=new CameraThreadHelper();

    connect(cameraThread,SIGNAL(environmentComplete()),this,SLOT(nextFrame()));//环境初始化完毕，即开始请求下一帧
    connect(cameraThread,SIGNAL(cameraError()),this,SLOT(cameraErrorCatch()));//摄像机错误处理
    connect(this,SIGNAL(askCameraNextFrame()),cameraThread,SLOT(captureFrame()));//接收下一帧请求后，读取下一帧
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
        painter.drawText(QRect(400,300,840,67),QString::fromLocal8Bit("摄像头已断开，请插上后，重新打开程序"));
    }
}

//请求摄像机下一帧图片
void OpencvCameraWidget::nextFrame(){
    emit askCameraNextFrame();
}

/**
 * @brief OpencvCameraWidget::receiveFrame
 * 收到摄像机中一帧图片
 * @param image
 */
void OpencvCameraWidget::receiveFrame(QImage image){
    cameraViewWidget->updateView(image);
    if(needDiscern){
        emit sendCaptureImage(image);//发送截图去识别
        needDiscern=false;
    }
    update();
}

/**
 * @brief OpencvCameraWidget::needDiscernFromCamera
 * 需要截取一帧图片去识别
 */
void OpencvCameraWidget::needDiscernFromCamera(){
    if(!needDiscern){
        needDiscern=true;
    }else{
        qDebug()<<QString::fromLocal8Bit("上一帧还未完成显示");
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

    //人脸矩形出现
    if(insertList.size()>0){
        insertTerran(insertList);
    }

    //删除人脸矩形
    if(deleteList.size()>0){
        for(Terran terran:deleteList){
            cameraViewWidget->startDisappearAnimator(terran.id);
        }
    }

    //人脸矩形移动
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
 * 人脸矩形出现的处理
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
                /*向展示性控件通知有新人签到*/
                emit newTerranSign(terran);

                /*向服务器发送签到消息*/
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

        cameraViewWidget->addTerranRect(terran);//出现时自动开始动画
    }
}

void OpencvCameraWidget::clearSignedTerranCache(){
    qDebug()<<"OpencvCameraWidget::clearSignedTerranCache";
    signedList.clear();
    signedList={};
}

/**
 * @brief OpencvCameraWidget::cameraErrorCatch
 * 当摄像机错误时
 */
void OpencvCameraWidget::cameraErrorCatch(){
    backgroundPix=QPixmap(width,height);
    backgroundPix.fill(QColor(56,58,76));
    cameraErrored=true;
}
