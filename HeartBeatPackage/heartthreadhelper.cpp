#include "heartthreadhelper.h"

using namespace rapidjson;

HeartThreadHelper::HeartThreadHelper(QObject *obj):QObject(obj){

}

HeartThreadHelper::~HeartThreadHelper(){
    if(heartbeat!=nullptr){
        delete heartbeat;
        heartbeat=nullptr;
    }
    if(httpUtil!=nullptr){
        delete httpUtil;
        httpUtil=nullptr;
    }
}

void HeartThreadHelper::startThread(){
    thread=new QThread();
    this->moveToThread(thread);
    connect(thread,&QThread::finished,thread,&QObject::deleteLater);//线程结束时删除线程
    connect(thread,&QThread::finished,this,&QObject::deleteLater);//线程结束时删除此对象
    connect(this,SIGNAL(endThread()),thread,SLOT(quit()));//endThread()信号使线程结束

    connect(this,SIGNAL(beginThread()),this,SLOT(environmentInit()));//beginThread()信号让线程正式开始
    thread->start();

    emit beginThread();//必须以发信号的方式来正式让函数调用进入子线程中，而不是直接调用函数
}

/**
 * @brief HeartThreadHelper::environmentInit
 * 线程开始时，初始化环境
 */
void HeartThreadHelper::environmentInit(){
    qDebug()<<QString::fromLocal8Bit("当前线程id:")<<(int)QThread::currentThreadId();
    heartbeat=new HeartBeat();
    connect(heartbeat,SIGNAL(receiveResponseStr(QString&)),this,SLOT(receiveTerrans(QString&)));
    connect(heartbeat,SIGNAL(httpError(int)),this,SLOT(httpErrorInHeart(int)));
    connect(heartbeat,SIGNAL(httpConnectionDelay(int)),this,SLOT(heartBeatDelay(int)));
    httpUtil=new HttpUtil();

    bool success=SQLDataBase::instance()->connectionDB(HEART_DB_CONNECTION_NAME);//打开数据库连接
    if(!success){
        emit httpServerError(QString::fromLocal8Bit("程序文件被破坏，初始化错误"));
    }
    terranList={};
    SQLDataBase::instance()->operationDB(HEART_DB_CONNECTION_NAME,SQLDataBase::OperationWay::SelectDB,terranList);

//    qDebug()<<QString::fromLocal8Bit("加载数据库中数据到缓存,缓存数据长度:")<<terranList.size();

    heartbeat->beat();//开始心跳
}

/**
 * @brief HeartThreadHelper::receiveTerrans
 * 收到心跳中服务器的回应，解析json字符串
 * 使用了叶劲峰的rapidjson模块
 * @param str
 */
void HeartThreadHelper::receiveTerrans(QString &str){
    QList<Terran> list={};
    Document document;
    document.Parse<0>(str.toStdString().c_str());
    Value &jsonDataArray=document["Data"];
    if(jsonDataArray.IsArray()){
        for(int i=0;i<jsonDataArray.Size();i++){
            Value &terranjson=jsonDataArray[i];
            if(terranjson.IsObject()){
                Terran terran;
                terran.id = terranjson["Id"].GetInt();
                terran.name=terranjson["Name"].GetString();
                terran.openId=terranjson["OpenID"].GetString();
                terran.photoUrl=terranjson["PhotoUrl"].GetString();
                terran.position=terranjson["Position"].GetString();
                terran.faceToken=terranjson["Face_token"].GetString();
                terran.departmentId=terranjson["DepartmentId"].GetInt();
                terran.department=terranjson["Department"].GetString();
                terran.begTime=terranjson["BegTime"].GetString();
                terran.endTime=terranjson["EndTime"].GetString();
                terran.date=terranjson["Date"].GetString();
                terran.typeOfWork=terranjson["TypeOfWork"].GetBool();
                terran.isUpdate=terranjson["UpData"].GetBool();
                list.append(terran);
            }else{
                qDebug()<<"json解析错误";
            }
        }
    }else{
        qDebug()<<"json解析错误";
    }

    synchronizationData(list);
}

/**
 * @brief HeartThreadHelper::httpErrorInHeart
 * 心跳中出现的网络错误
 * 除非是严重错误，否则只提示连接断开
 * @param errorCode
 */
void HeartThreadHelper::httpErrorInHeart(int errorCode){
    switch(errorCode){
    case 404:
        emit httpServerError(QString::fromLocal8Bit("服务器地址不存在"));
        emit httpConnectionAlive(false);//通知连接断开
        break;
    case 503:
        emit httpServerError(QString::fromLocal8Bit("服务不可用"));
        emit httpConnectionAlive(false);//通知连接断开
        break;
    default:
        emit httpConnectionAlive(false);//通知连接断开
        break;
    }
}

void HeartThreadHelper::heartBeatDelay(int mesc){
    emit httpConnectionDelay(mesc);
    emit httpConnectionAlive(true);//通知连接建立
}

/**
 * @brief HeartThreadHelper::synchronizationData
 * 比对服务器上数据和缓存的数据的差异
 * 如果服务器上数据多了，那么增到数据库
 * 如果服务器上数据少了，那么删
 * 如果服务器上数据不一样，那么改(更新)
 * 原则上尽量减少DB操作，以内存中操作为代价
 * @param listFromServer
 */
void HeartThreadHelper::synchronizationData(QList<Terran> &listFromServer){
//    qDebug()<<terranList.size();
//    qDebug()<<listFromServer.size();

    //保证服务器上数据的次序，一般可省略，因为服务器上的次序一般是按照id从小到大排列的
    Terran::quickSort(listFromServer,0,listFromServer.size()-1);
    QList<Terran> insertList={};
    QList<Terran> deleteList={};
    QList<Terran> updateList={};

    int localIndex=0;//本地list的位置指示
    int serverIndex=0;//服务器list的位置指示

    while(true){
        if(localIndex>=terranList.size()&&serverIndex<listFromServer.size()){
            //如果本地缓存先循环到底，那么服务器上剩下全部数据应该添加进数据库
            for(int i=serverIndex;i<listFromServer.size();i++){
                insertList.append(listFromServer.at(i));
            }
            break;
        }else if(localIndex<terranList.size()&&serverIndex>=listFromServer.size()){
            //如果服务器数据先循环到底,那么本地缓存剩下的数据应该全部在数据库中删除
            for(int i=localIndex;i<terranList.size();i++){
                deleteList.append(terranList.at(i));
            }
            break;
        }else if(localIndex>=terranList.size()&&serverIndex>=listFromServer.size()){
            //如果同时循环到底，不需要做任何附加操作
            break;
        }

        Terran localIndextTerran=terranList.at(localIndex);
        Terran serverIndexTerran=listFromServer.at(serverIndex);

        if(localIndextTerran.ServerDataequals(serverIndexTerran)){
            localIndex++;
            serverIndex++;
            continue;
        }else{
            if(localIndextTerran.id==serverIndexTerran.id){
                //本地数据和服务器数据id相同，但数据不一致，需要更新
                qDebug()<<QString::fromLocal8Bit("本地数据和服务器数据id相同，但数据不一致，需要更新");

                qDebug()<<localIndextTerran.toString();
                qDebug()<<serverIndexTerran.toString();

                updateList.append(serverIndexTerran);
                localIndex++;
                serverIndex++;
                continue;
            }else if(localIndextTerran.id>serverIndexTerran.id){
                //本地数据id大于服务器数据，本地数据需删除，服务器数据需添加
                deleteList.append(localIndextTerran);
                insertList.append(serverIndexTerran);
                localIndex++;
                serverIndex++;
                continue;
            }else if(localIndextTerran.id<serverIndexTerran.id){
                //服务器数据id大于本地数据，本地数据需删除，服务器数据不着急添加
                deleteList.append(localIndextTerran);
                localIndex++;
            }
        }
    }

    if(insertList.size()>0){
        SQLDataBase::instance()->operationDB(HEART_DB_CONNECTION_NAME,SQLDataBase::OperationWay::InsertDB,insertList);
        downAndInsertImageToDB(insertList);
    }
    if(deleteList.size()>0){
        SQLDataBase::instance()->operationDB(HEART_DB_CONNECTION_NAME,SQLDataBase::OperationWay::DeleteDB,deleteList);
        SQLDataBase::instance()->operationDB(HEART_DB_CONNECTION_NAME,SQLDataBase::OperationWay::DeleteImage,deleteList);
    }
    if(updateList.size()>0){
        SQLDataBase::instance()->operationDB(HEART_DB_CONNECTION_NAME,SQLDataBase::OperationWay::UpdateDB,updateList);

        /*更新时，删除图片，再次下载,与直接更新是一样的，懒得再写downAndUpdateImageToDB函数了*/
        SQLDataBase::instance()->operationDB(HEART_DB_CONNECTION_NAME,SQLDataBase::OperationWay::DeleteImage,updateList);
        downAndInsertImageToDB(updateList);
    }

    //在所有的数据库操作完成后再发送信号，虽然数据库操作有同步锁，这是没有必要的
    if(insertList.size()>0){
        emit insertDBsynchronized(insertList);
    }
    if(deleteList.size()>0){
        emit deleteDBsynchronized(deleteList);
    }
    if(updateList.size()>0){
        emit updateDBsynchronized(updateList);
    }

    if(insertList.size()==0&&deleteList.size()==0&&updateList.size()==0){
//        qDebug()<<QString::fromLocal8Bit("与服务器上数据一致，不需要同步");
    }else{
        //重新查找数据库中全部数据，加入缓存，
        //用数据库查找的，而不是内存中的数据，以确保数据库操作后的一致性，以防操作失败
        terranList.clear();
        SQLDataBase::instance()->operationDB(HEART_DB_CONNECTION_NAME,SQLDataBase::OperationWay::SelectDB,terranList);
    }

}



void HeartThreadHelper::downAndInsertImageToDB(QList<Terran> &list){
    for(Terran needDownterran:list){
        QString url=Constant::DOWNLOAD_PIC_URL_PRE+needDownterran.photoUrl;
        QImage* image;
        image=httpUtil->downLoadPic(url);//同步方式在下载

        if(image!=nullptr){
            Terran terran;
            terran.id=needDownterran.id;
            QList<Terran> insertList={};
            insertList.append(terran);
            SQLDataBase::instance()->operationDB(HEART_DB_CONNECTION_NAME,SQLDataBase::OperationWay::SavaImage,insertList,image);

            delete image;
            qDebug()<<QString::fromLocal8Bit("继续下载下一张图片");
        }else{
        }
    }
}
