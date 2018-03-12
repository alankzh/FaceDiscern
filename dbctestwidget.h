#ifndef DBCTESTWIDGET_H
#define DBCTESTWIDGET_H

#include <QWidget>
#include <QtWidgets>
#include <QtGui>
#include "DBC/sqldatabase.h"
#include "terran.h"

class DBCTestWidget:public QWidget
{
    Q_OBJECT
public:
    DBCTestWidget(QWidget *parent=nullptr);
    QString testDBName;
};

#endif // DBCTESTWIDGET_H
