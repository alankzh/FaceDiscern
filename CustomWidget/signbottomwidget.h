#ifndef SIGNBOTTOMWIDGET_H
#define SIGNBOTTOMWIDGET_H

#include <QtWidgets>
#include <QtGui>
#include "CustomWidget/signbottomleftwidget.h"
#include "CustomWidget/signbottomrightwidget.h"
#include "terran.h"

/**
 * @brief The SignBottomWidget class
 * 底部签到人数汇总栏
 */
class SignBottomWidget:public QWidget
{
    Q_OBJECT
public:
    SignBottomWidget(QWidget *parent=nullptr);
    const int width=728;
    const int height=116;
    void addSignNum(int num);
public slots:
    void signIn(Terran terran);
protected:
    virtual void paintEvent(QPaintEvent *event);
private:
    void init();
    QPixmap backgroundPix;
    SignBottomLeftWidget *leftWidget;
    SignBottomRightWidget *rightWidget;
};

#endif // SIGNBOTTOMWIDGET_H
