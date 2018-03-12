#ifndef CONVERTIMAGE_H
#define CONVERTIMAGE_H

#include <QImage>
#include "qtwinextras/qwinfunctions.h"
#include "qt_windows.h"
#include "wingdi.h"
#include "stdint.h"
#include <QDebug>
#include "stdio.h"
#include <bitset>
#include <QString>

class ConvertImage
{
public:
    ConvertImage();
    static bool readBmp24(QImage &screenShot,uint8_t **imageData,int *pWidth,int *pHeight);

    static bool readBmp24_fromPath(const char* path,uint8_t **imageData,int *pWidth,int *pHeight);
};

#endif // CONVERTIMAGE_H
