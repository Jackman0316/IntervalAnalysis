#include "edgeseffects.h"
#include <QDebug>
#include "varinterval.h"
#include "arithmetic.h"
EdgesEffects::EdgesEffects()
{

}

QMap<QString, VarInterval> EdgesEffects::extract_interval(ConsystemItem consystem_pp){
    if(consystem_pp.code.contains("<while,_>")){
        if(consystem_pp.code.left(7)=="<pos,_>"){
            QString code = consystem_pp.code;
            code = code.split("<(,_>")[1];
            code = code.split("<),_>")[0];
            //qDebug()<<code;
            return eval_pos(code, consystem_pp.var_interval);
        }
        else if(consystem_pp.code.left(7)=="<neg,_>"){
            QString code = consystem_pp.code;
            code = code.split("<(,_>")[1];
            code = code.split("<),_>")[0];
            //qDebug()<<code;
            return eval_neg(code, consystem_pp.var_interval);
        }
        else{
            //qDebug()<<"---------------"<<consystem_pp.code;
            qDebug()<<"error: while statement have no pos or neg mark!";
            return consystem_pp.var_interval;
        }
    }
    else if(consystem_pp.code.contains("<if,_>")){
        if(consystem_pp.code.left(7)=="<pos,_>"){
            QString code = consystem_pp.code;
            code = code.split("<(,_>")[1];
            code = code.split("<),_>")[0];
            //qDebug()<<code;
            return eval_pos(code, consystem_pp.var_interval);
        }
        else if(consystem_pp.code.left(7)=="<neg,_>"){
            QString code = consystem_pp.code;
            code = code.split("<(,_>")[1];
            code = code.split("<),_>")[0];
            //qDebug()<<code;
            return eval_neg(code, consystem_pp.var_interval);
        }
        else{
            qDebug()<<"error: if statement have no pos or neg mark!";
            return consystem_pp.var_interval;
        }

    }
    else if(consystem_pp.code.contains("<[,_>")&&consystem_pp.code.contains("<],_>")){
        QString code = consystem_pp.code;
        QString left = code.split("=")[0];
        QString right = code.split("=")[1];
        if((left.contains("<[,_>")&&left.contains("<],_>"))&&(!right.contains("<[,_>")&&!right.contains("<],_>"))){
            return eval_write(code, consystem_pp.var_interval);
        }
        else if((!left.contains("<[,_>")&&!left.contains("<],_>"))&&(right.contains("<[,_>")&&right.contains("<],_>"))){
            return eval_load(code, consystem_pp.var_interval);
        }
        else if((left.contains("<[,_>")&&left.contains("<],_>"))&&(right.contains("<[,_>")&&right.contains("<],_>"))){
            return eval_write(code, consystem_pp.var_interval);
        }
        else{
            qDebug()<<"error:[] out of control!";
            return consystem_pp.var_interval;
        }
    }
    else if(consystem_pp.code=="<;,_>"){
        QString code = consystem_pp.code;
        return eval_nop(code, consystem_pp.var_interval);
    }
    else if(consystem_pp.code.contains("<=,_>")){
        QString code = consystem_pp.code;
        int flag = 0;
        QStringList temp = code.split(">");
        for(int j=0;j<temp.length();j++){
            if(temp.at(j).left(5)=="<IDN,"&&((j+1>=temp.length())||(temp.at(j+1).left(4)!="<[,_"))){
                if(temp.at(j).split(",").at(1).left(1).contains(QRegExp("[A-Z]")))
                    flag = 1;
                //qDebug()<<vars.last();
            }

        }

        if(flag ==1){
            return eval_write(code, consystem_pp.var_interval);
        }
        else
            return eval_assign(code, consystem_pp.var_interval);
        //qDebug()<<"state: " + code +" is translated as assignment statement. Am I right?";

    }
    else{
        qDebug()<<"error:extract_interval out of control! at:" + consystem_pp.code;
        return consystem_pp.var_interval;
    }
}

QMap<QString, VarInterval> EdgesEffects::eval_nop(QString code, QMap<QString, VarInterval> var_interval){
    if(code =="<;,_>"){
        return var_interval;
    }
    else{
        qDebug()<<"error:NOP is not NOP!";
        return var_interval;
    }
}

QMap<QString, VarInterval> EdgesEffects::eval_assign(QString code, QMap<QString, VarInterval> var_interval){
    QString left = code.split("<=,_>")[0];
    QString right = code.split("<=,_>")[1];
    QString target = "";
    if(left.left(5)=="<IDN,"){
        if(left.split("<").length()>2){
            qDebug()<<"error: assignment leftside have more than one token!";
            return var_interval;
        }
        else{
            target = left.split(",")[1];
            target = target.split(">")[0];

            var_interval[target] = eval_expression(right.split("<;,_>")[0], var_interval);
            //qDebug()<<"[[[[[[["<<var_interval[target].c;

            return var_interval;
        }
    }
    else{
        qDebug()<<"error: assignment leftside have more than one token!";
        return var_interval;
    }

}

QMap<QString, VarInterval> EdgesEffects::eval_load(QString code, QMap<QString, VarInterval> var_interval){
    QString left = code.split("<=,_>")[0];
    QString right = code.split("<=,_>")[1];
    QString target = "";
    if(left.left(5)=="<IDN,"){
        if(left.split("<").length()>2){
            qDebug()<<"error:memory lode leftside have more than one token!";
            return var_interval;
        }
        else{
            target = left.split(",")[1];
            target = target.split(">")[0];
            var_interval[target] = VarInterval();  //memory load, set to top!
            return var_interval;
        }
    }
    else{
        qDebug()<<"error: assignment leftside have more than one token!";
        return var_interval;
    }
}

QMap<QString, VarInterval> EdgesEffects::eval_write(QString code, QMap<QString, VarInterval> var_interval){

    return var_interval;  //memory write do nothing!
}

QMap<QString, VarInterval> EdgesEffects::eval_pos(QString code, QMap<QString, VarInterval> var_interval){
     // != | == | > | < | >= | <=
    code = code.remove(0,1);
    code = code.remove((code.length()-1), 1);
    //qDebug()<<code;
    int op_counter=0;
    //qDebug()<<"1111111"<<Arithmetic::op_lower(var_interval["i"])<<","<<Arithmetic::op_upper(var_interval["i"]);
    QStringList pos =code.split("><");
    for(int i=0;i<pos.length();i++){
        if(pos.at(i).contains("!=,")||pos.at(i).contains("==,")
                ||pos.at(i).contains(">,")||pos.at(i).contains("<,")||pos.at(i).contains(">=,")||pos.at(i).contains("<=,")){
            op_counter ++;
        }
    }
    //qDebug()<<"op_counter"<<op_counter;
    if(op_counter==1){
        if(code.contains("CONST,")){
            QString var;
            int number=0;
            //int const
            int flag = 0;
            if(pos.at(0).contains("IDN,")&&pos.at(2).contains("CONST,")){
                var = pos.at(0).split(",")[1];
                number = pos.at(2).split(",")[1].toInt();
                flag = 0;
            }
            else if(pos.at(0).contains("CONST,")&&pos.at(2).contains("IDN,")){
                var = pos.at(2).split(",")[1];
                number = pos.at(0).split(",")[1].toInt();
                flag = 2;
            }
            else{
                qDebug()<<"error:operator not between factors!";
                return var_interval;
            }

            if(flag ==2){ // IDN put in front
                QString swap = pos.at(2);
                pos[2] = pos.at(0);
                pos[0] = swap;

                for(int j=0;j<pos.length();j++){
                    if(pos.at(j).contains(">,"))
                        pos[j].replace(">","<");
                    else if(pos.at(j).contains(">=,"))
                        pos[j].replace(">=","<=");
                    else if(pos.at(j).contains("<,"))
                        pos[j].replace("<",">");
                    else if(pos.at(j).contains("<=,"))
                        pos[j].replace("<=",">=");
                }
            }

            //qDebug()<<pos.at(flag)<<"111111";
            VarInterval const_number = Arithmetic::op_range2interval(number,number);

            if(pos.at(1)=="!=,_"){
                //should not happened, if != we should negate
            }
            else if(pos.at(1)=="==,_"){
                var_interval[var] = Arithmetic::op_intersection(var_interval[var], const_number);
                if(var_interval[var].bottom==true){//all set to bottom
                    QStringList variables =  var_interval.keys();
                    for(int v=0;v<variables.length();v++){
                        var_interval[variables.at(v)]= VarInterval::create_bottom();
                    }
                }
                else{
                    return var_interval;
                }
            }
            else if(pos.at(1)==">,_"||pos.at(1)==">=,_"){
                if(pos.at(1)==">,_")
                    const_number = Arithmetic::op_add(const_number, 1);
                const_number.uINF=true;
                var_interval[var] = Arithmetic::op_intersection(var_interval[var], const_number);

               // qDebug()<<Arithmetic::op_upper(var_interval[var])<<",temp:"<<Arithmetic::op_lower(var_interval[var]);

                if(var_interval[var].bottom==true){//all set to bottom
                    QStringList variables = var_interval.keys();
                    for(int v=0;v<variables.length();v++){
                        var_interval[variables.at(v)]= VarInterval::create_bottom();
                    }
                }
                else{
                    return var_interval;
                }
            }
            else if(pos.at(1)=="<,_"||pos.at(1)=="<=,_"){
                if(pos.at(1)=="<,_")
                    const_number = Arithmetic::op_add(const_number, -1);

                //qDebug()<<"2222222"<<const_number.c;
                const_number.lINF=true;
                qDebug()<<"2222222"<<Arithmetic::op_lower(var_interval[var]);

                var_interval[var] = Arithmetic::op_intersection(var_interval[var], const_number);

                if(var_interval[var].bottom==true){//all set to bottom
                    QStringList variables = var_interval.keys();
                    for(int v=0;v<variables.length();v++){
                        var_interval[variables.at(v)]= VarInterval::create_bottom();
                    }
                }
                else{
                    return var_interval;
                }
            }
        }
        else{
            qDebug()<<"error: we should not allowed pure variables guard!";
            return var_interval;
        }
    }
    else if(op_counter==2){
        //qDebug()<<pos.at(0)<<" "<<pos.at(1)<<" "<<pos.at(2)<<" "<<pos.at(3)<<" "<<pos.at(4);
        if(pos.at(0).contains("CONST,")&&pos.at(4).contains("CONST,")){
            if(pos.at(2).contains("IDN,")){
                QString var;
                int numberleft=0;
                int numberright=0;
                var = pos.at(2).split(",")[1];
                numberleft = pos.at(0).split(",")[1].toInt();
                numberright = pos.at(4).split(",")[1].toInt();
                VarInterval const_number;
                if(pos.at(1)==">=,_" || pos.at(1)==">,_"){
                    if(pos.at(1)==">,_")
                        numberleft--;
                    if(pos.at(3)==">,_")
                        numberright++;
                    const_number = Arithmetic::op_range2interval(numberleft,numberright);
                }
                else if(pos.at(1)=="<=,_" || pos.at(1)=="<,_"){
                    if(pos.at(1)=="<,_")
                        numberleft++;
                    if(pos.at(3)=="<,_")
                        numberright--;
                    const_number = Arithmetic::op_range2interval(numberleft,numberright);
                    //qDebug()<<",,,,,,"<< Arithmetic::op_upper(const_number);
                }
                else{
                    qDebug()<<"error:eval pos error at 2 operators(1)";
                }
                //qDebug()<<numberleft<<","<<numberright;
                var_interval[var] = Arithmetic::op_intersection(var_interval[var], const_number);

                qDebug()<<"1111111"<<Arithmetic::op_lower(var_interval["i"])<<","<<Arithmetic::op_upper(var_interval["i"]);

                if(var_interval[var].bottom==true){//all set to bottom
                    QStringList variables = var_interval.keys();
                    for(int v=0;v<variables.length();v++){
                        var_interval[variables.at(v)]= VarInterval::create_bottom();
                    }
                }
                else{
                    return var_interval;
                }
            }
            else{
                qDebug()<<"error:eval pos error at 2 operators(2)";
            }
        }
        else{
            qDebug()<<"error:eval pos error at 2 operators(3)";
        }
    }
    else{
        qDebug()<<"error: wrong boolean expression! too much operators";
    }
    return var_interval;
}

QMap<QString, VarInterval> EdgesEffects::eval_neg(QString code, QMap<QString, VarInterval> var_interval){
    code = code.remove(0,1);
    code = code.remove(code.length()-1, 1);
    int op_counter=0;

    QStringList neg =code.split("><");
    for(int i=0;i<neg.length();i++){
        if(neg.at(i).contains("!=,")||neg.at(i).contains("==,")
                ||neg.at(i).contains(">,")||neg.at(i).contains("<,")||neg.at(i).contains(">=,")||neg.at(i).contains("<=,")){
            op_counter ++;
        }
    }
    for(int i=0;i<neg.length();i++){
        if(neg.at(i).contains(">,"))
            neg[i].replace(">","<=");
        else if(neg.at(i).contains(">=,"))
            neg[i].replace(">=","<");
        else if(neg.at(i).contains("<,"))
            neg[i].replace("<",">=");
        else if(neg.at(i).contains("<=,"))
            neg[i].replace("<=",">");
        else if(neg.at(i).contains("==,"))
            neg[i].replace("==","!=");
    }

    if(op_counter==1){
        if(code.contains("CONST,")){
            QString var;
            int number=0;
            //int const
            int flag = 0;
            if(neg.at(0).contains("IDN,")&&neg.at(2).contains("CONST,")){
                var = neg.at(0).split(",")[1];
                number = neg.at(2).split(",")[1].toInt();
                flag = 0;
            }
            else if(neg.at(0).contains("CONST,")&&neg.at(2).contains("IDN,")){
                var = neg.at(2).split(",")[1];
                number = neg.at(0).split(",")[1].toInt();
                flag = 2;
            }
            else{
                qDebug()<<"error:operator not between factors!";
                return var_interval;
            }

            if(flag ==2){ // IDN put in front
                QString swap = neg.at(2);
                neg[2] = neg.at(0);
                neg[0] = swap;

                for(int j=0;j<neg.length();j++){
                    if(neg.at(j).contains(">,"))
                        neg[j].replace(">","<");
                    else if(neg.at(j).contains(">=,"))
                        neg[j].replace(">=","<=");
                    else if(neg.at(j).contains("<,"))
                        neg[j].replace("<",">");
                    else if(neg.at(j).contains("<=,"))
                        neg[j].replace("<=",">=");
                }
            }

            //qDebug()<<neg.at(flag)<<"111111";
            VarInterval const_number = Arithmetic::op_range2interval(number,number);

            if(neg.at(1)=="!=,_"){
                //should not happened, if != we should negate
            }
            else if(neg.at(1)=="!=,_"){
//                var_interval[var] = Arithmetic::op_intersection(var_interval[var], const_number);

//                if(var_interval[var].bottom==true){//all set to bottom
//                    QStringList variables =  var_interval.keys();
//                    for(int v=0;v<variables.length();v++){
//                        var_interval[variables.at(v)]= VarInterval::create_bottom();
//                    }
//                }
//                else{
//                    return var_interval;
//                }
                qDebug()<<"error:at neg '==' we cannot represent separated set!";
                return var_interval;
            }
            else if(neg.at(1)==">,_"||neg.at(1)==">=,_"){
                if(neg.at(1)==">,_")
                    const_number = Arithmetic::op_add(const_number, 1);
                const_number.uINF=true;
                var_interval[var] = Arithmetic::op_intersection(var_interval[var], const_number);

                if(var_interval[var].bottom==true){//all set to bottom
                    QStringList variables = var_interval.keys();
                    for(int v=0;v<variables.length();v++){
                        var_interval[variables.at(v)]= VarInterval::create_bottom();
                    }
                }
                else{
                    return var_interval;
                }
            }
            else if(neg.at(1)=="<,_"||neg.at(1)=="<=,_"){
                if(neg.at(1)=="<,_")
                    const_number = Arithmetic::op_add(const_number, -1);

                //qDebug()<<"2222222"<<const_number.c;
                const_number.lINF=true;
               // qDebug()<<"2222222"<<Arithmetic::op_lower(var_interval[var]);

                var_interval[var] = Arithmetic::op_intersection(var_interval[var], const_number);

                if(var_interval[var].bottom==true){//all set to bottom
                    QStringList variables = var_interval.keys();
                    for(int v=0;v<variables.length();v++){
                        var_interval[variables.at(v)]= VarInterval::create_bottom();
                    }

                }
                else{
                    return var_interval;
                }
            }
        }
        else{
            qDebug()<<"error: we should not allowed pure variables guard!";
            return var_interval;
        }
    }

    else if(op_counter==2){

        //qDebug()<<neg.at(0)<<" "<<neg.at(1)<<" "<<neg.at(2)<<" "<<neg.at(3)<<" "<<neg.at(4);
        if(neg.at(0).contains("CONST,")&&neg.at(4).contains("CONST,")){
            if(neg.at(2).contains("IDN,")){
                QString var;
                int numberleft=0;
                int numberright=0;
                var = neg.at(2).split(",")[1];
                numberleft = neg.at(0).split(",")[1].toInt();
                numberright = neg.at(4).split(",")[1].toInt();
                VarInterval const_number_left;
                VarInterval const_number_right;
                if(neg.at(1)==">=,_" || neg.at(1)==">,_"){
                    if(neg.at(1)==">,_")
                        numberleft--;
                    if(neg.at(3)==">,_")
                        numberright++;
                    const_number_left = Arithmetic::op_range2interval(numberleft,numberleft);
                    const_number_left.lINF=true;
                    const_number_right = Arithmetic::op_range2interval(numberright,numberright);
                    const_number_right.uINF=true;
                }
                else if(neg.at(1)=="<=,_" || neg.at(1)=="<,_"){
                    if(neg.at(1)=="<,_")
                        numberleft++;
                    if(neg.at(3)=="<,_")
                        numberright--;
                    const_number_left = Arithmetic::op_range2interval(numberleft,numberleft);
                    const_number_left.uINF=true;
                    const_number_right = Arithmetic::op_range2interval(numberright,numberright);
                    const_number_right.lINF=true;
                    //qDebug()<<",,,,,,"<< Arithmetic::op_upper(const_number);
                }
                else{
                    qDebug()<<"error:eval neg error at 2 operators(1)";
                }
                //qDebug()<<Arithmetic::op_upper(const_number_left)<<",left:"<<Arithmetic::op_lower(const_number_left);
                //qDebug()<<Arithmetic::op_upper(const_number_right)<<",right:"<<Arithmetic::op_lower(const_number_right);
                //qDebug()<<numberleft<<","<<numberright;
               //qDebug()<<Arithmetic::op_upper(var_interval["i"])<<",original:"<<Arithmetic::op_lower(var_interval["i"]);
                VarInterval left_union = Arithmetic::op_intersection(var_interval[var], const_number_left);
                VarInterval right_union = Arithmetic::op_intersection(var_interval[var], const_number_right);
                //qDebug()<<Arithmetic::op_upper(left_union)<<",left:"<<Arithmetic::op_lower(left_union);
               // qDebug()<<Arithmetic::op_upper(right_union)<<",right:"<<Arithmetic::op_lower(right_union);

                var_interval[var] = Arithmetic::op_union(left_union, right_union);
               //qDebug()<<"TTTTTT"<<code<<var_interval[var].bottom;

               // qDebug()<<"1111111"<<Arithmetic::op_lower(var_interval["i"])<<","<<Arithmetic::op_upper(var_interval["i"]);

                if(var_interval[var].bottom==true){//all set to bottom
                    QStringList variables = var_interval.keys();

                    for(int v=0;v<variables.length();v++){
                        var_interval[variables.at(v)]= VarInterval::create_bottom();
                    }
                    //qDebug()<<"TTTTTT"<<code<<var_interval[var].bottom;
                    return var_interval;
                }
                else{
                    return var_interval;
                }
            }
            else{
                qDebug()<<"error:eval neg error at 2 operators(2)";
            }
        }
        else{
            qDebug()<<"error:eval neg error at 2 operators(3)";
        }
    }
    else{
        qDebug()<<"error: wrong boolean expression! too much operators";
    }
    //qDebug()<<"jump neg";
    return var_interval;

}

VarInterval EdgesEffects::eval_expression(QString expr, QMap<QString, VarInterval> var_interval){

    QStringList factors = expr.split(">");
    //qDebug()<<"[[[[[[[[["<<expr<<"::"<<factors.length();
    if(factors.length()==2){
        if(factors.at(0).contains("<CONST,")){

            int number = 0;
            number = factors.at(0).split(",")[1].toInt();
            //qDebug()<<"[[[[[[[[["<<expr<<"::"<<number;
            return Arithmetic::op_range2interval(number, number);
        }
        else if(factors.at(0).contains("<IDN,")){
            QString sss = factors.at(0).split(",")[1];
            return var_interval[sss];
        }
    }
    QList<VarInterval> subs;
    //VarInterval sub2 = VarInterval();
    QString var;
    int number = 0;
    QList<QString> operators;
    for(int i=0;i<factors.length();i++){
        if(factors.at(i).contains("<IDN,")){
            var = factors.at(i).split(",")[1];
            subs.append(var_interval[var]);
        }
        else if(factors.at(i).contains("<CONST,")){
            number = factors.at(i).split(",")[1].toInt();
            subs.append(Arithmetic::op_range2interval(number, number));
        }
        else{
            if(factors.at(i).contains("<+,"))
                operators.append("+");
            else if(factors.at(i).contains("<-,"))
                operators.append("-");
            else if(factors.at(i).contains("<*,"))
                operators.append("*");
            else if(factors.at(i).contains("</,"))
                operators.append("/");
        }
    }
    VarInterval subexpr;
    for(int i=0;i<operators.length();i++){
        if(i==0){
            if(operators.at(i)=="+")
                subexpr = Arithmetic::op_add(subs.at(0),subs.at(1));
            else if(operators.at(i)=="-"){
                VarInterval s = Arithmetic::op_neg(subs.at(1));
                subexpr = Arithmetic::op_add(subs.at(0), s);
            }
            else if(operators.at(i)=="*")
                subexpr = Arithmetic::op_multiply(subs.at(0),subs.at(1));
            else if(operators.at(i)=="/"){
                //subexpr = Arithmetic::op_divide(subs.at(0),subs.at(1));
            }

        }
        else{
            if(operators.at(i)=="+")
                subexpr = Arithmetic::op_add(subexpr,subs.at(i+1));
            else if(operators.at(i)=="-"){
                VarInterval s = Arithmetic::op_neg(subs.at(i+1));
                subexpr = Arithmetic::op_add(subexpr, s);
            }
            else if(operators.at(i)=="*")
                subexpr = Arithmetic::op_multiply(subexpr,subs.at(i+1));
            else if(operators.at(i)=="/"){
                //subexpr = Arithmetic::op_divide(subexpr,subs.at(i+1));
            }

        }
    }
    return subexpr;
}
