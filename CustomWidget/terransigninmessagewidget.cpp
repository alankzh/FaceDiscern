#include "terransigninmessagewidget.h"

TerranSignInMessageWidget::TerranSignInMessageWidget(QWidget *parent):QWidget(parent)
{
    init();
}

TerranSignInMessageWidget::~TerranSignInMessageWidget(){
    if(headerCircleImageView!=nullptr){
        delete headerCircleImageView;
        headerCircleImageView=nullptr;
    }
    if(nameTextView!=nullptr){
        delete nameTextView;
        nameTextView=nullptr;
    }
    if(departmentTextView!=nullptr){
        delete departmentTextView;
        departmentTextView=nullptr;
    }
    if(signInLabelImageView!=nullptr){
        delete signInLabelImageView;
        signInLabelImageView=nullptr;
    }
    if(mainLayout!=nullptr){
        delete mainLayout;
        mainLayout=nullptr;
    }
}

void TerranSignInMessageWidget::init(){
    resize(width,height);
    setFixedSize(width,height);
    backgroundPix=QPixmap(width,height);
    backgroundPix.fill(QColor(0,0,0,102));
    this->setMask(backgroundPix.createHeuristicMask());//���屳��Ϊ͸����40%
}

/**
 * @brief TerranSignInMessageWidget::loadData
 * װ������
 */
void TerranSignInMessageWidget::loadData(QImage headerImage,QString name,QString department,TerranSignInMessageWidget::LabelType labelType){

    mainLayout=new QHBoxLayout();
    mainLayout->setContentsMargins(35,10,30,10);
    mainLayout->setSpacing(38);

    headerCircleImageView=new CustomCircleImageView(this);
    headerCircleImageView->setBackground(headerImage);

    nameTextView=new CustomTextView(this);
    nameTextView->setFontStyle(23);
    nameTextView->setText(name);

    departmentTextView=new CustomTextView(this);
    departmentTextView->setFontStyle(26);
    departmentTextView->setText(department);

    signInLabelImageView=new CustomImageView();
    signInLabelImageView->setPos(492,0);
    if(labelType==TerranSignInMessageWidget::LabelType::Work){
        signInLabelImageView->setBackground(":/ResourcesPackage/faceIsEmployee.png","png");//�ϰ�ǩ�����ϰ��ǩ
    }else if(labelType==TerranSignInMessageWidget::LabelType::OffWork){
        //TODO �°�ǩ�����°��ǩ
    }

    mainLayout->addWidget(headerCircleImageView,1,Qt::AlignVCenter);
    mainLayout->addWidget(nameTextView,100,Qt::AlignVCenter|Qt::AlignLeft);
    mainLayout->addWidget(departmentTextView,100,Qt::AlignCenter|Qt::AlignLeft);
    mainLayout->addWidget(signInLabelImageView,Qt::AlignRight|Qt::AlignVCenter);

    this->setLayout(mainLayout);
}


void TerranSignInMessageWidget::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);

    QPainter painter(this);

    painter.drawPixmap(event->rect(),backgroundPix,event->rect());
}
