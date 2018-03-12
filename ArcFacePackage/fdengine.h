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
 * 主要用于静态图片的人脸检测，检测结果保存在LPAFD_FSDK_FACERES结构体中
 */
class FDEngine
{
private:
    MInt32 nScale;//人脸相对于所在图片的长边占比范围[0,32]
    MInt32 nMaxFace;//检测到的最多人脸个数
    MByte *pWorkMem;//引擎工作内存地址
    MHandle hEngine;//引擎句柄
public:
    FDEngine();
    ~FDEngine();
    int init();
    int uninit();
    int FaceDetection(LPASVLOFFSCREEN pImgData, LPAFD_FSDK_FACERES *pFaceRes);
};

#endif // FDENGINE_H
