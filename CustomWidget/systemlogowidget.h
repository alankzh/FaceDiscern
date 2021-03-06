#ifndef SYSTEMLOGOWIDGET_H
#define SYSTEMLOGOWIDGET_H

#include <QtWidgets>
#include <QtGui>
#include "CustomWidget/customimageview.h"
#include "constant.h"
#include "CustomWidget/customtextview.h"
#include <QTimer>
#include "DBC/sqldatabase.h"
#include "applicationutil.h"

/**
 * @brief The SystemLogoWidget class
 * 标题栏之前，显示LOGO 和 时间的控件
 * 他被赋予了新意思，软件跟时间周期有关的由他负责
 */
class SystemLogoWidget:public QWidget
{
    Q_OBJECT
public:
    SystemLogoWidget(QWidget *parent=nullptr);

    const int width=1920;
    const int height=150;
signals:
    void clearTerranSignCache();//通知清空缓存
    void clearTerranSignNum();//通知清空签到人数
    void restartApplication();//通知重启应用
protected:
    virtual void paintEvent(QPaintEvent *event);
private:
    void init();
    QPixmap backgroundPix;
    CustomImageView *CIDILogo;
    CustomTextView *systemName;
    CustomTextView *timeText;
    CustomTextView *calendarText;
    QTimer *qtimer;
    int timeMsec;

    QTime restartTime;
    QTime restartTime1;
    QTime restartTime2;
    QTime restartTime3;
    QTime restartTime4;
    QTime restartTime5;
private slots:
    void updateTime();
};

#endif // SYSTEMLOGOWIDGET_H
