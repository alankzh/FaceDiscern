#include "dbctestwidget.h"

DBCTestWidget::DBCTestWidget(QWidget *parent):QWidget(parent)
{
    testDBName="xxxx";
    QLabel *label=new QLabel(this);
    label->setGeometry(100,100,400,400);

    SQLDataBase::instance()->connectionDB(testDBName);

//    QImage *image=new QImage();

//    QImage image;
//    Terran terran;
//    terran.id=1;
//    QList<Terran> list;
//    list.append(terran);

//    SQLDataBase::instance()->operationDB(testDBName, SQLDataBase::OperationWay::LoadDBImage,list,&image);

    Terran terran2;
    terran2.id=350;
    QList<Terran> list2;
    list2.append(terran2);
    SQLDataBase::instance()->operationDB(testDBName, SQLDataBase::OperationWay::SelectDBWithId,list2);

    qDebug()<<list2.first().department;

//    qDebug()<<image.byteCount();
//    if(image.byteCount()>0){
//        label->setPixmap(QPixmap::fromImage(image));
//    }
}
