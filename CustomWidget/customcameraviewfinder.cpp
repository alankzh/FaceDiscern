#include "customcameraviewfinder.h"

CustomCameraViewFinder::CustomCameraViewFinder(QWidget *parent):QCameraViewfinder(parent){
}

CustomCameraViewFinder::~CustomCameraViewFinder(){
    for(FacePainterRect* facePainterRect:faceRectList){
        if(facePainterRect!=nullptr){
            facePainterRect->deleteLater();
            facePainterRect=nullptr;
        }
    }
}

void CustomCameraViewFinder::paintEvent(QPaintEvent *event){

    QCameraViewfinder::paintEvent(event);
    QPainter painter(this);

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
        rectPen.setWidth(8);//默认宽8个像素
        painter.setPen(rectPen);

        /*绘制四角的线*/
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
            int fontSize=height*0.1;//三行字体，共占高度比例十分之3

            QFont font;
            //            font.setFamily(QString::fromLocal8Bit("Microsoft YaHei UI"));
            font.setFamily(QString::fromLocal8Bit("黑体"));
            font.setPixelSize(fontSize);
            font.setWeight(75);
            painter.setFont(font);
            QPen fontPen;
            fontPen.setColor(fontColor);
            fontPen.setWidth(4);
            painter.setPen(fontColor);
            if(appearFontCount >= (terran.name.size())){
                painter.drawText(QRect(left,top+height*0.35,width,fontSize+1),Qt::AlignLeft,terran.name);//第一行字 姓名
                if(appearFontCount >= (terran.name.size()+terran.department.size())){
                    painter.drawText(QRect(left,top+height*0.45+2,width,fontSize+1),Qt::AlignLeft,terran.department);//第二行字 部门

                    QDateTime dateTime =QDateTime::currentDateTime();
                    QString currentTime=dateTime.toString("hh:mm");
                    if(appearFontCount>=(terran.name.size()+terran.department.size()+currentTime.size())){
                        painter.drawText(QRect(left,top+height*0.55+4,width,fontSize+1),Qt::AlignLeft,currentTime);//第三行字 时间
                    }else{
                        painter.drawText(QRect(left,top+height*0.55+4,width,fontSize+1),Qt::AlignLeft,currentTime.mid(0,appearFontCount-terran.name.size()-terran.department.size()));//第三行字 时间
                    }
                }else{
                    painter.drawText(QRect(left,top+height*0.45+2,width,fontSize+1),Qt::AlignLeft,terran.department.mid(0,appearFontCount-terran.name.size()));//第二行字 部门
                }

            }else{
                painter.drawText(QRect(left,top+height*0.35,width,fontSize+1),Qt::AlignLeft,terran.name.mid(0,appearFontCount));//第一行字
            }
        }
    }
}

/**
 * @brief CustomCameraViewFinder::addTerranRect
 * 新增一个人脸矩形，增加进来时，动画自动开始
 * @param terran
 */
void CustomCameraViewFinder::addTerranRect(Terran terran){
    FacePainterRect *faceRect=new FacePainterRect();

    faceRect->setTerran(terran);

    connect(faceRect,SIGNAL(askUpdateUI()),this,SLOT(answerUpdateUI()));
    connect(faceRect,SIGNAL(disappeartAnimatorEnd(int)),this,SLOT(onDisappearAnimatorEnd(int)));

    faceRectList.append(faceRect);

    if(terran.id>0){
        faceRect->startAppearAnimator();
    }

//    qDebug()<<QString::fromLocal8Bit("新加入之后的人脸数量")<<faceRectList.size();
//    for(FacePainterRect* faceRect:faceRectList){
//        qDebug()<<"terran in camera insert id:"<<terran.id<<"left:"<<faceRect->left<<"top:"<<faceRect->top<<"right:"<<faceRect->right<<"bottom"<<faceRect->bottom;
//    }
}

/**
 * @brief CustomCameraViewFinder::onDisappearAnimatorEnd
 * 消失动画结束后删除
 * @param terranId
 */
void CustomCameraViewFinder::onDisappearAnimatorEnd(int terranId){
    QMutableListIterator<FacePainterRect*> iter(faceRectList);
    while(iter.hasNext()){
        FacePainterRect* faceRect=iter.next();
        if(faceRect->getTerranId()==terranId){
            faceRect->deleteLater();
            qDebug()<<QString::fromLocal8Bit("删除绘制人脸, terran id")<<terranId;
            iter.remove();
        }
    }
    qDebug()<<faceRectList.size();
}

/**
 * @brief CustomCameraViewFinder::startAppearAnimator
 * 开始出现动画，一般在addTerranRect时动画已经开始
 * @param terranId
 */
void CustomCameraViewFinder::startAppearAnimator(int terranId){
    for(FacePainterRect* faceRect:faceRectList){
        if(faceRect->getTerranId()==terranId){
            faceRect->startAppearAnimator();
        }
    }
}

/**
 * @brief CustomCameraViewFinder::startDisappearAnimator
 * 开始消失动画
 * @param terranId
 */
void CustomCameraViewFinder::startDisappearAnimator(int terranId){
    for(FacePainterRect* faceRect:faceRectList){
        if(faceRect->getTerranId()==terranId){
            faceRect->startDisappearAnimator();
        }
    }
}

/**
 * @brief CustomCameraViewFinder::updateFaceRect
 * 更新人脸矩形位置
 * @param terran
 */
void CustomCameraViewFinder::updateFaceRect(Terran terran){
    for(FacePainterRect* faceRect:faceRectList){
        if(faceRect->getTerranId()==terran.id){
            faceRect->setTerran(terran);
//            qDebug()<<"terran id:"<<terran.id<<"in camera left:"<<faceRect->left<<"top:"<<faceRect->top<<"right:"<<faceRect->right<<"bottom"<<faceRect->bottom;
        }
    }

}

void CustomCameraViewFinder::answerUpdateUI(){
    update();
}
