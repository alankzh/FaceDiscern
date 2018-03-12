#ifndef FDENGINE_H
#define FDENGINE_H

#include "ArcFacePackage/merror.h"
#include "ArcFacePackage/arcsoft_fsdk_face_detection.h"
#include "ArcFacePackage/arcfacedeploy.h"
#include <cstdlib>
#include <iostream>

#define WORKBUF_SIZE_FD (20*1024*1024)
/**
 * @brief The FDEngine class
 * ��Ҫ���ھ�̬ͼƬ��������⣬�����������LPAFD_FSDK_FACERES�ṹ����
 */
class FDEngine
{
private:
    MInt32 nScale;//�������������ͼƬ�ĳ���ռ�ȷ�Χ[0,32]
    MInt32 nMaxFace;//��⵽�������������
    MByte *pWorkMem;//���湤���ڴ��ַ
    MHandle hEngine;//������
public:
    FDEngine();
    ~FDEngine();
    int init();
    int uninit();
    int FaceDetection(LPASVLOFFSCREEN pImgData, LPAFD_FSDK_FACERES *pFaceRes);
};

#endif // FDENGINE_H
