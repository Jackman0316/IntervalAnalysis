#include "intervalanalysis.h"
#include "varinterval.h"
#include "constraintsystem.h"
#include "consystemitem.h"
#include <QMap>
#include <QStack>
#include <QVariant>
#include <QList>
#include "edgeseffects.h"
#include "arithmetic.h"
#include <QDebug>
using namespace std;
IntervalAnalysis::IntervalAnalysis()
{

}
void IntervalAnalysis::Plain(){
    consystem_new = consystem_old;
    for(int i=0; i<consystem_new.length();i++)
    {
        qDebug()<<consystem_new.at(i).code;
        if(i==0||consystem_new.at(i).consystem.contains("start")){  // start point set TOP
            for(int v=0;v<vars.length();v++){
                VarInterval top = VarInterval();
                consystem_new[i].var_interval[vars.at(v)]= top;
            }
        }
        else{
            if(consystem_new.at(i).consystem.contains("union")){
                QMap<QString, VarInterval> left_temp = consystem_new.at(i).var_interval;
                QMap<QString, VarInterval> right_temp = consystem_new.at(i).var_interval;

                QString union_left = consystem_new.at(i).consystem.split("union")[0];
                QString union_right = consystem_new.at(i).consystem.split("union")[1];

                if(union_left.contains("neg"))
                {
                    int real_union_left = union_left.split("@")[1].toInt();
                    ConsystemItem negative = consystem_new.at(real_union_left);
                    negative.code = "<neg,_>" + negative.code;
                    left_temp = EdgesEffects::extract_interval(negative);
                }
                else{
                    int real_union_left = union_left.toInt();
                    ConsystemItem positive = consystem_new.at(real_union_left);
                    if(positive.code.contains("<while,_>")||positive.code.contains("<if,_>"))
                        positive.code = "<pos,_>" + positive.code;
                    //qDebug()<<"****************************"<<positive.code;
                    left_temp = EdgesEffects::extract_interval(positive);
                }

                if(union_right.contains("neg"))
                {
                    int real_union_right = union_right.split("@")[1].toInt();
                    ConsystemItem negative = consystem_new.at(real_union_right);
                    negative.code = "<neg,_>" + negative.code;
                    right_temp = EdgesEffects::extract_interval(negative);
                }
                else{

                    int real_union_right = union_right.toInt();
                    ConsystemItem positive = consystem_new.at(real_union_right);
                    //qDebug()<<consystem_new.at(real_union_right).var_interval["i"].lINF;
                    if(positive.code.contains("<while,_>")||positive.code.contains("<if,_>"))
                        positive.code = "<pos,_>" + positive.code;
                    //qDebug()<<"****************************"<<positive.code;
                    right_temp = EdgesEffects::extract_interval(positive);
                }

                QMap<QString, VarInterval> temp;
                for(int v=0;v<vars.length();v++){ //union
                    //qDebug()
                    //qDebug()<<Arithmetic::op_upper(left_temp[vars.at(v)])<<",left:"<<Arithmetic::op_lower(left_temp[vars.at(v)]);
                    //qDebug()<<Arithmetic::op_upper(right_temp[vars.at(v)])<<",right:"<<Arithmetic::op_lower(right_temp[vars.at(v)]);
                    temp.insert(vars.at(v) , Arithmetic::op_union(left_temp[vars.at(v)], right_temp[vars.at(v)]));
                   // qDebug()<<Arithmetic::op_upper(temp[vars.at(v)])<<",temp:"<<Arithmetic::op_lower(temp[vars.at(v)]);

                }
                QMap<QString, VarInterval> old = consystem_old.at(i).var_interval;
                consystem_new[i].var_interval = temp;
//                for(int v=0;v<vars.length();v++){ //widening
//                    consystem_new[i].var_interval[vars.at(v)] = Arithmetic::op_widening(old[vars.at(v)], temp[vars.at(v)]);
//                }
            }
            else if(consystem_new.at(i).consystem.contains("neg")&&!(consystem_new.at(i).consystem.contains("union")))
            {
                QMap<QString, VarInterval> temp;
                int real_neg = consystem_new.at(i).consystem.split("@")[1].toInt();
                ConsystemItem negative = consystem_new.at(real_neg);
                negative.code = "<neg,_>" + negative.code;
                temp = EdgesEffects::extract_interval(negative);
                QMap<QString, VarInterval> old = consystem_old.at(i).var_interval;
                consystem_new[i].var_interval = temp;
//                for(int v=0;v<vars.length();v++){ //widening
//                    consystem_new[i].var_interval[vars.at(v)] = Arithmetic::op_widening(old[vars.at(v)], temp[vars.at(v)]);
//                }

            }
            else{

                QMap<QString, VarInterval> temp;
                int from = consystem_new.at(i).consystem.toInt();
                ConsystemItem sequential = consystem_new.at(from);

                if(sequential.code.contains("<while,_>")||sequential.code.contains("<if,_>"))
                    sequential.code = "<pos,_>" + sequential.code;
                temp = EdgesEffects::extract_interval(sequential);

                QMap<QString, VarInterval> old = consystem_old.at(i).var_interval;
                consystem_new[i].var_interval = temp;
//                for(int v=0;v<vars.length();v++){ //widening
//                    consystem_new[i].var_interval[vars.at(v)] = Arithmetic::op_widening(old[vars.at(v)], temp[vars.at(v)]);
//                }

            }
        }
    }
    updateUI(consystem_new,"Plain");
    // new value got, here update UI!
    qDebug()<<"--------------------------------------";
    for(int i=0;i<consystem_new.length();i++)
    {

        qDebug()<<consystem_new.at(i).pp<<":"<<consystem_new.at(i).consystem<<":"<<consystem_new.at(i).code;
        QStringList varstest = consystem_new.at(i).var_interval.keys();
        for(int v=0;v<varstest.length();v++)
            if(consystem_new.at(i).var_interval[varstest.at(v)].bottom==false)
                qDebug()<<varstest.at(v)<<":["<<Arithmetic::op_lower(consystem_new.at(i).var_interval[varstest.at(v)])<<","<<
                    Arithmetic::op_upper(consystem_new.at(i).var_interval[varstest.at(v)]) << "]";
            else{
                qDebug()<<varstest.at(v)<<":["<<"⊥"<<"]";
            }
    }

    if(ConstraintSystem::conequal(consystem_new, consystem_old, vars))
    {
        return ;
    }
    else{
        consystem_old = consystem_new;
        Plain();
    }
}

void IntervalAnalysis::Widening(){
    consystem_new = consystem_old;
    for(int i=0; i<consystem_new.length();i++)
    {
        qDebug()<<consystem_new.at(i).code;
        if(i==0||consystem_new.at(i).consystem.contains("start")){  // start point set TOP
            for(int v=0;v<vars.length();v++){
                VarInterval top = VarInterval();
                consystem_new[i].var_interval[vars.at(v)]= top;
            }
        }
        else{
            if(consystem_new.at(i).consystem.contains("union")){
                QMap<QString, VarInterval> left_temp = consystem_new.at(i).var_interval;
                QMap<QString, VarInterval> right_temp = consystem_new.at(i).var_interval;

                QString union_left = consystem_new.at(i).consystem.split("union")[0];
                QString union_right = consystem_new.at(i).consystem.split("union")[1];

                if(union_left.contains("neg"))
                {
                    int real_union_left = union_left.split("@")[1].toInt();
                    ConsystemItem negative = consystem_new.at(real_union_left);
                    negative.code = "<neg,_>" + negative.code;
                    left_temp = EdgesEffects::extract_interval(negative);
                }
                else{
                    int real_union_left = union_left.toInt();
                    ConsystemItem positive = consystem_new.at(real_union_left);
                    if(positive.code.contains("<while,_>")||positive.code.contains("<if,_>"))
                        positive.code = "<pos,_>" + positive.code;
                    //qDebug()<<"****************************"<<positive.code;
                    left_temp = EdgesEffects::extract_interval(positive);
                }

                if(union_right.contains("neg"))
                {
                    int real_union_right = union_right.split("@")[1].toInt();
                    ConsystemItem negative = consystem_new.at(real_union_right);
                    negative.code = "<neg,_>" + negative.code;
                    right_temp = EdgesEffects::extract_interval(negative);
                }
                else{

                    int real_union_right = union_right.toInt();
                    ConsystemItem positive = consystem_new.at(real_union_right);
                    //qDebug()<<consystem_new.at(real_union_right).var_interval["i"].lINF;
                    if(positive.code.contains("<while,_>")||positive.code.contains("<if,_>"))
                        positive.code = "<pos,_>" + positive.code;
                    //qDebug()<<"****************************"<<positive.code;
                    right_temp = EdgesEffects::extract_interval(positive);
                }

                QMap<QString, VarInterval> temp;
                for(int v=0;v<vars.length();v++){ //union
                    //qDebug()
                    //qDebug()<<Arithmetic::op_upper(left_temp[vars.at(v)])<<",left:"<<Arithmetic::op_lower(left_temp[vars.at(v)]);
                   // qDebug()<<Arithmetic::op_upper(right_temp[vars.at(v)])<<",right:"<<Arithmetic::op_lower(right_temp[vars.at(v)]);
                    temp.insert(vars.at(v) , Arithmetic::op_union(left_temp[vars.at(v)], right_temp[vars.at(v)]));
                   // qDebug()<<Arithmetic::op_upper(temp[vars.at(v)])<<",temp:"<<Arithmetic::op_lower(temp[vars.at(v)]);

                }
                QMap<QString, VarInterval> old = consystem_old.at(i).var_interval;
               // consystem_new[i].var_interval = temp;
                for(int v=0;v<vars.length();v++){ //widening
                    consystem_new[i].var_interval[vars.at(v)] = Arithmetic::op_widening(old[vars.at(v)], temp[vars.at(v)]);
                }
            }
            else if(consystem_new.at(i).consystem.contains("neg")&&!(consystem_new.at(i).consystem.contains("union")))
            {
                QMap<QString, VarInterval> temp;
                int real_neg = consystem_new.at(i).consystem.split("@")[1].toInt();
                ConsystemItem negative = consystem_new.at(real_neg);
                negative.code = "<neg,_>" + negative.code;
                temp = EdgesEffects::extract_interval(negative);
                QMap<QString, VarInterval> old = consystem_old.at(i).var_interval;
                //consystem_new[i].var_interval = temp;
                for(int v=0;v<vars.length();v++){ //widening
                    consystem_new[i].var_interval[vars.at(v)] = Arithmetic::op_widening(old[vars.at(v)], temp[vars.at(v)]);
                }

            }
            else{

                QMap<QString, VarInterval> temp;
                int from = consystem_new.at(i).consystem.toInt();
                ConsystemItem sequential = consystem_new.at(from);

                if(sequential.code.contains("<while,_>")||sequential.code.contains("<if,_>"))
                    sequential.code = "<pos,_>" + sequential.code;

                temp = EdgesEffects::extract_interval(sequential);
                qDebug()<<"333"<<"+"<<i<<temp["i"].uINF<<sequential.var_interval["i"].uINF;
                //qDebug()<<"333"<<"+"<<i<<temp["i"].uINF;
                QMap<QString, VarInterval> old = consystem_old.at(i).var_interval;
                //consystem_new[i].var_interval = temp;
                for(int v=0;v<vars.length();v++){ //widening
                    consystem_new[i].var_interval[vars.at(v)] = Arithmetic::op_widening(old[vars.at(v)], temp[vars.at(v)]);
                }

            }
        }
    }

    // new value got, here update UI!
    qDebug()<<"-------WIDENING-------";
    for(int i=0;i<consystem_new.length();i++)
    {

        qDebug()<<consystem_new.at(i).pp<<":"<<consystem_new.at(i).consystem<<":"<<consystem_new.at(i).code;
        QStringList varstest = consystem_new.at(i).var_interval.keys();
        for(int v=0;v<varstest.length();v++)
            if(consystem_new.at(i).var_interval[varstest.at(v)].bottom==false)
                qDebug()<<varstest.at(v)<<":["<<Arithmetic::op_lower(consystem_new.at(i).var_interval[varstest.at(v)])<<","<<
                    Arithmetic::op_upper(consystem_new.at(i).var_interval[varstest.at(v)]) << "]";
            else{
                qDebug()<<varstest.at(v)<<":["<<"⊥"<<"]";
            }
    }
    updateUI(consystem_new,"Widening");

    if(ConstraintSystem::conequal(consystem_new, consystem_old, vars))
    {
        return ;
    }
    else{
        consystem_old = consystem_new;
        Widening();
    }


}

void IntervalAnalysis::Narrowing(){
    consystem_new = consystem_old;
    for(int i=0; i<consystem_new.length();i++)
    {
        qDebug()<<consystem_new.at(i).code;
        if(i==0||consystem_new.at(i).consystem.contains("start")){  // start point set TOP
            for(int v=0;v<vars.length();v++){
                VarInterval top = VarInterval();
                consystem_new[i].var_interval[vars.at(v)]= top;
            }
        }
        else{
            if(consystem_new.at(i).consystem.contains("union")){
                QMap<QString, VarInterval> left_temp = consystem_new.at(i).var_interval;
                QMap<QString, VarInterval> right_temp = consystem_new.at(i).var_interval;

                QString union_left = consystem_new.at(i).consystem.split("union")[0];
                QString union_right = consystem_new.at(i).consystem.split("union")[1];

                if(union_left.contains("neg"))
                {
                    int real_union_left = union_left.split("@")[1].toInt();
                    ConsystemItem negative = consystem_new.at(real_union_left);
                    negative.code = "<neg,_>" + negative.code;
                    left_temp = EdgesEffects::extract_interval(negative);
                }
                else{
                    int real_union_left = union_left.toInt();
                    ConsystemItem positive = consystem_new.at(real_union_left);
                    if(positive.code.contains("<while,_>")||positive.code.contains("<if,_>"))
                        positive.code = "<pos,_>" + positive.code;
                    //qDebug()<<"****************************"<<positive.code;
                    left_temp = EdgesEffects::extract_interval(positive);
                }

                if(union_right.contains("neg"))
                {
                    int real_union_right = union_right.split("@")[1].toInt();
                    ConsystemItem negative = consystem_new.at(real_union_right);
                    negative.code = "<neg,_>" + negative.code;
                    right_temp = EdgesEffects::extract_interval(negative);
                }
                else{

                    int real_union_right = union_right.toInt();
                    ConsystemItem positive = consystem_new.at(real_union_right);
                    //qDebug()<<consystem_new.at(real_union_right).var_interval["i"].lINF;
                    if(positive.code.contains("<while,_>")||positive.code.contains("<if,_>"))
                        positive.code = "<pos,_>" + positive.code;
                    //qDebug()<<"****************************"<<positive.code;
                    right_temp = EdgesEffects::extract_interval(positive);
                }

                QMap<QString, VarInterval> temp;
                for(int v=0;v<vars.length();v++){ //union
                    //qDebug()
                    //qDebug()<<Arithmetic::op_upper(left_temp[vars.at(v)])<<",left:"<<Arithmetic::op_lower(left_temp[vars.at(v)]);
                   // qDebug()<<Arithmetic::op_upper(right_temp[vars.at(v)])<<",right:"<<Arithmetic::op_lower(right_temp[vars.at(v)]);
                    temp.insert(vars.at(v) , Arithmetic::op_union(left_temp[vars.at(v)], right_temp[vars.at(v)]));
                   // qDebug()<<Arithmetic::op_upper(temp[vars.at(v)])<<",temp:"<<Arithmetic::op_lower(temp[vars.at(v)]);

                }
                QMap<QString, VarInterval> old = consystem_old.at(i).var_interval;
               // consystem_new[i].var_interval = temp;
                for(int v=0;v<vars.length();v++){ // narrowing
                    consystem_new[i].var_interval[vars.at(v)] = Arithmetic::op_narrowing(old[vars.at(v)], temp[vars.at(v)]);
                }
            }
            else if(consystem_new.at(i).consystem.contains("neg")&&!(consystem_new.at(i).consystem.contains("union")))
            {
                QMap<QString, VarInterval> temp;
                int real_neg = consystem_new.at(i).consystem.split("@")[1].toInt();
                ConsystemItem negative = consystem_new.at(real_neg);
                negative.code = "<neg,_>" + negative.code;
                temp = EdgesEffects::extract_interval(negative);
                QMap<QString, VarInterval> old = consystem_old.at(i).var_interval;
                //consystem_new[i].var_interval = temp;
                for(int v=0;v<vars.length();v++){ //op_narrowing
                    consystem_new[i].var_interval[vars.at(v)] = Arithmetic::op_narrowing(old[vars.at(v)], temp[vars.at(v)]);
                }

            }
            else{

                QMap<QString, VarInterval> temp;
                int from = consystem_new.at(i).consystem.toInt();
                ConsystemItem sequential = consystem_new.at(from);

                if(sequential.code.contains("<while,_>")||sequential.code.contains("<if,_>"))
                    sequential.code = "<pos,_>" + sequential.code;

                temp = EdgesEffects::extract_interval(sequential);
                //qDebug()<<"333"<<"+"<<i<<temp["i"].uINF<<sequential.var_interval["i"].uINF;
                //qDebug()<<"333"<<"+"<<i<<temp["i"].uINF;
                QMap<QString, VarInterval> old = consystem_old.at(i).var_interval;
                //consystem_new[i].var_interval = temp;
                for(int v=0;v<vars.length();v++){ //op_narrowing
                    consystem_new[i].var_interval[vars.at(v)] = Arithmetic::op_narrowing(old[vars.at(v)], temp[vars.at(v)]);
                }

            }
        }
    }
    updateUI(consystem_new,"Narrowing");
    // new value got, here update UI!
    qDebug()<<"-------NARROWING-------";
    for(int i=0;i<consystem_new.length();i++)
    {

        qDebug()<<consystem_new.at(i).pp<<":"<<consystem_new.at(i).consystem<<":"<<consystem_new.at(i).code;
        QStringList varstest = consystem_new.at(i).var_interval.keys();
        for(int v=0;v<varstest.length();v++)
            if(consystem_new.at(i).var_interval[varstest.at(v)].bottom==false)
                qDebug()<<varstest.at(v)<<":["<<Arithmetic::op_lower(consystem_new.at(i).var_interval[varstest.at(v)])<<","<<
                    Arithmetic::op_upper(consystem_new.at(i).var_interval[varstest.at(v)]) << "]";
            else{
                qDebug()<<varstest.at(v)<<":["<<"⊥"<<"]";
            }
    }

    if(ConstraintSystem::conequal(consystem_new, consystem_old, vars))
    {
        return ;
    }
    else{
        consystem_old = consystem_new;
        Narrowing();
    }
}

void IntervalAnalysis::updateUI(QList<ConsystemItem> consystem_ui, QString mode){

    int now = tableWidget_interval->columnCount();//获取当前列数
    //qDebug()<<"155";
    //qDebug()<<now;
    tableWidget_interval->setColumnCount(now +vars.length());//添加一列

    //qDebug()<<"121";
    tableWidget_interval->setItem(0,now,new QTableWidgetItem(mode));
    tableWidget_interval->item(0,now)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    //tableWidget_interval->setItem(0,now+1,new QTableWidgetItem("round"));
    //qDebug()<<"111";
    QStringList varstest = consystem_ui.at(0).var_interval.keys();
    for(int j =0;j<varstest.length();j++){
        tableWidget_interval->setItem(1,now+j,new QTableWidgetItem(varstest.at(j)));
        tableWidget_interval->item(1,now+j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    }

    for(int i=0;i<consystem_ui.length();i++){
        //qDebug()<<"111";
        varstest = consystem_new.at(i).var_interval.keys();
        for(int v=0;v<varstest.length();v++){
            if(consystem_ui.at(i).var_interval[varstest.at(v)].bottom==false){
                QString range = QString("["+QString::number(Arithmetic::op_lower(consystem_new.at(i).var_interval[varstest.at(v)]))+","
                                                                                       +QString::number(Arithmetic::op_upper(consystem_new.at(i).var_interval[varstest.at(v)])) + "]");
                //qDebug()<<range;
                tableWidget_interval->setItem(i+2,v+now,new QTableWidgetItem(range));
                tableWidget_interval->item(i+2,v+now)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            }
            else{
                tableWidget_interval->setItem(i+2,v+now,new QTableWidgetItem(QString("⊥")));
                tableWidget_interval->item(i+2,v+now)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            }

        }

    }

}
