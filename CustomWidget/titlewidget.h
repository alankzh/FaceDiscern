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
 * 主界面顶部界面
 * 宽1920，高20
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

    const int width=1920;
    const int height=50;

public slots:
    void setDelay(int ms);

    void setHttpAlive(bool alive);
protected:
    virtual void paintEvent(QPaintEvent *event);
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
