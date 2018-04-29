#include "facepainterrect.h"

FacePainterRect::FacePainterRect(QWidget *parent):QWidget(parent)
{
    //    rectColor=QColor(71,144,181);//Ĭ��Ϊ��ɫ
    //    fontColor=QColor(255,255,255);//Ĭ��Ϊ��ɫ

    rectColor=QColor(255,255,255);//Ĭ��Ϊ��ɫ
    //    fontColor=QColor(71,144,181);//Ĭ��Ϊ��ɫ
    fontColor=QColor(23,213,230);//����ɫ
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
 * ����״̬�ı���߼�ִ��
 */
void FacePainterRect::animatorStatusChange(){
    int scaleSize=0;
    int needShowFontSize=0;
    switch(animatorStatus){
    case FaceAnimatorStatus::SizeInited:
        qDebug()<<QString::fromLocal8Bit("���ο򶯻�״̬����");
        break;
    case FaceAnimatorStatus::AppearAnimatorStep1:
        //        rectColor=QColor(71,144,181,0);
        rectColor=QColor(255,255,255,0);
        emit askUpdateUI();
        animatorStatus=FaceAnimatorStatus::AppearAnimatorStep2;
        break;
    case FaceAnimatorStatus::AppearAnimatorStep2:
        //        rectColor=QColor(71,144,181,153);//͸����Ϊ0.6
        rectColor=QColor(255,255,255,153);//͸����Ϊ0.6
        scaleSize=width*0.05;//����Ϊԭ�߳���0.9
        left=left+scaleSize;
        right=right-scaleSize;
        top=top+scaleSize;
        bottom=bottom-scaleSize;
        sizeAdapter();
        emit askUpdateUI();
        animatorStatus=FaceAnimatorStatus::AppearAnimatorStep3;
        break;
    case FaceAnimatorStatus::AppearAnimatorStep3:
        //        rectColor=QColor(71,144,181,51);//͸����Ϊ0.2
        rectColor=QColor(255,255,255,51);//͸����Ϊ0.2
        scaleSize=width*0.05555555;//����Ϊԭ�߳���0.8������width��Ϊԭ�߳���0.9
        left=left+scaleSize;
        right=right-scaleSize;
        top=top+scaleSize;
        bottom=bottom-scaleSize;
        sizeAdapter();
        emit askUpdateUI();
        animatorStatus=FaceAnimatorStatus::AppearAnimatorStep4;
        break;
    case FaceAnimatorStatus::AppearAnimatorStep4:
        //        rectColor=QColor(71,144,181,204);//͸����Ϊ0.8    ���Ų��䣬��Ϊ0.8
        rectColor=QColor(255,255,255,204);//͸����Ϊ0.8    ���Ų��䣬��Ϊ0.8
        emit askUpdateUI();
        animatorStatus=FaceAnimatorStatus::AppearAnimatorStep5;
        break;
    case FaceAnimatorStatus::AppearAnimatorStep5:
        //        rectColor=QColor(71,144,181,102);//͸����Ϊ0.4
        rectColor=QColor(255,255,255,102);//͸����Ϊ0.4
        scaleSize=width*0.0625;//�Ŵ�Ϊԭ�߳���0.9������width��Ϊԭ�߳���0.8
        left=left-scaleSize;
        right=right+scaleSize;
        top=top-scaleSize;
        bottom=bottom+scaleSize;
        sizeAdapter();
        emit askUpdateUI();
        animatorStatus=FaceAnimatorStatus::AppearAnimatorStep6;
        break;
    case FaceAnimatorStatus::AppearAnimatorStep6:
        //        rectColor=QColor(71,144,181,255);//͸����Ϊ1
        rectColor=QColor(255,255,255,255);//͸����Ϊ1
        scaleSize=width*0.05555555;//�Ŵ�Ϊԭ�߳�������width��Ϊԭ�߳���0.9
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
            needShowFontSize=terran.name.size()+terran.department.size()+5;//hh:mm ��ʱ���ʾ��ʽ��ʱ�䳤��Ϊ5
            if(appearFontCount>=needShowFontSize){
                //                fontColor=QColor(255,255,255);
                //                fontColor=QColor(71,144,181);
                fontColor=QColor(23,213,230);
                emit askUpdateUI();
                animatorStatus=FaceAnimatorStatus::Appeared;
            }else{
                if(appearFontCount==0){
                    //                    fontColor=QColor(255,255,255,0);
                    //                    fontColor=QColor(71,144,181,0);
                    fontColor=QColor(23,213,230,0);
                }else if(appearFontCount==1){
                    //                    fontColor=QColor(255,255,255,153);
                    //                    fontColor=QColor(71,144,181,153);
                    fontColor=QColor(23,213,230,153);
                }else if(appearFontCount==2){
                    //                    fontColor=QColor(255,255,255,51);
                    //                    fontColor=QColor(71,144,181,51);

                    fontColor=QColor(23,213,230,51);
                }else if(appearFontCount==3){
                    //                    fontColor=QColor(255,255,255,204);
                    //                    fontColor=QColor(71,144,181,204);

                    fontColor=QColor(23,213,230,204);
                }else if(appearFontCount==4){
                    //                    fontColor=QColor(255,255,255,102);
                    //                    fontColor=QColor(71,144,181,102);

                    fontColor=QColor(23,213,230,102);
                }else if(appearFontCount==5){
                    //                    fontColor=QColor(255,255,255);
                    //                    fontColor=QColor(71,144,181);
                    fontColor=QColor(23,213,230);
                }else{
                    //                    fontColor=QColor(255,255,255);
                    //                    fontColor=QColor(71,144,181);

                    fontColor=QColor(23,213,230);
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
        //        rectColor=QColor(71,144,181,255);//͸����Ϊ1
        //        fontColor=QColor(255,255,255);
        fontColor=QColor(71,144,181,255);//͸����Ϊ1
        rectColor=QColor(255,255,255);
        emit askUpdateUI();
        animatorStatus=FaceAnimatorStatus::DisApearedAnimatorStep2;
        break;
    case FaceAnimatorStatus::DisApearedAnimatorStep2:
        //        rectColor=QColor(71,144,181,102);//͸����Ϊ0.4
        //        fontColor=QColor(255,255,255,102);
        fontColor=QColor(71,144,181,102);//͸����Ϊ0.4
        rectColor=QColor(255,255,255,102);
        emit askUpdateUI();
        animatorStatus=FaceAnimatorStatus::DisApearedAnimatorStep3;
        break;
    case FaceAnimatorStatus::DisApearedAnimatorStep3:
        //        rectColor=QColor(71,144,181,204);//͸����Ϊ0.8
        //        fontColor=QColor(255,255,255,204);
        fontColor=QColor(71,144,181,204);//͸����Ϊ0.8
        rectColor=QColor(255,255,255,204);
        emit askUpdateUI();
        animatorStatus=FaceAnimatorStatus::DisApearedAnimatorStep4;
        break;
    case FaceAnimatorStatus::DisApearedAnimatorStep4:
        //        rectColor=QColor(71,144,181,51);//͸����Ϊ0.2
        //        fontColor=QColor(255,255,255,51);
        fontColor=QColor(71,144,181,51);//͸����Ϊ0.2
        rectColor=QColor(255,255,255,51);
        emit askUpdateUI();
        animatorStatus=FaceAnimatorStatus::DisApearedAnimatorStep5;
        break;
    case FaceAnimatorStatus::DisApearedAnimatorStep5:
        //        rectColor=QColor(71,144,181,153);//͸����Ϊ0.6
        //        fontColor=QColor(255,255,255,153);
        fontColor=QColor(71,144,181,153);//͸����Ϊ0.6
        rectColor=QColor(255,255,255,153);
        emit askUpdateUI();
        animatorStatus=FaceAnimatorStatus::DisApearedAnimatorStep6;
        break;
    case FaceAnimatorStatus::DisApearedAnimatorStep6:
        //        rectColor=QColor(71,144,181,0);//͸����Ϊ0
        //        fontColor=QColor(255,255,255,0);
        fontColor=QColor(71,144,181,0);//͸����Ϊ0
        rectColor=QColor(255,255,255,0);
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
        //        animatorStatus+=1;  c++11��׼֧�֣��ҵı�������msvc13����֧��c++11 ctm
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
 * ���ο�ᱣ��Ϊ�����Σ����һ���һ���Ŵ�
 * @param terran
 */
void FacePainterRect::setTerran(Terran terran){
    this->terran=terran;

    this->left=terran.frFaceInput.rcFace.left*1.8;
    this->top=terran.frFaceInput.rcFace.top*1.8;
    this->right=terran.frFaceInput.rcFace.right*1.8;
    this->bottom=terran.frFaceInput.rcFace.bottom*1.8;

    //��֤���ο�Ϊ������,���ұ�Ĭ�ϵıߴ���0.25��,����˵�����βźÿ���
    int rectWidth=(right-left)*1.25;
    int rectHeight=(bottom-top)*1.25;

    //��֤���ο���̱߳�Ϊ80
    if(rectWidth<=150&&rectHeight<=150){
        rectWidth=150;
    }

    int midX=(right+left)*0.5;//���ĵ�x����
    int midY=(bottom+top)*0.5;//���ĵ�y����
    if(rectWidth>=rectHeight){
        int halfRectLength=(rectWidth*0.5);

        this->left=midX-halfRectLength;
        this->right=midX+halfRectLength;
        this->top=midY-halfRectLength;
        this->bottom=midY+halfRectLength;
    }else{
        int halfRectLength=(rectHeight*0.5);

        this->left=midX-halfRectLength;
        this->right=midX+halfRectLength;
        this->top=midY-halfRectLength;
        this->bottom=midY+halfRectLength;
    }

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
