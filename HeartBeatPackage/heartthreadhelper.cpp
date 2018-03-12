#include "heartthreadhelper.h"

using namespace rapidjson;

HeartThreadHelper::HeartThreadHelper(QObject *obj):QObject(obj)
{

}

HeartThreadHelper::~HeartThreadHelper(){
    if(heartbeat!=nullptr){
        delete heartbeat;
        heartbeat=nullptr;
    }
}

void HeartThreadHelper::startThread(){
    thread=new QThread();
    this->moveToThread(thread);
    connect(thread,&QThread::finished,thread,&QObject::deleteLater);//�߳̽���ʱɾ���߳�
    connect(thread,&QThread::finished,this,&QObject::deleteLater);//�߳̽���ʱɾ���˶���
    connect(this,SIGNAL(endThread()),thread,SLOT(quit()));//endThread()�ź�ʹ�߳̽���

    connect(this,SIGNAL(beginThread()),this,SLOT(environmentInit()));//beginThread()�ź����߳���ʽ��ʼ
    thread->start();

    emit beginThread();//�����Է��źŵķ�ʽ����ʽ�ú������ý������߳��У�������ֱ�ӵ��ú���
}

/**
 * @brief HeartThreadHelper::environmentInit
 * �߳̿�ʼʱ����ʼ������
 */
void HeartThreadHelper::environmentInit(){
    qDebug()<<(int)QThread::currentThreadId();
    heartbeat=new HeartBeat();
    connect(heartbeat,SIGNAL(receiveResponseStr(QString&)),this,SLOT(receiveTerrans(QString&)));
    connect(heartbeat,SIGNAL(httpError(int)),this,SLOT(httpErrorInHeart(int)));

    bool success=SQLDataBase::instance()->connectionDB(HEART_DB_CONNECTION_NAME);//�����ݿ�����
    if(!success){
        emit httpServerError(QString::fromLocal8Bit("�����ļ����ƻ�����ʼ������"));
    }
    terranList={};
    SQLDataBase::instance()->operationDB(HEART_DB_CONNECTION_NAME,SQLDataBase::OperationWay::SelectDB,terranList);

    qDebug()<<QString::fromLocal8Bit("�������ݿ������ݵ�����,�������ݳ���:")<<terranList.size();

    heartbeat->beat();//��ʼ����
}

/**
 * @brief HeartThreadHelper::receiveTerrans
 * �յ������з������Ļ�Ӧ������json�ַ���
 * ʹ����Ҷ�����rapidjsonģ��
 * @param str
 */
void HeartThreadHelper::receiveTerrans(QString &str){
    QList<Terran> list;
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
                qDebug()<<"json��������";
            }
        }
    }else{
        qDebug()<<"json��������";
    }

    synchronizationData(list);
}

/**
 * @brief HeartThreadHelper::httpErrorInHeart
 * �����г��ֵ��������
 * ���������ش��󣬷���ֻ��ʾ���ӶϿ�
 * @param errorCode
 */
void HeartThreadHelper::httpErrorInHeart(int errorCode){
    switch(errorCode){
    case 404:
        emit httpServerError(QString::fromLocal8Bit("��������ַ������"));
        break;
    case 503:
        emit httpServerError(QString::fromLocal8Bit("���񲻿���"));
        break;
    default:
        emit httpConnectionBreak();
        break;
    }
}

/**
 * @brief HeartThreadHelper::synchronizationData
 * �ȶԷ����������ݺͻ�������ݵĲ���
 * ��������������ݶ��ˣ���ô�������ݿ�
 * ������������������ˣ���ôɾ
 * ��������������ݲ�һ������ô��(����)
 * ԭ���Ͼ�������DB���������ڴ��в���Ϊ����
 * @param listFromServer
 */
void HeartThreadHelper::synchronizationData(QList<Terran> &listFromServer){
    //��֤�����������ݵĴ���һ���ʡ�ԣ���Ϊ�������ϵĴ���һ���ǰ���id��С�������е�
    Terran::quickSort(listFromServer,0,listFromServer.size()-1);
    QList<Terran> insertList={};
    QList<Terran> deleteList={};
    QList<Terran> updateList={};

    int localIndex=0;//����list��λ��ָʾ
    int serverIndex=0;//������list��λ��ָʾ

    while(true){
        if(localIndex>=terranList.size()&&serverIndex<listFromServer.size()){
            //������ػ�����ѭ�����ף���ô��������ʣ��ȫ������Ӧ����ӽ����ݿ�
            for(int i=serverIndex;i<listFromServer.size();i++){
                insertList.append(listFromServer.at(i));
            }
            break;
        }else if(localIndex<terranList.size()&&serverIndex>=listFromServer.size()){
            //���������������ѭ������,��ô���ػ���ʣ�µ�����Ӧ��ȫ�������ݿ���ɾ��
            for(int i=localIndex;i<terranList.size();i++){
                deleteList.append(terranList.at(i));
            }
            break;
        }else if(localIndex>=terranList.size()&&serverIndex>=listFromServer.size()){
            //���ͬʱѭ�����ף�����Ҫ���κθ��Ӳ���
            break;
        }

        Terran localIndextTerran=terranList.at(localIndex);
        Terran serverIndexTerran=terranList.at(serverIndex);
        if(localIndextTerran.equals(serverIndexTerran)){
            localIndex++;
            serverIndex++;
            continue;
        }else{
            if(localIndextTerran.id==serverIndexTerran.id){
                //�������ݺͷ���������id��ͬ�������ݲ�һ�£���Ҫ����
                updateList.append(serverIndexTerran);
                localIndex++;
                serverIndex++;
                continue;
            }else if(localIndextTerran.id>serverIndexTerran.id){
                //��������id���ڷ��������ݣ�����������ɾ�������������������
                deleteList.append(localIndextTerran);
                insertList.append(serverIndexTerran);
                localIndex++;
                serverIndex++;
                continue;
            }else if(localIndextTerran.id<serverIndexTerran.id){
                //����������id���ڱ������ݣ�����������ɾ�������������ݲ��ż����
                deleteList.append(localIndextTerran);
                localIndex++;
            }
        }
    }

    if(insertList.size()>0){
        SQLDataBase::instance()->operationDB(HEART_DB_CONNECTION_NAME,SQLDataBase::OperationWay::InsertDB,insertList);
    }
    if(deleteList.size()>0){
        SQLDataBase::instance()->operationDB(HEART_DB_CONNECTION_NAME,SQLDataBase::OperationWay::DeleteDB,deleteList);
    }
    if(updateList.size()>0){
        SQLDataBase::instance()->operationDB(HEART_DB_CONNECTION_NAME,SQLDataBase::OperationWay::UpdateDB,updateList);
    }

    //�����е����ݿ������ɺ��ٷ����źţ���Ȼ���ݿ������ͬ����������û�б�Ҫ��
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
        qDebug()<<QString::fromLocal8Bit("�������������һ�£�����Ҫͬ��");
    }else{
        //���²������ݿ���ȫ�����ݣ����뻺�棬
        //�����ݿ���ҵģ��������ڴ��е����ݣ���ȷ�����ݿ�������һ���ԣ��Է�����ʧ��
        terranList.clear();
        SQLDataBase::instance()->operationDB(HEART_DB_CONNECTION_NAME,SQLDataBase::OperationWay::SelectDB,terranList);
    }
}
