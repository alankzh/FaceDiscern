#include "widget.h"
#include <QApplication>
#include <QMetaType>
#include <QList>
#include "terran.h"
#include <QByteArray>
#include "uitestwidget.h"
#include "CustomWidget/titlewidget.h"
#include "CustomWidget/customtextview.h"
#include "CustomWidget/systemlogowidget.h"
#include "CustomWidget/camerashowwidget.h"
#include "dbctestwidget.h"
#include "CustomWidget/customcircleimageview.h"
#include <QImage>
#include "CustomWidget/terransigninmessagewidget.h"
#include "CustomWidget/signinwidget.h"
#include "CustomWidget/signgatherwidget.h"
#include "CustomWidget/signbottomwidget.h"
#include "facediscerntestwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<QList<Terran>>("QList<Terran>");//注册信号槽中通信的数据为元数据
    qRegisterMetaType<QByteArray>("QByteArray&");
    qRegisterMetaType<Terran>("Terran");

    Widget w;//主界面
    w.show();

    //            DBCTestWidget dbTest; 数据库测试
    //            dbTest.show();

    //    FaceDiscernTestWidget faceDiscernWidget; //人脸对比测试
    //    faceDiscernWidget.show();

    return a.exec();
}
