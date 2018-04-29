#ifndef TERRANFACEFEATURE_H
#define TERRANFACEFEATURE_H

#include "arcsoft_fsdk_face_recognition.h"
#include "arcsoft_fsdk_face_detection.h"
#include "arcsoft_fsdk_face_tracking.h"
#include <cstdlib>
#include <cstring>
#include <QDebug>

/**
 * @brief The TerranFaceFeature class
 * FR引擎提取的脸部特征封装Bean类
 */
class TerranFaceFeature
{
public:
    TerranFaceFeature();
    ~TerranFaceFeature();
    TerranFaceFeature(const TerranFaceFeature &terranFaceFeature);
    TerranFaceFeature& operator=(const TerranFaceFeature &terranFaceFeature);

    void copyInjectFaceFeature(AFR_FSDK_FACEMODEL enginesFaceModelFeature);

    int id=0;
    AFR_FSDK_FACEMODEL frFaceModelFeature;
};

#endif // TERRANFACEFEATURE_H
