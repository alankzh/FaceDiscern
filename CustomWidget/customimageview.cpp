#include "customimageview.h"

CustomImageView::CustomImageView(QWidget *parent):QWidget(parent)
{
    xPos=this->x();
    yPos=this->y();
    width=0;
    height=0;
}

/**
 * @brief CustomImageView::setBackground
 * ���ñ���ͼ
 * @param fileName
 * @param flag
 */
void CustomImageView::setBackground(const char *fileName, const char *flag){
    backgroundPix=QPixmap(fileName,flag);

    width=backgroundPix.size().width();
    height=backgroundPix.size().height();
    resize(backgroundPix.size());
    setFixedSize( width, height);
    setGeometry(xPos,yPos,width,height);
    setMask(backgroundPix.createHeuristicMask());
    update();
}

/**
 * @brief CustomImageView::setScaleBackground
 * �Թ̶�������ñ���ͼ
 * @param width
 * @param height
 * @param fileName
 * @param flag
 */
void CustomImageView::setScaleBackground(int width,int height,const char *fileName, const char *flag){
    backgroundPix=QPixmap(fileName,flag).scaled(width,height,
                                                Qt::IgnoreAspectRatio,
                                                Qt::SmoothTransformation);
    this->width=backgroundPix.size().width();
    this->height=backgroundPix.size().height();
    resize(backgroundPix.size());
    setFixedSize( this->width, this->height);
    setGeometry(xPos,yPos,this->width,this->height);
    setMask(backgroundPix.createHeuristicMask());
    update();
}

void CustomImageView::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawPixmap(event->rect(), backgroundPix, event->rect());
}

/**
 * @brief CustomImageView::setPos
 * ��������ڸ��ؼ�������
 * @param x
 * @param y
 * @return
 */
bool CustomImageView::setPos(int x,int y){
    this->xPos=x;
    this->yPos=y;
    setGeometry(xPos,yPos,width,height);
    update();
    return true;
}


