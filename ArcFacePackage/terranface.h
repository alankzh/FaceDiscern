#ifndef TERRANFACE_H
#define TERRANFACE_H

#include "arcsoft_fsdk_face_recognition.h"
#include "arcsoft_fsdk_face_detection.h"
#include "arcsoft_fsdk_face_tracking.h"
#include <cstdlib>

/**
 * @brief The TerranFace class
 * 根据FR引擎需要，封装脸部矩形模型的Bean类
 * 只保存一张脸
 */
class TerranFace
{
public:
    TerranFace();
    ~TerranFace();
    int getId();
    void setId(unsigned int id);
    AFR_FSDK_FACEINPUT getFaceModel();
    void setFaceInputRect(LPAFT_FSDK_FACERES lftFaceRes,int index=0);
    void setFaceInputRect(LPAFD_FSDK_FACERES lfdFaceRes,int index=0);
private:
    int id;//所属脸的id
    AFR_FSDK_FACEINPUT frFaceInput;//人脸矩形
};

#endif // TERRANFACE_H
