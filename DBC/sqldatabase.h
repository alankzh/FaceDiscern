#ifndef SQLDATABASE_H
#define SQLDATABASE_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QMutex>
#include <QMap>
#include <QStringList>
#include <QDebug>
#include <QObject>
#include <terran.h>
#include <QImage>
#include <QFile>
#include "constant.h"

/**
 * @brief The SQLDataBase class
 * sql数据库管理，创建连接池，供不同的线程根据连接池名调用
 * 自身为单例
 * 设置Mutex保护，从addDatabase/database到open的部分,保证其原子性
 */
class SQLDataBase:public QObject
{
    Q_OBJECT
private:
    SQLDataBase(QObject* parent = 0);
    static SQLDataBase* createInstance();

    QMutex mutex;//实例互斥锁

    QMap<QString,QSqlDatabase*> dbMap;

    const QString FACE_TABLE_NAME="terran";
    const QString FACE_IMAGE_TABLE_NAME="terranImageData";

    void insertDB(QSqlDatabase *sqldb,QList<Terran> &list);
    void updateDB(QSqlDatabase *sqldb,QList<Terran> &list);
    void deleteDB(QSqlDatabase *sqldb,QList<Terran> &list);
    void selectDB(QSqlDatabase *sqldb,QList<Terran> &list);
    void selectDBWithId(QSqlDatabase *sqldb,QList<Terran> &list);

    void saveImage(QSqlDatabase *sqldb,QList<Terran> &list,QImage *image);
    void deleteImage(QSqlDatabase *sqldb,QList<Terran> &list,QImage *image);
    void updateImage(QSqlDatabase *sqldb,QList<Terran> &list,QImage *image);
    void loadImage(QSqlDatabase *sqldb,QList<Terran> &list,QImage *image);
public:
    bool connectionDB(QString connectionName);

    enum OperationWay{
        InsertDB,
        DeleteDB,
        UpdateDB,
        SelectDB,
        SelectDBWithId,
        SavaImage,
        DeleteImage,
        UpdateImage,
        LoadDBImage
    };

    bool operationDB(QString connectionName,SQLDataBase::OperationWay operationWay,QList<Terran> &list,QImage *image=nullptr);

    ~SQLDataBase();
    static SQLDataBase* instance();
};

#endif // SQLDATABASE_H
