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
 * ��ʼ��FD����
 * @return
 * �ɹ�����MOK
 * ʧ�ܷ��أ�
 * MERR_INVALID_PARAM �����Ƿ�
 * MERR_NO_MEMORY �ڴ治��
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
 * �������棬�ͷ���Դ
 * @return
 * �ɹ�����MOK
 * ʧ�ܷ��أ�
 * MERR_INVALID_PARAM �����Ƿ�
 * MERR_NO_MEMORY �ڴ治��
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
 * ��ȡ��������
 * @param pImgData
 * �����ͼ������
 * @param pFaceRes
 * �Ѿ���⵽��������Ϣ
 * @param pFaceModels
 * ��ȡ������������
 * @return
 * �ɹ�����MOK
 * ʧ�ܷ��أ�
 * MERR_INVALID_PARAM �����Ƿ�
 * MERR_NO_MEMORY �ڴ治��
 */
int FREngine::ExtractFRFeature(LPASVLOFFSCREEN pImgData, LPAFR_FSDK_FACEINPUT pFaceRes, LPAFR_FSDK_FACEMODEL pFaceModels)
{
    int ret = AFR_FSDK_ExtractFRFeature(hEngine, pImgData, pFaceRes, pFaceModels);
    return ret;
}

/**
 * @brief FREngine::FacePairMatching
 * ���������Ƚ�
 * @param reffeature
 * ����������Ϣ
 * @param probefeature
 * ���Ƚϵ�������Ϣ
 * @param pfSimilScore
 * �ȽϽ�� 0~1
 * @return
 * �ɹ�����MOK
 * ʧ�ܷ��أ�
 * MERR_INVALID_PARAM �����Ƿ�
 * MERR_NO_MEMORY �ڴ治��
 */
int FREngine::FacePairMatching(AFR_FSDK_FACEMODEL *reffeature, AFR_FSDK_FACEMODEL *probefeature, MFloat *pfSimilScore)
{
    int ret = AFR_FSDK_FacePairMatching(hEngine, reffeature, probefeature, pfSimilScore);
    return ret;
}
