#include "convertimage.h"

ConvertImage::ConvertImage()
{

}

/**
 * @brief ConvertImage::readBmp24
 * ��QImage ��װ��ͼ������
 * @param screenShot
 * QImage
 * @param imageData
 * װ�ص���ͼ�����ݵ�ַ
 * @param pWidth
 * ͼ���
 * @param pHeight
 * ͼ���
 * @return
 */
bool ConvertImage::readBmp24(QImage &screenShot, uint8_t **imageData, int *pWidth, int *pHeight){
    uint8_t* data=screenShot.bits();

//    if(((int)screenShot.format())!=4){
//        qDebug()<<QString::fromLocal8Bit("ͼƬ��ʽת��");
//        screenShot=screenShot.convertToFormat(QImage::Format_RGB32);
//    }

    //�����ļ�ͷ(BITMAPFILEHEADER,BITMAPINFOHEADER)������ֱ�ӻ�ȡ�ڴ�����ص�ַͷ���ʲ���Ҫ����
    //      data+=54;

    *pWidth=screenShot.width();
    *pHeight=screenShot.height();

    *imageData = (uint8_t *)malloc((*pWidth) * (*pHeight) * 3);

    if(screenShot.format()==QImage::Format_RGB32){
        //QImage::Format_RGB32 Camera��ȡ��ͼƬ��ʽĬ��Ϊ�ˣ����Դ�Ϊ�������д���
        for (int i = 0; i < *pHeight; i++)   {
            for (int j = 0; j < *pWidth; j++)    {
                memcpy((*imageData) + i * (*pWidth) * 3 + j * 3, data + i * (*pWidth)*4 + j * 4, 3);
            }
        }
    }else if(screenShot.format()==QImage::Format_ARGB32){
        for (int i = 0; i < *pHeight; i++)   {
            for (int j = 0; j < *pWidth; j++)    {
                memcpy((*imageData) + i * (*pWidth) * 3 + j * 3, data + i * (*pWidth)*4 + j * 4 + 1, 3);
            }
        }
    }else if(screenShot.format()==QImage::Format_RGB888){
        for (int i = 0; i < *pHeight; i++)   {
            for (int j = 0; j < *pWidth; j++)    {
                memcpy((*imageData) + i * (*pWidth) * 3 + j * 3, data + i * (*pWidth)*3 + j * 3 + 1, 3);
            }
        }
    }else{
       qDebug()<<QString::fromLocal8Bit("ͼƬ��ʽΪ:")<<(int)screenShot.format();
       return false;
    }

    return true;
}

/**
 * @brief ConvertImage::readBmp24_fromPath
 * ���ļ��ж�ȡͼ��װ������Դ
 * @param path
 * ͼ���ļ����Ե�ַ
 * @param imageData
 * װ�ص������ݵ�ַ
 * @param pWidth
 * ͼ���
 * @param pHeight
 * ͼ���
 * @return
 */
bool ConvertImage::readBmp24_fromPath(const char* path, uint8_t **imageData, int *pWidth, int *pHeight){
    if (path == NULL || imageData == NULL || pWidth == NULL || pHeight == NULL)  {
        return false;
    }
    FILE *fp = fopen(path, "rb");
    if (fp == NULL)  {
        return false;
    }
    fseek(fp, sizeof(BITMAPFILEHEADER), 0);
    BITMAPINFOHEADER head;
    fread(&head, sizeof(BITMAPINFOHEADER), 1, fp);
    *pWidth = head.biWidth;
    *pHeight = head.biHeight;
    int biBitCount = head.biBitCount;
    if (24 == biBitCount)  {
        int lineByte = ((*pWidth) * biBitCount / 8 + 3) / 4 * 4;
        *imageData = (uint8_t *)malloc(lineByte * (*pHeight));
        uint8_t * data = (uint8_t *)malloc(lineByte * (*pHeight));
        fseek(fp, 54, SEEK_SET);
        fread(data, 1, lineByte * (*pHeight), fp);
        for (int i = 0; i < *pHeight; i++){
            for (int j = 0; j < *pWidth; j++){

                //ͼ�������ֽ����С�ͼ���ÿһɨ�����ɱ�ʾͼ�����ص��������ֽ���ɣ�ÿһ�е��ֽ���ȡ����ͼ�����ɫ��Ŀ�������ر�ʾ��ͼ���ȡ�
                //ɨ�������ɵ����ϴ洢�ģ������˵�������еĵ�һ���ֽڱ�ʾλͼ���½ǵ����أ������һ���ֽڱ�ʾλͼ���Ͻǵ����ء�
                memcpy((*imageData) + i * (*pWidth) * 3 + j * 3, data + (((*pHeight) - 1) - i) * lineByte + j * 3, 3);
            }
        }
        free(data);
    }  else{
        fclose(fp);
        return false;
    }

    fclose(fp);
    return true;
}
