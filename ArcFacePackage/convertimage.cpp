#include "convertimage.h"

ConvertImage::ConvertImage()
{

}

/**
 * @brief ConvertImage::readBmp24
 * 从QImage 中装载图像数据
 * @param screenShot
 * QImage
 * @param imageData
 * 装载到的图像数据地址
 * @param pWidth
 * 图像宽
 * @param pHeight
 * 图像高
 * @return
 */
bool ConvertImage::readBmp24(QImage &screenShot, uint8_t **imageData, int *pWidth, int *pHeight){
    uint8_t* data=screenShot.bits();

//    if(((int)screenShot.format())!=4){
//        qDebug()<<QString::fromLocal8Bit("图片格式转换");
//        screenShot=screenShot.convertToFormat(QImage::Format_RGB32);
//    }

    //跳过文件头(BITMAPFILEHEADER,BITMAPINFOHEADER)，由于直接获取内存的像素地址头，故不需要跳过
    //      data+=54;

    *pWidth=screenShot.width();
    *pHeight=screenShot.height();

    *imageData = (uint8_t *)malloc((*pWidth) * (*pHeight) * 3);

    if(screenShot.format()==QImage::Format_RGB32){
        //QImage::Format_RGB32 Camera截取的图片格式默认为此，故以此为基础进行处理
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
       qDebug()<<QString::fromLocal8Bit("图片格式为:")<<(int)screenShot.format();
       return false;
    }

    return true;
}

/**
 * @brief ConvertImage::readBmp24_fromPath
 * 从文件中读取图像装载数据源
 * @param path
 * 图像文件绝对地址
 * @param imageData
 * 装载到的数据地址
 * @param pWidth
 * 图像宽
 * @param pHeight
 * 图像高
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

                //图像数据字节阵列。图像的每一扫描行由表示图像像素的连续的字节组成，每一行的字节数取决于图像的颜色数目和用像素表示的图像宽度。
                //扫描行是由底向上存储的，这就是说，阵列中的第一个字节表示位图左下角的像素，而最后一个字节表示位图右上角的像素。
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
