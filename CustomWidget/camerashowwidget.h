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

/**
 * @brief The CameraShowWidget class
 * œ‘ æ…„œÒª˙æµÕ∑
 */
class CameraShowWidget:public QWidget
{
    Q_OBJECT
public:
    CameraShowWidget(QWidget *parent=nullptr);

    const int width=1140;
    const int height=845;

    void captureImageFromCamera();

    CustomCameraViewFinder *customViewFinder;
public slots:
    void onTerranEnter(QList<Terran> terranList);
signals:
    void sendCaptureImage(QImage image);

    void newTerranSign(Terran terran);
protected:
    virtual void paintEvent(QPaintEvent *event);
private slots:
    void receiveCapturedImage(int,QImage);
private:
    void init();
    QPixmap noCameraInputBackgroundPix;

    QCamera *camera;
    QCameraImageCapture *cameraImageCapture;

    QList<Terran> inCameraTerranList={};
};

#endif // CAMERASHOWWIDGET_H
