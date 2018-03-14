#ifndef TERRANSIGNINMESSAGEWIDGET_H
#define TERRANSIGNINMESSAGEWIDGET_H

#include <QtWidgets>
#include <QtGui>
#include "CustomWidget/customimageview.h"
#include "constant.h"
#include "CustomWidget/customtextview.h"
#include "CustomWidget/customcircleimageview.h"



/**
 * @brief The TerranSignInMessageWidget class
 * һ�� ǩ����Ϣչʾ��
 */
class TerranSignInMessageWidget:public QWidget
{
public:
    TerranSignInMessageWidget(QWidget *parent=nullptr);
    const int width=570;
    const int height=116;

    enum LabelType{
        Work,
        OffWork
    };

    void loadData(QImage headerImage,QString name,QString department,TerranSignInMessageWidget::LabelType labelType=TerranSignInMessageWidget::LabelType::Work);


protected:
    virtual void paintEvent(QPaintEvent *event);
private:
    void init();
    QPixmap backgroundPix;

    CustomCircleImageView *headerCircleImageView;
    CustomTextView  *nameTextView;
    CustomTextView *departmentTextView;
    CustomImageView *signInLabelImageView;
};

#endif // TERRANSIGNINMESSAGEWIDGET_H
