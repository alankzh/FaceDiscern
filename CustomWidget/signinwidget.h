#ifndef SIGNINWIDGET_H
#define SIGNINWIDGET_H

#include <QtWidgets>
#include <QtGui>
#include "CustomWidget/terransigninmessagewidget.h"
#include "CustomWidget/terransignintimewidget.h"
#include "terran.h"
#include "DBC/sqldatabase.h"
#include <QtNetwork>
#include <QUrl>
#include "httputil.h"
#include "constant.h"

/**
 * @brief The SignInWidget class
 * 一行签到信息栏 包含左边人员信息和右边时间信息
 */
class SignInWidget:public QWidget
{
public:
    SignInWidget(QWidget *parent=nullptr);
    ~SignInWidget();
    const int width=728;
    const int height=116;
    int id=0;
    void insertTerranInformation(Terran terran,TerranSignInMessageWidget::LabelType labelType=TerranSignInMessageWidget::LabelType::Work);
protected:
    virtual void paintEvent(QPaintEvent *event);
private:
    void init();

    QPixmap backgroundPix;
    TerranSignInMessageWidget *messageWidget;
    TerranSignInTimeWidget *timeWidget;
};

#endif // SIGNINWIDGET_H
