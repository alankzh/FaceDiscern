#include "facediscernthreadhelper.h"

FaceDiscernThreadHelper::FaceDiscernThreadHelper(QObject *obj):QObject(obj)
{
}

FaceDiscernThreadHelper::~FaceDiscernThreadHelper(){
    if(faceDiscern!=nullptr){
        delete faceDiscern;
        faceDiscern=nullptr;
    }
    if(httpUtil!=nullptr){
        delete httpUtil;
        httpUtil=nullptr;
    }
}

void FaceDiscernThreadHelper::startThread(){
    thread=new QThread();
    this->moveToThread(thread);
    connect(thread,&QThread::finished,thread,&QObject::deleteLater);//�߳̽���ʱɾ���߳�
    connect(thread,&QThread::finished,this,&QObject::deleteLater);//�߳̽���ʱɾ���˶���
    connect(this,SIGNAL(endThread()),thread,SLOT(quit()));//endThread()�ź�ʹ�߳̽���


    connect(this,SIGNAL(beginThread()),this,SLOT(environmentInit()));//beginThread()�ź��û�����ʼ��ʼ��
    thread->start();

    emit beginThread();//�����Է��źŵķ�ʽ����ʽ�ú������ý������߳��У�������ֱ�ӵ��ú���
}

void FaceDiscernThreadHelper::environmentInit(){
    qDebug()<<QString::fromLocal8Bit("��ǰ�߳�id")<<(int)QThread::currentThreadId();

    //��ʼ����������
    faceDiscern=new FaceDiscern();
    int errorCode=0;
    faceDiscern->engineInit(errorCode);
    if(errorCode!=MOK){
        errorCatch(errorCode,FaceDiscern::FailedType::FaceInitFailed);
    }
    bool success=SQLDataBase::instance()->connectionDB(FACE_DB_CONNECTION_NAME);//�������ݿ������
    if(!success){
        emit error(QString::fromLocal8Bit("�����ļ����ƻ�����ʼ������"));
    }

    httpUtil=new HttpUtil();

    QList<Terran> terranList;//�������Ա��Ϣ�б�
    featureList={};
    //    downPicIndex=0;
    SQLDataBase::instance()->operationDB(FACE_DB_CONNECTION_NAME,SQLDataBase::OperationWay::SelectDB,terranList);

    qDebug()<<QString::fromLocal8Bit("FaceDiscernThreadHelper ��ȡ�������ݿ��е����ݳ���:")<<terranList.size();

    loadPicFeatureFromDb(terranList);
    bool sucess=downPicFromServer(terranList);  //TODO
    if(success){
        requestScreenShot();//�����ͼ
        qDebug()<<"22222222222222222222222222222";
    }
}

/**
 * @brief FaceDiscernThreadHelper::receiveCaptureImage
 * ������Ļ��ͼ�Ĳ�
 * @param image
 */
void FaceDiscernThreadHelper::receiveCaptureImage(QImage image){
    QImage scaledImage = image.scaled(QSize(1140,845),
                                      Qt::IgnoreAspectRatio,
                                      Qt::SmoothTransformation);
//  TODO  scaledImage.save("E:\\sava.jpg");
//    qDebug()<<scaledImage.size();
    int errorCode=0;
    LPAFT_FSDK_FACERES ftFaceRes=faceDiscern->FTEngineDiscern(scaledImage,errorCode);
    if(errorCode!=MOK){
        errorCatch(errorCode,FaceDiscern::FailedType::FaceTrackingFailed);
    }else{

        QList<Terran> selectList={};
        QList<Terran> selectVisitorList={};
        QList<TerranFaceFeature> selectVisitorFeatureList={};
        qDebug()<<"featureList.size:"<<featureList.size();
        int faceNum=ftFaceRes->nFace;
        TerranFaceFeature xiaofang;
        TerranFaceFeature meijie;
        for(int i=0;i<faceNum;i++){
            TerranFaceFeature terranFaceFeature;
            AFR_FSDK_FACEINPUT frInput=faceDiscern->getFREngineFaceInput(ftFaceRes,i);
            terranFaceFeature.frFaceModelFeature=faceDiscern->FREngineExtractFaceFeature(scaledImage,frInput,errorCode);
            bool isEmployee=false;
            for(TerranFaceFeature cacheTerranFaceFeature:featureList){
                float similarity=0.0;
                similarity=faceDiscern->FREngineCompareFaceFeature(&(cacheTerranFaceFeature.frFaceModelFeature),&(terranFaceFeature.frFaceModelFeature),errorCode);
                if(errorCode!=MOK){
                    errorCatch(errorCode,FaceDiscern::FailedType::FacePairMatchFailed);
                }else{
                    if(similarity>=0.6){
                        //��Ա��
                        qDebug()<<"is Employ";
                        Terran terran;
                        terran.id=cacheTerranFaceFeature.id;
                        QList<Terran> list={};
                        list.append(terran);
                        SQLDataBase::instance()->operationDB(FACE_DB_CONNECTION_NAME,SQLDataBase::OperationWay::SelectDBWithId,list);
                        Terran terran2=list.first();
                        qDebug()<<terran2.name;
                        qDebug()<<"frInput, left:"<<frInput.rcFace.left<<"right:"<<frInput.rcFace.right<<"top:"<<frInput.rcFace.top<<"bottom:"<<frInput.rcFace.bottom;
                        terran2.frFaceInput=frInput;//������������
                         qDebug()<<"terran2, left:"<<terran2.frFaceInput.rcFace.left<<"right:"<<terran2.frFaceInput.rcFace.right<<"top:"<<terran2.frFaceInput.rcFace.top<<"bottom:"<<terran2.frFaceInput.rcFace.bottom;
                        selectList.append(terran2);
                        isEmployee=true;
                        break;
                    }
                }
            }

            qDebug()<<QString::fromLocal8Bit("�Ƿ��ǹ�Ա:")<<isEmployee;
            if(!isEmployee){
                bool isCacheVisitor=false;
                for(TerranFaceFeature cacheVisitorFeature:visitorFeatureList){
                    float similarity=0.0;
                    similarity=faceDiscern->FREngineCompareFaceFeature(&(cacheVisitorFeature.frFaceModelFeature),&(terranFaceFeature.frFaceModelFeature),errorCode);
                    qDebug()<<QString::fromLocal8Bit("�뻺��ÿ����ƶ� similarity:")<<similarity;
                    if(errorCode!=MOK){
                        qDebug()<<QString::fromLocal8Bit("fr ƥ����� �������:")<<errorCode;
                        errorCatch(errorCode,FaceDiscern::FailedType::FacePairMatchFailed);
                    }else{
                        if(similarity>=0.6){
                            //ͬһ���ˣ��ɷÿ�
                            qDebug()<<QString::fromLocal8Bit("�ɷÿ�");
                            Terran terran;
                            terran.id=cacheVisitorFeature.id;
                            terran.frFaceInput=frInput;//������������
                            isCacheVisitor=true;

                            selectVisitorList.append(terran);//��Ϊid��С�������У���Խ���terranidԽ�󣬼��ڶ���ĩβ
                            terranFaceFeature.id= terran.id;
                            break;
                        }else{
                            QImage faceImg=image.copy(ftFaceRes->rcFace[i].left,ftFaceRes->rcFace[i].top,ftFaceRes->rcFace[i].right-ftFaceRes->rcFace[i].left,ftFaceRes->rcFace[i].bottom-ftFaceRes->rcFace[i].top);
//    TODO                        faceImg.save("E:\\visitornew.jpg");
                        }
                    }
                }

                if(!isCacheVisitor){
                    //û�зÿͻ��棬�·ÿ�
                    qDebug()<<QString::fromLocal8Bit("�·ÿ�");
                    Terran terran;
                    terran.frFaceInput=frInput;
                    terran.id=Visitor_Offsert_Id_static;
                    Visitor_Offsert_Id_static-=1;//ÿ���зÿ�������Сid�����ϼ�1
                    terranFaceFeature.id= terran.id;
                    selectVisitorList.prepend(terran);//�²����ķÿ�id��С������ǰ
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


        emit sendTerran(selectList);//���ʹ�֡��⵽����������

        //���洫������Rect����left��right��top��right���ֱ��ʾ��������߾���xΪ0����롢���ұ߾���xΪ0����롢���ϱ߾���yΪ0����롢��ױ߾���yΪ0�����
        //qt��Rect�ı�ʾ��2�֣�����һ��Ϊx��y��width��height����ʾ���Ͻ�x���ꡢ���Ͻ�y���ꡢ���ο������θ�
        //�����qt�б���Rectλ�õķ�ʽ���������Ͻ�Ϊ��ԭ�㡢������x��������y��
        //                QImage faceImg=image.copy(ftFaceRes->rcFace[0].left,ftFaceRes->rcFace[0].top,ftFaceRes->rcFace[0].right-ftFaceRes->rcFace[0].left,ftFaceRes->rcFace[0].bottom-ftFaceRes->rcFace[0].top);
        //                emit sendFaceImage(faceImg);
    }
    //200ms���ٴ��������̷߳��ͽ�ͼ����
    //        QTimer::singleShot(SCREEN_SHOT_CIRCLE,this,SLOT(requestScreenShot()));
    QTimer::singleShot(0,this,SLOT(requestScreenShot()));
}



/**
 * @brief FaceDiscernThreadHelper::downPicFromServer
 * �ӷ�����������ͼƬ����ȡ���������뻺�棬����ʽ
 */
bool FaceDiscernThreadHelper::downPicFromServer(QList<Terran> terranList,int downPicIndex){
    int breakCount=0;
    while(true){
        qDebug()<<QString::fromLocal8Bit("���ڶ�ȡ������:")<<downPicIndex;
        if(downPicIndex>=terranList.size()){
            break;
        }

        QString url=DOWNLOAD_PIC_URL_PRE+terranList.at(downPicIndex).photoUrl;

        QImage* image;

        image=httpUtil->downLoadPic(url);

        if(image!=nullptr){
            insertPicToDb(terranList.at(downPicIndex).id,image);//ͼƬ�������ݿ⻺��

            TerranFaceFeature terranfaceFeature;
            terranfaceFeature.id=terranList.at(downPicIndex).id;
            int errorCode=0;
            LPAFD_FSDK_FACERES fdFaceResult=faceDiscern->FDEngineDiscern(*image,errorCode);
            if(errorCode!=MOK){
                errorCatch(errorCode,FaceDiscern::FailedType::FaceDetectFailed);
                return false;
            }
            AFR_FSDK_FACEINPUT frInput=faceDiscern->getFREngineFaceInput(fdFaceResult);
            terranfaceFeature.frFaceModelFeature=faceDiscern->FREngineExtractFaceFeature(*image,frInput,errorCode);
            if(errorCode!=MOK){
                errorCatch(errorCode,FaceDiscern::FailedType::FaceExtractFeatureFailed);
                return false;
            }
            featureList.append(terranfaceFeature);
            downPicIndex++;
            delete image;
            qDebug()<<QString::fromLocal8Bit("����������һ��ͼƬ");
        }else{
            //            //����ǰһ��ͼƬ������
            //            breakCount++;
            //            //��ʱ�����˳�ѭ������ӡ����
            //            if(breakCount>=3){
            //                break;
            //            }
            downPicIndex++;//������һ��ͼƬ����ϵ��̣��Ҳ�ǿ��
        }
    }

    if(downPicIndex>=terranList.size()){
        //ͼƬȫ��������ϣ����Կ�ʼʶ��
        qDebug()<<QString::fromLocal8Bit("ͼƬȫ��������ϣ����Կ�ʼʶ��");
        //        requestScreenShot();
        return true;
    }else{
        //֪ͨ�������
        emit error(QString::fromLocal8Bit("���ط���������ʧ��"));
        return false;
    }
}

/**
 * @brief FaceDiscernThreadHelper::insertPicToDb
 * ����ͼƬ�����ݿ���
 * @param terranId
 * @param image
 */
void FaceDiscernThreadHelper::insertPicToDb(int terranId, QImage *image){
    Terran terran;
    terran.id=terranId;
    QList<Terran> insertList={};
    insertList.append(terran);
    SQLDataBase::instance()->operationDB(FACE_DB_CONNECTION_NAME,SQLDataBase::OperationWay::SavaImage,insertList,image);
}

/**
 * @brief FaceDiscernThreadHelper::loadPicFeatureFromDb
 * �ӻ����м���ͼƬ
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
            iter.remove();//�б��ػ����ˣ��Ͳ���down��ҳ�ϵ���
            qDebug()<<QString::fromLocal8Bit("���سɹ�������������һ��ͼƬ����");
        }else{
            delete image;
            qDebug()<<QString::fromLocal8Bit("����ʧ�ܣ������ӷ���Ե�ɣ�����������һ��ͼƬ����");
        }
    }
}


/**
 * @brief FaceDiscernThreadHelper::requestScreenShot
 * �������̷߳��ͽ�ͼ����
 */
void FaceDiscernThreadHelper::requestScreenShot(){
    emit askCapture();
}

/**
 * @brief FaceDiscernThreadHelper::receiveInsertFaceData
 * ��֪ͨ��Ҫ��������������
 * @param list
 */
void FaceDiscernThreadHelper::receiveInsertFaceData(QList<Terran> list){
//    //�ٴβ�ѯ���ݿ⣬��ȷ�ϲ�����ȷ��
//    SQLDataBase::instance()->operationDB(FACE_DB_CONNECTION_NAME,SQLDataBase::OperationWay::SelectDBWithId,list);

    downPicFromServer(list);
}

/**
 * @brief FaceDiscernThreadHelper::receiveDeleteFaceData
 * ��֪ͨ��Ҫ��������������
 * @param list
 */
void FaceDiscernThreadHelper::receiveDeleteFaceData(QList<Terran> list){
    //    //�ٴβ�ѯ���ݿ⣬��ȷ��ɾ����ȷ��
    //    SQLDataBase::instance()->operationDB(FACE_DB_CONNECTION_NAME,SQLDataBase::OperationWay::SelectDBWithId,list);

    for(Terran terran:list){
        QMutableListIterator<TerranFaceFeature> iter(featureList);
        while(iter.hasNext()){
            TerranFaceFeature terranFaceFeature=iter.next();
            if(terran.id==terranFaceFeature.id){
                iter.remove();//ɾ������
            }
        }
    }
}

/**
 * @brief FaceDiscernThreadHelper::receiveUpdateFaceDataInfrom
 * ��֪ͨ��Ҫ��������������
 * @param list
 */
void FaceDiscernThreadHelper::receiveUpdateFaceData(QList<Terran> list){
    //    //�ٴβ�ѯ���ݿ⣬��ȷ�ϸ�����ȷ��
    //    SQLDataBase::instance()->operationDB(FACE_DB_CONNECTION_NAME,SQLDataBase::OperationWay::SelectDBWithId,list);

    for(Terran terran:list){
        QMutableListIterator<TerranFaceFeature> iter(featureList);
        while(iter.hasNext()){
            TerranFaceFeature terranFaceFeature=iter.next();
            if(terran.id==terranFaceFeature.id){
                iter.remove();//ɾ������
            }
        }
    }

    downPicFromServer(list);//�ټ��뻺��
}

/**
 * @brief FaceDiscernThreadHelper::errorCatch
 * ���������
 */
void FaceDiscernThreadHelper::errorCatch(int errorCode,FaceDiscern::FailedType failedType){
    switch(failedType){
    case FaceDiscern::FailedType::FaceInitFailed:
        if(errorCode==MERR_INVALID_PARAM){
            emit error(QString::fromLocal8Bit("�����Ƿ�"));
        }else if(errorCode==MERR_NO_MEMORY){
            emit error(QString::fromLocal8Bit("��ʼ���ڴ治��"));
        }
        break;
    case FaceDiscern::FailedType::FaceDetectFailed:
        if(errorCode==-1){
            qDebug()<<QString::fromLocal8Bit("FD ������image��ʽ����");
        }else if(errorCode==MERR_NO_MEMORY){
            //TODO �ڴ治�㿼����������
            emit error(QString::fromLocal8Bit("FD�ڴ治��"));
            qDebug()<<QString::fromLocal8Bit("FD ����ʶ�����ڴ治�㣬������:")<<errorCode;
        }else if(errorCode==MERR_INVALID_PARAM){
            //�����Ƿ�������û�м�⵽���������ô�������
            qDebug()<<QString::fromLocal8Bit("FD ��������Ƿ�");
        }else if(errorCode==MERR_BAD_STATE){
            //״̬����ȷ ��֪����ʲô�龰
            qDebug()<<QString::fromLocal8Bit("FD ����״̬����ȷ");
        }
        break;
    case FaceDiscern::FailedType::FaceTrackingFailed:
        if(errorCode==-1){
            qDebug()<<QString::fromLocal8Bit("FT ������image��ʽ����");
        }else if(errorCode==MERR_NO_MEMORY){
            //TODO �ڴ治�㿼����������
            emit error(QString::fromLocal8Bit("FT�ڴ治��"));
            qDebug()<<QString::fromLocal8Bit("FT ����ʶ�����ڴ治�㣬������:")<<errorCode;
        }else if(errorCode==MERR_INVALID_PARAM){
            //�����Ƿ�������û�м�⵽���������ô�������
            qDebug()<<QString::fromLocal8Bit("FT ��������Ƿ�");
        }else if(errorCode==MERR_BAD_STATE){
            //״̬����ȷ ��֪����ʲô�龰
            qDebug()<<QString::fromLocal8Bit("FT ����״̬����ȷ");
        }
        break;
    case FaceDiscern::FailedType::FacePairMatchFailed:
        if(errorCode==MERR_NO_MEMORY){
            //TODO �ڴ治�㿼����������
            emit error(QString::fromLocal8Bit("FR�ڴ治��"));
            qDebug()<<QString::fromLocal8Bit("FR ����ʶ�����ڴ治�㣬������:")<<errorCode;
        }else if(errorCode==MERR_INVALID_PARAM){
            //�����Ƿ�������û�м�⵽���������ô�������
            qDebug()<<QString::fromLocal8Bit("FR ��������Ƿ�");
        }
        break;
    case FaceDiscern::FailedType::FaceExtractFeatureFailed:
        if(errorCode==-1){
            qDebug()<<QString::fromLocal8Bit("FR ������image��ʽ����");
        }else if(errorCode==MERR_NO_MEMORY){
            //TODO �ڴ治�㿼����������
            emit error(QString::fromLocal8Bit("FR�ڴ治��"));
            qDebug()<<QString::fromLocal8Bit("FR ����ʶ�����ڴ治�㣬������:")<<errorCode;
        }else if(errorCode==MERR_INVALID_PARAM){
            //�����Ƿ�������û�м�⵽���������ô�������
            qDebug()<<QString::fromLocal8Bit("FR ��������Ƿ�");
        }else if(errorCode==MERR_FSDK_FR_INVALID_FACE_INFO){
            qDebug()<<QString::fromLocal8Bit("��Ч��������Ϣ")<<errorCode;
        }
        break;
    }
}