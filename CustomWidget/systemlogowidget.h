#ifndef SYSTEMLOGOWIDGET_H
#define SYSTEMLOGOWIDGET_H

#include <QtWidgets>
#include <QtGui>
#include "CustomWidget/customimageview.h"
#include "constant.h"
#include "CustomWidget/customtextview.h"
#include <QTimer>
#include "DBC/sqldatabase.h"

/**
 * @brief The SystemLogoWidget class
 * ������֮ǰ����ʾLOGO �� ʱ��Ŀؼ�
 * ��������������˼�������ʱ�������йص���������
 */
class SystemLogoWidget:public QWidget
{
    Q_OBJECT
public:
    SystemLogoWidget(QWidget *parent=nullptr);

    const int width=1920;
    const int height=150;
signals:
    void clearTerranSignCache();//֪ͨ��ջ���
    void clearTerranSignNum();//֪ͨ���ǩ������
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
private slots:
    void updateTime();
};

#endif // SYSTEMLOGOWIDGET_H
