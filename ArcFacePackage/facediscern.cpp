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
 * 使用FD引擎进行脸部识别
 * @param image
 * 输入的QImage图像数据
 * @param resultCode
 * 引擎识别返回码
 * @return
 * 返回FD引擎封装的识别结果LPAFT_FSDK_FACERES
 */
LPAFD_FSDK_FACERES FaceDiscern::FDEngineDiscern(QImage &image,int &resultCode){
    // 人脸检测结果
    LPAFD_FSDK_FACERES FaceRes = nullptr;
    // 人脸引擎需要的图像输入数据
    ASVLOFFSCREEN offInput = { 0 };

    offInput.u32PixelArrayFormat = ASVL_PAF_RGB24_B8G8R8;
    offInput.ppu8Plane[0] = nullptr;

    bool readed=ConvertImage::readBmp24(image, (uint8_t**)&offInput.ppu8Plane[0], &offInput.i32Width, &offInput.i32Height);

    offInput.pi32Pitch[0] = offInput.i32Width * 3;
    if (!offInput.ppu8Plane[0]||!readed)  {
        //        qDebug()<<"读取截屏失败";
        resultCode=-1;//错误码-1不在虹软的merror.h定义之中
        std::free(offInput.ppu8Plane[0]);
        return FaceRes;
    }  else  {
        //        qDebug()<<"读取截屏成功";
    }

    resultCode = fdEngine->FaceDetection(&offInput,&FaceRes);
    if (resultCode != MOK)  {
        qDebug()<<QString::fromLocal8Bit("FD 识别错误，错误码:")<<resultCode;
        std::free(offInput.ppu8Plane[0]);
        return FaceRes;
    }  else  {
                qDebug()<<QString::fromLocal8Bit("FD 识别成功");
        for (int i = 0; i < FaceRes->nFace; ++i){
//            qDebug()<<"face num:"<<i+1<<" left:"<<FaceRes->rcFace[i].left<<" top:"<<FaceRes->rcFace[i].top<<" right:"<<FaceRes->rcFace[i].right<<" bottom:"<<FaceRes->rcFace[i].bottom<<" 人脸方向:"<<FaceRes->lfaceOrient[i]<<"\n";
        }
    }

    std::free(offInput.ppu8Plane[0]);

    return FaceRes;
}

/**
 * @brief FaceDiscern::FTEngineDiscern
 * 使用FR引擎进行人脸识别
 * @param image
 * 输入的QImage图像数据
 * @param resultCode
 * 引擎识别返回码
 * @return
 */
LPAFT_FSDK_FACERES FaceDiscern::FTEngineDiscern(QImage &image, int &resultCode){
    // 人脸检测结果
    LPAFT_FSDK_FACERES  FaceRes = nullptr;
    // 人脸引擎需要的图像输入数据
    ASVLOFFSCREEN offInput = { 0 };

    offInput.u32PixelArrayFormat = ASVL_PAF_RGB24_B8G8R8;
    offInput.ppu8Plane[0] = nullptr;

    bool readed=ConvertImage::readBmp24(image, (uint8_t**)&offInput.ppu8Plane[0], &offInput.i32Width, &offInput.i32Height);

    offInput.pi32Pitch[0] = offInput.i32Width * 3;
    if (!offInput.ppu8Plane[0]||!readed)  {
        qDebug()<<QString::fromLocal8Bit("FT 读取截屏失败");
        resultCode=-1;//错误码-1不在虹软的merror.h定义之中
        std::free(offInput.ppu8Plane[0]);
        return FaceRes;
    }  else  {
//        qDebug()<<QString::fromLocal8Bit("FT 读取截屏成功");
    }

    resultCode = ftEngine->FaceTracking(&offInput,&FaceRes);
    if (resultCode != MOK)  {
        qDebug()<<"识别错误，错误码:"<<resultCode;
        std::free(offInput.ppu8Plane[0]);
        return FaceRes;
    }  else  {
        qDebug()<<QString::fromLocal8Bit("FT 识别成功");
        for (int i = 0; i < FaceRes->nFace; ++i){
            qDebug()<<"face num:"<<i+1<<" left:"<<FaceRes->rcFace[i].left<<" top:"<<FaceRes->rcFace[i].top<<" right:"<<FaceRes->rcFace[i].right<<" bottom:"<<FaceRes->rcFace[i].bottom<<" orientation:"<<FaceRes->lfaceOrient<<"\n";
        }
    }

    std::free(offInput.ppu8Plane[0]);

    return FaceRes;
}

/**
 * @brief FaceDiscern::getFREngineFaceInput
 * 从fd引擎的识别结果转换为fr引擎需要的输入
 * @param lfdFaceRes
 * @param index
 * @return
 */
AFR_FSDK_FACEINPUT FaceDiscern::getFREngineFaceInput(LPAFD_FSDK_FACERES lfdFaceRes, int index){
    AFR_FSDK_FACEINPUT frFaceInput;
    frFaceInput.lOrient = (_AFR_FSDK_OrientCode)lfdFaceRes->lfaceOrient[index];//人脸方向
    //人脸框位置
    frFaceInput.rcFace.left = lfdFaceRes->rcFace[index].left;
    frFaceInput.rcFace.top = lfdFaceRes->rcFace[index].top;
    frFaceInput.rcFace.right = lfdFaceRes->rcFace[index].right;
    frFaceInput.rcFace.bottom = lfdFaceRes->rcFace[index].bottom;
    return frFaceInput;
}

/**
 * @brief FaceDiscern::getFREngineFaceInput
 * 从ft引起的识别结果转换为fr引起需要的输入
 * @param lftFaceRes
 * @param index
 * @return
 */
AFR_FSDK_FACEINPUT FaceDiscern::getFREngineFaceInput(LPAFT_FSDK_FACERES lftFaceRes, int index){
    AFR_FSDK_FACEINPUT frFaceInput;
    frFaceInput.lOrient = (_AFR_FSDK_OrientCode)lftFaceRes->lfaceOrient;//人脸方向
    //人脸框位置
    frFaceInput.rcFace.left = lftFaceRes->rcFace[index].left;
    frFaceInput.rcFace.top = lftFaceRes->rcFace[index].top;
    frFaceInput.rcFace.right = lftFaceRes->rcFace[index].right;
    frFaceInput.rcFace.bottom = lftFaceRes->rcFace[index].bottom;
    return frFaceInput;
}

/**
 * @brief FaceDiscern::FREngineExtractFaceFeature
 * 提取图片中人脸特征
 * @param image
 * 图片数据
 * @param FaceInput
 * FT或FR引起中识别结果，转换为FR引擎的输入
 * @param resultCode
 * 返回码
 * @return
 */
AFR_FSDK_FACEMODEL FaceDiscern::FREngineExtractFaceFeature(QImage &image, AFR_FSDK_FACEINPUT FaceInput,int &resultCode){
    AFR_FSDK_FACEMODEL faceFeature = { 0 };
    AFR_FSDK_FACEMODEL localFaceFeature = { 0 };
    // 人脸引擎需要的图像输入数据
    ASVLOFFSCREEN offInput = { 0 };
    offInput.u32PixelArrayFormat = ASVL_PAF_RGB24_B8G8R8;
    offInput.ppu8Plane[0] = nullptr;
    bool readed=ConvertImage::readBmp24(image, (uint8_t**)&offInput.ppu8Plane[0], &offInput.i32Width, &offInput.i32Height);
    offInput.pi32Pitch[0] = offInput.i32Width * 3;

    if (!offInput.ppu8Plane[0]||!readed)  {
        qDebug()<<QString::fromLocal8Bit("读取图片失败");
        resultCode=-1;//错误码-1不在虹软的merror.h定义之中
        std::free(offInput.ppu8Plane[0]);
        return faceFeature;
    }  else  {
//        qDebug()<<QString::fromLocal8Bit("读取图片成功");
    }

    resultCode=frEngine->ExtractFRFeature(&offInput,&FaceInput,&localFaceFeature);

    /* 拷贝人脸特征结果,以防下一次提取后，引擎中内容改变*/
    faceFeature.lFeatureSize = localFaceFeature.lFeatureSize;
    faceFeature.pbFeature = (MByte*)malloc(faceFeature.lFeatureSize);
    memcpy(faceFeature.pbFeature, localFaceFeature.pbFeature, faceFeature.lFeatureSize);

    std::free(offInput.ppu8Plane[0]);
//    std::free(localFaceFeature.pbFeature); //引擎内的内存不管
    return faceFeature;
}

/**
 * @brief FaceDiscern::FREngineCompareFaceFeature
 * 比较人脸特征
 * @param reffeature
 * @param probefeature
 * @param resultCode
 * @return
 * 返回匹配度，匹配度0.5以上为同一个人
 * 目前使用的匹配度建议为0.6
 */
float FaceDiscern::FREngineCompareFaceFeature(AFR_FSDK_FACEMODEL *reffeature, AFR_FSDK_FACEMODEL *probefeature, int &resultCode){
    MFloat  fSimilScore = 0.0f;
    resultCode=frEngine->FacePairMatching(reffeature,probefeature,&fSimilScore);

    return (float)fSimilScore;
}
