#ifndef VARINTERVAL_H
#define VARINTERVAL_H
#include <QList>

using namespace std;
class VarInterval
{
public:
    float c;
    QList<float> e;
    bool uINF;
    bool lINF;
    bool bottom;
    VarInterval();
    VarInterval(float init_c, QList<float> init_e);
    static VarInterval create_bottom();
};

#endif // VARINTERVAL_H
