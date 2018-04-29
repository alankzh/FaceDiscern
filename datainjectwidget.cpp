#include "datainjectwidget.h"

DataInjectWidget::DataInjectWidget(QWidget *parent):QWidget(parent){
    QString  connectionName=QString::fromLocal8Bit("doudingTemporary");
    Terran terran;
    terran.id=9527;
    terran.name=QString::fromLocal8Bit("���S");
    terran.department=QString::fromLocal8Bit("��Ƽ�CEO");

    QList<Terran> list={};
    list.append(terran);

    SQLDataBase::instance()->connectionDB(connectionName);
    SQLDataBase::instance()->operationDB(connectionName,SQLDataBase::OperationWay::InsertDB,list);

    QImage image("E:\\wy.jpg");
    SQLDataBase::instance()->operationDB(connectionName,SQLDataBase::OperationWay::SavaImage,list,&image);
}
