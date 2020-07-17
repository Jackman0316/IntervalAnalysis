#include "consystemitem.h"

ConsystemItem::ConsystemItem()
{

}

ConsystemItem:: ConsystemItem(int pp, QString consystem, QString code, QMap<QString, VarInterval> var_interval)
{
    this->pp = pp;
    this->consystem = consystem;
    this->code = code;
    this->var_interval = var_interval;
}
