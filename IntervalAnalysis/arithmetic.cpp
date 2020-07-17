#include "arithmetic.h"
#include <cmath>
#include "varinterval.h"
#include <iostream>
#include <QDebug>
#include "varinterval.h"
using namespace std;
Arithmetic::Arithmetic()
{

}


float Arithmetic::op_upper(VarInterval a){
    if(a.bottom==true)
        return 0;
    if(a.uINF==false)
    {
        float sum = 0;
        for(int i=0; i<a.e.length() ;i++)
        {
            sum+=fabs(a.e.at(i)); // here may need to abs(), can e be minus?
        }
        float upper = a.c + sum;
        //cout<<"a.c:"<<a.c<<endl;
        if(fabs(upper)<=0.0001)
            return 0;
        return upper;
    }
    else
        return INFINITY;
}

float Arithmetic::op_lower(VarInterval a){
    if(a.bottom==true)
        return 0;

    if(a.lINF==false)
    {
        float sum = 0;
        for(int i=0; i<a.e.length() ;i++)
        {
            sum+=fabs(a.e.at(i)); // here may need to abs(), can e be minus?
        }
        float lower = a.c - sum;
        if(fabs(lower)<=0.0001)
            return 0;
        return lower;
    }
    else
        return -INFINITY;
}

float Arithmetic::op_mid(VarInterval a){
    if(a.lINF==false&&a.uINF==false)
    {
        float mid = (((op_upper(a) + op_lower(a)) / 2)  );
        return mid;
    }
    else{
        if(a.lINF ==true&&a.uINF==false)
            return op_upper(a);
        else if(a.lINF ==false&&a.uINF==true)
            return op_lower(a);
        else{
            qDebug()<<"error:calculate -INF,INF's mid";
        }
    }
}

float Arithmetic::op_dev(VarInterval a){
    if(a.lINF==false&&a.uINF==false)
    {
        float dev = fmax(( op_upper(a) - op_mid(a) ), (op_mid(a) - op_lower(a)));
        return dev;
    }
    else{
        return 0;
    }
}

VarInterval Arithmetic::op_range2interval(float a, float b){
    float init_c = 0;
    QList<float> init_e;
    if(a<=b)
    {
        init_c = (a + b) / 2;
        if(((b - a) /2) != 0)
            init_e.append((b - a) /2);
    }
    else
    {
        init_c = (b + a) / 2;
        if(((a - b) /2) != 0)
            init_e.append((a - b) /2);
    }
    VarInterval var(init_c, init_e);
    return var;
}

VarInterval Arithmetic::op_union(VarInterval a, VarInterval b){ //affine union is not so precise, it may leads to a bigger range

    if(a.bottom==true && b.bottom!=true)
        return b;
    else if(a.bottom!=true && b.bottom==true)
        return a;
    else if(a.bottom==true && b.bottom==true){
        VarInterval empty;
        empty.bottom = true;
        cout<<"empty union result!"<<endl;
        return empty;
    }
    else
    {
//        float a_upper = op_upper(a);
//        float a_lower = op_lower(a);
//        float b_upper = op_upper(b);
//        float b_lower = op_lower(b);
        if((a.lINF==true && a.uINF==true) || (b.lINF==true&&b.uINF==true)){
            return VarInterval();  //return top
        }
        else{ //else
            VarInterval a0b0 = op_range2interval(a.c, b.c);
            float c = op_mid(a0b0);
            //cout<<"c:"<<c<<endl;
            QList<float> e;
            if(true || (a.e.length()!=0 &&b.e.length()!=0)) //calculate Σ(ai∪bi)
            {
                float ei = 0;
                for(int i =0; i<max(a.e.length(), b.e.length()); i++){
                    VarInterval ai;
                    VarInterval bi;
                    if(i<a.e.length())
                        ai = op_range2interval(a.e.at(i), a.e.at(i));
                    else
                        ai.bottom = true;
                    if(i<b.e.length())
                        bi = op_range2interval(b.e.at(i), b.e.at(i));
                    else
                        bi.bottom = true;
                    //cout<<"ai:"<<ai.c<<"bi:"<<bi.c<<endl;
                    VarInterval aibi = op_union(ai, bi);
                    ei += op_upper(aibi);
                    //cout<<"ei:"<<ei<<endl;
                }
                if(ei!=0)
                    e.append(ei);
            }

            float ek = op_dev(a0b0);

            e.append(ek);
            VarInterval union_result(c, e);
            if(a.lINF==true || b.lINF==true){
                union_result.lINF = true;
            }
            if(a.uINF==true || b.uINF==true){
                union_result.uINF = true;
            }
            return union_result;
        }
    }
}

VarInterval Arithmetic::op_intersection(VarInterval a, VarInterval b){

    if(a.bottom==true || b.bottom==true){
        VarInterval empty;
        empty.bottom = true;
        //cout<<"empty intersection result!"<<endl;
        return empty;
    }
    else
    {
       // qDebug()<<"55555"<<a.lINF<<","<<b.lINF;
        if(1) // upper bounds not all inf, lower bounds not all inf
            // both infinity now can be solved guard before: !(a.lINF==true&&b.lINF==true)&&!(a.uINF==true&&b.uINF==true)
        {
            VarInterval temp =  op_accintersection(a, b);
            if(temp.bottom == true){
                return  temp;
            }
//            {
//                if(temp.lINF==true||temp.uINF==true)
//                    return temp;
//            }
            float c = op_mid(temp);
            //cout<<"c:"<<c<<endl;
            QList<float> e;
            float ek = op_dev(temp);
            e.append(ek);
            VarInterval intersection_result(c, e);
            if(temp.lINF==true)
                intersection_result.lINF=true;
            if(temp.uINF ==true)
                intersection_result.uINF=true;
            return intersection_result;
        }
        else{
            VarInterval empty;
            empty.bottom = true;
            //cout<<"empty intersection result!"<<endl;
            return empty;
            //qDebug()<<"error:when intersection, both lower bounds or both upper bounds are infinity!";
            return VarInterval();  //return TOP
        }
    }
}

VarInterval Arithmetic::op_accintersection(VarInterval a, VarInterval b){
    float a_upper = op_upper(a);
    float a_lower = op_lower(a);
    float b_upper = op_upper(b);
    float b_lower = op_lower(b);

    float lower=0;
    float upper=0;
    if(a_upper<b_lower){
        VarInterval empty;
        empty.bottom = true;
       // cout<<"empty intersection result!"<<endl;
        return empty;
    }
    else if(b_upper<a_lower){
        VarInterval empty;
        empty.bottom = true;
       // cout<<"empty intersection result!"<<endl;
        return empty;
    }
    else{

        if(a_lower<b_lower)
            lower = b_lower;
        else
            lower = a_lower;

        if(a_upper<b_upper)
            upper = a_upper;
        else
            upper = b_upper;
    }

    VarInterval accintersection_result;

    if(upper==INFINITY&&lower==-INFINITY){
        return VarInterval();
        //accunion_result = op_range2interval(lower, upper);
    }
    else if(upper ==INFINITY&&lower!=-INFINITY){
        accintersection_result = op_range2interval(lower, lower);
        accintersection_result.uINF =true;
    }
    else if(upper !=INFINITY&&lower==-INFINITY){
        accintersection_result = op_range2interval(upper, upper);
        accintersection_result.lINF =true;
    }
    else{
        accintersection_result = op_range2interval(lower, upper);
    }
    //qDebug()<<"*******"<<lower<<","<<upper;
    //accintersection_result = op_range2interval(lower, upper);
    //accintersection_result = op_range2interval(lower, upper);
    return accintersection_result;
}

VarInterval Arithmetic::op_accunion(VarInterval a, VarInterval b){

    if(a.bottom==true && b.bottom!=true)
        return b;
    else if(a.bottom!=true && b.bottom==true)
        return a;
    else if(a.bottom==true && b.bottom==true){
        VarInterval empty;
        empty.bottom = true;
        //cout<<"empty union result!"<<endl;
        return empty;
    }
    VarInterval accunion_result;
    float a_upper = op_upper(a);
    float a_lower = op_lower(a);
    float b_upper = op_upper(b);
    float b_lower = op_lower(b);

    float lower=0;
    float upper=0;

    if(a_lower<b_lower)
        lower = a_lower;
    else
        lower = b_lower;
    if(a_upper>b_upper)
        upper = a_upper;
    else
        upper = b_upper;

    if(upper==INFINITY&&lower==-INFINITY){
        return VarInterval();
        //accunion_result = op_range2interval(lower, upper);
    }
    else if(upper ==INFINITY&&lower!=-INFINITY){
        accunion_result = op_range2interval(lower, lower);
        accunion_result.uINF =true;
    }
    else if(upper !=INFINITY&&lower==-INFINITY){
        accunion_result = op_range2interval(upper, upper);
        accunion_result.lINF =true;
    }
    else{
        accunion_result = op_range2interval(lower, upper);
    }
    return accunion_result;
}

VarInterval Arithmetic::op_add(VarInterval a, VarInterval b){
    if(a.bottom==true||b.bottom==true)
        return VarInterval::create_bottom();
    float c = a.c + b.c;
    QList<float> e;
    for(int i =0; i<max(a.e.length(), b.e.length()); i++){
        float ei=0;
        if(i<a.e.length())
            ei += a.e.at(i);
        if(i<b.e.length())
            ei += b.e.at(i);
        e.append(ei);
    }

    VarInterval add_result = VarInterval(c, e);
    if(a.lINF==true||b.lINF==true){
        add_result.lINF=true;
    }
    if(a.uINF==true||b.uINF==true){
        add_result.uINF=true;
    }
    return add_result;
}

VarInterval Arithmetic::op_add(VarInterval a, float b){
    if(a.bottom==true)
        return VarInterval::create_bottom();
    a.c = a.c + b;
    return a;
}

VarInterval Arithmetic::op_add(float a, VarInterval b){
    if(b.bottom==true)
        return VarInterval::create_bottom();
    b.c = b.c + a;
    return b;
}

VarInterval Arithmetic::op_multiply(VarInterval a, VarInterval b){
    if(a.bottom==true||b.bottom==true)
        return VarInterval::create_bottom();
    float c = a.c * b.c;
    QList<float> e;

    for(int i=0; i< fmax(a.e.length(), b.e.length());i++){
        float ei = 0;  //n ei
        float eia = 0;
        float eib = 0;
        if(i<a.e.length()){
            eia = a.e.at(i) * b.c;
        }

        if(i<b.e.length()){
            eib = b.e.at(i) * a.c;
        }
        ei = eia + eib;

        e.append(ei);

    }

//    float ek = 0;  // multiplication ek version 2, more accurate, but not our definition!.
//    float ekii = 0;
//    float ekij = 0;

//    for(int i=0; i< min(a.e.length(), b.e.length());i++)
//    {
//        ekii += fabs(a.e.at(i)*b.e.at(i));
//    }
//    ekii = ekii /2;
//    for(int i=0; i< min(a.e.length(), b.e.length());i++)
//        for(int j=0; j< min(a.e.length(), b.e.length()); j++){
//            if(i!=j)
//                ekij += fabs(a.e.at(i)*b.e.at(j));
//        }
//    ek = ekii + ekij;

    float ek=0; // here is the multiplication ek version 1, not so accurate, but good in our definition!!!
    float eka=0;
    float ekb=0;
    for(int i=0; i<a.e.length();i++){
        eka += fabs(a.e.at(i));
    }
    for(int j=0; j<b.e.length();j++){
        ekb += fabs(b.e.at(j));
    }
    ek = eka*ekb;

    e.append(ek);

    VarInterval multiply_result = VarInterval(c, e);
    if(a.lINF==true||b.lINF==true){
        multiply_result.lINF=true;
    }
    if(a.uINF==true||b.uINF==true){
        multiply_result.uINF=true;
    }
    return multiply_result;

}

VarInterval Arithmetic::op_multiply(VarInterval a, float b){
    if(a.bottom==true)
        return VarInterval::create_bottom();
    a.c = a.c * b;
    for(int i=0; i<a.e.length();i++){
        a.e[i] = a.e.at(i) * b;
    }
    return a;
}

VarInterval Arithmetic::op_multiply(float a, VarInterval b){
    if(b.bottom==true)
        return VarInterval::create_bottom();
    b.c = b.c * a;
    for(int i=0; i<b.e.length();i++){
       b.e[i] = b.e.at(i) * a;
    }
    return b;
}

VarInterval Arithmetic::op_neg(VarInterval a){
    if(a.bottom==true)
        return VarInterval::create_bottom();
    return op_multiply(-1, a);
}

VarInterval Arithmetic::op_equal(VarInterval a, VarInterval b){
    if(a.bottom==true||b.bottom==true)
        return VarInterval::create_bottom();
    if(op_upper(a)==op_upper(b) && op_lower(a)==op_lower(b)){
        return op_range2interval(1, 1);
    }
    else if(op_upper(a)<op_lower(b)&&op_upper(b)<op_lower(a)){
        return op_range2interval(0, 0);
    }
    else
        return op_range2interval(0, 1);
}

VarInterval Arithmetic::op_less(VarInterval a, VarInterval b){
    if(a.bottom==true||b.bottom==true)
        return VarInterval::create_bottom();
    if(op_upper(a)<op_lower(b))
        return op_range2interval(1, 1);
    else if(op_upper(b)<=op_lower(a))
        return op_range2interval(0, 0);
    else
        return op_range2interval(0, 1);
}

VarInterval Arithmetic::op_widening(VarInterval a, VarInterval b){

    if(a.bottom==true && b.bottom!=true)
        return b;
    else if(a.bottom!=true && b.bottom==true)
        return a;
    else if(a.bottom==true && b.bottom==true){
        VarInterval empty;
        empty.bottom = true;
        //cout<<"empty union result!"<<endl;
        return empty;
    }

    //VarInterval widening_result = VarInterval();
//    bool lINFflag = false;
//    bool uINFflag = false;

    float lower = 0;
    float upper = 0;

    float a_upper = op_upper(a);
    float a_lower = op_lower(a);
    float b_upper = op_upper(b);
    float b_lower = op_lower(b);

    if(a_lower<=b_lower){
        lower = a_lower;
    }
    else{
        lower = -INFINITY;
    }

    if(a_upper>=b_upper){
        upper = a_upper;
    }
    else{
        upper = INFINITY;
    }


    if(lower!= -INFINITY && upper!= INFINITY){
        return op_range2interval(lower, upper);
    }
    else if(lower!= -INFINITY && upper ==INFINITY){
        VarInterval widening_result = VarInterval(a.c, a.e);
        widening_result.uINF =true;
        return widening_result;
    }
    else if(lower== -INFINITY && upper !=INFINITY){
        VarInterval widening_result = VarInterval(a.c, a.e);
        widening_result.lINF =true;
        return widening_result;
    }
    else{
        return VarInterval();
    }


}
VarInterval Arithmetic::op_narrowing(VarInterval a, VarInterval b){

    if(a.bottom==true && b.bottom!=true)
        return a;
    else if(a.bottom!=true && b.bottom==true)
        return b;
    else if(a.bottom==true && b.bottom==true){
        VarInterval empty;
        empty.bottom = true;
        //cout<<"empty union result!"<<endl;
        return empty;
    }

    float lower = 0;
    float upper = 0;
    float a_upper = op_upper(a);
    float a_lower = op_lower(a);
    float b_upper = op_upper(b);
    float b_lower = op_lower(b);

    if(a_lower == -INFINITY){
        lower = b_lower;
    }
    else{
        lower = a_lower;
    }

    if(a_upper == INFINITY){
        upper = b_upper;
    }
    else{
        upper = a_upper;
    }

    if(lower != -INFINITY && upper!= INFINITY){
        return op_range2interval(lower, upper);
    }
    else if(lower!=-INFINITY&& upper==INFINITY){
        if(a_lower == -INFINITY){
            VarInterval widening_result = VarInterval(b.c, b.e);
            widening_result.uINF =true;
            return widening_result;
        }
        else if(a_lower != -INFINITY){
            VarInterval widening_result = VarInterval(a.c, a.e);
            widening_result.uINF =true;
            return widening_result;
        }
        else{
            qDebug()<<"error: narrowing out of control!";
            return VarInterval();
        }
    }
    else if(lower == -INFINITY && upper !=INFINITY){
        if(a_upper == INFINITY){
            VarInterval widening_result = VarInterval(b.c, b.e);
            widening_result.lINF =true;
            return widening_result;
        }
        else if(a_upper != INFINITY){
            VarInterval widening_result = VarInterval(a.c, a.e);
            widening_result.lINF =true;
            return widening_result;
        }
        else{
            qDebug()<<"error: narrowing out of control!";
            return VarInterval();
        }
    }
    else if(lower == -INFINITY && upper ==INFINITY){
        return VarInterval();
    }
    else{
        qDebug()<<"error: narrowing out of control!";
        return VarInterval();
    }
}

