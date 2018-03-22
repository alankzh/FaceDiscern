#include "customcircleimageview.h"

CustomCircleImageView::CustomCircleImageView(QWidget *parent,int diameter):QWidget(parent)
{
    this->xPos=0;
    this->yPos=0;
    this->diameter=diameter;
    resize(diameter,diameter);
    setFixedSize(diameter,diameter);
    backgroundPix=new QPixmap(diameter,diameter);
    backgroundPix->fill(QColor(0,0,0,0));//透明度为0
    this->setMask(backgroundPix->createHeuristicMask());//先将窗口总体背景透明
}

CustomCircleImageView::~CustomCircleImageView(){
    if(backgroundPix!=nullptr){
        delete backgroundPix;
        backgroundPix=nullptr;
    }
}

void CustomCircleImageView::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    if(backgroundPix!=nullptr){
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);//抗锯齿
        painter.drawPixmap(event->rect(),*backgroundPix,event->rect());
    }
}

/**
 * @brief CustomCircleImageView::setBackground
 * 设置圆角ImageView的背景图片
 * @param image
 */
void CustomCircleImageView::setBackground(QImage image){
    if(!image.byteCount()>0){
        return;
    }
    /*先将图片放大为1024*1024的大图，在大图的基础上成圆，锯齿能够得到明显改善*/
    QImage rectImage=image.scaled(QSize(bigDiameter,bigDiameter),
                                  Qt::IgnoreAspectRatio,
                                  Qt::SmoothTransformation);

    int radius=bigDiameter*0.5;
    int radiusDistance=bigDiameter*bigDiameter*0.25;
    for(int x=0;x<bigDiameter;x++){
        for(int y=0;y<bigDiameter;y++){
            if(((x-radius)*(x-radius)+(y-radius)*(y-radius))>radiusDistance){
                rectImage.setPixelColor(QPoint(x,y),QColor(184,184,184,0));//透明
            }
        }
    }

    QImage scaledImage=rectImage.scaled(QSize(diameter,diameter),
                                        Qt::KeepAspectRatio,
                                        Qt::SmoothTransformation);
    if(backgroundPix!=nullptr){
        delete backgroundPix;
        backgroundPix=nullptr;
    }
    backgroundPix=new QPixmap(QPixmap::fromImage(scaledImage));
    setMask(backgroundPix->createHeuristicMask());
    update();
}

void CustomCircleImageView::setPos(int x,int y){
    this->xPos=x;
    this->yPos=y;
    setGeometry(xPos,yPos,diameter,diameter);
    update();
}
