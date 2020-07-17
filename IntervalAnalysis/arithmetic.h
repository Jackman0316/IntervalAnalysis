#ifndef ARITHMETIC_H
#define ARITHMETIC_H
#include <varinterval.h>

class Arithmetic
{
public:
    Arithmetic();
    static const int INFINITY = 99999999;
    static VarInterval op_union(VarInterval a, VarInterval b);
    static VarInterval op_intersection(VarInterval a, VarInterval b);

    static VarInterval op_add(VarInterval a, VarInterval b);
    static VarInterval op_add(VarInterval a, float b);
    static VarInterval op_add(float a, VarInterval b);


    static VarInterval op_multiply(VarInterval a, VarInterval b);
    static VarInterval op_multiply(VarInterval a, float b);
    static VarInterval op_multiply(float a, VarInterval b);

    static VarInterval op_neg(VarInterval a);
    static VarInterval op_divide(VarInterval a, VarInterval b);
    static VarInterval op_equal(VarInterval a, VarInterval b);
    static VarInterval op_less(VarInterval a, VarInterval b);

    static float op_upper(VarInterval a); // calculate the upper bound, with abs to error terms
    static float op_lower(VarInterval a); // calculate the lower bound, with abs to error terms
    static float op_mid(VarInterval a); //for union and intersection calculate the mid
    static float op_dev(VarInterval a); //for union and intersection calculate the dev

    static VarInterval op_range2interval(float a, float b);

    static VarInterval op_accunion(VarInterval a, VarInterval b);
    static VarInterval op_accintersection(VarInterval a, VarInterval b);

    static VarInterval op_widening(VarInterval a, VarInterval b);
    static VarInterval op_narrowing(VarInterval a, VarInterval b);

};

#endif // ARITHMETIC_H
