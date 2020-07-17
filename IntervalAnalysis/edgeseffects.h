#ifndef EDGESEFFECTS_H
#define EDGESEFFECTS_H
#include "varinterval.h"
#include "consystemitem.h"
class EdgesEffects
{
public:
    EdgesEffects();
    static QMap<QString, VarInterval> extract_interval(ConsystemItem consystem_pp);
    static QMap<QString, VarInterval> eval_nop(QString code, QMap<QString, VarInterval> var_interval);
    static QMap<QString, VarInterval> eval_assign(QString code, QMap<QString, VarInterval> var_interval);
    static QMap<QString, VarInterval> eval_load(QString code, QMap<QString, VarInterval> var_interval);
    static QMap<QString, VarInterval> eval_write(QString code, QMap<QString, VarInterval> var_interval);
    static QMap<QString, VarInterval> eval_pos(QString code, QMap<QString, VarInterval> var_interval);
    static QMap<QString, VarInterval> eval_neg(QString code, QMap<QString, VarInterval> var_interval);

    static VarInterval eval_expression(QString expr, QMap<QString, VarInterval> var_interval);
};

#endif // EDGESEFFECTS_H
