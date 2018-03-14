#ifndef TERRAN_H
#define TERRAN_H

#include <QString>
#include <QList>
#include <QMetaType>
#include "ArcFacePackage/arcsoft_fsdk_face_recognition.h"

/**
 * @brief The Terran class
 * 需要管理签到的员工类
 * 类似Java Bean
 */
class Terran
{
public:
    Terran();
    int id;//数据库id
    QString name;//姓名
    QString openId;//唯一id
    QString photoUrl;//头像图片下载地址
    QString position;//职位
    QString faceToken;
    int departmentId;//所属部门id
    QString department;//人力资源部
    QString begTime;
    QString endTime;
    QString date;
    bool typeOfWork=false;
    bool isUpdate=false;//update更名为isUpdate，原名与数据库关键字冲突

    bool equals(Terran &terran);
    bool ServerDataequals(Terran &terran);
    QString toString();

    static void quickSort(QList<Terran> &list,int begin,int end);


    //此字段Terran类本来没有，来源于TerranFace类，存储矩形框，
    //后来觉得信号发送连个参数不如发送一个参数，故添加此属性
    AFR_FSDK_FACEINPUT frFaceInput;//人脸矩形


    //    bool operator==(const Terran &ref){
    //        return id==ref.id;
    //    }
    //    bool operator!=(const Terran &ref){
    //        return !(id==ref.id);
    //    }

private:
    static int partition(QList<Terran> &list,int begin,int end);
};

#endif // TERRAN_H
