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
    CIDILogo->setBackground(":/ResourcesPackage/CISDILogo.png","png");

    systemName=new CustomTextView(this);
    systemName->setFontStyle(36);
    systemName->setText(QString::fromLocal8Bit("人脸识别签到系统"));

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

    qtimer=new QTimer(this);
    qtimer->setInterval(5000);//5s钟更新一次时间
    connect(qtimer,SIGNAL(timeout()),this,SLOT(updateTime()));
    qtimer->start();
}

void SystemLogoWidget::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawPixmap(event->rect(), backgroundPix, event->rect());
}

//更新时间
void SystemLogoWidget::updateTime(){
    QDateTime dateTime =QDateTime::currentDateTime();
    QString currentDate =dateTime.toString(QString::fromLocal8Bit("yyyy年MM月dd日"));
    QString currentTime=dateTime.toString("hh:mm");
    timeText->setText(currentTime);
    calendarText->setText(currentDate);
    update();
}

