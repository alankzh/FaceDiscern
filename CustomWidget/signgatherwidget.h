#ifndef SIGNGATHERWIDGET_H
#define SIGNGATHERWIDGET_H

#include <QtWidgets>
#include <QtGui>
#include "CustomWidget/signinwidget.h"
#include "CustomWidget/terransigninmessagewidget.h"
#include "httputil.h"

/**
 * @brief The SignGatherWidget class
 * 签到信息汇总控件
 */
class SignGatherWidget:public QWidget
{
    Q_OBJECT
public:
    SignGatherWidget(QWidget *parent=nullptr);
    ~SignGatherWidget();
    const int width=730;
    const int height=721;//space为5
    enum AnimatorStatus{
        Moving,
        End
    };
    void terranSignIn(Terran terran,TerranSignInMessageWidget::LabelType labelType=TerranSignInMessageWidget::LabelType::Work);
public slots:
    void signIn(Terran terran);
signals:
    void animatorEnd();
protected:
    virtual void paintEvent(QPaintEvent *event);
private slots:
    void onMovingEnd();
    void animatorStageTwo();//第二段动画
private:
    void init();
    void startSignInAnimator();
    SignGatherWidget::AnimatorStatus status= SignGatherWidget::AnimatorStatus::End;
    QPixmap backgroundPix;
    QList<SignInWidget*> widgetList={};//6个签到信息展示控件的队列，从队列末先进，从队列首先出
    QList<Terran> needAnimatorList={};//动画缓存
    const int Y_FALL=121;//(116+5) 往下坠落高度+间距
    Terran nowAnimatorTerran;
};

#endif // SIGNGATHERWIDGET_H
