#ifndef FACEDISCERNTESTWIDGET_H
#define FACEDISCERNTESTWIDGET_H

#include <QWidget>
#include <QtWidgets>
#include <QtGui>
#include "terran.h"
#include "ArcFacePackage/terranfacefeature.h"
#include "ArcFacePackage/facediscern.h"

class FaceDiscernTestWidget:public QWidget
{
    Q_OBJECT
public:
    FaceDiscernTestWidget(QWidget *parent=nullptr);
private:
    FaceDiscern *faceDiscern;
private slots:
    void startPair(bool);
};

#endif // FACEDISCERNTESTWIDGET_H
