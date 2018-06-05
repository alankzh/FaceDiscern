#include "systemlogowidget.h"


SystemLogoWidget::SystemLogoWidget(QWidget *parent):QWidget(parent)
{
    init();
}


void SystemLogoWidget::init(){
    resize(width,height);
    setFixedSize(width,height);
    backgroundPix=QPixmap(width,height);
    backgroundPix.fill(QColor(0,0,0,102));
    this->setMask(backgroundPix.createHeuristicMask());

    QHBoxLayout *mainLayout=new QHBoxLayout();
    mainLayout->setContentsMargins(58,10,35,0);
    mainLayout->setSpacing(20);

    CIDILogo=new CustomImageView(this);
    CIDILogo->setPos(0,0);
//    CIDILogo->setScaleBackground(144,100,":/ResourcesPackage/LOGO.png","png");
    CIDILogo->setBackground(":/ResourcesPackage/CISDILogo.png","png");

    systemName=new CustomTextView(this);
    systemName->setFontStyle(36);
    systemName->setText(QString::fromLocal8Bit("人脸识别考勤系统"));

    QDateTime dateTime =QDateTime::currentDateTime();
    QString currentDate =dateTime.toString(QString::fromLocal8Bit("yyyy年MM月dd日"));
    QString currentTime=dateTime.toString("hh:mm");

    CustomTextView *blankText=new CustomTextView(this);//什么都不干，只是占位置

    timeText=new CustomTextView(this);
    timeText->setFontStyle(66);
    timeText->setText(currentTime);

    calendarText=new CustomTextView(this);
    calendarText->setFontStyle(22);
    calendarText->setText(currentDate);

    QVBoxLayout *rightLayout=new QVBoxLayout();
    rightLayout->setContentsMargins(0,30,0,0);
    rightLayout->setSpacing(5);

    mainLayout->addWidget(CIDILogo,Qt::AlignLeft|Qt::AlignVCenter);
    mainLayout->addWidget(systemName,Qt::AlignLeft|Qt::AlignVCenter);

    rightLayout->addWidget(timeText,1,Qt::AlignCenter);
    rightLayout->addWidget(calendarText,1,Qt::AlignTop|Qt::AlignRight);

    mainLayout->addWidget(blankText,5000,Qt::AlignRight);
    mainLayout->addLayout(rightLayout,1);

    this->setLayout(mainLayout);

    restartTime=QTime(3,0,0);
    restartTime1=QTime(1,0,0);
    restartTime2=QTime(6,0,0);
    restartTime3=QTime(10,30,0);
    restartTime4=QTime(15,0,0);
    restartTime5=QTime(19,30,0);

    qtimer=new QTimer(this);
    qtimer->setInterval(5000);//5s钟更新一次时间
    connect(qtimer,SIGNAL(timeout()),this,SLOT(updateTime()));
    qtimer->start();
    timeMsec=0;
}

void SystemLogoWidget::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawPixmap(event->rect(), backgroundPix, event->rect());
}

//更新时间
void SystemLogoWidget::updateTime(){
    qDebug()<<"SystemLogoWidget::updateTime";
    timeMsec+=5000;
    if(timeMsec>=CLEAR_SIGN_CACHE_INTERVAL){
        emit clearTerranSignCache();//通知清空缓存
        timeMsec=0;
    }

    QTime currentQtime=QTime::currentTime();
    int nowMsec=currentQtime.msecsSinceStartOfDay();
    if(nowMsec<=5000){
        emit clearTerranSignNum();//每日0点，清空签到人数

        /*清空是否第一次签到*/
        QList<Terran> list={};
        SQLDataBase::instance()->operationDB(UI_DB_CONNECTION_NAME,SQLDataBase::OperationWay::ClearSignType,list);
    }


    int msecToRestart=currentQtime.msecsTo(restartTime);
    if(msecToRestart>=-5000&&msecToRestart<0){
        /*凌晨3点重启电脑*/
//        emit restartApplication();
        ApplicationUtil::restartSystem();//重启电脑
    }

    int msecToRestart1=currentQtime.msecsTo(restartTime1);
    if(msecToRestart1>=-5000&&msecToRestart1<0){
        /*凌晨1点重启应用程序*/
        emit restartApplication();
    }

    int msecToRestart2=currentQtime.msecsTo(restartTime2);
    if(msecToRestart2>=-5000&&msecToRestart2<0){
        /*早晨6点重启应用程序*/
        emit restartApplication();
    }

    int msecToRestart3=currentQtime.msecsTo(restartTime3);
    if(msecToRestart3>=-5000&&msecToRestart3<0){
        /*上午10点半重启应用程序*/
        emit restartApplication();
    }

    int msecToRestart4=currentQtime.msecsTo(restartTime4);
    if(msecToRestart4>=-5000&&msecToRestart4<0){
        /*下午3点重启应用程序*/
        emit restartApplication();
    }

    int msecToRestart5=currentQtime.msecsTo(restartTime5);
    if(msecToRestart5>=-5000&&msecToRestart5<0){
        /*下午7点半重启应用程序*/
        emit restartApplication();
    }

    QDateTime dateTime =QDateTime::currentDateTime();
    QString currentDate =dateTime.toString(QString::fromLocal8Bit("yyyy年MM月dd日"));
    QString currentTime=dateTime.toString("hh:mm");
    timeText->setText(currentTime);
    calendarText->setText(currentDate);
    update();
}

