#ifndef FACEDISCERN_H
#define FACEDISCERN_H

#include "fdengine.h"
#include "ftengine.h"
#include "frengine.h"
#include <QObject>
#include <cstdlib>
#include "convertimage.h"
#include <QDebug>

/**
 * @brief The FaceDiscern class
 * 人脸识别引擎封装类
 */
class FaceDiscern:public QObject
{
    Q_OBJECT
public:
    enum FailedType{
        FaceInitFailed,//引擎初始化错误，需关闭应用
        FaceDetectFailed,//引擎FD检测错误，不是内存错误MERR_NO_MEMORY就无所谓
        FaceTrackingFailed,//引擎FT检测错误，不是内存错误MERR_NO_MEMORY就无所谓
        FacePairMatchFailed,//引擎FR匹配错误，不是内存错误MERR_NO_MEMORY就无所谓
        FaceExtractFeatureFailed//引擎FR提取脸部特征错误，不是内存错误MERR_NO_MEMORY就无所谓
    };

    FaceDiscern(QObject *obj=nullptr);
    ~FaceDiscern();
    void engineInit(int &errorCode);

    LPAFD_FSDK_FACERES FDEngineDiscern(QImage &image,int &resultCode);

    LPAFT_FSDK_FACERES FTEngineDiscern(QImage &image,int &resultCode);

    AFR_FSDK_FACEINPUT getFREngineFaceInput(LPAFT_FSDK_FACERES lftFaceRes,int index=0);
    AFR_FSDK_FACEINPUT getFREngineFaceInput(LPAFD_FSDK_FACERES lfdFaceRes,int index=0);

    AFR_FSDK_FACEMODEL FREngineExtractFaceFeature(QImage &image, AFR_FSDK_FACEINPUT pFaceInput,int &resultCode);

    float FREngineCompareFaceFeature(AFR_FSDK_FACEMODEL *reffeature, AFR_FSDK_FACEMODEL *probefeature,int &resultCode);

private:
    FDEngine *fdEngine;
    FREngine *frEngine;
    FTEngine *ftEngine;

};

#endif // FACEENGINE_H
