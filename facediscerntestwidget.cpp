#include "facediscerntestwidget.h"

FaceDiscernTestWidget::FaceDiscernTestWidget(QWidget *parent):QWidget(parent)
{
    faceDiscern=new FaceDiscern();
    int errorCode=0;
    faceDiscern->engineInit(errorCode);

    QPushButton *pushButton=new QPushButton(this);
    pushButton->move(200,200);
    connect(pushButton,SIGNAL(clicked(bool)),this,SLOT(startPair(bool)));
}

void FaceDiscernTestWidget::startPair(bool){
    int errorCode=0;
    QImage xiaofang("E:\\xiaofang.jpg");
    QImage meijie("E:\\meijie.jpg");

    //    LPAFT_FSDK_FACERES xiaofangFtRes=faceDiscern->FTEngineDiscern(xiaofang,errorCode);

    //    LPAFT_FSDK_FACERES meijieFtRes=faceDiscern->FTEngineDiscern(meijie,errorCode);

    LPAFD_FSDK_FACERES xiaofangFtRes=faceDiscern->FDEngineDiscern(xiaofang,errorCode);

    LPAFD_FSDK_FACERES meijieFtRes=faceDiscern->FDEngineDiscern(meijie,errorCode);

    TerranFaceFeature xiaofangFeature;
    AFR_FSDK_FACEINPUT xiaofangInput=faceDiscern->getFREngineFaceInput(xiaofangFtRes);
    xiaofangFeature.frFaceModelFeature=faceDiscern->FREngineExtractFaceFeature(xiaofang,xiaofangInput,errorCode);

    TerranFaceFeature meijieFeature;
    AFR_FSDK_FACEINPUT meijieInput=faceDiscern->getFREngineFaceInput(meijieFtRes);
    meijieFeature.frFaceModelFeature=faceDiscern->FREngineExtractFaceFeature(meijie,meijieInput,errorCode);

    float similarity=0.0;
    similarity=faceDiscern->FREngineCompareFaceFeature(&(xiaofangFeature.frFaceModelFeature),&(meijieFeature.frFaceModelFeature),errorCode);

    qDebug()<<QString::fromLocal8Bit("ÏàËÆ¶È:")<<similarity;
}
