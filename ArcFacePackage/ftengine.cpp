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
 * ��ʼ��FT����
 * @return
 * �ɹ�����MOK
 * ʧ�ܷ��أ�
 * MERR_INVALID_PARAM �����Ƿ�
 * MERR_NO_MEMORY �ڴ治��
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
 * �������棬�ͷ���Դ
 * @return
 * �ɹ�����MOK
 * ʧ�ܷ��أ�
 * MERR_INVALID_PARAM �����Ƿ�
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
 * ���������ͼ�����������⣬һ�����ھ�̬ͼ����
 * @param pImgData
 * �����ͼ����Ϣ
 * @param pFaceRes
 * ���������
 * @return
 */
int FTEngine::FaceTracking(LPASVLOFFSCREEN pImgData, LPAFT_FSDK_FACERES *pFaceRes)
{
    int ret;
    ret = AFT_FSDK_FaceFeatureDetect(hEngine, pImgData, pFaceRes);
    return ret;
}
