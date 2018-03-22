#ifndef TERRANSIGNINTIMEWIDGET_H
#define TERRANSIGNINTIMEWIDGET_H

#include <QtWidgets>
#include <QtGui>
#include "constant.h"
#include "CustomWidget/customtextview.h"

/**
 * @brief The TerranSignInTimeWidget class
 * һ��  ǩ����Ϣչʾ�� �Ҳ�� ǩ��ʱ��չʾ��
 */
class TerranSignInTimeWidget:public QWidget
{
    Q_OBJECT
public:
    TerranSignInTimeWidget(QWidget *parent=nullptr);
    ~TerranSignInTimeWidget();
    void setTimeAndDate();
    const int width=157;
    const int height=116;
protected:
    virtual void paintEvent(QPaintEvent *event);
private:
    void init();
    QPixmap backgroundPix;
    CustomTextView *timeTextView;
    CustomTextView *dateTextVoew;
    QVBoxLayout *mainLayout;
};

#endif // TERRANSIGNINTIMEWIDGET_H
