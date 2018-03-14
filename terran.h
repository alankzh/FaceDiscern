#ifndef TERRAN_H
#define TERRAN_H

#include <QString>
#include <QList>
#include <QMetaType>
#include "ArcFacePackage/arcsoft_fsdk_face_recognition.h"

/**
 * @brief The Terran class
 * ��Ҫ����ǩ����Ա����
 * ����Java Bean
 */
class Terran
{
public:
    Terran();
    int id;//���ݿ�id
    QString name;//����
    QString openId;//Ψһid
    QString photoUrl;//ͷ��ͼƬ���ص�ַ
    QString position;//ְλ
    QString faceToken;
    int departmentId;//��������id
    QString department;//������Դ��
    QString begTime;
    QString endTime;
    QString date;
    bool typeOfWork=false;
    bool isUpdate=false;//update����ΪisUpdate��ԭ�������ݿ�ؼ��ֳ�ͻ

    bool equals(Terran &terran);
    bool ServerDataequals(Terran &terran);
    QString toString();

    static void quickSort(QList<Terran> &list,int begin,int end);


    //���ֶ�Terran�౾��û�У���Դ��TerranFace�࣬�洢���ο�
    //���������źŷ��������������緢��һ������������Ӵ�����
    AFR_FSDK_FACEINPUT frFaceInput;//��������


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
