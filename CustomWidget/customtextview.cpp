#include "customtextview.h"

CustomTextView::CustomTextView(QWidget *parent):QWidget(parent)
{
    backgroundPix=QPixmap(computerWidth,computerHeight);
    backgroundColor=QColor(0,0,0,0);//默认背景透明
    fontColor=QColor(255,255,255);//默认白色字体
    chineseCount=0;
    computerWidth=0;
    computerHeight=0;
}


void CustomTextView::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawPixmap(event->rect(), backgroundPix, event->rect());
    QFont font;
    font.setFamily(QString::fromLocal8Bit("黑体"));
    font.setPixelSize(fontSize);
    painter.setFont(font);
    painter.setPen(fontColor);
    painter.drawText(event->rect(),Qt::AlignCenter,text);
}

void CustomTextView::setText(QString text){
    this->text=text;
    chineseStrAdapter();
    sizeAdapter();
    update();
}

void CustomTextView::setFontStyle(int fontSize, QColor fontColor){
    this->fontSize=fontSize;
    this->fontColor=fontColor;
    sizeAdapter();
    update();
}

void CustomTextView::setBackgroundColor(QColor backgroundColor){
    this->backgroundColor=backgroundColor;
    backgroundPix=QPixmap(computerWidth,computerHeight);
    backgroundPix.fill(backgroundColor);
    update();
}

//判断字符中是否有中文
void CustomTextView::chineseStrAdapter(){
    chineseCount=0;
    QString str=this->text;
    int nCount = str.count();
    for(int i = 0 ; i <nCount ; i++)
    {
        QChar cha = str.at(i);
        ushort uni = cha.unicode();
        if(uni >= 0x4E00 && uni <= 0x9FA5)
        {
            //这个字符是中文
            chineseCount++;
        }
    }
}

void CustomTextView::sizeAdapter(){
    computerWidth = ((text.size()-chineseCount)*fontSize*0.5) + (chineseCount*fontSize);
    computerHeight=fontSize;
    resize(computerWidth,computerHeight);
    setFixedSize(computerWidth,computerHeight);
    backgroundPix=QPixmap(computerWidth,computerHeight);
    backgroundPix.fill(backgroundColor);
}
