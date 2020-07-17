#ifndef CONSYSTEMITEM_H
#define CONSYSTEMITEM_H
#include <QString>
#include <QMap>
#include "varinterval.h"

class ConsystemItem
{
public:
    ConsystemItem();
    ConsystemItem(int pp, QString consystem, QString code, QMap<QString, VarInterval> var_interval);
    int pp;
    QString consystem;
    QString code;
    QMap<QString, VarInterval> var_interval;
};

#endif // CONSYSTEMITEM_H
