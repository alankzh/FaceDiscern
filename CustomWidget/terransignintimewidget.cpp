#include "terransignintimewidget.h"

TerranSignInTimeWidget::TerranSignInTimeWidget(QWidget *parent):QWidget(parent){
    init();
}

TerranSignInTimeWidget::~TerranSignInTimeWidget(){
    if(timeTextView!=nullptr){
        delete timeTextView;
        timeTextView=nullptr;
    }
    if(dateTextVoew!=nullptr){
        delete dateTextVoew;
        dateTextVoew=nullptr;
    }
    if(mainLayout!=nullptr){
        delete mainLayout;
        mainLayout=nullptr;
    }
}

void TerranSignInTimeWidget::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawPixmap(event->rect(),backgroundPix,event->rect());
}

void TerranSignInTimeWidget::init(){
    resize(width,height);
    setFixedSize(width,height);
    backgroundPix=QPixmap(width,height);
    backgroundPix.fill(QColor(0,0,0,102));
    this->setMask(backgroundPix.createHeuristicMask());//总体背景为透明度40%

    mainLayout=new QVBoxLayout();
    mainLayout->setContentsMargins(10,30,10,10);
    mainLayout->setSpacing(10);

    timeTextView=new CustomTextView(this);
    dateTextVoew=new CustomTextView(this);

    mainLayout->addWidget(timeTextView,100,Qt::AlignHCenter|Qt::AlignBottom);
    mainLayout->addWidget(dateTextVoew,100,Qt::AlignHCenter|Qt::AlignTop);


    this->setLayout(mainLayout);
}


void TerranSignInTimeWidget::setTimeAndDate(){
    QDateTime dateTime =QDateTime::currentDateTime();
    QString currentDate =dateTime.toString(QString::fromLocal8Bit(" yyyy年MM月dd日"));
    QString currentTime=dateTime.toString("hh:mm");
    timeTextView->setFontStyle(36);
    timeTextView->setText(currentTime);
    dateTextVoew->setFontStyle(12);
    dateTextVoew->setText(currentDate);
}
