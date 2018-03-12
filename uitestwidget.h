#ifndef UITESTWIDGET_H
#define UITESTWIDGET_H

#include <QWidget>
#include "constant.h"
#include "CustomWidget/customimageview.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QBitmap>
#include <QPaintEvent>
#include <QtWidgets>
#include <QPainter>
#include <QtGui>
#include "CustomWidget/titlewidget.h"
#include "CustomWidget/systemlogowidget.h"

class UITestWidget:public QWidget
{
    Q_OBJECT
public:
    UITestWidget(QWidget *parent=nullptr);
protected:
    virtual void paintEvent(QPaintEvent* event);
private:
    void init();
    QPixmap background;


    TitleWidget *titleWidget;
    SystemLogoWidget *systemLogoWidget;
};

#endif // UITESTWIDGET_H
