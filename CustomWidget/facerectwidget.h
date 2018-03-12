#ifndef FACERECTWIDGET_H
#define FACERECTWIDGET_H

#include <QtWidgets>
#include <QtGui>
#include "terran.h"

/**
 * @brief The FaceRectWidget class
 * 人脸矩形框  已废弃，功能由FacePainterRect代替
 */
class FaceRectWidget:public QWidget
{
    Q_OBJECT
public:
    FaceRectWidget(QWidget *parent=nullptr);
    void setTerran(Terran terran);
    int getTerranId();
    void startAppear();
    void startDisappear();

    /**
     * @brief The FaceAnimatorStatus enum
     * 动画状态，
     * 丽桃做了一个炫酷的动画，
     * 出现时透明度和缩放同时变化
     * 透明度6帧，变化为0、60、20、80、40、100
     * 缩放6帧，变化为100、插值(90)、80、80、插值(90)、100
     * 值为80时，对应引擎识别到的五官，默认将之放大为100，显示完整的头部
     * 消失时，透明度变化
     * 6帧，变化为100、40、80、20、60、0
     * 字体出现时，每2帧出现一个字，并且前6帧有透明度变化
     */
    enum FaceAnimatorStatus{
        SizeInited=0,
        AppearAnimatorStep1=1,
        AppearAnimatorStep2=2,
        AppearAnimatorStep3=3,
        AppearAnimatorStep4=4,
        AppearAnimatorStep5=5,
        AppearAnimatorStep6=6,
        FontAppearStep=7,
        Appeared=8,
        DisApearedAnimatorStep1=9,
        DisApearedAnimatorStep2=10,
        DisApearedAnimatorStep3=11,
        DisApearedAnimatorStep4=12,
        DisApearedAnimatorStep5=13,
        DisApearedAnimatorStep6=14,
        DisApeared=15
    };

signals:
    void appeartAnimatorEnd(int terranId);
    void disappeartAnimatorEnd(int terranId);
protected:
    virtual void paintEvent(QPaintEvent *event);
private slots:
    void animatorStatusChange();
private:
    QTimer *qtimer;
    QPixmap backgroundPix;
    QColor rectColor;
    QColor fontColor;

    Terran terran;

    const int animatorInterval=40;//丽桃演示的动画帧间隔

    int width=0;
    int height=0;
    int left=0;
    int right=0;
    int top=0;
    int bottom=0;

    int appearFontCount=0;//需要绘制的字体长度

    FaceRectWidget::FaceAnimatorStatus animatorStatus=FaceRectWidget::FaceAnimatorStatus::SizeInited;

    void sizeAdapter();
};

#endif // FACERECTWIDGET_H
