#ifndef CONSTRAINTSYSTEM_H
#define CONSTRAINTSYSTEM_H
#include <QString>
#include <QList>
#include "varinterval.h"
#include <QMap>
#include <QStack>
#include <QVariant>
#include "consystemitem.h"
class ConstraintSystem
{
public:
    ConstraintSystem();
    QList<QString> codelist;
    QList<QString> vars;
    QStack<QString> operation; // operation stack, dealing with jump and union
    //QMap<int, QList<QVariant>> consystem; // QMap<program point No., QMap<3⊔4, QMap<x, VarInterval>>>
    QList<ConsystemItem> consystem;
    QList<QString> extract_var();
    QList<ConsystemItem> extract_consystem();
    static bool conequal(QList<ConsystemItem> consystem_new, QList<ConsystemItem> consystem_old, QList<QString> vars);
};

#endif // CONSTRAINTSYSTEM_H
