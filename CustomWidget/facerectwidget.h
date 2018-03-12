#ifndef FACERECTWIDGET_H
#define FACERECTWIDGET_H

#include <QtWidgets>
#include <QtGui>
#include "terran.h"

/**
 * @brief The FaceRectWidget class
 * �������ο�  �ѷ�����������FacePainterRect����
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
     * ����״̬��
     * ��������һ���ſ�Ķ�����
     * ����ʱ͸���Ⱥ�����ͬʱ�仯
     * ͸����6֡���仯Ϊ0��60��20��80��40��100
     * ����6֡���仯Ϊ100����ֵ(90)��80��80����ֵ(90)��100
     * ֵΪ80ʱ����Ӧ����ʶ�𵽵���٣�Ĭ�Ͻ�֮�Ŵ�Ϊ100����ʾ������ͷ��
     * ��ʧʱ��͸���ȱ仯
     * 6֡���仯Ϊ100��40��80��20��60��0
     * �������ʱ��ÿ2֡����һ���֣�����ǰ6֡��͸���ȱ仯
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

    const int animatorInterval=40;//������ʾ�Ķ���֡���

    int width=0;
    int height=0;
    int left=0;
    int right=0;
    int top=0;
    int bottom=0;

    int appearFontCount=0;//��Ҫ���Ƶ����峤��

    FaceRectWidget::FaceAnimatorStatus animatorStatus=FaceRectWidget::FaceAnimatorStatus::SizeInited;

    void sizeAdapter();
};

#endif // FACERECTWIDGET_H
