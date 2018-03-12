#include "frengine.h"

FREngine::FREngine()
{
    pWorkMem = nullptr;
    hEngine = nullptr;
}
FREngine::~FREngine()
{
}

/**
 * @brief FREngine::init
 * 初始化FD引擎
 * @return
 * 成功返回MOK
 * 失败返回：
 * MERR_INVALID_PARAM 参数非法
 * MERR_NO_MEMORY 内存不足
 */
int FREngine::init()
{
    pWorkMem = (MByte *)std::malloc(WORKBUF_SIZE_FR);
    if (pWorkMem == nullptr)
    {
        return -1;
    }
    int ret = AFR_FSDK_InitialEngine(const_cast<char*>(APPID), const_cast<char*>(FR_SDKKEY), pWorkMem, WORKBUF_SIZE_FR, &hEngine);
    return ret;
}

/**
 * @brief FREngine::uninit
 * 销毁引擎，释放资源
 * @return
 * 成功返回MOK
 * 失败返回：
 * MERR_INVALID_PARAM 参数非法
 * MERR_NO_MEMORY 内存不足
 */
int FREngine::uninit()
{
    int ret = AFR_FSDK_UninitialEngine(hEngine);
    if (pWorkMem != nullptr)
    {
        std::free(pWorkMem);
    }
    return ret;
}

/**
 * @brief FREngine::ExtractFRFeature
 * 获取脸部特征
 * @param pImgData
 * 输入的图像数据
 * @param pFaceRes
 * 已经检测到的脸部信息
 * @param pFaceModels
 * 提取到的脸部特征
 * @return
 * 成功返回MOK
 * 失败返回：
 * MERR_INVALID_PARAM 参数非法
 * MERR_NO_MEMORY 内存不足
 */
int FREngine::ExtractFRFeature(LPASVLOFFSCREEN pImgData, LPAFR_FSDK_FACEINPUT pFaceRes, LPAFR_FSDK_FACEMODEL pFaceModels)
{
    int ret = AFR_FSDK_ExtractFRFeature(hEngine, pImgData, pFaceRes, pFaceModels);
    return ret;
}

/**
 * @brief FREngine::FacePairMatching
 * 脸部特征比较
 * @param reffeature
 * 已有脸部信息
 * @param probefeature
 * 被比较的脸部信息
 * @param pfSimilScore
 * 比较结果 0~1
 * @return
 * 成功返回MOK
 * 失败返回：
 * MERR_INVALID_PARAM 参数非法
 * MERR_NO_MEMORY 内存不足
 */
int FREngine::FacePairMatching(AFR_FSDK_FACEMODEL *reffeature, AFR_FSDK_FACEMODEL *probefeature, MFloat *pfSimilScore)
{
    int ret = AFR_FSDK_FacePairMatching(hEngine, reffeature, probefeature, pfSimilScore);
    return ret;
}
