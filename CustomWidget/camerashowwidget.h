#ifndef CAMERASHOWWIDGET_H
#define CAMERASHOWWIDGET_H

#include <QtWidgets>
#include <QtGui>
#include <QPixmap>
#include "terran.h"
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QList>
#include "facerectwidget.h"
#include "customcameraviewfinder.h"
#include "httputil.h"
#include "constant.h"
#include "DBC/sqldatabase.h"

/**
 * @brief The CameraShowWidget class
 * 显示摄像机镜头
 */
class CameraShowWidget:public QWidget
{
    Q_OBJECT
public:
    CameraShowWidget(QWidget *parent=nullptr);
    ~CameraShowWidget();

    const int width=1140;
    const int height=845;

    void captureImageFromCamera();

    CustomCameraViewFinder *customViewFinder;
public slots:
    void onTerranEnter(QList<Terran> terranList);

    void clearSignedTerranCache();
signals:
    void sendCaptureImage(QImage &image);

    void newTerranSign(Terran terran);
protected:
    virtual void paintEvent(QPaintEvent *event);
private slots:
    void receiveCapturedImage(int,QImage);
private:
    void init();
    void insertTerran(QList<Terran> &insertList);
    QPixmap noCameraInputBackgroundPix;

    QCamera *camera;
    QCameraImageCapture *cameraImageCapture;

    QList<Terran> inCameraTerranList={};

    QList<Terran> signedList={};//已经签到的人，5分钟清空一次

    HttpUtil *httpUtil;
};

#endif // CAMERASHOWWIDGET_H
