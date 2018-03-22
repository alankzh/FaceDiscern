#ifndef TERRANSIGNINTIMEWIDGET_H
#define TERRANSIGNINTIMEWIDGET_H

#include <QtWidgets>
#include <QtGui>
#include "constant.h"
#include "CustomWidget/customtextview.h"

/**
 * @brief The TerranSignInTimeWidget class
 * 一行  签到信息展示栏 右侧的 签到时间展示栏
 */
class TerranSignInTimeWidget:public QWidget
{
    Q_OBJECT
public:
    TerranSignInTimeWidget(QWidget *parent=nullptr);
    ~TerranSignInTimeWidget();
    void setTimeAndDate();
    const int width=157;
    const int height=116;
protected:
    virtual void paintEvent(QPaintEvent *event);
private:
    void init();
    QPixmap backgroundPix;
    CustomTextView *timeTextView;
    CustomTextView *dateTextVoew;
    QVBoxLayout *mainLayout;
};

#endif // TERRANSIGNINTIMEWIDGET_H
