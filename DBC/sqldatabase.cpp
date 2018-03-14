#include "sqldatabase.h"
#include "singleton.h"

SQLDataBase::SQLDataBase(QObject* parent):QObject(parent){
}

SQLDataBase* SQLDataBase::createInstance(){
    return new SQLDataBase();
}

SQLDataBase::~SQLDataBase(){
    //对没有关闭的数据库连接进行关闭
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
 * 根据连接名，建立连接，加入连接池中
 * 不同线程建立的连接只能自己使用
 * @param connectionName
 * 连接名
 */
bool SQLDataBase::connectionDB(QString connectionName){
    QMutexLocker locker(&mutex);//锁
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
    //若没有表terran，则创建
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

    //若没有表terranImageData，则创建
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
 * 数据库操作，目前只存在一张表，故不需要对表对象进行封装
 * 如果有多张表，那么QList<Terran> 应该为QList<TableObject>
 * TableObject 中对象应该存放map<字段名，字段值>
 * 字段值也应该是TableFiledObject，里面存放：值类型、值
 * 值类型对应数据库中字段类型
 * 这样扩展后，理论上可以将数据库中表映射为对象，对对象的操作，即是对数据库的操作
 * @param connectionName
 * 连接名
 * @param operationWay
 * 操作方式
 * @param list
 * 操作数据,或返回数据
 * @return
 */
bool SQLDataBase::operationDB(QString connectionName, SQLDataBase::OperationWay operationWay, QList<Terran> &list,QImage *image){
    QMutexLocker locker(&mutex);//锁

    QMap<QString,QSqlDatabase*>::iterator iterator=dbMap.find(connectionName);
    if(iterator!=dbMap.end()){
        if(!dbMap[connectionName]->isOpen()){
            dbMap[connectionName]->open();//打开数据库连接
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
    dbMap[connectionName]->close();//连接不使用时关闭为好
    return true;
}

//增
void SQLDataBase::insertDB(QSqlDatabase *sqldb, QList<Terran> &list){
    sqldb->transaction();//开始事务，批量提交更有效率

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
            //对于插入而言，一条语句失败就失败了，不必回滚全部
            //            sqldb->rollback();//回滚
            qDebug()<<QString::fromLocal8Bit("插入失败,id为:")<<terran.id<<" 错误为:"<<sqlQuery.lastError();
        }else{
            qDebug()<<QString::fromLocal8Bit("插入成功:")<<terran.name;
        }

    }

    sqldb->commit();
}

//删
void SQLDataBase::deleteDB(QSqlDatabase *sqldb, QList<Terran> &list){
    sqldb->transaction();//开始事务，批量提交更有效率

    QString sqlStr="delete from terran where id = :id";
    QSqlQuery sqlQuery(*sqldb);

    bool success=false;
    for(Terran terran:list){
        sqlQuery.prepare(sqlStr);
        sqlQuery.bindValue(":id",terran.id);
        success=sqlQuery.exec();

        if(!success){
            //对于删除而言，一条语句失败就失败了，不必回滚全部
            //            sqldb->rollback();//回滚
            qDebug()<<"删除失败,id为:"<<terran.id<<" 错误为:"<<sqlQuery.lastError();
        }
    }

    sqldb->commit();
}

//更新
void SQLDataBase::updateDB(QSqlDatabase *sqldb, QList<Terran> &list){
    sqldb->transaction();//开始事务，批量提交更有效率

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

        qDebug()<<QString::fromLocal8Bit("更新:")<<terran.typeOfWork;

        sqlQuery.bindValue(":isUpdate",terran.isUpdate);
        sqlQuery.bindValue(":id",terran.id);

        success=sqlQuery.exec();

        if(!success){
            //对于更新而言，一条语句失败就失败了，不必回滚全部
            //            sqldb->rollback();//回滚
            qDebug()<<"更新失败,id为:"<<terran.id<<" 错误为:"<<sqlQuery.lastError();
        }else{
        }
    }

    sqldb->commit();
}

//全部查找
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
        qDebug()<<"查询全部失败";
    }

}

//依据id查找
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
            //对于查找而言，一条语句失败就失败了，不必回滚全部
            //            sqldb->rollback();//回滚
            qDebug()<<"查找失败,id为:"<<terran.id<<" 错误为:"<<sqlQuery.lastError();
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
    sqldb->transaction();//开始事务，批量提交更有效率
    Terran terran=list.first();//只存第一个
    QByteArray byteArray;
    image->save(DIRPATH_QIMAGE_TEMPORARY,"JPG");//不管服务器上图片什么格式，通通保存为jpg，压缩大小，并且保证了位运算的格式可控
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
        //对于更新而言，一条语句失败就失败了，不必回滚全部
        //            sqldb->rollback();//回滚
        qDebug()<<QString::fromLocal8Bit("插入图片失败,id为:")<<terran.id<<" 错误为:"<<sqlQuery.lastError();
    }else{
        qDebug()<<QString::fromLocal8Bit("插入图片成功:")<<terran.id;
    }
    sqldb->commit();
}

void SQLDataBase::deleteImage(QSqlDatabase *sqldb, QList<Terran> &list){
    sqldb->transaction();//开始事务，批量提交更有效率
    QString sqlStr="delete from terranImageData where id = :id";
    QSqlQuery sqlQuery(*sqldb);

    bool success=false;

    for(Terran deleteTerran:list){
        sqlQuery.prepare(sqlStr);
        sqlQuery.bindValue(":id",deleteTerran.id);
        success=sqlQuery.exec();
        if(!success){
            //对于查找而言，一条语句失败就失败了，不必回滚全部
            //            sqldb->rollback();//回滚
            qDebug()<<"删除图片失败,id为:"<<deleteTerran.id<<" 错误为:"<<sqlQuery.lastError();
        }else{
        }
    }
    sqldb->commit();
}

void SQLDataBase::updateImage(QSqlDatabase *sqldb, QList<Terran> &list, QImage *image){
    Terran terran=list.first();//只存第一个
    QByteArray byteArray;
    image->save(DIRPATH_QIMAGE_TEMPORARY,"JPG");//不管服务器上图片什么格式，通通保存为jpg，压缩大小，并且保证了位运算的格式可控
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
        //对于查找而言，一条语句失败就失败了，不必回滚全部
        //            sqldb->rollback();//回滚
        qDebug()<<"更新图片失败,id为:"<<terran.id<<" 错误为:"<<sqlQuery.lastError();
    }else{
    }
}

void SQLDataBase::loadImage(QSqlDatabase *sqldb, QList<Terran> &list, QImage *image){
    Terran terran=list.first();//只存第一个
    QString sqlStr="select * from terranImageData where id=:id";
    QSqlQuery sqlQuery(*sqldb);

    sqlQuery.prepare(sqlStr);
    sqlQuery.bindValue(":id",terran.id);

    bool success=false;
    success=sqlQuery.exec();
    if(!success){
        //对于查找而言，一条语句失败就失败了，不必回滚全部
        //            sqldb->rollback();//回滚
        qDebug()<<"查找失败,id为:"<<terran.id<<" 错误为:"<<sqlQuery.lastError();
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
