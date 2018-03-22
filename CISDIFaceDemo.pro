#-------------------------------------------------
#
# Project created by QtCreator 2018-02-22T15:27:14
#
#-------------------------------------------------

QT       += core gui
QT += multimedia multimediawidgets
QT += network
QT +=sql
QT += winextras
QT += xml

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
include(ArcFace.pri)

TARGET = CISDIFaceDemo
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    ArcFacePackage/convertimage.cpp \
    ArcFacePackage/fdengine.cpp \
    ArcFacePackage/frengine.cpp \
    ArcFacePackage/ftengine.cpp \
    ArcFacePackage/facediscernthreadhelper.cpp \
    ArcFacePackage/facediscern.cpp \
    terran.cpp \
    ArcFacePackage/terranface.cpp \
    ArcFacePackage/terranfacefeature.cpp \
    HeartBeatPackage/heartbeat.cpp \
    HeartBeatPackage/heartthreadhelper.cpp \
    DBC/sqldatabase.cpp \
    constant.cpp \
    httputil.cpp \
    CustomWidget/customimageview.cpp \
    uitestwidget.cpp \
    CustomWidget/titlewidget.cpp \
    CustomWidget/systemlogowidget.cpp \
    CustomWidget/customtextview.cpp \
    CustomWidget/camerashowwidget.cpp \
    CustomWidget/facerectwidget.cpp \
    CustomWidget/customcameraviewfinder.cpp \
    CustomWidget/facepainterrect.cpp \
    CustomWidget/terransigninmessagewidget.cpp \
    dbctestwidget.cpp \
    CustomWidget/customcircleimageview.cpp \
    CustomWidget/terransignintimewidget.cpp \
    CustomWidget/signinwidget.cpp \
    CustomWidget/signgatherwidget.cpp \
    CustomWidget/signbottomwidget.cpp \
    CustomWidget/signbottomleftwidget.cpp \
    CustomWidget/signbottomrightwidget.cpp

HEADERS  += widget.h \
    ArcFacePackage/amcomdef.h \
    ArcFacePackage/ammem.h \
    ArcFacePackage/arcfacedeploy.h \
    ArcFacePackage/arcsoft_fsdk_age_estimation.h \
    ArcFacePackage/arcsoft_fsdk_face_detection.h \
    ArcFacePackage/arcsoft_fsdk_face_recognition.h \
    ArcFacePackage/arcsoft_fsdk_face_tracking.h \
    ArcFacePackage/arcsoft_fsdk_gender_estimation.h \
    ArcFacePackage/asvloffscreen.h \
    ArcFacePackage/convertimage.h \
    ArcFacePackage/fdengine.h \
    ArcFacePackage/frengine.h \
    ArcFacePackage/ftengine.h \
    ArcFacePackage/merror.h \
    ArcFacePackage/facediscernthreadhelper.h \
    ArcFacePackage/facediscern.h \
    terran.h \
    ArcFacePackage/terranface.h \
    ArcFacePackage/terranfacefeature.h \
    HeartBeatPackage/heartbeat.h \
    HeartBeatPackage/heartthreadhelper.h \
    DBC/sqldatabase.h \
    call_once.h \
    singleton.h \
    constant.h \
    httputil.h \
    CustomWidget/customimageview.h \
    uitestwidget.h \
    CustomWidget/titlewidget.h \
    CustomWidget/systemlogowidget.h \
    CustomWidget/customtextview.h \
    CustomWidget/camerashowwidget.h \
    CustomWidget/facerectwidget.h \
    CustomWidget/customcameraviewfinder.h \
    CustomWidget/facepainterrect.h \
    CustomWidget/terransigninmessagewidget.h \
    dbctestwidget.h \
    CustomWidget/customcircleimageview.h \
    CustomWidget/terransignintimewidget.h \
    CustomWidget/signinwidget.h \
    CustomWidget/signgatherwidget.h \
    CustomWidget/signbottomwidget.h \
    CustomWidget/signbottomleftwidget.h \
    CustomWidget/signbottomrightwidget.h


FORMS    += widget.ui

RESOURCES += \
    resources.qrc
