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
 * ����ʶ�������װ��
 */
class FaceDiscern:public QObject
{
    Q_OBJECT
public:
    enum FailedType{
        FaceInitFailed,//�����ʼ��������ر�Ӧ��
        FaceDetectFailed,//����FD�����󣬲����ڴ����MERR_NO_MEMORY������ν
        FaceTrackingFailed,//����FT�����󣬲����ڴ����MERR_NO_MEMORY������ν
        FacePairMatchFailed,//����FRƥ����󣬲����ڴ����MERR_NO_MEMORY������ν
        FaceExtractFeatureFailed//����FR��ȡ�����������󣬲����ڴ����MERR_NO_MEMORY������ν
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
