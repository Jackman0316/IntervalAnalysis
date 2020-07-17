#ifndef INTERVALANALYSIS_H
#define INTERVALANALYSIS_H
#include <QList>
#include "constraintsystem.h"
#include "consystemitem.h"
#include <QString>
#include "varinterval.h"
#include <QMap>
#include <QStack>
#include <QVariant>
#include "edgeseffects.h"
#include "arithmetic.h"
#include <QTableWidget>
using namespace std;
class IntervalAnalysis
{
public:
    IntervalAnalysis();

    QList<ConsystemItem> consystem_old;
    QList<ConsystemItem> consystem_new;
    QList<QString> vars;

    void Widening();
    void Narrowing();
    void Plain();
    void updateUI(QList<ConsystemItem> consystem_ui, QString mode);
    QTableWidget* tableWidget_interval;
};

#endif // INTERVALANALYSIS_H
