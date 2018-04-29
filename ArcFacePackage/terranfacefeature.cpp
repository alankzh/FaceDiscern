#include "terranfacefeature.h"

TerranFaceFeature::TerranFaceFeature()
{
    id=0;
    frFaceModelFeature={0};
}

TerranFaceFeature::~TerranFaceFeature(){
//    qDebug()<<"TerranFaceFeature::~TerranFaceFeature and id:"<<id;
    if(frFaceModelFeature.pbFeature!=nullptr){
        std::free(frFaceModelFeature.pbFeature);
        frFaceModelFeature.pbFeature=nullptr;
    }
}

TerranFaceFeature::TerranFaceFeature(const TerranFaceFeature &terranFaceFeature){
    id=terranFaceFeature.id;

    /* ���������������,�Է���һ����ȡ�����������ݸı�*/
    frFaceModelFeature.lFeatureSize = terranFaceFeature.frFaceModelFeature.lFeatureSize;
    frFaceModelFeature.pbFeature = (MByte*)malloc(terranFaceFeature.frFaceModelFeature.lFeatureSize);
    memcpy(frFaceModelFeature.pbFeature, terranFaceFeature.frFaceModelFeature.pbFeature, frFaceModelFeature.lFeatureSize);
}

TerranFaceFeature& TerranFaceFeature::operator=(const TerranFaceFeature &terranFaceFeature){
    id=terranFaceFeature.id;
    qDebug()<<"TerranFaceFeature::operator=";
    /* ���������������,�Է���һ����ȡ�����������ݸı�*/
    frFaceModelFeature.lFeatureSize = terranFaceFeature.frFaceModelFeature.lFeatureSize;
    frFaceModelFeature.pbFeature = (MByte*)malloc(terranFaceFeature.frFaceModelFeature.lFeatureSize);
    memcpy(frFaceModelFeature.pbFeature, terranFaceFeature.frFaceModelFeature.pbFeature, frFaceModelFeature.lFeatureSize);

    return *this;
}


/**
 * @brief TerranFaceFeature::copyInjectFaceFeature
 * �������е�����������������
 * �������������������ͬ���������������ȡʱ�Ḳ����һ�εõ�������������Ϣ
 * @param enginesFaceModelFeature
 */
void TerranFaceFeature::copyInjectFaceFeature(AFR_FSDK_FACEMODEL enginesFaceModelFeature){
    frFaceModelFeature.lFeatureSize = enginesFaceModelFeature.lFeatureSize;
    frFaceModelFeature.pbFeature = (MByte*)malloc(frFaceModelFeature.lFeatureSize);
    memcpy(frFaceModelFeature.pbFeature, enginesFaceModelFeature.pbFeature, frFaceModelFeature.lFeatureSize);
}

