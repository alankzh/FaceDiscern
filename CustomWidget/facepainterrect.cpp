#include "facepainterrect.h"

FacePainterRect::FacePainterRect(QWidget *parent):QWidget(parent)
{
    rectColor=QColor(71,144,181);
    fontColor=QColor(255,255,255);
    int width=0;
    int height=0;
    int left=0;
    int right=0;
    int top=0;
    int bottom=0;

    qtimer=new QTimer(this);
    qtimer->setInterval(ANIMATOR_INTERVAL);
    connect(qtimer,SIGNAL(timeout()),this,SLOT(animatorStatusChange()));
}

/**
 * @brief FaceRectWidget::animatorStatusChange
 * 动画状态改变的逻辑执行
 */
void FacePainterRect::animatorStatusChange(){
    int scaleSize=0;
    int needShowFontSize=0;
    switch(animatorStatus){
    case FaceAnimatorStatus::SizeInited:
        qDebug()<<QString::fromLocal8Bit("矩形框动画状态错误");
        break;
    case FaceAnimatorStatus::AppearAnimatorStep1:
        rectColor=QColor(71,144,181,0);
        emit askUpdateUI();
        animatorStatus=FaceAnimatorStatus::AppearAnimatorStep2;
        break;
    case FaceAnimatorStatus::AppearAnimatorStep2:
        rectColor=QColor(71,144,181,153);//透明度为0.6
        scaleSize=width*0.05;//缩放为原边长的0.9
        left=left+scaleSize;
        right=right-scaleSize;
        top=top+scaleSize;
        bottom=bottom-scaleSize;
        sizeAdapter();
        emit askUpdateUI();
        animatorStatus=FaceAnimatorStatus::AppearAnimatorStep3;
        break;
    case FaceAnimatorStatus::AppearAnimatorStep3:
        rectColor=QColor(71,144,181,51);//透明度为0.2
        scaleSize=width*0.05555555;//缩放为原边长的0.8，这里width已为原边长的0.9
        left=left+scaleSize;
        right=right-scaleSize;
        top=top+scaleSize;
        bottom=bottom-scaleSize;
        sizeAdapter();
        emit askUpdateUI();
        animatorStatus=FaceAnimatorStatus::AppearAnimatorStep4;
        break;
    case FaceAnimatorStatus::AppearAnimatorStep4:
        rectColor=QColor(71,144,181,204);//透明度为0.8    缩放不变，仍为0.8
        emit askUpdateUI();
        animatorStatus=FaceAnimatorStatus::AppearAnimatorStep5;
        break;
    case FaceAnimatorStatus::AppearAnimatorStep5:
        rectColor=QColor(71,144,181,102);//透明度为0.4
        scaleSize=width*0.0625;//放大为原边长的0.9，这里width已为原边长的0.8
        left=left-scaleSize;
        right=right+scaleSize;
        top=top-scaleSize;
        bottom=bottom+scaleSize;
        sizeAdapter();
        emit askUpdateUI();
        animatorStatus=FaceAnimatorStatus::AppearAnimatorStep6;
        break;
    case FaceAnimatorStatus::AppearAnimatorStep6:
        rectColor=QColor(71,144,181,255);//透明度为1
        scaleSize=width*0.05555555;//放大为原边长，这里width已为原边长的0.9
        left=left-scaleSize;
        right=right+scaleSize;
        top=top-scaleSize;
        bottom=bottom+scaleSize;
        sizeAdapter();
        emit askUpdateUI();
        animatorStatus=FaceAnimatorStatus::FontAppearStep;
        break;
    case FaceAnimatorStatus::FontAppearStep:
        if(terran.id<0){
            animatorStatus=FaceAnimatorStatus::Appeared;
        }else{
            needShowFontSize=terran.name.size()+terran.department.size()+5;//hh:mm 的时间表示方式，时间长度为5
            if(appearFontCount>=needShowFontSize){
                fontColor=QColor(255,255,255);
                emit askUpdateUI();
                animatorStatus=FaceAnimatorStatus::Appeared;
            }else{
                if(appearFontCount==0){
                    fontColor=QColor(255,255,255,0);
                }else if(appearFontCount==1){
                    fontColor=QColor(255,255,255,153);
                }else if(appearFontCount==2){
                    fontColor=QColor(255,255,255,51);
                }else if(appearFontCount==3){
                    fontColor=QColor(255,255,255,204);
                }else if(appearFontCount==4){
                    fontColor=QColor(255,255,255,102);
                }else if(appearFontCount==5){
                    fontColor=QColor(255,255,255);
                }else{
                    fontColor=QColor(255,255,255);
                }
                emit askUpdateUI();
                appearFontCount++;
            }
        }
        break;
    case FaceAnimatorStatus::Appeared:
        emit appeartAnimatorEnd(terran.id);
        qtimer->stop();
        break;
    case FaceAnimatorStatus::DisApearedAnimatorStep1:
        rectColor=QColor(71,144,181,255);//透明度为1
        fontColor=QColor(255,255,255);
        emit askUpdateUI();
        animatorStatus=FaceAnimatorStatus::DisApearedAnimatorStep2;
        break;
    case FaceAnimatorStatus::DisApearedAnimatorStep2:
        rectColor=QColor(71,144,181,102);//透明度为0.4
        fontColor=QColor(255,255,255,102);
        emit askUpdateUI();
        animatorStatus=FaceAnimatorStatus::DisApearedAnimatorStep3;
        break;
    case FaceAnimatorStatus::DisApearedAnimatorStep3:
        rectColor=QColor(71,144,181,204);//透明度为0.8
        fontColor=QColor(255,255,255,204);
        emit askUpdateUI();
        animatorStatus=FaceAnimatorStatus::DisApearedAnimatorStep4;
        break;
    case FaceAnimatorStatus::DisApearedAnimatorStep4:
        rectColor=QColor(71,144,181,51);//透明度为0.2
        fontColor=QColor(255,255,255,51);
        emit askUpdateUI();
        animatorStatus=FaceAnimatorStatus::DisApearedAnimatorStep5;
        break;
    case FaceAnimatorStatus::DisApearedAnimatorStep5:
        rectColor=QColor(71,144,181,153);//透明度为0.6
        fontColor=QColor(255,255,255,153);
        emit askUpdateUI();
        animatorStatus=FaceAnimatorStatus::DisApearedAnimatorStep6;
        break;
    case FaceAnimatorStatus::DisApearedAnimatorStep6:
        rectColor=QColor(71,144,181,0);//透明度为0
        fontColor=QColor(255,255,255,0);
        emit askUpdateUI();
        animatorStatus=FaceAnimatorStatus::DisApeared;
        break;
    case FaceAnimatorStatus::DisApeared:
        emit disappeartAnimatorEnd(terran.id);
        qtimer->stop();
        break;
    }
}


void FacePainterRect::startAppearAnimator(){
    if(animatorStatus==FaceAnimatorStatus::SizeInited){
        //        animatorStatus+=1;  c++11标准支持，我的编译器是msvc13，不支持c++11 ctm
        animatorStatus=FaceAnimatorStatus::AppearAnimatorStep1;
        animatorStatusChange();
        qtimer->start();
    }
}

void FacePainterRect::startDisappearAnimator(){
    animatorStatus=FaceAnimatorStatus::DisApeared;
    qtimer->stop();
    emit disappeartAnimatorEnd(terran.id);
    //    if(animatorStatus==FaceAnimatorStatus::Appeared){
    //        //        animatorStatus+=1;
    //        animatorStatus=FaceAnimatorStatus::DisApearedAnimatorStep1;
    //        animatorStatusChange();
    //        qtimer->start();
    //    }else{
    //        qtimer->stop();
    //        qtimer->start();
    //        animatorStatus=FaceAnimatorStatus::DisApearedAnimatorStep1;
    //    }

}

/**
 * @brief FacePainterRect::setTerran
 * 矩形框会保持为正方形，并且会有一个放大
 * @param terran
 */
void FacePainterRect::setTerran(Terran terran){
    this->terran=terran;

    this->left=terran.frFaceInput.rcFace.left;
    this->top=terran.frFaceInput.rcFace.top;
    this->right=terran.frFaceInput.rcFace.right;
    this->bottom=terran.frFaceInput.rcFace.bottom;

    //    int rectWidth=terran.frFaceInput.rcFace.right-terran.frFaceInput.rcFace.left;
    //    int rectHeight=terran.frFaceInput.rcFace.bottom-terran.frFaceInput.rcFace.top;

    //    //保证矩形框为正方形,并且比默认的边大其0.25倍,丽桃说正方形才好看！
    //    if(rectWidth>=rectHeight){
    //        int widthZoomSize=rectWidth*0.125;
    //        this->left=terran.frFaceInput.rcFace.left-widthZoomSize;
    //        this->right=terran.frFaceInput.rcFace.right+widthZoomSize;

    //        this->top=terran.frFaceInput.rcFace.top-((rectWidth-rectHeight)*0.5)-widthZoomSize;
    //        this->bottom=terran.frFaceInput.rcFace.bottom+((rectWidth-rectHeight)*0.5)+widthZoomSize;
    //    }else{
    //        int heightZoomSize=rectHeight*0.125;
    //        this->top=terran.frFaceInput.rcFace.top-heightZoomSize;
    //        this->bottom=terran.frFaceInput.rcFace.bottom+heightZoomSize;

    //        this->left=terran.frFaceInput.rcFace.left-((rectHeight-rectWidth)*0.5)-heightZoomSize;
    //        this->right=terran.frFaceInput.rcFace.right+((rectHeight-rectWidth)*0.5)+heightZoomSize;
    //    }

    sizeAdapter();
    emit askUpdateUI();
}

int FacePainterRect::getTerranId(){
    return terran.id;
}

Terran FacePainterRect::getTerran(){
    return terran;
}

void FacePainterRect::sizeAdapter(){
    width=right-left;
    height=bottom-top;
}
