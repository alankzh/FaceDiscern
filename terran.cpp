#include "terran.h"

Terran::Terran()
{

}

/**
 * @brief Terran::equals
 * 对比两个Terran对象是否相同
 * @param terran
 * @return
 * 返回false表示不同
 */
bool Terran::equals(Terran &terran){
    if(this->id!=terran.id){
        return false;
    }else if(this->departmentId!=terran.departmentId){
        return false;
    }else if(this->name!=terran.name){
        return false;
    }else if(this->openId!=terran.openId){
        return false;
    }else if(this->photoUrl!=terran.photoUrl){
        return false;
    }else if(this->position!=terran.position){
        return false;
    }else if(this->faceToken!=terran.faceToken){
        return false;
    }else if(this->department!=terran.department){
        return false;
    }else if(this->begTime!=terran.begTime){
        return false;
    }else if(this->endTime!=terran.endTime){
        return false;
    }else if(this->date!=terran.date){
        return false;
    }else if(this->typeOfWork!=terran.typeOfWork){
        return false;
    }else if(this->isUpdate!=terran.isUpdate){
        return false;
    }else if(this->frFaceInput.rcFace.left!=terran.frFaceInput.rcFace.left){
        return false;
    }else if(this->frFaceInput.rcFace.right!=terran.frFaceInput.rcFace.right){
        return false;
    }else if(this->frFaceInput.rcFace.top!=terran.frFaceInput.rcFace.top){
        return false;
    }else if(this->frFaceInput.rcFace.bottom!=terran.frFaceInput.rcFace.bottom){
        return false;
    }

    return true;
}

/**
 * @brief Terran::ServerDataequals
 * 对比服务器上数据库是否相同
 * @param terran
 * @return
 */
bool Terran::ServerDataequals(Terran &terran){
    if(this->id!=terran.id){
        return false;
    }else if(this->departmentId!=terran.departmentId){
        return false;
    }else if(this->name!=terran.name){
        return false;
    }else if(this->openId!=terran.openId){
        return false;
    }else if(this->photoUrl!=terran.photoUrl){
        return false;
    }else if(this->position!=terran.position){
        return false;
    }else if(this->faceToken!=terran.faceToken){
        return false;
    }else if(this->department!=terran.department){
        return false;
    }else if(this->begTime!=terran.begTime){
        return false;
    }else if(this->endTime!=terran.endTime){
        return false;
    }else if(this->date!=terran.date){
        return false;
    }else if(this->typeOfWork!=terran.typeOfWork){
        return false;
    }else if(this->isUpdate!=terran.isUpdate){
        return false;
    }

    return true;
}

QString Terran::toString(){
    QString str;
    str.append(QString::fromLocal8Bit(" terran id:"));
    str.append(QString::number(this->id));

    str.append(QString::fromLocal8Bit(" terran name:"));
    str.append(this->name);
    str.append(QString::fromLocal8Bit(" terran openid:"));
    str.append(this->openId);
    str.append(QString::fromLocal8Bit(" terran photoUrl:"));
    str.append(this->photoUrl);
    str.append(QString::fromLocal8Bit(" terran position:"));
    str.append(this->position);
    str.append(QString::fromLocal8Bit(" terran faceToken:"));
    str.append(this->faceToken);
    str.append(QString::fromLocal8Bit(" terran departmentid:"));
    str.append(QString::number(this->departmentId));
    str.append(QString::fromLocal8Bit(" terran department:"));
    str.append(this->department);
    str.append(QString::fromLocal8Bit(" terran begTime:"));
    str.append(this->begTime);
    str.append(QString::fromLocal8Bit(" terran endTime:"));
    str.append(this->endTime);
    str.append(QString::fromLocal8Bit(" terran date:"));
    str.append(this->date);
    str.append(QString::fromLocal8Bit(" terran typeOfWork:"));
    str.append(QString::number(this->typeOfWork));
    str.append(QString::fromLocal8Bit(" terran isUpdate:"));
    str.append(QString::number(this->isUpdate));
    str.append(QString::fromLocal8Bit(" terran uglyType:"));
    str.append(QString::number(this->uglyType));

    return str;
}


/**
 * @brief Terran::quickSort
 * 将QList<Terran>中元素快速排序，id最小者在最前
 * @param list
 */
void Terran::quickSort(QList<Terran> &list,int begin, int end){
    if(begin >= end || list.size()<=0) {
        return;
    }
    int p = partition(list, begin, end);
    quickSort(list, begin, p-1);
    quickSort(list, p+1, end);
}

//快排需要的分割函数
int Terran::partition(QList<Terran> &list, int begin, int end){
    Terran last = list.at(end);
    int i = begin -1;
    for (int j = begin; j <= end-1; j++) {
        if(list[j].id<= last.id){
            i++;
            if(i != j){
                Terran temp;
                temp=list[j];
                list[j]=list[i];
                list[i]=temp;
            }
        }
    }
    if((i+1) != end){
        Terran temp;
        temp=list[end];
        list[end]=list[i+1];
        list[i+1]=temp;
    }
    return i+1;
}
