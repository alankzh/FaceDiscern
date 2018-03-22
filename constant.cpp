#include "constant.h"

//const QString HEART_BEAT_URL=QString::fromLocal8Bit("http://119.23.151.219/api/Cisdi.ashx?method=GetUrlAll&Number=0");

//const QString DOWNLOAD_PIC_URL_PRE="http://119.23.151.219/UpLoad/";

//const QString SEND_SIGN_IN_MESSAGE_URL="http://119.23.151.219/api/Cisdi.ashx?method=ClockData";
//示例  http://119.23.151.219/api/Cisdi.ashx?method=ClockData&Name=吕云逍&UserId=350&Type=true&Department=24&Work=true

 QString Constant::SERVER_URL="";

 QString Constant::HEART_BEAT_URL="";

 QString Constant::DOWNLOAD_PIC_URL_PRE="";

 QString Constant::SEND_SIGN_IN_MESSAGE_URL="";

 bool Constant::SAVE_DISCERN=false;
void Constant::Read_Configuration(){
//    qDebug()<<QDir::currentPath();
    QDomDocument doc;
    QFile file(QDir::currentPath()+QString::fromLocal8Bit("/config/config.xml"));
    QString error = "";
    int row = 0, column = 0;
    if (!file.open(QIODevice::ReadOnly)) return ;

    if(!doc.setContent(&file, false, &error, &row, &column)){
        qDebug() << "parse file failed:" << row << "---" << column <<":" <<error;
        file.close();
        return ;
    }

    file.close();
    QDomElement root = doc.documentElement();
    QDomNode node = root.firstChild();
    while(!node.isNull()) {
        QDomElement element = node.toElement(); // try to convert the node to an element.
        if(element.tagName()==(QString::fromLocal8Bit("SERVER_ADDRESS"))){
            if(!element.text().isNull()){
                SERVER_URL=element.text();
            }
        }
        if(element.tagName()==(QString::fromLocal8Bit("SAVE_DISCERN"))){
            if(!element.text().isNull()){
                QVariant tempValue=element.text();
                Constant::SAVE_DISCERN=tempValue.toBool();
            }
        }
        node = node.nextSibling();
    }
    if(Constant::SERVER_URL.isEmpty()){
        Constant::SERVER_URL=QString::fromLocal8Bit("http://119.23.151.219");
    }
    Constant::HEART_BEAT_URL=Constant::SERVER_URL+QString::fromLocal8Bit("/api/Cisdi.ashx?method=GetUrlAll&Number=0");
    Constant::DOWNLOAD_PIC_URL_PRE=Constant::SERVER_URL+QString::fromLocal8Bit("/UpLoad/");
    Constant::SEND_SIGN_IN_MESSAGE_URL=Constant::SERVER_URL+QString::fromLocal8Bit("/api/Cisdi.ashx?method=ClockData");

//    qDebug()<<"1SAVE_DISCERN:"<<Constant::SAVE_DISCERN;
    //        qDebug()<<HEART_BEAT_URL;
    //        qDebug()<<DOWNLOAD_PIC_URL_PRE;
    //        qDebug()<<SEND_SIGN_IN_MESSAGE_URL;
    return ;
}


const QString FACE_DB_CONNECTION_NAME="FACE_DB_CONNECTION_NAME";

const QString HEART_DB_CONNECTION_NAME="HEART_DB_CONNECTION_NAME";

const QString UI_DB_CONNECTION_NAME="UI_DB_CONNECTION_NAME";


const unsigned int BEAT_CIRCLE=5000;

const unsigned int SCREEN_SHOT_CIRCLE=200;

const unsigned int MAIN_WIDGET_WIDTH=1920;

const unsigned int MAIN_WIDGET_HEIGHT=1080;

const unsigned int CLEAR_SIGN_CACHE_INTERVAL=60000;//60s清空一次

