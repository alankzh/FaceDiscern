#include "ftengine.h"

FTEngine::FTEngine()
{
    nScale = 16;
    nMaxFace = 12;
    pWorkMem = nullptr;
    hEngine = nullptr;
}
FTEngine::~FTEngine()
{
}

/**
 * @brief FTEngine::init
 * 初始化FT引擎
 * @return
 * 成功返回MOK
 * 失败返回：
 * MERR_INVALID_PARAM 参数非法
 * MERR_NO_MEMORY 内存不足
 */
int FTEngine::init()
{
    pWorkMem = (MByte *)std::malloc(WORKBUF_SIZE_FT);
    if (pWorkMem == nullptr)
    {
        return -1;
    }
    int ret = 0;
    ret = AFT_FSDK_InitialFaceEngine(const_cast<char*>(APPID), const_cast<char*>(FT_SDKKEY), pWorkMem, WORKBUF_SIZE_FT, &hEngine, AFT_FSDK_OPF_0_HIGHER_EXT, nScale, nMaxFace);
    return ret;
}

/**
 * @brief FTEngine::uninit
 * 销毁引擎，释放资源
 * @return
 * 成功返回MOK
 * 失败返回：
 * MERR_INVALID_PARAM 参数非法
 */
int FTEngine::uninit()
{
    int ret = 0;
    ret = AFT_FSDK_UninitialFaceEngine(hEngine);
    if (pWorkMem != nullptr)
    {
        std::free(pWorkMem);
    }
    return ret;
}

/**
 * @brief FTEngine::FaceTracking
 * 根据输入的图像进行人脸检测，一般用于静态图像检测
 * @param pImgData
 * 待检测图像信息
 * @param pFaceRes
 * 人脸检测结果
 * @return
 */
int FTEngine::FaceTracking(LPASVLOFFSCREEN pImgData, LPAFT_FSDK_FACERES *pFaceRes)
{
    int ret;
    ret = AFT_FSDK_FaceFeatureDetect(hEngine, pImgData, pFaceRes);
    return ret;
}
