#ifndef SIGNBOTTOMRIGHTWIDGET_H
#define SIGNBOTTOMRIGHTWIDGET_H

#include <QtWidgets>
#include <QtGui>
#include "CustomWidget/customtextview.h"

class SignBottomRightWidget:public QWidget
{
    Q_OBJECT
public:
    SignBottomRightWidget(QWidget *parent=nullptr);
    const int width=157;
    const int height=116;
    void addSignNum(int num);

    void clearSignedTerranNum();//每日12点清空已签到人数
protected:
    virtual void paintEvent(QPaintEvent *event);
private:
    void init();
    QPixmap backgroundPix;
    CustomTextView *numText;
    CustomTextView *unitText;
    int nowNum;
};

#endif // SIGNBOTTOMRIGHTWIDGET_H
