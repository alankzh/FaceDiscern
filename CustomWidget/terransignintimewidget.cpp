#include "terransignintimewidget.h"

TerranSignInTimeWidget::TerranSignInTimeWidget(QWidget *parent):QWidget(parent){
    init();
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
    this->setMask(backgroundPix.createHeuristicMask());//���屳��Ϊ͸����40%

    QVBoxLayout *mainLayout=new QVBoxLayout();
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
    QString currentDate =dateTime.toString(QString::fromLocal8Bit(" yyyy��MM��dd��"));
    QString currentTime=dateTime.toString("hh:mm");
    timeTextView->setFontStyle(36);
    timeTextView->setText(currentTime);
    dateTextVoew->setFontStyle(12);
    dateTextVoew->setText(currentDate);
}
