#include "widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
    QPalette   palette;
    QPixmap pixmap(":/ResourcesPackage/background.jpg", "jpg");
    palette.setBrush(this->backgroundRole(),QBrush(pixmap));
    this->setPalette(palette);
    //      this->setMask(pixmap.mask());  //���Խ�ͼƬ��͸��������ʾΪ͸����
    this->setAutoFillBackground(true);
    showFullScreen(); //TODO  ���ֱ�������
    init();
}

Widget::~Widget(){
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
    connect(cameraShowWidget,SIGNAL(sendCaptureImage(QImage)),this,SLOT(receiveCaptureImage(QImage)));

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

    connect(cameraShowWidget,SIGNAL(newTerranSign(Terran)),signMessageWidget,SLOT(signIn(Terran)));//��Ա��ǩ��
    connect(cameraShowWidget,SIGNAL(newTerranSign(Terran)),signNumWidget,SLOT(signIn(Terran)));//��Ա��ǩ������ǩ������+1

    this->setLayout(mainLayout);

    faceDiscernThreadHelper=new FaceDiscernThreadHelper();
    connect(faceDiscernThreadHelper,SIGNAL(askCapture()),this,SLOT(receiveAskCapture()));//���������ͼ�ź�askCapture() ->> ���յ������ͼreceiveAskCapture()
    connect(this,SIGNAL(sendImageToFaceEngine(QImage)),faceDiscernThreadHelper,SLOT(receiveCaptureImage(QImage)));
    connect(faceDiscernThreadHelper,SIGNAL(error(QString)),this,SLOT(errorDispose(QString)));
    connect(faceDiscernThreadHelper,SIGNAL(sendTerran(QList<Terran>)),this,SLOT(receiveDetectTerran(QList<Terran>)));

    qDebug()<<QString::fromLocal8Bit("��ǰ�߳�id")<<(int)QThread::currentThreadId();

    SQLDataBase::instance()->connectionDB(UI_DB_CONNECTION_NAME);

    heartBeatThreadHelper=new HeartThreadHelper();
    connect(heartBeatThreadHelper,SIGNAL(httpServerError(QString)),this,SLOT(errorDispose(QString)));
    connect(heartBeatThreadHelper,SIGNAL(httpConnectionBreak()),this,SLOT(httpDisconnect()));
    connect(heartBeatThreadHelper,SIGNAL(insertDBsynchronized(QList<Terran>)),faceDiscernThreadHelper,SLOT(receiveInsertFaceData(QList<Terran>)));
    connect(heartBeatThreadHelper,SIGNAL(deleteDBsynchronized(QList<Terran>)),faceDiscernThreadHelper,SLOT(receiveDeleteFaceData(QList<Terran>)));
    connect(heartBeatThreadHelper,SIGNAL(updateDBsynchronized(QList<Terran>)),faceDiscernThreadHelper,SLOT(receiveUpdateFaceData(QList<Terran>)));

    faceDiscernThreadHelper->startThread();//��ʼ����ʶ���߳�
//    heartBeatThreadHelper->startThread();//��ʼ�����߳�
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
        qDebug()<<"Widget::receiveAskCapture";
    cameraShowWidget->captureImageFromCamera();
}

void Widget::receiveCaptureImage(QImage image){
        qDebug()<<" Widget::receiveCaptureImage";
    emit sendImageToFaceEngine(image);//���ͽ�ͼ������ʶ������
}

void Widget::receiveDetectTerran(QList<Terran> terranList){
    //    qDebug()<<"Widget::receiveDetectTerran";
    cameraShowWidget->onTerranEnter(terranList);
}

/**
 * @brief Widget::errorDispose
 * ������
 * @param str
 */
void Widget::errorDispose(QString str){
    qDebug()<<"errorDispose";

    //TODO ������ʾ���� ��Ȼ�����˳�
    QCoreApplication::instance()->quit();
}

/**
 * @brief Widget::httpDisconnect
 * ����������ӶϿ�
 * ����ǿ�˴���������ʾ���ߣ�����ʹ�����߰�
 */
void Widget::httpDisconnect(){
//    titleWidget->setAlive(false);
}
