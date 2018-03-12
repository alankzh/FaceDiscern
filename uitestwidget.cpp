#include "uitestwidget.h"

UITestWidget::UITestWidget(QWidget *parent):QWidget(parent)
{

    QPalette   palette;
    QPixmap pixmap(":/ResourcesPackage/background.jpg", "jpg");
    background=pixmap;
    palette.setBrush(this->backgroundRole(),QBrush(pixmap));
//    this->setPalette(palette);
    //      this->setMask(pixmap.mask());  //可以将图片中透明部分显示为透明的
//    this->setAutoFillBackground(true);
    showFullScreen();
    init();
}

void UITestWidget::init(){
    this->setStyleSheet("QWidget {background-image:url(:/ResourcesPackage/background.jpg)}");
    QVBoxLayout *mainLayout=new QVBoxLayout();
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    titleWidget=new TitleWidget(this);
    titleWidget->setGeometry(0,0,1920,50);

    systemLogoWidget=new SystemLogoWidget(this);

    mainLayout->addWidget(titleWidget,Qt::AlignTop);
    mainLayout->addWidget(systemLogoWidget,Qt::AlignTop|Qt::AlignHCenter);


    this->setLayout(mainLayout);
}

void UITestWidget::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
//        painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
    painter.drawPixmap(event->rect(), background, event->rect());
    titleWidget->setGeometry(0,0,1920,50);
    systemLogoWidget->setGeometry(0,50,1920,150);
}
