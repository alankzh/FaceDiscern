#ifndef CUSTOMIMAGEVIEW_H
#define CUSTOMIMAGEVIEW_H
#include <QWidget>
#include <QPaintEvent>
#include <QPixmap>
#include <QPainter>
#include <QBitmap>
#include <QDebug>
/**
 * @brief The CustomImageView class
 * 自定义ImageView
 * 什么都不干，显示一张图片，美美哒
 */
class CustomImageView:public QWidget
{
    Q_OBJECT
public:
    CustomImageView(QWidget *parent=nullptr);

    void setBackground(const char* fileName,const char* flag);

    void setScaleBackground(int width,int height,const char* fileName,const char* flag);

    bool setPos(int x,int y);
    inline int getXpos(){
        return xPos;
    }

    inline int getYpos(){
        return yPos;
    }

    inline int getWidth(){
        return width;
    }

    inline int getHeight(){
        return height;
    }

protected :
    virtual void paintEvent(QPaintEvent *event);
private:
    QPixmap backgroundPix;
    int xPos;
    int yPos;
    int width;
    int height;
};

#endif // CUSTOMIMAGEVIEW_H
