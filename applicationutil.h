#ifndef APPLICATIONUTIL_H
#define APPLICATIONUTIL_H

#include <QtWidgets>


class ApplicationUtil:QObject
{
    Q_OBJECT
private:
    ApplicationUtil(QObject *parent=0);
public:
    static void restartApplication();
    static void restartSystem();
};

#endif // APPLICATIONUTIL_H
