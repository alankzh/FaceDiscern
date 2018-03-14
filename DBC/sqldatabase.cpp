#include "sqldatabase.h"
#include "singleton.h"

SQLDataBase::SQLDataBase(QObject* parent):QObject(parent){
}

SQLDataBase* SQLDataBase::createInstance(){
    return new SQLDataBase();
}

SQLDataBase::~SQLDataBase(){
    //��û�йرյ����ݿ����ӽ��йر�
    if (dbMap.count() > 0)
    {
        QMap<QString,QSqlDatabase*>::iterator it = dbMap.begin();
        if (it != dbMap.end())
        {
            if (it.value()->isOpen())
            {
                it.value()->close();
            }
        }
        dbMap.clear();
    }
}

SQLDataBase* SQLDataBase::instance(){
    return Singleton<SQLDataBase>::instance(SQLDataBase::createInstance);
}

/**
 * @brief SQLDataBase::connectionDB
 * �������������������ӣ��������ӳ���
 * ��ͬ�߳̽���������ֻ���Լ�ʹ��
 * @param connectionName
 * ������
 */
bool SQLDataBase::connectionDB(QString connectionName){
    QMutexLocker locker(&mutex);//��
    QMap<QString,QSqlDatabase*>::iterator iterator=dbMap.find(connectionName);
    if(iterator!=dbMap.end()){
        if(!dbMap[connectionName]->isOpen()){
            dbMap[connectionName]->open();
        }
        return true;
    }

    QSqlDatabase* sqlDB=new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE",connectionName));
    sqlDB->setDatabaseName("FaceDataBase.db");
    sqlDB->setUserName("alankzh");
    sqlDB->setPassword("123456");

    if(!sqlDB->open()){
        return false;
        qDebug()<<sqlDB->lastError();
    }
    dbMap.insert(connectionName,sqlDB);

    QStringList list=sqlDB->tables();
    //��û�б�terran���򴴽�
    if(!list.contains(FACE_TABLE_NAME)){
        QSqlQuery sqlQuery(*sqlDB);

        QString createTable="create table terran (id int,name text,openId text,photoUrl text,position text,faceToken text,departmentId int,department text,begTime text,endTime text,date text,typeOfwork bool,isUpdate bool)";

        sqlQuery.prepare(createTable);
        if(!sqlQuery.exec())
        {
            qDebug()<<sqlQuery.lastError();
        }
        else
        {
            qDebug()<<"table created!";
        }

    }

    //��û�б�terranImageData���򴴽�
    if(!list.contains(FACE_IMAGE_TABLE_NAME)){
        QSqlQuery sqlQuery(*sqlDB);

        QString createTable="create table terranImageData (id int primary key,imageData blob)";

        sqlQuery.prepare(createTable);
        if(!sqlQuery.exec())
        {
            qDebug()<<sqlQuery.lastError();
        }
        else
        {
            qDebug()<<"table created!";
        }

    }


    return true;
}

/**
 * @brief SQLDataBase::operationDB
 * ���ݿ������Ŀǰֻ����һ�ű��ʲ���Ҫ�Ա������з�װ
 * ����ж��ű���ôQList<Terran> Ӧ��ΪQList<TableObject>
 * TableObject �ж���Ӧ�ô��map<�ֶ������ֶ�ֵ>
 * �ֶ�ֵҲӦ����TableFiledObject�������ţ�ֵ���͡�ֵ
 * ֵ���Ͷ�Ӧ���ݿ����ֶ�����
 * ������չ�������Ͽ��Խ����ݿ��б�ӳ��Ϊ���󣬶Զ���Ĳ��������Ƕ����ݿ�Ĳ���
 * @param connectionName
 * ������
 * @param operationWay
 * ������ʽ
 * @param list
 * ��������,�򷵻�����
 * @return
 */
bool SQLDataBase::operationDB(QString connectionName, SQLDataBase::OperationWay operationWay, QList<Terran> &list,QImage *image){
    QMutexLocker locker(&mutex);//��

    QMap<QString,QSqlDatabase*>::iterator iterator=dbMap.find(connectionName);
    if(iterator!=dbMap.end()){
        if(!dbMap[connectionName]->isOpen()){
            dbMap[connectionName]->open();//�����ݿ�����
        }
    }else{
        qDebug()<<"connection:"<<connectionName<<" not connection";
        return false;
    }

    switch(operationWay){
    case OperationWay::InsertDB:
        insertDB(dbMap[connectionName],list);
        break;
    case OperationWay::DeleteDB:
        deleteDB(dbMap[connectionName],list);
        break;
    case OperationWay::UpdateDB:
        updateDB(dbMap[connectionName],list);
        break;
    case OperationWay::SelectDB:
        selectDB(dbMap[connectionName],list);
        break;
    case OperationWay::SelectDBWithId:
        selectDBWithId(dbMap[connectionName],list);
        break;
    case OperationWay::SavaImage:
        saveImage(dbMap[connectionName],list,image);
        break;
    case OperationWay::DeleteImage:
        deleteImage(dbMap[connectionName],list);
        break;
    case OperationWay::UpdateImage:
        updateImage(dbMap[connectionName],list,image);
        break;
    case OperationWay::LoadDBImage:
        loadImage(dbMap[connectionName],list,image);
        break;
    }
    dbMap[connectionName]->close();//���Ӳ�ʹ��ʱ�ر�Ϊ��
    return true;
}

//��
void SQLDataBase::insertDB(QSqlDatabase *sqldb, QList<Terran> &list){
    sqldb->transaction();//��ʼ���������ύ����Ч��

    QString sqlStr = "insert into terran values (:id,:name,:openId,:photoUrl,:position,:faceToken,:departmentId,:department,:begTime,:endTime,:date,:typeOfwork,:isUpdate)";
    QSqlQuery sqlQuery(*sqldb);

    bool success=false;
    for(Terran terran:list){
        sqlQuery.prepare(sqlStr);
        sqlQuery.bindValue(":id",terran.id);
        sqlQuery.bindValue(":name",terran.name);
        sqlQuery.bindValue(":openId",terran.openId);
        sqlQuery.bindValue(":photoUrl",terran.photoUrl);
        sqlQuery.bindValue(":position",terran.position);
        sqlQuery.bindValue(":faceToken",terran.faceToken);
        sqlQuery.bindValue(":departmentId",terran.departmentId);
        sqlQuery.bindValue(":department",terran.department);
        sqlQuery.bindValue(":begTime",terran.begTime);
        sqlQuery.bindValue(":endTime",terran.endTime);
        sqlQuery.bindValue(":date",terran.date);
        sqlQuery.bindValue(":typeOfwork",terran.typeOfWork);
        sqlQuery.bindValue(":isUpdate",terran.isUpdate);

        success=sqlQuery.exec();
        if(!success){
            //���ڲ�����ԣ�һ�����ʧ�ܾ�ʧ���ˣ����ػع�ȫ��
            //            sqldb->rollback();//�ع�
            qDebug()<<QString::fromLocal8Bit("����ʧ��,idΪ:")<<terran.id<<" ����Ϊ:"<<sqlQuery.lastError();
        }else{
            qDebug()<<QString::fromLocal8Bit("����ɹ�:")<<terran.name;
        }

    }

    sqldb->commit();
}

//ɾ
void SQLDataBase::deleteDB(QSqlDatabase *sqldb, QList<Terran> &list){
    sqldb->transaction();//��ʼ���������ύ����Ч��

    QString sqlStr="delete from terran where id = :id";
    QSqlQuery sqlQuery(*sqldb);

    bool success=false;
    for(Terran terran:list){
        sqlQuery.prepare(sqlStr);
        sqlQuery.bindValue(":id",terran.id);
        success=sqlQuery.exec();

        if(!success){
            //����ɾ�����ԣ�һ�����ʧ�ܾ�ʧ���ˣ����ػع�ȫ��
            //            sqldb->rollback();//�ع�
            qDebug()<<"ɾ��ʧ��,idΪ:"<<terran.id<<" ����Ϊ:"<<sqlQuery.lastError();
        }
    }

    sqldb->commit();
}

//����
void SQLDataBase::updateDB(QSqlDatabase *sqldb, QList<Terran> &list){
    sqldb->transaction();//��ʼ���������ύ����Ч��

    QString sqlStr="update terran set name=:name,openId=:openId,photoUrl=:photoUrl,position=:position,faceToken=:faceToken,departmentID=:departmentId,department=:department,begTime=:begTime,endTime=:endTime,date=:date,typeOfwork=:typeOfwork,isUpdate=:isUpdate where id = :id";

    QSqlQuery sqlQuery(*sqldb);
    bool success=false;
    for(Terran terran:list){
        sqlQuery.prepare(sqlStr);
        sqlQuery.bindValue(":name",terran.name);
        sqlQuery.bindValue(":openId",terran.openId);
        sqlQuery.bindValue(":photoUrl",terran.photoUrl);
        sqlQuery.bindValue(":position",terran.position);
        sqlQuery.bindValue(":faceToken",terran.faceToken);
        sqlQuery.bindValue(":departmentId",terran.departmentId);
        sqlQuery.bindValue(":department",terran.department);
        sqlQuery.bindValue(":begTime",terran.begTime);
        sqlQuery.bindValue(":endTime",terran.endTime);
        sqlQuery.bindValue(":date",terran.date);
        sqlQuery.bindValue(":typeOfwork",terran.typeOfWork);

        qDebug()<<QString::fromLocal8Bit("����:")<<terran.typeOfWork;

        sqlQuery.bindValue(":isUpdate",terran.isUpdate);
        sqlQuery.bindValue(":id",terran.id);

        success=sqlQuery.exec();

        if(!success){
            //���ڸ��¶��ԣ�һ�����ʧ�ܾ�ʧ���ˣ����ػع�ȫ��
            //            sqldb->rollback();//�ع�
            qDebug()<<"����ʧ��,idΪ:"<<terran.id<<" ����Ϊ:"<<sqlQuery.lastError();
        }else{
        }
    }

    sqldb->commit();
}

//ȫ������
void SQLDataBase::selectDB(QSqlDatabase *sqldb,QList<Terran> &list){
    list.clear();
    QString sqlStr="select * from terran";
    QSqlQuery sqlQuery(*sqldb);
    sqlQuery.prepare(sqlStr);

    if(sqlQuery.exec()){
        while(sqlQuery.next())
        {
            Terran terran;
            terran.id = sqlQuery.value("id").toInt();
            terran.name=sqlQuery.value("name").toString();
            terran.openId=sqlQuery.value("openId").toString();
            terran.photoUrl=sqlQuery.value("photoUrl").toString();
            terran.position=sqlQuery.value("position").toString();
            terran.faceToken=sqlQuery.value("faceToken").toString();
            terran.departmentId=sqlQuery.value("departmentId").toInt();
            terran.department=sqlQuery.value("department").toString();
            terran.begTime=sqlQuery.value("begTime").toString();
            terran.endTime=sqlQuery.value("endTime").toString();
            terran.date=sqlQuery.value("date").toString();
            terran.typeOfWork=sqlQuery.value("typeOfwork").toBool();
            terran.isUpdate=sqlQuery.value("isUpdate").toBool();

            list.append(terran);
        }
    }else{
        qDebug()<<"��ѯȫ��ʧ��";
    }

}

//����id����
void SQLDataBase::selectDBWithId(QSqlDatabase *sqldb, QList<Terran> &list){
    QList<Terran> readlist={};
    QString sqlStr="select * from terran where id=:id";
    QSqlQuery sqlQuery(*sqldb);

    bool success=false;
    for(Terran terran:list){
        sqlQuery.prepare(sqlStr);
        sqlQuery.bindValue(":id",terran.id);

        success=sqlQuery.exec();
        if(!success){
            //���ڲ��Ҷ��ԣ�һ�����ʧ�ܾ�ʧ���ˣ����ػع�ȫ��
            //            sqldb->rollback();//�ع�
            qDebug()<<"����ʧ��,idΪ:"<<terran.id<<" ����Ϊ:"<<sqlQuery.lastError();
        }else{
            while(sqlQuery.next())
            {
                Terran terran;
                terran.id = sqlQuery.value("id").toInt();
                terran.name=sqlQuery.value("name").toString();
                terran.openId=sqlQuery.value("openId").toString();
                terran.photoUrl=sqlQuery.value("photoUrl").toString();
                terran.position=sqlQuery.value("position").toString();
                terran.faceToken=sqlQuery.value("faceToken").toString();
                terran.departmentId=sqlQuery.value("departmentId").toInt();
                terran.department=sqlQuery.value("department").toString();
                terran.begTime=sqlQuery.value("begTime").toString();
                terran.endTime=sqlQuery.value("endTime").toString();
                terran.date=sqlQuery.value("date").toString();
                terran.typeOfWork=sqlQuery.value("typeOfwork").toBool();
                terran.isUpdate=sqlQuery.value("isUpdate").toBool();

                qDebug()<<"111111:"<<sqlQuery.value("typeOfwork").toBool();

                readlist.append(terran);
            }
        }
    }

    list.clear();
    list=readlist;
}

void SQLDataBase::saveImage(QSqlDatabase *sqldb, QList<Terran> &list, QImage *image){
    if(image==nullptr){
        return;
    }
    sqldb->transaction();//��ʼ���������ύ����Ч��
    Terran terran=list.first();//ֻ���һ��
    QByteArray byteArray;
    image->save(DIRPATH_QIMAGE_TEMPORARY,"JPG");//���ܷ�������ͼƬʲô��ʽ��ͨͨ����Ϊjpg��ѹ����С�����ұ�֤��λ����ĸ�ʽ�ɿ�
    QFile file(DIRPATH_QIMAGE_TEMPORARY);
    file.open(QIODevice::ReadOnly);
    byteArray=file.readAll();
    file.close();

    QString sqlStr = "insert into terranImageData values (:id,:imageData)";
    QSqlQuery sqlQuery(*sqldb);

    sqlQuery.prepare(sqlStr);
    sqlQuery.bindValue(":id",terran.id);
    sqlQuery.bindValue(":imageData",byteArray);

    bool success=false;
    success=sqlQuery.exec();

    if(!success){
        //���ڸ��¶��ԣ�һ�����ʧ�ܾ�ʧ���ˣ����ػع�ȫ��
        //            sqldb->rollback();//�ع�
        qDebug()<<QString::fromLocal8Bit("����ͼƬʧ��,idΪ:")<<terran.id<<" ����Ϊ:"<<sqlQuery.lastError();
    }else{
        qDebug()<<QString::fromLocal8Bit("����ͼƬ�ɹ�:")<<terran.id;
    }
    sqldb->commit();
}

void SQLDataBase::deleteImage(QSqlDatabase *sqldb, QList<Terran> &list){
    sqldb->transaction();//��ʼ���������ύ����Ч��
    QString sqlStr="delete from terranImageData where id = :id";
    QSqlQuery sqlQuery(*sqldb);

    bool success=false;

    for(Terran deleteTerran:list){
        sqlQuery.prepare(sqlStr);
        sqlQuery.bindValue(":id",deleteTerran.id);
        success=sqlQuery.exec();
        if(!success){
            //���ڲ��Ҷ��ԣ�һ�����ʧ�ܾ�ʧ���ˣ����ػع�ȫ��
            //            sqldb->rollback();//�ع�
            qDebug()<<"ɾ��ͼƬʧ��,idΪ:"<<deleteTerran.id<<" ����Ϊ:"<<sqlQuery.lastError();
        }else{
        }
    }
    sqldb->commit();
}

void SQLDataBase::updateImage(QSqlDatabase *sqldb, QList<Terran> &list, QImage *image){
    Terran terran=list.first();//ֻ���һ��
    QByteArray byteArray;
    image->save(DIRPATH_QIMAGE_TEMPORARY,"JPG");//���ܷ�������ͼƬʲô��ʽ��ͨͨ����Ϊjpg��ѹ����С�����ұ�֤��λ����ĸ�ʽ�ɿ�
    QFile file(DIRPATH_QIMAGE_TEMPORARY);
    file.open(QIODevice::ReadOnly);
    byteArray=file.readAll();
    file.close();
    QString sqlStr="update terranImageData set imageData=:imageData where id = :id";

    QSqlQuery sqlQuery(*sqldb);

    sqlQuery.prepare(sqlStr);
    sqlQuery.bindValue(":imageData",byteArray);

    bool success=false;
    success=sqlQuery.exec();
    if(!success){
        //���ڲ��Ҷ��ԣ�һ�����ʧ�ܾ�ʧ���ˣ����ػع�ȫ��
        //            sqldb->rollback();//�ع�
        qDebug()<<"����ͼƬʧ��,idΪ:"<<terran.id<<" ����Ϊ:"<<sqlQuery.lastError();
    }else{
    }
}

void SQLDataBase::loadImage(QSqlDatabase *sqldb, QList<Terran> &list, QImage *image){
    Terran terran=list.first();//ֻ���һ��
    QString sqlStr="select * from terranImageData where id=:id";
    QSqlQuery sqlQuery(*sqldb);

    sqlQuery.prepare(sqlStr);
    sqlQuery.bindValue(":id",terran.id);

    bool success=false;
    success=sqlQuery.exec();
    if(!success){
        //���ڲ��Ҷ��ԣ�һ�����ʧ�ܾ�ʧ���ˣ����ػع�ȫ��
        //            sqldb->rollback();//�ع�
        qDebug()<<"����ʧ��,idΪ:"<<terran.id<<" ����Ϊ:"<<sqlQuery.lastError();
    }else{
        while(sqlQuery.next()){
            QByteArray byte=sqlQuery.value(1).toByteArray();
            if(byte.size()>0){
                //                image=new QImage();
                image->loadFromData(byte,"JPG");
                //    TODO            image->save("E:\\dbimage.jpg");
            }
        }
    }

}
