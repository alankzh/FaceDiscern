#ifndef SIGNGATHERWIDGET_H
#define SIGNGATHERWIDGET_H

#include <QtWidgets>
#include <QtGui>
#include "CustomWidget/signinwidget.h"
#include "CustomWidget/terransigninmessagewidget.h"
#include "httputil.h"

/**
 * @brief The SignGatherWidget class
 * ǩ����Ϣ���ܿؼ�
 */
class SignGatherWidget:public QWidget
{
    Q_OBJECT
public:
    SignGatherWidget(QWidget *parent=nullptr);
    ~SignGatherWidget();
    const int width=730;
    const int height=721;//spaceΪ5
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
    void animatorStageTwo();//�ڶ��ζ���
private:
    void init();
    void startSignInAnimator();
    SignGatherWidget::AnimatorStatus status= SignGatherWidget::AnimatorStatus::End;
    QPixmap backgroundPix;
    QList<SignInWidget*> widgetList={};//6��ǩ����Ϣչʾ�ؼ��Ķ��У��Ӷ���ĩ�Ƚ����Ӷ������ȳ�
    QList<Terran> needAnimatorList={};//��������
    const int Y_FALL=121;//(116+5) ����׹��߶�+���
    Terran nowAnimatorTerran;
};

#endif // SIGNGATHERWIDGET_H
