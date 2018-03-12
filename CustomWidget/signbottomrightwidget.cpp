#include "signbottomrightwidget.h"

SignBottomRightWidget::SignBottomRightWidget(QWidget *parent):QWidget(parent){
    init();
}

void SignBottomRightWidget::init(){
    nowNum=0;
    resize(width,height);
    setFixedSize(width,height);
    backgroundPix=QPixmap(width,height);
    backgroundPix.fill(QColor(0,0,0,102));
    this->setMask(backgroundPix.createHeuristicMask());//总体背景为透明度40%

    QHBoxLayout *mainLayout=new QHBoxLayout();
    mainLayout->setContentsMargins(0,0,25,30);
    mainLayout->setSpacing(0);

    numText=new CustomTextView(this);
    numText->setFontStyle(60,QColor(255,255,255));
    numText->setText(QString::number(nowNum));

    unitText=new CustomTextView(this);
    unitText->setFontStyle(26,QColor(100,160,215));
    unitText->setText(QString::fromLocal8Bit("人"));

    mainLayout->addWidget(numText,200,Qt::AlignRight|Qt::AlignBottom);
    mainLayout->addWidget(unitText,40,Qt::AlignLeft|Qt::AlignBottom);
    this->setLayout(mainLayout);
}

void SignBottomRightWidget::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawPixmap(event->rect(),backgroundPix,event->rect());

    unitText->move(unitText->geometry().x(),55);
}

/**
 * @brief SignBottomRightWidget::addSignNum
 * 增加签到人数
 * @param num
 */
void SignBottomRightWidget::addSignNum(int num){
    nowNum+=num;
    numText->setText(QString::number(nowNum));
    update();
}
