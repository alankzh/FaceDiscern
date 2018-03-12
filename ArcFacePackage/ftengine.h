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
 * 主要用于视频帧中的人脸检测，检测结果保存在LPAFD_FSDK_FACERES结构体中
 * 与FDEngine相比，速度更快，内存更少
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
