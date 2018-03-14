#include "signbottomleftwidget.h"

SignBottomLeftWidget::SignBottomLeftWidget(QWidget *parent):QWidget(parent)
{
    init();
}

void SignBottomLeftWidget::init(){
    resize(width,height);
    setFixedSize(width,height);
    backgroundPix=QPixmap(width,height);
    backgroundPix.fill(QColor(0,0,0,102));
    this->setMask(backgroundPix.createHeuristicMask());//总体背景为透明度40%

    QHBoxLayout *mainLayout=new QHBoxLayout();
    mainLayout->setContentsMargins(35,0,0,0);

    textView=new CustomTextView(this);
    textView->setFontStyle(26,QColor(100,160,215));
    textView->setText(QString::fromLocal8Bit("今日签到人数"));

    mainLayout->addWidget(textView,100,Qt::AlignLeft|Qt::AlignVCenter);
    this->setLayout(mainLayout);
}

void SignBottomLeftWidget::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawPixmap(event->rect(),backgroundPix,event->rect());
}

