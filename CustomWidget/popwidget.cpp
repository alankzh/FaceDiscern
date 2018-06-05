#include "popwidget.h"

PopWidget::PopWidget(QWidget *parent):QWidget(parent)
{
    QHBoxLayout *mainLayout=new QHBoxLayout();
    hintImage=new CustomImageView(this);
    hintImage->setPos(0,0);
    hintImage->setBackground(":/ResourcesPackage/dataLoading.png","png");
    mainLayout->addWidget(hintImage);
    this->setLayout(mainLayout);
}

PopWidget::~PopWidget(){

}

void PopWidget::showWindow(){
    this->show();
}

void PopWidget::hideWindow(){
    this->hide();
}


