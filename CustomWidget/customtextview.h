#ifndef CUSTOMTEXTVIEW_H
#define CUSTOMTEXTVIEW_H

#include <QWidget>
#include <QPaintEvent>
#include <QPixmap>
#include <QPainter>
#include <QBitmap>
#include <QDebug>
#include <QString>
#include <QColor>
#include <QSize>

/**
 * @brief The CustomTextView class
 * 自定义TextView，什么都不干，写字
 * 解决了QLabel背景色在多层背景的QWidget下的bug
 */
class CustomTextView:public QWidget
{
    Q_OBJECT
public:
    CustomTextView(QWidget *parent=nullptr);

    void setBackgroundColor(QColor backgroundColor);
    void setFontStyle(int fontSize,QColor fontColor=QColor(255,255,255));
    void setText(QString text);
protected:
    virtual void paintEvent(QPaintEvent *event);
private:
    QColor backgroundColor=QColor(0,0,0,0);//背景默认为透明
    QColor fontColor;
    int fontSize;
    QPixmap backgroundPix;
    QString text;

    int chineseCount;

    int computerWidth;
    int computerHeight;

    void chineseStrAdapter();
    void sizeAdapter();
};

#endif // CUSTOMTEXTVIEW_H
