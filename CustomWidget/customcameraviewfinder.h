#ifndef CUSTOMCAMERAVIEWFINDER_H
#define CUSTOMCAMERAVIEWFINDER_H

#include <QtWidgets>
#include <QtGui>

#include <QCameraViewfinder>
#include <QVideoWidget>
#include <QPaintEvent>
#include "terran.h"
#include <QList>
#include "facepainterrect.h"


/**
 * @brief The CustomCameraViewFinder class
 * Ϊ������ϻ�ͼ���̳�֮
 */
class CustomCameraViewFinder:public QCameraViewfinder
{
    Q_OBJECT
public:
    CustomCameraViewFinder(QWidget *parent=nullptr);
    ~CustomCameraViewFinder();

    void addTerranRect(Terran terran);
    void startAppearAnimator(int terranId);
    void startDisappearAnimator(int terranId);
    void updateFaceRect(Terran terran);
protected:
    virtual void paintEvent(QPaintEvent *event);

private slots:
    void answerUpdateUI();
    void onDisappearAnimatorEnd(int terranId);
private:
    QList<FacePainterRect*> faceRectList;
};

#endif // CUSTOMCAMERAVIEWFINDER_H
