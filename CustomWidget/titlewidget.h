#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#include <QtWidgets>
#include <QtGui>
#include "CustomWidget/customimageview.h"
#include "constant.h"
#include "CustomWidget/customtextview.h"
#include <cstdlib>

/**
 * @brief The TitleWidget class
 * �����涥������
 * ��1920����20
 */
class TitleWidget:public QWidget
{
    Q_OBJECT
public:
    TitleWidget(QWidget *parent=nullptr);

    enum RunningStatus{
        Running,
        Disconnection
    };

    void setStatus(TitleWidget::RunningStatus status);

    void setDelay(int ms);

    void setAlive(bool alive);

    const int width=1920;
    const int height=50;
protected:
    virtual void paintEvent(QPaintEvent *event);
private slots:
    void fakeMs();
private:
    void init();
    QPixmap backgroundPix;
    CustomImageView *logoImageView;
    CustomTextView *delayText;
    CustomTextView *statuText;
    CustomImageView *labelView;

    QTimer *qtimer;
};

#endif // TITLEWIDGET_H
