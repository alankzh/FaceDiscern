#ifndef FRENGINE_H
#define FRENGINE_H

#include "ArcFacePackage/merror.h"
#include "ArcFacePackage/arcfacedeploy.h"
#include "ArcFacePackage/arcsoft_fsdk_face_recognition.h"
#include <cstdlib>
#include <iostream>

#define WORKBUF_SIZE_FR (40*1024*1024)
/**
 * @brief The FREngine class
 * 人脸追踪引擎，主要用于提取脸部特征LPAFR_FSDK_FACEMODEL
 * 以及比对两个脸部特征
 */
class FREngine
{
private:
    MByte *pWorkMem;//引擎工作内存地址
    MHandle hEngine;//引擎句柄
public:
    FREngine();
    ~FREngine();
    int init();
    int uninit();
    int ExtractFRFeature(LPASVLOFFSCREEN pImgData, LPAFR_FSDK_FACEINPUT pFaceRes, LPAFR_FSDK_FACEMODEL pFaceModels);
    int FacePairMatching(AFR_FSDK_FACEMODEL *reffeature, AFR_FSDK_FACEMODEL *probefeature, MFloat *pfSimilScore);
};
#endif // FRENGINE_H
