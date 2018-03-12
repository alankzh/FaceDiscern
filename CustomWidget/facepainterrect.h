#ifndef FACEPAINTERRECT_H
#define FACEPAINTERRECT_H

#include <QWidget>
#include <QTimer>
#include "terran.h"
#include <QDebug>

/**
 * @brief The FacePainterRect class
 * 封装了绘制人脸需要的颜色、字体、大小
 */
class FacePainterRect:public QWidget
{
    Q_OBJECT
public:
    FacePainterRect(QWidget *parent=nullptr);

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

    QColor rectColor;
    QColor fontColor;
    int width;
    int height;
    int left;
    int right;
    int top;
    int bottom;
    int appearFontCount=0;//需要绘制的字体长度

    int getTerranId();
    void setTerran(Terran terran);
    Terran getTerran();

    void startAppearAnimator();//开始出现动画
    void startDisappearAnimator();//开始消失动画
signals:
    void askUpdateUI();//请求刷新UI
    void appeartAnimatorEnd(int terranId);//开始动画结束
    void disappeartAnimatorEnd(int terranId);//结束动画结束
private slots:
    void animatorStatusChange();
private:
    FacePainterRect::FaceAnimatorStatus animatorStatus=FacePainterRect::FaceAnimatorStatus::SizeInited;
    const int ANIMATOR_INTERVAL=100;//丽桃演示的动画帧间隔
    void sizeAdapter();
    Terran terran;
    QTimer *qtimer;
};

#endif // FACEPAINTERRECT_H
