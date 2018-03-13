#include "signinwidget.h"

SignInWidget::SignInWidget(QWidget *parent):QWidget(parent){
    init();
}

SignInWidget::~SignInWidget(){
}

void SignInWidget::init(){

    resize(width,height);
    setFixedSize(width,height);
    backgroundPix=QPixmap(width,height);
    backgroundPix.fill(QColor(255,255,255,0));
    this->setMask(backgroundPix.createHeuristicMask());//总体背景为透明度0%

    QHBoxLayout *mainLayout=new QHBoxLayout();
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(1);

    messageWidget=new TerranSignInMessageWidget(this);
    timeWidget=new TerranSignInTimeWidget(this);
    mainLayout->addWidget(messageWidget,1,Qt::AlignLeft);
    mainLayout->addWidget(timeWidget,1,Qt::AlignRight);

    this->setLayout(mainLayout);
}

void SignInWidget::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawPixmap(event->rect(),backgroundPix,event->rect());
}

void SignInWidget::insertTerranInformation(Terran terran, TerranSignInMessageWidget::LabelType labelType){
    this->id=terran.id;
    QImage image;
    QList<Terran> list={};
    list.append(terran);
    SQLDataBase::instance()->connectionDB(UI_DB_CONNECTION_NAME);
    SQLDataBase::instance()->operationDB(UI_DB_CONNECTION_NAME,SQLDataBase::OperationWay::LoadDBImage,list,&image);
    qDebug()<<terran.department;
    messageWidget->loadData(image,terran.name,terran.department,labelType);
    timeWidget->setTimeAndDate();
}

