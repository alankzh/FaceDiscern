#ifndef FTENGINE_H
#define FTENGINE_H

#include "ArcFacePackage/arcfacedeploy.h"
#include "ArcFacePackage/merror.h"
#include "ArcFacePackage/arcsoft_fsdk_face_tracking.h"
#include <cstdlib>
#include <iostream>

#define WORKBUF_SIZE_FT (30*1024*1024)
/**
 * @brief The FTEngine class
 * ��Ҫ������Ƶ֡�е�������⣬�����������LPAFD_FSDK_FACERES�ṹ����
 * ��FDEngine��ȣ��ٶȸ��죬�ڴ����
 */
class FTEngine
{
private:
    MInt32 nScale;
    MInt32 nMaxFace;
    MByte *pWorkMem;
    MHandle hEngine;
public:
    FTEngine();
    ~FTEngine();
    int init();
    int uninit();
    int FaceTracking(LPASVLOFFSCREEN pImgData, LPAFT_FSDK_FACERES *pFaceRes);
};

#endif // FTENGINE_H
