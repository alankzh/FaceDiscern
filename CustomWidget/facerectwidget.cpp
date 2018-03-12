#include "facerectwidget.h"

FaceRectWidget::FaceRectWidget(QWidget *parent):QWidget(parent){
    backgroundPix=QPixmap(width,height);
    backgroundPix.fill(QColor(0,0,0,0));
    rectColor=QColor(71,144,181);//���ο���ɫĬ��Ϊ��ɫ
    fontColor=QColor(255,255,255);//������ɫĬ��Ϊ��ɫ

    qtimer=new QTimer(this);
    qtimer->setInterval(animatorInterval);
    connect(qtimer,SIGNAL(timeout()),this,SLOT(animatorStatusChange()));
}

void FaceRectWidget::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
//    painter.drawPixmap(event->rect(),backgroundPix,event->rect());
    QPen rectPen;
    rectPen.setColor(rectColor);
    rectPen.setWidth(8);//Ĭ�Ͽ�8������
    painter.setPen(rectPen);

    /*�����Ľǵ���*/
    int lineLength=width*0.25;
    painter.drawLine(QPoint(left,top),QPoint(left+lineLength,top));
    painter.drawLine(QPoint(left,top),QPoint(left,top+lineLength));

    painter.drawLine(QPoint(right,top),QPoint(right-lineLength,top));
    painter.drawLine(QPoint(right,top),QPoint(right,top+lineLength));

    painter.drawLine(QPoint(left,bottom),QPoint(left+lineLength,bottom));
    painter.drawLine(QPoint(left,bottom),QPoint(left,bottom-lineLength));

    painter.drawLine(QPoint(right,bottom),QPoint(right-lineLength,bottom));
    painter.drawLine(QPoint(right,bottom),QPoint(right,bottom-lineLength));

    if(appearFontCount=0||terran.id<0){
        return;
    }else{
        int fontSize=height*0.1;//�������壬��ռ�߶ȱ���ʮ��֮3
        QFont font;
        font.setFamily(QString::fromLocal8Bit("����"));
        font.setPixelSize(fontSize);
        painter.setPen(fontColor);
        if(appearFontCount>=terran.name.size()){
            painter.drawText(QRect(left,top+height*0.35,width,fontSize),Qt::AlignCenter,terran.name);//��һ���� ����
            if(appearFontCount >= (terran.name.size()+terran.department.size())){
                painter.drawText(QRect(left,top+height*0.45+1,width,fontSize),Qt::AlignCenter,terran.department);//�ڶ����� ����

                QDateTime dateTime =QDateTime::currentDateTime();
                QString currentTime=dateTime.toString("hh:mm");
                if(appearFontCount>=(terran.name.size()+terran.department.size()+currentTime.size())){
                    painter.drawText(QRect(left,top+height*0.55+2,width,fontSize),Qt::AlignCenter,currentTime);//�������� ʱ��
                }else{
                    painter.drawText(QRect(left,top+height*0.55+2,width,fontSize),Qt::AlignCenter,currentTime.mid(0,appearFontCount-terran.name.size()-terran.department.size()));//�������� ʱ��
                }
            }else{
                painter.drawText(QRect(left,top+height*0.45+1,width,fontSize),Qt::AlignCenter,terran.department.mid(0,appearFontCount-terran.name.size()));//�ڶ����� ����
            }

        }else{
            painter.drawText(QRect(left,top+height*0.35,width,fontSize),Qt::AlignCenter,terran.name.mid(0,appearFontCount));//��һ����
        }
    }

}

void FaceRectWidget::startAppear(){
    if(animatorStatus==FaceAnimatorStatus::SizeInited){
        //        animatorStatus+=1;  c++11��׼֧�֣��ҵı�������msvc13����֧��c++11 ctm
        animatorStatus=FaceAnimatorStatus::AppearAnimatorStep1;
        animatorStatusChange();
        qtimer->start();
    }
}

void FaceRectWidget::startDisappear(){
    if(animatorStatus==FaceAnimatorStatus::Appeared){
        //        animatorStatus+=1;
        animatorStatus=FaceAnimatorStatus::DisApearedAnimatorStep1;
        animatorStatusChange();
        qtimer->start();
    }
    //��������״̬��Ӧ�����ظ�ɾ���������κα��
}

/**
 * @brief FaceRectWidget::animatorStatusChange
 * ����״̬�ı���߼�ִ��
 */
void FaceRectWidget::animatorStatusChange(){
    int scaleSize=0;
    int needShowFontSize=0;
    switch(animatorStatus){
    case FaceAnimatorStatus::SizeInited:
        qDebug()<<QString::fromLocal8Bit("���ο򶯻�״̬����");
        break;
    case FaceAnimatorStatus::AppearAnimatorStep1:
        rectColor=QColor(71,144,181,0);
        update();
        animatorStatus=FaceAnimatorStatus::AppearAnimatorStep2;
        break;
    case FaceAnimatorStatus::AppearAnimatorStep2:
        rectColor=QColor(71,144,181,153);//͸����Ϊ0.6
        scaleSize=width*0.05;//����Ϊԭ�߳���0.9
        left=left+scaleSize;
        right=right-scaleSize;
        top=top+scaleSize;
        bottom=bottom-scaleSize;
        sizeAdapter();
        update();
        animatorStatus=FaceAnimatorStatus::AppearAnimatorStep3;
        break;
    case FaceAnimatorStatus::AppearAnimatorStep3:
        rectColor=QColor(71,144,181,51);//͸����Ϊ0.2
        scaleSize=width*0.05555555;//����Ϊԭ�߳���0.8������width��Ϊԭ�߳���0.9
        left=left+scaleSize;
        right=right-scaleSize;
        top=top+scaleSize;
        bottom=bottom-scaleSize;
        sizeAdapter();
        update();
        animatorStatus=FaceAnimatorStatus::AppearAnimatorStep4;
        break;
    case FaceAnimatorStatus::AppearAnimatorStep4:
        rectColor=QColor(71,144,181,204);//͸����Ϊ0.8    ���Ų��䣬��Ϊ0.8
        update();
        animatorStatus=FaceAnimatorStatus::AppearAnimatorStep5;
        break;
    case FaceAnimatorStatus::AppearAnimatorStep5:
        rectColor=QColor(71,144,181,102);//͸����Ϊ0.4
        scaleSize=width*0.0625;//�Ŵ�Ϊԭ�߳���0.9������width��Ϊԭ�߳���0.8
        left=left-scaleSize;
        right=right+scaleSize;
        top=top-scaleSize;
        bottom=bottom+scaleSize;
        sizeAdapter();
        update();
        animatorStatus=FaceAnimatorStatus::AppearAnimatorStep6;
        break;
    case FaceAnimatorStatus::AppearAnimatorStep6:
        rectColor=QColor(71,144,181,255);//͸����Ϊ1
        scaleSize=width*0.05555555;//�Ŵ�Ϊԭ�߳�������width��Ϊԭ�߳���0.9
        left=left-scaleSize;
        right=right+scaleSize;
        top=top-scaleSize;
        bottom=bottom+scaleSize;
        sizeAdapter();
        update();
        animatorStatus=FaceAnimatorStatus::FontAppearStep;
        break;
    case FaceAnimatorStatus::FontAppearStep:
        if(terran.id<0){
            animatorStatus=FaceAnimatorStatus::Appeared;
        }else{
            needShowFontSize=terran.name.size()+terran.department.size()+5;//hh:mm ��ʱ���ʾ��ʽ��ʱ�䳤��Ϊ5
            if(appearFontCount>=needShowFontSize){
                fontColor=QColor(255,255,255);
                update();
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
                update();
                appearFontCount++;
            }
        }
        break;
    case FaceAnimatorStatus::Appeared:
        emit appeartAnimatorEnd(terran.id);
        qtimer->stop();
        break;
    case FaceAnimatorStatus::DisApearedAnimatorStep1:
        rectColor=QColor(71,144,181,255);//͸����Ϊ1
        fontColor=QColor(255,255,255);
        update();
        animatorStatus=FaceAnimatorStatus::DisApearedAnimatorStep2;
        break;
    case FaceAnimatorStatus::DisApearedAnimatorStep2:
        rectColor=QColor(71,144,181,102);//͸����Ϊ0.4
        fontColor=QColor(255,255,255,102);
        update();
        animatorStatus=FaceAnimatorStatus::DisApearedAnimatorStep3;
        break;
    case FaceAnimatorStatus::DisApearedAnimatorStep3:
        rectColor=QColor(71,144,181,204);//͸����Ϊ0.8
        fontColor=QColor(255,255,255,204);
        update();
        animatorStatus=FaceAnimatorStatus::DisApearedAnimatorStep4;
        break;
    case FaceAnimatorStatus::DisApearedAnimatorStep4:
        rectColor=QColor(71,144,181,51);//͸����Ϊ0.2
        fontColor=QColor(255,255,255,51);
        update();
        animatorStatus=FaceAnimatorStatus::DisApearedAnimatorStep5;
        break;
    case FaceAnimatorStatus::DisApearedAnimatorStep5:
        rectColor=QColor(71,144,181,153);//͸����Ϊ0.6
        fontColor=QColor(255,255,255,153);
        update();
        animatorStatus=FaceAnimatorStatus::DisApearedAnimatorStep6;
        break;
    case FaceAnimatorStatus::DisApearedAnimatorStep6:
        rectColor=QColor(71,144,181,0);//͸����Ϊ0
        fontColor=QColor(255,255,255,0);
        update();
        animatorStatus=FaceAnimatorStatus::DisApeared;
        break;
    case FaceAnimatorStatus::DisApeared:
        emit disappeartAnimatorEnd(terran.id);
        qtimer->stop();
        break;
    }
}

void FaceRectWidget::setTerran(Terran terran){
    this->terran=terran;
    int rectWidth=terran.frFaceInput.rcFace.right-terran.frFaceInput.rcFace.left;
    int rectHeight=terran.frFaceInput.rcFace.bottom-terran.frFaceInput.rcFace.top;

    //��֤���ο�Ϊ������,���ұ�Ĭ�ϵıߴ���0.25��,����˵�����βźÿ���
    if(rectWidth>=rectHeight){
        int widthZoomSize=rectWidth*0.125;
        this->left=terran.frFaceInput.rcFace.left-widthZoomSize;
        this->right=terran.frFaceInput.rcFace.right+widthZoomSize;

        this->top=terran.frFaceInput.rcFace.top-((rectWidth-rectHeight)*0.5)-widthZoomSize;
        this->bottom=terran.frFaceInput.rcFace.bottom+((rectWidth-rectHeight)*0.5)+widthZoomSize;
    }else{
        int heightZoomSize=rectHeight*0.125;
        this->top=terran.frFaceInput.rcFace.top-heightZoomSize;
        this->bottom=terran.frFaceInput.rcFace.bottom+heightZoomSize;

        this->left=terran.frFaceInput.rcFace.left-((rectHeight-rectWidth)*0.5)-heightZoomSize;
        this->right=terran.frFaceInput.rcFace.right+((rectHeight-rectWidth)*0.5)+heightZoomSize;
    }

    sizeAdapter();
    update();
}

int FaceRectWidget::getTerranId(){
    return terran.id;
}

void FaceRectWidget::sizeAdapter(){
    width=right-left;
    height=bottom-top;
    resize(width,height);
    setFixedSize(width,height);
    backgroundPix=QPixmap(width,height);
    backgroundPix.fill(QColor(0,0,0,0));
}
