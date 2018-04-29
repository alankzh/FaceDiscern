#ifndef DATAINJECTWIDGET_H
#define DATAINJECTWIDGET_H
#include <QtWidgets>
#include <QtGui>
#include "terran.h"
#include "DBC/sqldatabase.h"

class DataInjectWidget:public QWidget
{
    Q_OBJECT
public:
    DataInjectWidget(QWidget *parent=nullptr);
};

#endif // DATAINJECTWIDGET_H
