#ifndef POPWIDGET_H
#define POPWIDGET_H

#include <QtWidgets>
#include "customimageview.h"

class PopWidget:public QWidget
{
    Q_OBJECT
public:
    PopWidget(QWidget *parent=nullptr);
    ~PopWidget();
    void showWindow();
    void hideWindow();

private:
    CustomImageView *hintImage;
};

#endif // POPWIDGET_H
