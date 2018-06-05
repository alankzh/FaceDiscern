#include "applicationutil.h"

ApplicationUtil::ApplicationUtil(QObject *parent):QObject(parent){
}

void ApplicationUtil::restartApplication(){
    qDebug()<<"ApplicationUtil::restartApplication";
//    qApp->quit();
    QApplication::closeAllWindows();
//    QProcess::startDetached(qApp->applicationFilePath(), QStringList(qApp->applicationFilePath()));
    QProcess::startDetached(qApp->applicationFilePath());
    qApp->quit();
}

void ApplicationUtil::restartSystem(){
//    system("shutdown -r -t 0");//÷ÿ∆ÙµÁƒ‘
    QProcess::execute("shutdown -r -t 0");
}
