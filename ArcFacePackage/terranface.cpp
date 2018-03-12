#include "terranface.h"

TerranFace::TerranFace()
{
    id=-1;//-1表示未设置的初始值
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
 * 根据从FD引擎中提取到的人脸矩阵组装FR引擎所需的model
 * @param lftFaceRes
 * FD引擎识别结果
 * @oaram index
 * 提取的第几张脸
 */
void TerranFace::setFaceInputRect(LPAFD_FSDK_FACERES lfdFaceRes,int index){
    frFaceInput.lOrient = (_AFR_FSDK_OrientCode)lfdFaceRes->lfaceOrient[index];//人脸方向
    //人脸框位置
    frFaceInput.rcFace.left = lfdFaceRes->rcFace[index].left;
    frFaceInput.rcFace.top = lfdFaceRes->rcFace[index].top;
    frFaceInput.rcFace.right = lfdFaceRes->rcFace[index].right;
    frFaceInput.rcFace.bottom = lfdFaceRes->rcFace[index].bottom;
}

/**
 * @brief TerranFace::setFaceModel
 * 根据从FT引擎中提取到的人脸矩阵组装FR引擎所需的model
 * @param lftFaceRes
 * FT引擎识别结果
 * @oaram index
 * 提取的第几张脸
 */
void TerranFace::setFaceInputRect(LPAFT_FSDK_FACERES lftFaceRes,int index){
    frFaceInput.lOrient = (_AFR_FSDK_OrientCode)lftFaceRes->lfaceOrient;//人脸方向
    //人脸框位置
    frFaceInput.rcFace.left = lftFaceRes->rcFace[index].left;
    frFaceInput.rcFace.top = lftFaceRes->rcFace[index].top;
    frFaceInput.rcFace.right = lftFaceRes->rcFace[index].right;
    frFaceInput.rcFace.bottom = lftFaceRes->rcFace[index].bottom;
}

/**
 * @brief TerranFace::getFaceModel
 * 返回FR引擎的face model
 * @return
 */
AFR_FSDK_FACEINPUT TerranFace::getFaceModel(){
    return frFaceInput;
}
