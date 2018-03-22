#ifndef CUSTOMCIRCLEIMAGEVIEW_H
#define CUSTOMCIRCLEIMAGEVIEW_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QPaintEvent>
#include <QImage>
#include <QBitmap>
#include <QPalette>
#include <QLabel>

/**
 * @brief The CustomCircleImageView class
 * �Զ���Բ��ImageView
 */
class CustomCircleImageView:public QWidget
{
public:
    CustomCircleImageView(QWidget *parent=nullptr,int diameter=55);
    ~CustomCircleImageView();
    void setBackground(QImage image);
    void setPos(int x,int y);
    void setSize(int width,int height);
    inline int getXpos(){
        return xPos;
    }

    inline int getYpos(){
        return yPos;
    }

    /**
     * @brief getDiameter
     * ��ȡͼƬֱ��
     * @return
     */
    inline int getDiameter(){
        return diameter;
    }

protected:
    virtual void paintEvent(QPaintEvent *event);
private:
    QPixmap *backgroundPix=nullptr;
    const int bigDiameter=1024;//�������
    int xPos;
    int yPos;
    int diameter;
};

#endif // CUSTOMCIRCLEIMAGEVIEW_H
