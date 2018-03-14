#include "signbottomwidget.h"

SignBottomWidget::SignBottomWidget(QWidget *parent):QWidget(parent){
    init();
}


void SignBottomWidget::init(){
    resize(width,height);
    setFixedSize(width,height);
    backgroundPix=QPixmap(width,height);
    backgroundPix.fill(QColor(255,255,255,0));
    this->setMask(backgroundPix.createHeuristicMask());//总体背景为透明度0%

    QHBoxLayout *mainLayout=new QHBoxLayout();
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(1);

    leftWidget=new SignBottomLeftWidget(this);
    rightWidget=new SignBottomRightWidget(this);
    mainLayout->addWidget(leftWidget,1,Qt::AlignLeft);
    mainLayout->addWidget(rightWidget,1,Qt::AlignRight);

    this->setLayout(mainLayout);
}

void SignBottomWidget::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawPixmap(event->rect(),backgroundPix,event->rect());
}

void SignBottomWidget::addSignNum(int num){
    rightWidget->addSignNum(num);
}

void SignBottomWidget::signIn(Terran terran){
    addSignNum(1);
}

void SignBottomWidget::clearSignedTerranNum(){
    rightWidget->clearSignedTerranNum();
}
