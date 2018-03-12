#ifndef FACEPAINTERRECT_H
#define FACEPAINTERRECT_H

#include <QWidget>
#include <QTimer>
#include "terran.h"
#include <QDebug>

/**
 * @brief The FacePainterRect class
 * ��װ�˻���������Ҫ����ɫ�����塢��С
 */
class FacePainterRect:public QWidget
{
    Q_OBJECT
public:
    FacePainterRect(QWidget *parent=nullptr);

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

    QColor rectColor;
    QColor fontColor;
    int width;
    int height;
    int left;
    int right;
    int top;
    int bottom;
    int appearFontCount=0;//��Ҫ���Ƶ����峤��

    int getTerranId();
    void setTerran(Terran terran);
    Terran getTerran();

    void startAppearAnimator();//��ʼ���ֶ���
    void startDisappearAnimator();//��ʼ��ʧ����
signals:
    void askUpdateUI();//����ˢ��UI
    void appeartAnimatorEnd(int terranId);//��ʼ��������
    void disappeartAnimatorEnd(int terranId);//������������
private slots:
    void animatorStatusChange();
private:
    FacePainterRect::FaceAnimatorStatus animatorStatus=FacePainterRect::FaceAnimatorStatus::SizeInited;
    const int ANIMATOR_INTERVAL=100;//������ʾ�Ķ���֡���
    void sizeAdapter();
    Terran terran;
    QTimer *qtimer;
};

#endif // FACEPAINTERRECT_H
