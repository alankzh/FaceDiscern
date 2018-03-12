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
 * ����׷�����棬��Ҫ������ȡ��������LPAFR_FSDK_FACEMODEL
 * �Լ��ȶ�������������
 */
class FREngine
{
private:
    MByte *pWorkMem;//���湤���ڴ��ַ
    MHandle hEngine;//������
public:
    FREngine();
    ~FREngine();
    int init();
    int uninit();
    int ExtractFRFeature(LPASVLOFFSCREEN pImgData, LPAFR_FSDK_FACEINPUT pFaceRes, LPAFR_FSDK_FACEMODEL pFaceModels);
    int FacePairMatching(AFR_FSDK_FACEMODEL *reffeature, AFR_FSDK_FACEMODEL *probefeature, MFloat *pfSimilScore);
};
#endif // FRENGINE_H
