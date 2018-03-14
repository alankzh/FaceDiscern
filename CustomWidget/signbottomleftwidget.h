#ifndef SIGNBOTTOMLEFTWIDGET_H
#define SIGNBOTTOMLEFTWIDGET_H

#include <QtWidgets>
#include <QtGui>
#include "CustomWidget/customtextview.h"

class SignBottomLeftWidget:public QWidget
{
    Q_OBJECT
public:
    SignBottomLeftWidget(QWidget *parent=nullptr);
    const int width=570;
    const int height=116;

protected:
    virtual void paintEvent(QPaintEvent *event);
private:
    void init();
    QPixmap backgroundPix;
    CustomTextView *textView;
};

#endif // SIGNBOTTOMLEFTWIDGET_H
