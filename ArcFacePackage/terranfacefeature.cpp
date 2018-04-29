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

    /* 拷贝人脸特征结果,以防下一次提取后，引擎中内容改变*/
    frFaceModelFeature.lFeatureSize = terranFaceFeature.frFaceModelFeature.lFeatureSize;
    frFaceModelFeature.pbFeature = (MByte*)malloc(terranFaceFeature.frFaceModelFeature.lFeatureSize);
    memcpy(frFaceModelFeature.pbFeature, terranFaceFeature.frFaceModelFeature.pbFeature, frFaceModelFeature.lFeatureSize);
}

TerranFaceFeature& TerranFaceFeature::operator=(const TerranFaceFeature &terranFaceFeature){
    id=terranFaceFeature.id;
    qDebug()<<"TerranFaceFeature::operator=";
    /* 拷贝人脸特征结果,以防下一次提取后，引擎中内容改变*/
    frFaceModelFeature.lFeatureSize = terranFaceFeature.frFaceModelFeature.lFeatureSize;
    frFaceModelFeature.pbFeature = (MByte*)malloc(terranFaceFeature.frFaceModelFeature.lFeatureSize);
    memcpy(frFaceModelFeature.pbFeature, terranFaceFeature.frFaceModelFeature.pbFeature, frFaceModelFeature.lFeatureSize);

    return *this;
}


/**
 * @brief TerranFaceFeature::copyInjectFaceFeature
 * 将引擎中的脸部特征拷贝出来
 * 如果不拷贝出来，用相同的引擎进行特征提取时会覆盖上一次得到的人脸特征信息
 * @param enginesFaceModelFeature
 */
void TerranFaceFeature::copyInjectFaceFeature(AFR_FSDK_FACEMODEL enginesFaceModelFeature){
    frFaceModelFeature.lFeatureSize = enginesFaceModelFeature.lFeatureSize;
    frFaceModelFeature.pbFeature = (MByte*)malloc(frFaceModelFeature.lFeatureSize);
    memcpy(frFaceModelFeature.pbFeature, enginesFaceModelFeature.pbFeature, frFaceModelFeature.lFeatureSize);
}

