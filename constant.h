#ifndef CONSTANT_H
#define CONSTANT_H

#include <QString>
#include <QFont>
#include <QPalette>
#include <QColor>
#include <QDir>

/**
 *定义了一系列软件中使用的常量
 *分为.h和.cpp文件，而不是一个单独的.h文件。
 * 这是为了节省那微不足道的内存空间
*/

//获取全部人员信息的url
extern const QString HEART_BEAT_URL;

//下载图片的Url地址头
extern const QString DOWNLOAD_PIC_URL_PRE;

//发送签到信息的Url地址头
extern const QString SEND_SIGN_IN_MESSAGE_URL;

//face线程数据库连接，连接名
extern const QString FACE_DB_CONNECTION_NAME;

//heart线程数据库连接，连接名
extern const QString HEART_DB_CONNECTION_NAME;

//UI线程数据库连接，连接名
extern const QString UI_DB_CONNECTION_NAME;

//心跳颤动周期，5s一次心跳颤动
extern const unsigned int BEAT_CIRCLE;

//识别截图周期,0.2s一次
extern const unsigned int SCREEN_SHOT_CIRCLE;

//主窗口宽度
extern const unsigned int MAIN_WIDGET_WIDTH;

//主窗口高度
extern const unsigned int MAIN_WIDGET_HEIGHT;

//清空一次签到缓存的时间间隔 60s
extern const unsigned int CLEAR_SIGN_CACHE_INTERVAL;

static int Visitor_Offsert_Id_static=-1;

//QImage文件的临时暂存点
#define DIRPATH_QIMAGE_TEMPORARY (QDir::currentPath()+QString::fromLocal8Bit("/buff.jpg"))

class FontSetting{
public:
    static QFont get13FontSize(){
        QFont font13;
        font13.setFamily(QString::fromLocal8Bit("黑体"));
        font13.setPixelSize(13);
        return font13;
    }

    static QFont get21FontSize(){
        QFont font21;
        font21.setFamily(QString::fromLocal8Bit("黑体"));
        font21.setPixelSize(21);
        return font21;
    }

    static QFont get23FontSize(){
        QFont font23;
        font23.setFamily(QString::fromLocal8Bit("黑体"));
        font23.setPixelSize(23);
        return font23;
    }

    static QFont get26FontSize(){
        QFont font26;
        font26.setFamily(QString::fromLocal8Bit("黑体"));
        font26.setPixelSize(26);
        return font26;
    }

    static QFont get35FontSize(){
        QFont font35;
        font35.setFamily(QString::fromLocal8Bit("黑体"));
        font35.setPixelSize(35);
        return font35;
    }

    static QFont get36FontSize(){
        QFont font36;
        font36.setFamily(QString::fromLocal8Bit("黑体"));
        font36.setPixelSize(36);
        return font36;
    }

    /**
     * @brief getCommanFontColor
     * 常规字体,背景不透明度为0
     * @return
     */
    static QPalette getCommanFontColor(){
        QPalette palette;
        palette.setColor(QPalette::WindowText,QColor(255,255,255));
        palette.setColor(QPalette::Background,QColor(0,0,0,0));
        return palette;
    }

    /**
     * @brief getTransparentFontColor
     * 半透明字体,背景不透明度为0
     * @return
     */
    static QPalette getTransparentFontColor(){
        QPalette palette;
        palette.setColor(QPalette::WindowText,QColor(255,255,255,127));
        palette.setColor(QPalette::Background,QColor(0,0,0,0));
        return palette;
    }
};

#endif // CONSTANT_H
