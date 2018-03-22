#include "customcircleimageview.h"

CustomCircleImageView::CustomCircleImageView(QWidget *parent,int diameter):QWidget(parent)
{
    this->xPos=0;
    this->yPos=0;
    this->diameter=diameter;
    resize(diameter,diameter);
    setFixedSize(diameter,diameter);
    backgroundPix=new QPixmap(diameter,diameter);
    backgroundPix->fill(QColor(0,0,0,0));//͸����Ϊ0
    this->setMask(backgroundPix->createHeuristicMask());//�Ƚ��������屳��͸��
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
        painter.setRenderHint(QPainter::Antialiasing, true);//�����
        painter.drawPixmap(event->rect(),*backgroundPix,event->rect());
    }
}

/**
 * @brief CustomCircleImageView::setBackground
 * ����Բ��ImageView�ı���ͼƬ
 * @param image
 */
void CustomCircleImageView::setBackground(QImage image){
    if(!image.byteCount()>0){
        return;
    }
    /*�Ƚ�ͼƬ�Ŵ�Ϊ1024*1024�Ĵ�ͼ���ڴ�ͼ�Ļ����ϳ�Բ������ܹ��õ����Ը���*/
    QImage rectImage=image.scaled(QSize(bigDiameter,bigDiameter),
                                  Qt::IgnoreAspectRatio,
                                  Qt::SmoothTransformation);

    int radius=bigDiameter*0.5;
    int radiusDistance=bigDiameter*bigDiameter*0.25;
    for(int x=0;x<bigDiameter;x++){
        for(int y=0;y<bigDiameter;y++){
            if(((x-radius)*(x-radius)+(y-radius)*(y-radius))>radiusDistance){
                rectImage.setPixelColor(QPoint(x,y),QColor(184,184,184,0));//͸��
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
