#include "titlewidget.h"

TitleWidget::TitleWidget(QWidget *parent):QWidget(parent){
    init();
}

void TitleWidget::init(){
    resize(width,height);
    setFixedSize(width,height);
    backgroundPix=QPixmap(width,height);
    backgroundPix.fill(QColor(56,58,76));
    this->setMask(backgroundPix.createHeuristicMask());

    //    setStyleSheet(
    //                "background-color:#383a4c"
    //                );

    QHBoxLayout *mainLayout=new QHBoxLayout();
    mainLayout->setContentsMargins(10,0,10,0);
    mainLayout->setSpacing(20);

    logoImageView=new CustomImageView(this);
    logoImageView->setPos(0,0);
    logoImageView->setBackground(":/ResourcesPackage/CodingtechLogo.png","png");

    delayText=new CustomTextView(this);
    delayText->setFontStyle(14);
    delayText->setText("32 ms");


    statuText=new CustomTextView(this);
    statuText->setFontStyle(14);
    statuText->setText(QString::fromLocal8Bit("������"));

    labelView=new CustomImageView(this);
    labelView->setPos(1890,0);
    labelView->setBackground(":/ResourcesPackage/runningLabel.png","png");

    mainLayout->addWidget(logoImageView,Qt::AlignLeft|Qt::AlignVCenter);
    mainLayout->addWidget(delayText,5000,Qt::AlignRight|Qt::AlignVCenter);
    mainLayout->addWidget(statuText,Qt::AlignRight|Qt::AlignVCenter);
    mainLayout->addWidget(labelView,Qt::AlignRight|Qt::AlignVCenter);

    this->setLayout(mainLayout);

    qtimer=new QTimer(this);
    qtimer->setInterval(2000);//2s�Ӹ���һ���ӳ�
    connect(qtimer,SIGNAL(timeout()),this,SLOT(fakeMs()));
    qtimer->start();
}

/**
 * @brief TitleWidget::setStatus
 * ����״̬
 * @param status
 */
void TitleWidget::setStatus(TitleWidget::RunningStatus status){
    switch(status){
    case RunningStatus::Running:
        statuText->setText(QString::fromLocal8Bit("������"));
        break;
    case RunningStatus::Disconnection:
        statuText->setText(QString::fromLocal8Bit("�Ͽ���"));
        break;
    }
}

/**
 * @brief TitleWidget::setDelay
 * �����ӳ�
 * @param ms
 */
void TitleWidget::setDelay(int ms){
    QString str(QString::number(ms));
    str.append(QString::fromLocal8Bit(" ms"));
    delayText->setText(str);
}

void TitleWidget::setAlive(bool alive){
    if(alive){
        statuText->setText(QString::fromLocal8Bit("������"));
    }else{
        statuText->setText(QString::fromLocal8Bit("�Ͽ�"));
    }
}

void TitleWidget::fakeMs(){
    setDelay(rand()%200);
}

void TitleWidget::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.drawPixmap(event->rect(), backgroundPix, event->rect());
}
