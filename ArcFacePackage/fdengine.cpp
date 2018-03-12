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
 * ��ʼ��FD����
 * @return
 * �ɹ�����MOK
 * ʧ�ܷ��أ�
 * MERR_INVALID_PARAM �����Ƿ�
 * MERR_NO_MEMORY �ڴ治��
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
 * �������棬�ͷ���Դ
 * @return
 * �ɹ�����MOK
 * ʧ�ܷ��أ�
 * MERR_INVALID_PARAM �����Ƿ�
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
 * ���������ͼ�����������⣬һ�����ھ�̬ͼ����
 * @param pImgData
 * �����ͼ����Ϣ
 * @param pFaceRes
 * ���������
 * @return
 */
int FDEngine::FaceDetection(LPASVLOFFSCREEN pImgData, LPAFD_FSDK_FACERES *pFaceRes)
{
    int ret = AFD_FSDK_StillImageFaceDetection(hEngine, pImgData, pFaceRes);
    return ret;
}
