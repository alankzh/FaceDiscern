#include "fdengine.h"

FDEngine::FDEngine()
{
    nScale = 32;
    nMaxFace = 10;
    pWorkMem = nullptr;
    hEngine = nullptr;
}
FDEngine::~FDEngine()
{
}

/**
 * @brief FDEngine::init
 * 初始化FD引擎
 * @return
 * 成功返回MOK
 * 失败返回：
 * MERR_INVALID_PARAM 参数非法
 * MERR_NO_MEMORY 内存不足
 */
int FDEngine::init()
{
    pWorkMem = (MByte *)std::malloc(WORKBUF_SIZE_FD);
    if (pWorkMem == nullptr)
    {
        return -1;
    }
    int ret = AFD_FSDK_InitialFaceEngine(const_cast<char*>(APPID), const_cast<char*>(FD_SDKKEY), pWorkMem, WORKBUF_SIZE_FD, &hEngine, AFD_FSDK_OPF_0_HIGHER_EXT, nScale, nMaxFace);
    return ret;
}

/**
 * @brief FDEngine::uninit
 * 销毁引擎，释放资源
 * @return
 * 成功返回MOK
 * 失败返回：
 * MERR_INVALID_PARAM 参数非法
 */
int FDEngine::uninit()
{
    int ret = AFD_FSDK_UninitialFaceEngine(hEngine);
    if (pWorkMem != nullptr)
    {
        std::free(pWorkMem);
    }
    return ret;
}

/**
 * @brief FDEngine::FaceDetection
 * 根据输入的图像进行人脸检测，一般用于静态图像检测
 * @param pImgData
 * 待检测图像信息
 * @param pFaceRes
 * 人脸检测结果
 * @return
 */
int FDEngine::FaceDetection(LPASVLOFFSCREEN pImgData, LPAFD_FSDK_FACERES *pFaceRes)
{
    int ret = AFD_FSDK_StillImageFaceDetection(hEngine, pImgData, pFaceRes);
    return ret;
}
