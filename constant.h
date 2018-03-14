#ifndef CONSTANT_H
#define CONSTANT_H

#include <QString>
#include <QFont>
#include <QPalette>
#include <QColor>
#include <QDir>

/**
 *������һϵ�������ʹ�õĳ���
 *��Ϊ.h��.cpp�ļ���������һ��������.h�ļ���
 * ����Ϊ�˽�ʡ��΢��������ڴ�ռ�
*/

//��ȡȫ����Ա��Ϣ��url
extern const QString HEART_BEAT_URL;

//����ͼƬ��Url��ַͷ
extern const QString DOWNLOAD_PIC_URL_PRE;

//����ǩ����Ϣ��Url��ַͷ
extern const QString SEND_SIGN_IN_MESSAGE_URL;

//face�߳����ݿ����ӣ�������
extern const QString FACE_DB_CONNECTION_NAME;

//heart�߳����ݿ����ӣ�������
extern const QString HEART_DB_CONNECTION_NAME;

//UI�߳����ݿ����ӣ�������
extern const QString UI_DB_CONNECTION_NAME;

//�����������ڣ�5sһ����������
extern const unsigned int BEAT_CIRCLE;

//ʶ���ͼ����,0.2sһ��
extern const unsigned int SCREEN_SHOT_CIRCLE;

//�����ڿ��
extern const unsigned int MAIN_WIDGET_WIDTH;

//�����ڸ߶�
extern const unsigned int MAIN_WIDGET_HEIGHT;

//���һ��ǩ�������ʱ���� 60s
extern const unsigned int CLEAR_SIGN_CACHE_INTERVAL;

static int Visitor_Offsert_Id_static=-1;

//QImage�ļ�����ʱ�ݴ��
#define DIRPATH_QIMAGE_TEMPORARY (QDir::currentPath()+QString::fromLocal8Bit("/buff.jpg"))

class FontSetting{
public:
    static QFont get13FontSize(){
        QFont font13;
        font13.setFamily(QString::fromLocal8Bit("����"));
        font13.setPixelSize(13);
        return font13;
    }

    static QFont get21FontSize(){
        QFont font21;
        font21.setFamily(QString::fromLocal8Bit("����"));
        font21.setPixelSize(21);
        return font21;
    }

    static QFont get23FontSize(){
        QFont font23;
        font23.setFamily(QString::fromLocal8Bit("����"));
        font23.setPixelSize(23);
        return font23;
    }

    static QFont get26FontSize(){
        QFont font26;
        font26.setFamily(QString::fromLocal8Bit("����"));
        font26.setPixelSize(26);
        return font26;
    }

    static QFont get35FontSize(){
        QFont font35;
        font35.setFamily(QString::fromLocal8Bit("����"));
        font35.setPixelSize(35);
        return font35;
    }

    static QFont get36FontSize(){
        QFont font36;
        font36.setFamily(QString::fromLocal8Bit("����"));
        font36.setPixelSize(36);
        return font36;
    }

    /**
     * @brief getCommanFontColor
     * ��������,������͸����Ϊ0
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
     * ��͸������,������͸����Ϊ0
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
