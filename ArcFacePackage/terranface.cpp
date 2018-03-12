#include "terranface.h"

TerranFace::TerranFace()
{
    id=-1;//-1��ʾδ���õĳ�ʼֵ
}

TerranFace::~TerranFace(){
}

void TerranFace::setId(unsigned int id){
    this->id=id;
}

int TerranFace::getId(){
    return this->id;
}

/**
 * @brief TerranFace::setFaceModel
 * ���ݴ�FD��������ȡ��������������װFR���������model
 * @param lftFaceRes
 * FD����ʶ����
 * @oaram index
 * ��ȡ�ĵڼ�����
 */
void TerranFace::setFaceInputRect(LPAFD_FSDK_FACERES lfdFaceRes,int index){
    frFaceInput.lOrient = (_AFR_FSDK_OrientCode)lfdFaceRes->lfaceOrient[index];//��������
    //������λ��
    frFaceInput.rcFace.left = lfdFaceRes->rcFace[index].left;
    frFaceInput.rcFace.top = lfdFaceRes->rcFace[index].top;
    frFaceInput.rcFace.right = lfdFaceRes->rcFace[index].right;
    frFaceInput.rcFace.bottom = lfdFaceRes->rcFace[index].bottom;
}

/**
 * @brief TerranFace::setFaceModel
 * ���ݴ�FT��������ȡ��������������װFR���������model
 * @param lftFaceRes
 * FT����ʶ����
 * @oaram index
 * ��ȡ�ĵڼ�����
 */
void TerranFace::setFaceInputRect(LPAFT_FSDK_FACERES lftFaceRes,int index){
    frFaceInput.lOrient = (_AFR_FSDK_OrientCode)lftFaceRes->lfaceOrient;//��������
    //������λ��
    frFaceInput.rcFace.left = lftFaceRes->rcFace[index].left;
    frFaceInput.rcFace.top = lftFaceRes->rcFace[index].top;
    frFaceInput.rcFace.right = lftFaceRes->rcFace[index].right;
    frFaceInput.rcFace.bottom = lftFaceRes->rcFace[index].bottom;
}

/**
 * @brief TerranFace::getFaceModel
 * ����FR�����face model
 * @return
 */
AFR_FSDK_FACEINPUT TerranFace::getFaceModel(){
    return frFaceInput;
}
