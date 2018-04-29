#ifndef OPENCVCAMERAVIEWWIDGET_H
#define OPENCVCAMERAVIEWWIDGET_H

#include <QtWidgets>
#include <QtGui>
#include "terran.h"
#include "CustomWidget/facepainterrect.h"

class OpencvCameraViewWidget:public QWidget
{
    Q_OBJECT
public:
    OpencvCameraViewWidget(QWidget *parent=nullptr);
    ~OpencvCameraViewWidget();

    const int width=1152;//640*1.8  (640,480)为opencv默认截图大小
    const int height=864;//480*1.8

    void addTerranRect(Terran terran);
    void startAppearAnimator(int terranId);
    void startDisappearAnimator(int terranId);
    void updateFaceRect(Terran terran);
public slots:
        void updateView(QImage &image);
signals:
    void viewShowEnd();//一帧显示完毕
protected:
    virtual void paintEvent(QPaintEvent *event);
private slots:
    void answerUpdateUI();
    void onDisappearAnimatorEnd(int terranId);
private:
    QList<FacePainterRect*> faceRectList;
    QPixmap backgroundPix;
};

#endif // OPENCVCAMERAVIEWWIDGET_H
