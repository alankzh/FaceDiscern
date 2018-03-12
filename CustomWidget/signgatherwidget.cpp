#include "signgatherwidget.h"

SignGatherWidget::SignGatherWidget(QWidget *parent):QWidget(parent){
    init();
}

void SignGatherWidget::init(){
    resize(width,height);
    setFixedSize(width,height);
    backgroundPix=QPixmap(width,height);
    backgroundPix.fill(QColor(255,255,255,0));
    this->setMask(backgroundPix.createHeuristicMask());//总体背景为透明度0%

    SignInWidget *signInWidget1=new SignInWidget(this);
    signInWidget1->move(0,0);
    SignInWidget *signInWidget2=new SignInWidget(this);
    signInWidget2->move(0,121);
    SignInWidget *signInWidget3=new SignInWidget(this);
    signInWidget3->move(0,242);
    SignInWidget *signInWidget4=new SignInWidget(this);
    signInWidget4->move(0,363);
    SignInWidget *signInWidget5=new SignInWidget(this);
    signInWidget5->move(0,484);
    SignInWidget *signInWidget6=new SignInWidget(this);
    signInWidget6->move(0,605);

    widgetList.append(signInWidget6);
    widgetList.append(signInWidget5);
    widgetList.append(signInWidget4);
    widgetList.append(signInWidget3);
    widgetList.append(signInWidget2);
    widgetList.append(signInWidget1);

    connect(this,SIGNAL(animatorEnd()),this,SLOT(onMovingEnd()));
}

void SignGatherWidget::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawPixmap(event->rect(),backgroundPix,event->rect());
}

void SignGatherWidget::terranSignIn(Terran terran, TerranSignInMessageWidget::LabelType labelType){
    if(status==AnimatorStatus::End){
        SignInWidget *signInWidgetNew=new SignInWidget(this);
        signInWidgetNew->insertTerranInformation(terran,labelType);
        signInWidgetNew->move(0,0);
        signInWidgetNew->setWindowOpacity(0);//透明
        signInWidgetNew->hide();
        widgetList.append(signInWidgetNew);
        startSignInAnimator();
    }else{
        needAnimatorList.append(terran);
    }
}

void SignGatherWidget::signIn(Terran terran){
    terranSignIn(terran);
}

/**
 * @brief SignGatherWidget::startSignInAnimator
 * 开始签到动画
 */
void SignGatherWidget::startSignInAnimator(){
    status=AnimatorStatus::Moving;
    QParallelAnimationGroup *animationGroup=new QParallelAnimationGroup;
    QEasingCurve easing(QEasingCurve::OutCirc);
    /*除新加入的，在屏幕边缘的签到栏外，其余控件向下移动*/
    for(int i=0;i<widgetList.size()-1;i++){
        SignInWidget *signWidget=widgetList.at(i);
        QPropertyAnimation *animation=new QPropertyAnimation(signWidget,"geometry");
        animation->setDuration(1500);
        animation->setStartValue(signWidget->geometry());
        animation->setEndValue(QRect(signWidget->geometry().x(),signWidget->geometry().y()+Y_FALL,signWidget->geometry().width(),signWidget->geometry().height()));
        animation->setEasingCurve(easing);
        animationGroup->addAnimation(animation);
    }
    connect(animationGroup,SIGNAL(finished()),this,SLOT(animatorStageTwo()));
    animationGroup->start(QPropertyAnimation::DeleteWhenStopped);
}

/**
 * @brief SignGatherWidget::animatorStageTwo
 * 签到动画第二段
 */
void SignGatherWidget::animatorStageTwo(){
    SignInWidget *insertWidget=widgetList.last();
    QPropertyAnimation *animation=new QPropertyAnimation(insertWidget,"windowOpacity");
    QEasingCurve easing(QEasingCurve::OutCirc);
    animation->setDuration(1500);
    animation->setStartValue(0);
    animation->setEndValue(1);
    insertWidget->show();
    //    animation->setStartValue(insertWidget->geometry());
    //    animation->setEndValue(QRect(0,0,insertWidget->geometry().width(),insertWidget->geometry().height()));
    animation->setEasingCurve(easing);
    connect(animation,SIGNAL(finished()),this,SLOT(onMovingEnd()));
    animation->start(QPropertyAnimation::DeleteWhenStopped);
}

/**
 * @brief SignGatherWidget::onMovingEnd
 * 动画结束后
 */
void SignGatherWidget::onMovingEnd(){
    widgetList.last()->show();
    widgetList.removeFirst();//去掉第一个
    status=AnimatorStatus::End;
    if(needAnimatorList.size()>0){
        Terran terran=needAnimatorList.at(0);
        SignInWidget *signInWidgetNew=new SignInWidget(this);
        signInWidgetNew->insertTerranInformation(terran);
        signInWidgetNew->move(0,0);
        signInWidgetNew->setWindowOpacity(0);//透明
        signInWidgetNew->hide();
        widgetList.append(signInWidgetNew);
        needAnimatorList.removeFirst();
        startSignInAnimator();
    }
}

