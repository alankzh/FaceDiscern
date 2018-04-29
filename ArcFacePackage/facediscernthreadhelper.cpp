#include "facediscernthreadhelper.h"

FaceDiscernThreadHelper::FaceDiscernThreadHelper(QObject *obj):QObject(obj)
{
}

FaceDiscernThreadHelper::~FaceDiscernThreadHelper(){
    emit endThread();
    if(faceDiscern!=nullptr){
        delete faceDiscern;
        faceDiscern=nullptr;
    }
}

void FaceDiscernThreadHelper::startThread(){
    thread=new QThread();
    this->moveToThread(thread);
    connect(thread,&QThread::finished,thread,&QObject::deleteLater);//线程结束时删除线程
    connect(thread,&QThread::finished,this,&QObject::deleteLater);//线程结束时删除此对象
    connect(this,SIGNAL(endThread()),thread,SLOT(quit()));//endThread()信号使线程结束


    connect(this,SIGNAL(beginThread()),this,SLOT(environmentInit()));//beginThread()信号让环境开始初始化
    thread->start();

    emit beginThread();//必须以发信号的方式来正式让函数调用进入子线程中，而不是直接调用函数
}

void FaceDiscernThreadHelper::environmentInit(){
    qDebug()<<QString::fromLocal8Bit("当前线程id")<<(int)QThread::currentThreadId();

    //初始化人脸引擎
    faceDiscern=new FaceDiscern();
    int errorCode=0;
    faceDiscern->engineInit(errorCode);
    if(errorCode!=MOK){
        errorCatch(errorCode,FaceDiscern::FailedType::FaceInitFailed);
    }
    bool success=SQLDataBase::instance()->connectionDB(FACE_DB_CONNECTION_NAME);//建立数据库的连接
    if(!success){
        emit error(QString::fromLocal8Bit("程序文件被破坏，初始化错误"));
    }

    QList<Terran> terranList;//缓存的人员信息列表
    featureList={};
    //    downPicIndex=0;
    SQLDataBase::instance()->operationDB(FACE_DB_CONNECTION_NAME,SQLDataBase::OperationWay::SelectDB,terranList);

    loadPicFeatureFromDb(terranList);


    requestScreenShot();//请求截图
}

/**
 * @brief FaceDiscernThreadHelper::receiveCaptureImage
 * 接收屏幕截图的槽
 * @param image
 */
void FaceDiscernThreadHelper::receiveCaptureImage(QImage &image){
    /*使用opencvCamera不需要缩放图片，qtCamera需要*/
//    QImage scaledImage = image.scaled(QSize(1140,845),
//                                      Qt::KeepAspectRatio,
//                                      Qt::SmoothTransformation);
    //  TODO  scaledImage.save("E:\\sava.jpg");
    //    qDebug()<<scaledImage.size();
    int errorCode=0;
    LPAFT_FSDK_FACERES ftFaceRes=faceDiscern->FTEngineDiscern(image,errorCode);
    if(errorCode!=MOK){
        errorCatch(errorCode,FaceDiscern::FailedType::FaceTrackingFailed);
    }else{

        /*只追踪，不识别*/
//        int faceNum=ftFaceRes->nFace;
//        QList<Terran> terranList={};
//        for(int i=0;i<faceNum;i++){
//            AFR_FSDK_FACEINPUT frInput=faceDiscern->getFREngineFaceInput(ftFaceRes,i);
//            Terran terran;
//            terran.id=-1;
//            terran.frFaceInput=frInput;
//            terranList.append(terran);
//        }
//        emit sendTerran(terranList);

                QList<Terran> selectList={};
                QList<Terran> selectVisitorList={};
                QList<TerranFaceFeature> selectVisitorFeatureList={};

                int faceNum=ftFaceRes->nFace;
                for(int i=0;i<faceNum;i++){
                    TerranFaceFeature terranFaceFeature;
                    AFR_FSDK_FACEINPUT frInput=faceDiscern->getFREngineFaceInput(ftFaceRes,i);
                    terranFaceFeature.frFaceModelFeature=faceDiscern->FREngineExtractFaceFeature(image,frInput,errorCode);
                    bool isEmployee=false;
                    for(int cacheIndex=0;cacheIndex<featureList.size();cacheIndex++){
                        float similarity=0.0;
                        similarity=faceDiscern->FREngineCompareFaceFeature(&(featureList[cacheIndex].frFaceModelFeature),&(terranFaceFeature.frFaceModelFeature),errorCode);
                        if(errorCode!=MOK){
                            errorCatch(errorCode,FaceDiscern::FailedType::FacePairMatchFailed);
                        }else{
                            if(similarity>=0.65){
                                //是员工
        //                        qDebug()<<"is Employ";
                                Terran terran;
                                terran.id=featureList.at(cacheIndex).id;
                                QList<Terran> list={};
                                list.append(terran);
                                SQLDataBase::instance()->operationDB(FACE_DB_CONNECTION_NAME,SQLDataBase::OperationWay::SelectDBWithId,list);
                                Terran terran2=list.first();
                                terran2.frFaceInput=frInput;//加上人脸矩形
                                selectList.append(terran2);
                                isEmployee=true;

                                if(Constant::SAVE_DISCERN){
                                    QImage faceImg=image.copy(ftFaceRes->rcFace[i].left,ftFaceRes->rcFace[i].top,ftFaceRes->rcFace[i].right-ftFaceRes->rcFace[i].left,ftFaceRes->rcFace[i].bottom-ftFaceRes->rcFace[i].top);
                                    faceImg.save(DIRPATH_DISCERN_TERRAN+terran2.name+QString::fromLocal8Bit(".jpg"),"JPG");
        //                            qDebug()<<DIRPATH_DISCERN_TERRAN+terran2.name+QString::fromLocal8Bit(".jpg");
                                }
                                break;
                            }
                        }
                    }

                    if(!isEmployee){
                        bool isCacheVisitor=false;
                        for(TerranFaceFeature cacheVisitorFeature:visitorFeatureList){
                            float similarity=0.0;
                            similarity=faceDiscern->FREngineCompareFaceFeature(&(cacheVisitorFeature.frFaceModelFeature),&(terranFaceFeature.frFaceModelFeature),errorCode);
                            //                    qDebug()<<QString::fromLocal8Bit("与缓存访客相似度 similarity:")<<similarity;
                            if(errorCode!=MOK){
                                errorCatch(errorCode,FaceDiscern::FailedType::FacePairMatchFailed);
                            }else{
                                if(similarity>=0.6){
                                    //同一个人，旧访客
                                    qDebug()<<QString::fromLocal8Bit("旧访客");
                                    Terran terran;
                                    terran.id=cacheVisitorFeature.id;
                                    terran.frFaceInput=frInput;//加上人脸矩形
                                    isCacheVisitor=true;

                                    selectVisitorList.append(terran);//因为id从小到大排列，故越后的terranid越大，加在队列末尾
                                    terranFaceFeature.id= terran.id;
                                    break;
                                }else{
                                    //                            QImage faceImg=image.copy(ftFaceRes->rcFace[i].left,ftFaceRes->rcFace[i].top,ftFaceRes->rcFace[i].right-ftFaceRes->rcFace[i].left,ftFaceRes->rcFace[i].bottom-ftFaceRes->rcFace[i].top);
                                    //    TODO                        faceImg.save("E:\\visitornew.jpg");
                                }
                            }
                        }

                        if(!isCacheVisitor){
                            //没有访客缓存，新访客
                            qDebug()<<QString::fromLocal8Bit("新访客");
                            Terran terran;
                            terran.frFaceInput=frInput;
                            terran.id=Visitor_Offsert_Id_static;
                            Visitor_Offsert_Id_static-=1;//每次有访客在其最小id基础上减1
                            terranFaceFeature.id= terran.id;
                            selectVisitorList.prepend(terran);//新产生的访客id最小，放最前
                        }
                        selectVisitorFeatureList.append(terranFaceFeature);
                    }

                }

                visitorTerranList.clear();
                visitorTerranList=selectVisitorList;

                visitorFeatureList.clear();
                visitorFeatureList=selectVisitorFeatureList;

                ////        SQLDataBase::instance()->operationDB(FACE_DB_CONNECTION_NAME,SQLDataBase::OperationWay::SelectDBWithId,selectList);
                selectList.append(selectVisitorList);

                emit sendTerran(selectList);//发送此帧检测到的人脸数据

        //引擎传回来的Rect包含left、right、top、right，分别表示矩形最左边距离x为0点距离、最右边距离x为0点距离、最上边距离y为0点距离、最底边距离y为0点距离
        //qt中Rect的表示有2种，其中一种为x、y、width、height，表示左上角x坐标、左上角y坐标、矩形宽、矩形高
        //引擎和qt中表达Rect位置的方式，都以左上角为零原点、向右增x、向下增y。
        //                QImage faceImg=image.copy(ftFaceRes->rcFace[0].left,ftFaceRes->rcFace[0].top,ftFaceRes->rcFace[0].right-ftFaceRes->rcFace[0].left,ftFaceRes->rcFace[0].bottom-ftFaceRes->rcFace[0].top);
        //                emit sendFaceImage(faceImg);
    }
    //200ms后再次请求主线程发送截图过来
    QTimer::singleShot(SCREEN_SHOT_CIRCLE,this,SLOT(requestScreenShot()));
//            QTimer::singleShot(0,this,SLOT(requestScreenShot()));
}

/**
 * @brief FaceDiscernThreadHelper::loadPicFeatureFromDb
 * 从缓存中加入图片
 * @param terranList
 */
void FaceDiscernThreadHelper::loadPicFeatureFromDb(QList<Terran> &terranList){
    QMutableListIterator<Terran> iter(terranList);
    while(iter.hasNext()){
        QList<Terran> loadList={};
        Terran terran=iter.next();
        loadList.append(terran);
        QImage *image=new QImage();
        SQLDataBase::instance()->operationDB(FACE_DB_CONNECTION_NAME,SQLDataBase::OperationWay::LoadDBImage,loadList,image);
        if(image->byteCount()>0){
            TerranFaceFeature terranfaceFeature;
            terranfaceFeature.id=terran.id;
            int errorCode=0;
            LPAFD_FSDK_FACERES fdFaceResult=faceDiscern->FDEngineDiscern(*image,errorCode);
            if(errorCode!=MOK){
                errorCatch(errorCode,FaceDiscern::FailedType::FaceDetectFailed);
                return;
            }
            AFR_FSDK_FACEINPUT frInput=faceDiscern->getFREngineFaceInput(fdFaceResult);
            terranfaceFeature.frFaceModelFeature=faceDiscern->FREngineExtractFaceFeature(*image,frInput,errorCode);
            if(errorCode!=MOK){
                errorCatch(errorCode,FaceDiscern::FailedType::FaceExtractFeatureFailed);
                return;
            }
            featureList.append(terranfaceFeature);
            delete image;
            iter.remove();//有本地缓存了，就不用down网页上的了
            qDebug()<<QString::fromLocal8Bit("加载成功，terran id:")<<terran.id;
        }else{
            delete image;
            qDebug()<<QString::fromLocal8Bit("加载失败，阿弥陀佛，随缘吧，继续下载下一张图片缓存");
        }
    }
    qDebug()<<featureList.size();
}


/**
 * @brief FaceDiscernThreadHelper::requestScreenShot
 * 请求主线程发送截图过来
 */
void FaceDiscernThreadHelper::requestScreenShot(){
    emit askCapture();
}

/**
 * @brief FaceDiscernThreadHelper::receiveInsertFaceData
 * 被通知需要更新人脸库数据
 * @param list
 */
void FaceDiscernThreadHelper::receiveInsertFaceData(QList<Terran> list){
    loadPicFeatureFromDb(list);
}

/**
 * @brief FaceDiscernThreadHelper::receiveDeleteFaceData
 * 被通知需要更新人脸库数据
 * @param list
 */
void FaceDiscernThreadHelper::receiveDeleteFaceData(QList<Terran> list){
    for(Terran terran:list){
        QMutableListIterator<TerranFaceFeature> iter(featureList);
        while(iter.hasNext()){
            TerranFaceFeature terranFaceFeature=iter.next();
            if(terran.id==terranFaceFeature.id){
                iter.remove();//删除缓存
            }
        }
    }
}

/**
 * @brief FaceDiscernThreadHelper::receiveUpdateFaceDataInfrom
 * 被通知需要更新人脸库数据
 * @param list
 */
void FaceDiscernThreadHelper::receiveUpdateFaceData(QList<Terran> list){
    for(Terran terran:list){
        QMutableListIterator<TerranFaceFeature> iter(featureList);
        while(iter.hasNext()){
            TerranFaceFeature terranFaceFeature=iter.next();
            if(terran.id==terranFaceFeature.id){
                iter.remove();//删除缓存
            }
        }
    }

    loadPicFeatureFromDb(list);//再加入缓存
}

/**
 * @brief FaceDiscernThreadHelper::errorCatch
 * 引擎错误处理
 */
void FaceDiscernThreadHelper::errorCatch(int errorCode,FaceDiscern::FailedType failedType){
    switch(failedType){
    case FaceDiscern::FailedType::FaceInitFailed:
        if(errorCode==MERR_INVALID_PARAM){
            emit error(QString::fromLocal8Bit("参数非法"));
        }else if(errorCode==MERR_NO_MEMORY){
            emit error(QString::fromLocal8Bit("初始化内存不足"));
        }
        break;
    case FaceDiscern::FailedType::FaceDetectFailed:
        if(errorCode==-1){
            qDebug()<<QString::fromLocal8Bit("FD 引擎中image格式错误");
        }else if(errorCode==MERR_NO_MEMORY){
            //TODO 内存不足考虑重启电脑
            emit error(QString::fromLocal8Bit("FD内存不足"));
            qDebug()<<QString::fromLocal8Bit("FD 引擎识别脸内存不足，错误码:")<<errorCode;
        }else if(errorCode==MERR_INVALID_PARAM){
            //参数非法往往是没有检测到人脸，不用处理错误
            qDebug()<<QString::fromLocal8Bit("FD 引擎参数非法");
        }else if(errorCode==MERR_BAD_STATE){
            //状态不正确 不知这是什么情景
            qDebug()<<QString::fromLocal8Bit("FD 引擎状态不正确");
        }
        break;
    case FaceDiscern::FailedType::FaceTrackingFailed:
        if(errorCode==-1){
            qDebug()<<QString::fromLocal8Bit("FT 引擎中image格式错误");
        }else if(errorCode==MERR_NO_MEMORY){
            //TODO 内存不足考虑重启电脑
            emit error(QString::fromLocal8Bit("FT内存不足"));
            qDebug()<<QString::fromLocal8Bit("FT 引擎识别脸内存不足，错误码:")<<errorCode;
        }else if(errorCode==MERR_INVALID_PARAM){
            //参数非法往往是没有检测到人脸，不用处理错误
            qDebug()<<QString::fromLocal8Bit("FT 引擎参数非法");
        }else if(errorCode==MERR_BAD_STATE){
            //状态不正确 不知这是什么情景
            qDebug()<<QString::fromLocal8Bit("FT 引擎状态不正确");
        }
        break;
    case FaceDiscern::FailedType::FacePairMatchFailed:
        if(errorCode==MERR_NO_MEMORY){
            //TODO 内存不足考虑重启电脑
            emit error(QString::fromLocal8Bit("FR内存不足"));
            qDebug()<<QString::fromLocal8Bit("FR 引擎识别脸内存不足，错误码:")<<errorCode;
        }else if(errorCode==MERR_INVALID_PARAM){
            //参数非法往往是没有检测到人脸，不用处理错误
            qDebug()<<QString::fromLocal8Bit("FR 引擎参数非法");
        }
        break;
    case FaceDiscern::FailedType::FaceExtractFeatureFailed:
        if(errorCode==-1){
            qDebug()<<QString::fromLocal8Bit("FR 引擎中image格式错误");
        }else if(errorCode==MERR_NO_MEMORY){
            //TODO 内存不足考虑重启电脑
            emit error(QString::fromLocal8Bit("FR内存不足"));
            qDebug()<<QString::fromLocal8Bit("FR 引擎识别脸内存不足，错误码:")<<errorCode;
        }else if(errorCode==MERR_INVALID_PARAM){
            //参数非法往往是没有检测到人脸，不用处理错误
            qDebug()<<QString::fromLocal8Bit("FR 引擎参数非法");
        }else if(errorCode==MERR_FSDK_FR_INVALID_FACE_INFO){
            qDebug()<<QString::fromLocal8Bit("无效的脸部信息")<<errorCode;
        }
        break;
    }
}
