#include "opencvcameraviewwidget.h"

OpencvCameraViewWidget::OpencvCameraViewWidget(QWidget *parent):QWidget(parent){
    resize(width,height);
    setFixedSize(width,height);
}

OpencvCameraViewWidget::~OpencvCameraViewWidget(){
    for(FacePainterRect* facePainterRect:faceRectList){
        if(facePainterRect!=nullptr){
            facePainterRect->deleteLater();
            facePainterRect=nullptr;
        }
    }
}

void OpencvCameraViewWidget::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    /*���ͼ*/
    painter.drawPixmap(event->rect(),backgroundPix,event->rect());
    /*��������ͼ*/
    for(FacePainterRect* facePainterRect:faceRectList){
        Terran terran=facePainterRect->getTerran();
        int width=facePainterRect->width;
        int height=facePainterRect->height;
        int left=facePainterRect->left;
        int right=facePainterRect->right;
        int top=facePainterRect->top;
        int bottom=facePainterRect->bottom;
        int appearFontCount=facePainterRect->appearFontCount;

        QColor rectColor=facePainterRect->rectColor;
        QColor fontColor=facePainterRect->fontColor;

        QPen rectPen;
        rectPen.setColor(rectColor);
        rectPen.setWidth(8);//Ĭ�Ͽ�8������
        painter.setPen(rectPen);

        /*�����Ľǵ���*/
        int lineLength=(right-left)*0.25;
        painter.drawLine(QPoint(left,top),QPoint(left+lineLength,top));
        painter.drawLine(QPoint(left,top),QPoint(left,top+lineLength));

        painter.drawLine(QPoint(right,top),QPoint(right-lineLength,top));
        painter.drawLine(QPoint(right,top),QPoint(right,top+lineLength));

        painter.drawLine(QPoint(left,bottom),QPoint(left+lineLength,bottom));
        painter.drawLine(QPoint(left,bottom),QPoint(left,bottom-lineLength));

        painter.drawLine(QPoint(right,bottom),QPoint(right-lineLength,bottom));
        painter.drawLine(QPoint(right,bottom),QPoint(right,bottom-lineLength));

        if(appearFontCount==0||terran.id<0){
            continue;
        }else{
            int fontSize=height*0.1;//�������壬��ռ�߶ȱ���ʮ��֮3

            QFont font;
            //            font.setFamily(QString::fromLocal8Bit("Microsoft YaHei UI"));
            font.setFamily(QString::fromLocal8Bit("����"));
            font.setPixelSize(fontSize);
            font.setWeight(75);
            painter.setFont(font);
            QPen fontPen;
            fontPen.setColor(fontColor);
            fontPen.setWidth(4);
            painter.setPen(fontColor);
            if(appearFontCount >= (terran.name.size())){
                painter.drawText(QRect(left,top+height*0.35,width,fontSize+1),Qt::AlignLeft,terran.name);//��һ���� ����
                if(appearFontCount >= (terran.name.size()+terran.department.size())){
                    painter.drawText(QRect(left,top+height*0.45+2,width,fontSize+1),Qt::AlignLeft,terran.department);//�ڶ����� ����

                    QDateTime dateTime =QDateTime::currentDateTime();
                    QString currentTime=dateTime.toString("hh:mm");
                    if(appearFontCount>=(terran.name.size()+terran.department.size()+currentTime.size())){
                        painter.drawText(QRect(left,top+height*0.55+4,width,fontSize+1),Qt::AlignLeft,currentTime);//�������� ʱ��
                    }else{
                        painter.drawText(QRect(left,top+height*0.55+4,width,fontSize+1),Qt::AlignLeft,currentTime.mid(0,appearFontCount-terran.name.size()-terran.department.size()));//�������� ʱ��
                    }
                }else{
                    painter.drawText(QRect(left,top+height*0.45+2,width,fontSize+1),Qt::AlignLeft,terran.department.mid(0,appearFontCount-terran.name.size()));//�ڶ����� ����
                }

            }else{
                painter.drawText(QRect(left,top+height*0.35,width,fontSize+1),Qt::AlignLeft,terran.name.mid(0,appearFontCount));//��һ����
            }
        }
    }
    emit viewShowEnd();//һ֡��ʾ����ź�
}

/**
 * @brief OpencvCameraViewWidget::addTerranRect
 * ����һ���������Σ����ӽ���ʱ�������Զ���ʼ
 * @param terran
 */
void OpencvCameraViewWidget::addTerranRect(Terran terran){
    FacePainterRect *faceRect=new FacePainterRect();

    faceRect->setTerran(terran);

    connect(faceRect,SIGNAL(askUpdateUI()),this,SLOT(answerUpdateUI()));
    connect(faceRect,SIGNAL(disappeartAnimatorEnd(int)),this,SLOT(onDisappearAnimatorEnd(int)));

    faceRectList.append(faceRect);

    if(terran.id>0){
        faceRect->startAppearAnimator();
    }

//    qDebug()<<QString::fromLocal8Bit("�¼���֮�����������")<<faceRectList.size();
//    for(FacePainterRect* faceRect:faceRectList){
//        qDebug()<<"terran in camera insert id:"<<terran.id<<"left:"<<faceRect->left<<"top:"<<faceRect->top<<"right:"<<faceRect->right<<"bottom"<<faceRect->bottom;
//    }
}

/**
 * @brief OpencvCameraViewWidget::onDisappearAnimatorEnd
 * ��ʧ����������ɾ��
 * @param terranId
 */
void OpencvCameraViewWidget::onDisappearAnimatorEnd(int terranId){
    QMutableListIterator<FacePainterRect*> iter(faceRectList);
    while(iter.hasNext()){
        FacePainterRect* faceRect=iter.next();
        if(faceRect->getTerranId()==terranId){
            faceRect->deleteLater();
            qDebug()<<QString::fromLocal8Bit("ɾ����������, terran id")<<terranId;
            iter.remove();
        }
    }
    qDebug()<<faceRectList.size();
}

/**
 * @brief OpencvCameraViewWidget::startAppearAnimator
 * ��ʼ���ֶ�����һ����addTerranRectʱ�����Ѿ���ʼ
 * @param terranId
 */
void OpencvCameraViewWidget::startAppearAnimator(int terranId){
    for(FacePainterRect* faceRect:faceRectList){
        if(faceRect->getTerranId()==terranId){
            faceRect->startAppearAnimator();
        }
    }
}

/**
 * @brief OpencvCameraViewWidget::startDisappearAnimator
 * ��ʼ��ʧ����
 * @param terranId
 */
void OpencvCameraViewWidget::startDisappearAnimator(int terranId){
    for(FacePainterRect* faceRect:faceRectList){
        if(faceRect->getTerranId()==terranId){
            faceRect->startDisappearAnimator();
        }
    }
}

/**
 * @brief OpencvCameraViewWidget::updateFaceRect
 * ������������λ��
 * @param terran
 */
void OpencvCameraViewWidget::updateFaceRect(Terran terran){
    for(FacePainterRect* faceRect:faceRectList){
        if(faceRect->getTerranId()==terran.id){
            faceRect->setTerran(terran);
//            qDebug()<<"terran id:"<<terran.id<<"in camera left:"<<faceRect->left<<"top:"<<faceRect->top<<"right:"<<faceRect->right<<"bottom"<<faceRect->bottom;
        }
    }
}

void OpencvCameraViewWidget::answerUpdateUI(){
    update();
}

void OpencvCameraViewWidget::updateView(QImage &image){
    /*���Ĭ�Ϸֱ���640 480������Ϊ�ؼ���С*/
    QImage scaledImage = image.scaled(QSize(width,height),
                                      Qt::KeepAspectRatio,
                                      Qt::SmoothTransformation);
    backgroundPix=QPixmap::fromImage(scaledImage);
    update();
}
