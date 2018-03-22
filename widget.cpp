#include "widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
    QPalette   palette;
    QPixmap pixmap(":/ResourcesPackage/background.jpg", "jpg");
    palette.setBrush(this->backgroundRole(),QBrush(pixmap));
    this->setPalette(palette);
    //      this->setMask(pixmap.mask());  //可以将图片中透明部分显示为透明的
    this->setAutoFillBackground(true);
    showFullScreen(); //TODO  按分辨率适配
    init();
}

Widget::~Widget(){
    if(heartBeatThreadHelper!=nullptr){
        heartBeatThreadHelper->deleteLater();
        heartBeatThreadHelper=nullptr;
    }
    if(faceDiscernThreadHelper!=nullptr){
        faceDiscernThreadHelper->deleteLater();
        faceDiscernThreadHelper=nullptr;
    }
}


void Widget::init(){
    this->setStyleSheet("QWidget {background-image:url(:/ResourcesPackage/background.jpg)}");
    QVBoxLayout *mainLayout=new QVBoxLayout();
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    titleWidget=new TitleWidget(this);
    titleWidget->setGeometry(0,0,1920,50);

    systemLogoWidget=new SystemLogoWidget(this);

    cameraShowWidget=new CameraShowWidget(this);
    connect(cameraShowWidget,SIGNAL(sendCaptureImage(QImage&)),this,SLOT(receiveCaptureImage(QImage&)));

    QHBoxLayout *bottomLayout=new QHBoxLayout();
    bottomLayout->setContentsMargins(15,15,20,20);
    bottomLayout->setSpacing(17);
    bottomLayout->addWidget(cameraShowWidget,Qt::AlignLeft|Qt::AlignVCenter);

    QVBoxLayout *bottomRightLayout=new QVBoxLayout();
    bottomRightLayout->setContentsMargins(0,0,0,0);
    bottomRightLayout->setSpacing(5);

    signMessageWidget=new SignGatherWidget(this);
    signNumWidget=new SignBottomWidget(this);
    bottomRightLayout->addWidget(signMessageWidget,Qt::AlignTop);
    bottomRightLayout->addWidget(signNumWidget,Qt::AlignBottom);

    bottomLayout->addLayout(bottomRightLayout,Qt::AlignRight|Qt::AlignVCenter);

    mainLayout->addWidget(titleWidget,Qt::AlignTop);
    mainLayout->addWidget(systemLogoWidget,Qt::AlignTop|Qt::AlignHCenter);
    mainLayout->addLayout(bottomLayout,Qt::AlignLeft|Qt::AlignVCenter);

    connect(cameraShowWidget,SIGNAL(newTerranSign(Terran)),signMessageWidget,SLOT(signIn(Terran)));//新员工签到
    connect(cameraShowWidget,SIGNAL(newTerranSign(Terran)),signNumWidget,SLOT(signIn(Terran)));//新员工签到，已签到人数+1
    connect(systemLogoWidget,SIGNAL(clearTerranSignCache()),cameraShowWidget,SLOT(clearSignedTerranCache()));//清空已签到缓存
    connect(systemLogoWidget,SIGNAL(clearTerranSignNum()),signNumWidget,SLOT(clearSignedTerranNum()));//清空签到人数

    this->setLayout(mainLayout);

    faceDiscernThreadHelper=new FaceDiscernThreadHelper();
    connect(faceDiscernThreadHelper,SIGNAL(askCapture()),this,SLOT(receiveAskCapture()));//引擎请求截图信号askCapture() ->> 接收到请求截图receiveAskCapture()
    connect(this,SIGNAL(sendImageToFaceEngine(QImage&)),faceDiscernThreadHelper,SLOT(receiveCaptureImage(QImage&)));
    connect(faceDiscernThreadHelper,SIGNAL(error(QString)),this,SLOT(errorDispose(QString)));
    connect(faceDiscernThreadHelper,SIGNAL(sendTerran(QList<Terran>)),this,SLOT(receiveDetectTerran(QList<Terran>)));

    qDebug()<<QString::fromLocal8Bit("当前线程id")<<(int)QThread::currentThreadId();

    SQLDataBase::instance()->connectionDB(UI_DB_CONNECTION_NAME);

    heartBeatThreadHelper=new HeartThreadHelper();
    connect(heartBeatThreadHelper,SIGNAL(httpServerError(QString)),this,SLOT(errorDispose(QString)));
    connect(heartBeatThreadHelper,SIGNAL(httpConnectionDelay(int)),titleWidget,SLOT(setDelay(int)));//延迟毫秒数
    connect(heartBeatThreadHelper,SIGNAL(httpConnectionAlive(bool)),titleWidget,SLOT(setHttpAlive(bool)));//连接断开的ui提示
    connect(heartBeatThreadHelper,SIGNAL(insertDBsynchronized(QList<Terran>)),faceDiscernThreadHelper,SLOT(receiveInsertFaceData(QList<Terran>)));
    connect(heartBeatThreadHelper,SIGNAL(deleteDBsynchronized(QList<Terran>)),faceDiscernThreadHelper,SLOT(receiveDeleteFaceData(QList<Terran>)));
    connect(heartBeatThreadHelper,SIGNAL(updateDBsynchronized(QList<Terran>)),faceDiscernThreadHelper,SLOT(receiveUpdateFaceData(QList<Terran>)));

    faceDiscernThreadHelper->startThread();//开始人脸识别线程
    heartBeatThreadHelper->startThread();//开始心跳线程


    QPushButton *quitButton=new QPushButton(this);
    quitButton->setGeometry(200,200,200,200);
    quitButton->setText("QQQQuit");
    connect(quitButton,SIGNAL(clicked(bool)),this,SLOT());
    mainLayout->addWidget(quitButton);
}

void Widget::quitApplication(bool){
    QApplication::quit();
}

void Widget::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawPixmap(event->rect(), backgroundPix, event->rect());

    //    titleWidget->setGeometry(0,0,1920,50);
    //    systemLogoWidget->setGeometry(0,50,1920,150);
}


void Widget::receiveAskCapture()
{
    cameraShowWidget->captureImageFromCamera();
}

void Widget::receiveCaptureImage(QImage &image){
    emit sendImageToFaceEngine(image);//发送截图到人脸识别引擎
}

void Widget::receiveDetectTerran(QList<Terran> terranList){
    //    qDebug()<<"Widget::receiveDetectTerran";
    cameraShowWidget->onTerranEnter(terranList);
}

/**
 * @brief Widget::errorDispose
 * 错误处理
 * @param str
 */
void Widget::errorDispose(QString str){
    qDebug()<<"errorDispose";

    //TODO 弹出提示窗口 ，然后再退出
    QCoreApplication::instance()->quit();
}


