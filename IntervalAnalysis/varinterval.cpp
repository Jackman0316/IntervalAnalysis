#include "varinterval.h"
#include <iostream>

VarInterval::VarInterval()
{
    uINF = true;
    lINF = true;
    bottom = false;

}

VarInterval::VarInterval(float init_c, QList<float> init_e)
{
    this->c = init_c;
    this->e = init_e;
    uINF = false;
    lINF = false;
    bottom = false;

}

VarInterval VarInterval::create_bottom(){
    VarInterval bot;
    bot.bottom=true;
    return bot;

}
