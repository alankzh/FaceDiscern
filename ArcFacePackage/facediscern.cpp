#include "facediscern.h"

FaceDiscern::FaceDiscern(QObject *obj):QObject(obj)
{

}

FaceDiscern::~FaceDiscern(){
    if(fdEngine!=nullptr){
        fdEngine->uninit();
        delete fdEngine;
        fdEngine=nullptr;
    }
    if(frEngine!=nullptr){
        frEngine->uninit();
        delete frEngine;
        frEngine=nullptr;
    }
    if(ftEngine!=nullptr){
        ftEngine->uninit();
        delete ftEngine;
        ftEngine=nullptr;
    }
}

void FaceDiscern::engineInit(int &errorCode){

    fdEngine=new FDEngine();
    errorCode=fdEngine->init();
    if(errorCode!=MOK){
        qDebug()<<"FD init failed";
        return;
    }
    frEngine=new FREngine();
    errorCode=frEngine->init();
    if(errorCode!=MOK){
        qDebug()<<"FR init failed";
        return;
    }
    ftEngine=new FTEngine();
    errorCode=ftEngine->init();
    if(errorCode!=MOK){
        qDebug()<<"FT init failed";
        return;
    }
}


/**
 * @brief FaceDiscern::FDEngineDiscern
 * ʹ��FD�����������ʶ��
 * @param image
 * �����QImageͼ������
 * @param resultCode
 * ����ʶ�𷵻���
 * @return
 * ����FD�����װ��ʶ����LPAFT_FSDK_FACERES
 */
LPAFD_FSDK_FACERES FaceDiscern::FDEngineDiscern(QImage &image,int &resultCode){
    // ���������
    LPAFD_FSDK_FACERES FaceRes = nullptr;
    // ����������Ҫ��ͼ����������
    ASVLOFFSCREEN offInput = { 0 };

    offInput.u32PixelArrayFormat = ASVL_PAF_RGB24_B8G8R8;
    offInput.ppu8Plane[0] = nullptr;

    bool readed=ConvertImage::readBmp24(image, (uint8_t**)&offInput.ppu8Plane[0], &offInput.i32Width, &offInput.i32Height);

    offInput.pi32Pitch[0] = offInput.i32Width * 3;
    if (!offInput.ppu8Plane[0]||!readed)  {
        //        qDebug()<<"��ȡ����ʧ��";
        resultCode=-1;//������-1���ں����merror.h����֮��
        std::free(offInput.ppu8Plane[0]);
        return FaceRes;
    }  else  {
        //        qDebug()<<"��ȡ�����ɹ�";
    }

    resultCode = fdEngine->FaceDetection(&offInput,&FaceRes);
    if (resultCode != MOK)  {
        qDebug()<<QString::fromLocal8Bit("FD ʶ����󣬴�����:")<<resultCode;
        std::free(offInput.ppu8Plane[0]);
        return FaceRes;
    }  else  {
                qDebug()<<QString::fromLocal8Bit("FD ʶ��ɹ�");
        for (int i = 0; i < FaceRes->nFace; ++i){
//            qDebug()<<"face num:"<<i+1<<" left:"<<FaceRes->rcFace[i].left<<" top:"<<FaceRes->rcFace[i].top<<" right:"<<FaceRes->rcFace[i].right<<" bottom:"<<FaceRes->rcFace[i].bottom<<" ��������:"<<FaceRes->lfaceOrient[i]<<"\n";
        }
    }

    std::free(offInput.ppu8Plane[0]);

    return FaceRes;
}

/**
 * @brief FaceDiscern::FTEngineDiscern
 * ʹ��FR�����������ʶ��
 * @param image
 * �����QImageͼ������
 * @param resultCode
 * ����ʶ�𷵻���
 * @return
 */
LPAFT_FSDK_FACERES FaceDiscern::FTEngineDiscern(QImage &image, int &resultCode){
    // ���������
    LPAFT_FSDK_FACERES  FaceRes = nullptr;
    // ����������Ҫ��ͼ����������
    ASVLOFFSCREEN offInput = { 0 };

    offInput.u32PixelArrayFormat = ASVL_PAF_RGB24_B8G8R8;
    offInput.ppu8Plane[0] = nullptr;

    bool readed=ConvertImage::readBmp24(image, (uint8_t**)&offInput.ppu8Plane[0], &offInput.i32Width, &offInput.i32Height);

    offInput.pi32Pitch[0] = offInput.i32Width * 3;
    if (!offInput.ppu8Plane[0]||!readed)  {
        qDebug()<<QString::fromLocal8Bit("FT ��ȡ����ʧ��");
        resultCode=-1;//������-1���ں����merror.h����֮��
        std::free(offInput.ppu8Plane[0]);
        return FaceRes;
    }  else  {
//        qDebug()<<QString::fromLocal8Bit("FT ��ȡ�����ɹ�");
    }

    resultCode = ftEngine->FaceTracking(&offInput,&FaceRes);
    if (resultCode != MOK)  {
        qDebug()<<"ʶ����󣬴�����:"<<resultCode;
        std::free(offInput.ppu8Plane[0]);
        return FaceRes;
    }  else  {
        qDebug()<<QString::fromLocal8Bit("FT ʶ��ɹ�");
        for (int i = 0; i < FaceRes->nFace; ++i){
            qDebug()<<"face num:"<<i+1<<" left:"<<FaceRes->rcFace[i].left<<" top:"<<FaceRes->rcFace[i].top<<" right:"<<FaceRes->rcFace[i].right<<" bottom:"<<FaceRes->rcFace[i].bottom<<" orientation:"<<FaceRes->lfaceOrient<<"\n";
        }
    }

    std::free(offInput.ppu8Plane[0]);

    return FaceRes;
}

/**
 * @brief FaceDiscern::getFREngineFaceInput
 * ��fd�����ʶ����ת��Ϊfr������Ҫ������
 * @param lfdFaceRes
 * @param index
 * @return
 */
AFR_FSDK_FACEINPUT FaceDiscern::getFREngineFaceInput(LPAFD_FSDK_FACERES lfdFaceRes, int index){
    AFR_FSDK_FACEINPUT frFaceInput;
    frFaceInput.lOrient = (_AFR_FSDK_OrientCode)lfdFaceRes->lfaceOrient[index];//��������
    //������λ��
    frFaceInput.rcFace.left = lfdFaceRes->rcFace[index].left;
    frFaceInput.rcFace.top = lfdFaceRes->rcFace[index].top;
    frFaceInput.rcFace.right = lfdFaceRes->rcFace[index].right;
    frFaceInput.rcFace.bottom = lfdFaceRes->rcFace[index].bottom;
    return frFaceInput;
}

/**
 * @brief FaceDiscern::getFREngineFaceInput
 * ��ft�����ʶ����ת��Ϊfr������Ҫ������
 * @param lftFaceRes
 * @param index
 * @return
 */
AFR_FSDK_FACEINPUT FaceDiscern::getFREngineFaceInput(LPAFT_FSDK_FACERES lftFaceRes, int index){
    AFR_FSDK_FACEINPUT frFaceInput;
    frFaceInput.lOrient = (_AFR_FSDK_OrientCode)lftFaceRes->lfaceOrient;//��������
    //������λ��
    frFaceInput.rcFace.left = lftFaceRes->rcFace[index].left;
    frFaceInput.rcFace.top = lftFaceRes->rcFace[index].top;
    frFaceInput.rcFace.right = lftFaceRes->rcFace[index].right;
    frFaceInput.rcFace.bottom = lftFaceRes->rcFace[index].bottom;
    return frFaceInput;
}

/**
 * @brief FaceDiscern::FREngineExtractFaceFeature
 * ��ȡͼƬ����������
 * @param image
 * ͼƬ����
 * @param FaceInput
 * FT��FR������ʶ������ת��ΪFR���������
 * @param resultCode
 * ������
 * @return
 */
AFR_FSDK_FACEMODEL FaceDiscern::FREngineExtractFaceFeature(QImage &image, AFR_FSDK_FACEINPUT FaceInput,int &resultCode){
    AFR_FSDK_FACEMODEL faceFeature = { 0 };
    AFR_FSDK_FACEMODEL localFaceFeature = { 0 };
    // ����������Ҫ��ͼ����������
    ASVLOFFSCREEN offInput = { 0 };
    offInput.u32PixelArrayFormat = ASVL_PAF_RGB24_B8G8R8;
    offInput.ppu8Plane[0] = nullptr;
    bool readed=ConvertImage::readBmp24(image, (uint8_t**)&offInput.ppu8Plane[0], &offInput.i32Width, &offInput.i32Height);
    offInput.pi32Pitch[0] = offInput.i32Width * 3;

    if (!offInput.ppu8Plane[0]||!readed)  {
        qDebug()<<QString::fromLocal8Bit("��ȡͼƬʧ��");
        resultCode=-1;//������-1���ں����merror.h����֮��
        std::free(offInput.ppu8Plane[0]);
        return faceFeature;
    }  else  {
//        qDebug()<<QString::fromLocal8Bit("��ȡͼƬ�ɹ�");
    }

    resultCode=frEngine->ExtractFRFeature(&offInput,&FaceInput,&localFaceFeature);

    /* ���������������,�Է���һ����ȡ�����������ݸı�*/
    faceFeature.lFeatureSize = localFaceFeature.lFeatureSize;
    faceFeature.pbFeature = (MByte*)malloc(faceFeature.lFeatureSize);
    memcpy(faceFeature.pbFeature, localFaceFeature.pbFeature, faceFeature.lFeatureSize);

    std::free(offInput.ppu8Plane[0]);
//    std::free(localFaceFeature.pbFeature); //�����ڵ��ڴ治��
    return faceFeature;
}

/**
 * @brief FaceDiscern::FREngineCompareFaceFeature
 * �Ƚ���������
 * @param reffeature
 * @param probefeature
 * @param resultCode
 * @return
 * ����ƥ��ȣ�ƥ���0.5����Ϊͬһ����
 * Ŀǰʹ�õ�ƥ��Ƚ���Ϊ0.6
 */
float FaceDiscern::FREngineCompareFaceFeature(AFR_FSDK_FACEMODEL *reffeature, AFR_FSDK_FACEMODEL *probefeature, int &resultCode){
    MFloat  fSimilScore = 0.0f;
    resultCode=frEngine->FacePairMatching(reffeature,probefeature,&fSimilScore);

    return (float)fSimilScore;
}
