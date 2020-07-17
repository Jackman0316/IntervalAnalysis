#include "constraintsystem.h"
#include <QDebug>
#include "varinterval.h"
#include "consystemitem.h"
#include "arithmetic.h"
ConstraintSystem::ConstraintSystem()
{

}

QList<QString> ConstraintSystem::extract_var(){
    QString pattern("<IDN,*?>");
    QRegExp rx(pattern);
    QStringList temp;
    for(int i=0;i<codelist.length();i++)
    {
        temp = codelist.at(i).split(">");
        for(int j=0;j<temp.length();j++){
            if(temp.at(j).left(5)=="<IDN,"&&((j+1>=temp.length())||(temp.at(j+1).left(4)!="<[,_"))){
                if(!temp.at(j).split(",").at(1).left(1).contains(QRegExp("[A-Z]")))
                    vars.append(temp.at(j).split(",").at(1));
                //qDebug()<<vars.last();
            }

        }
    }
    vars.removeDuplicates();
    //qDebug()<<vars.length();
    return vars;
}

QList<ConsystemItem> ConstraintSystem::extract_consystem(){
    //qDebug()<<VarInterval().create_bottom().bottom;
    QMap<QString, VarInterval> var_init;
    //QMap<QString, QMap<QString, VarInterval>> contemp;

    for(int i=0;i<vars.length();i++){
       var_init.insert(vars.at(i), VarInterval::create_bottom());
    }

    for(int i=0;i<codelist.length();i++){ //traverse codelist to generete

//        if(!operation.isEmpty())
//            qDebug()<<operation.top();
        if(i==0){
            ConsystemItem conItem = ConsystemItem(consystem.size(), "start", codelist.at(i), var_init);
            consystem.append(conItem);
        }
        else if(codelist.at(i).contains("<while,_>")){
            ConsystemItem conItem = ConsystemItem(consystem.size(), QString::number(i-1), codelist.at(i), var_init);
            consystem.append(conItem);
            operation.push(QString("}@while_unionjump@%1").arg(consystem.size()-1));  //jump@1 : jump from 1 to a "}"
            //operation.push(QString("}@while_union@%1").arg(consystem.size())); //union@1 : 1 is waiting for union from a "}"

        }
        else if(codelist.at(i).contains("<if,_>")){
            ConsystemItem conItem = ConsystemItem(consystem.size(), QString::number(i-1), codelist.at(i), var_init);
            consystem.append(conItem);
            int if_flag = i;
            int else_flag = i;
            if(i+1<codelist.length())
            {
                for(int j=i+1;j<codelist.length();j++){
                    if(codelist.at(j).contains("<if,_>"))
                    {
                        if_flag = j;
                        break;
                    }
                }

                for(int j=i+1;j<codelist.length();j++){
                    if(codelist.at(j).contains("<else,_>"))
                    {
                        else_flag = j;
                        break;
                    }
                }
            }
            else{
                qDebug()<<"error:the last statement is 'if'!";
            }

            if((else_flag<if_flag && else_flag>i) || (else_flag>i&& if_flag==i)){

                operation.push(QString("else@if_jump@%1").arg(consystem.size()-1));  // If there is an "else" before next "if", the "else"
                                                                                    //should be jumped from current "if"
            }
            else if(else_flag==i||else_flag>if_flag){
                operation.push(QString("}@if_unionjump@%1").arg(consystem.size()-1));// If there is no "else" before next "if", or no "else" at all.
                                                     // a } should be jumped from current "if", and union with statement before }
            }
        }
        else if(codelist.at(i).contains("<{,_>")){
            // do nothing
        }
        else if(codelist.at(i).contains("<},_>")){
            //qDebug()<<operation.top();
            QStringList op = operation.top().split("@");
            if(op.at(0)=="}"){
                if(i+1<codelist.length())
                {
                    if(op.at(1)=="while_unionjump"){
                        int while_from = op.at(2).toInt();

                        qDebug()<<"while from:"<<consystem.size();
                        int loop_last=0;
                        for(int j = consystem.size()-1; j>0; j--)
                        {
                            if(consystem.at(j).code=="<else,_>"){
                                loop_last++;   //stupidly remove 'else', which is added by ourself
                            }

                        }
                        consystem[while_from].consystem = consystem[while_from].consystem +"union" + QString::number((consystem.size()-1-loop_last));

                        ConsystemItem conItem = ConsystemItem(consystem.size(), "neg@"+QString::number(while_from), codelist.at(i+1), var_init);
                        consystem.append(conItem);


                        i++;
                        operation.pop();
                    }
                    else if(op.at(1)=="if_unionjump"){
                        int if_from = op.at(2).toInt();
                        ConsystemItem conItem = ConsystemItem(consystem.size(),
                                                              "neg@"+QString::number(if_from)+"union"+QString::number(consystem.size()-1),
                                                              codelist.at(i+1), var_init);
                        consystem.append(conItem);
                        i++;
                        operation.pop();
                    }
                }
                else{
                    qDebug()<<"error:the last statement is '}'!";
                }

            }
            else if(op.at(0)=="else"){
                qDebug()<<"state: Ignore this '}', I need 'else'.";
                //operation.pop();
                //do nothing, just ignore this '}', because we are waiting for an 'else'

            }
            else{
                qDebug()<<"error: operation stack is broken! not 'else', not '}'";
            }

        }
        else if(codelist.at(i).contains("<else,_>")){
            QStringList op = operation.top().split("@");
            if(op.at(0)=="else"){
                if(i+1<codelist.length()){
                    if(op.at(1)=="if_jump"){
                        int if_from = op.at(2).toInt();
                        ConsystemItem conItem = ConsystemItem(consystem.size(),
                                                              "neg@"+QString::number(if_from), codelist.at(i), var_init);
                        consystem.append(conItem);
                        operation.pop();

                    }
                }
                else{
                    qDebug()<<"error:the last statement is 'else'!";
                }
            }
            else{
                qDebug()<<"error: operation stack is broken! cannot match a else!";
            }
        }
        else{
            ConsystemItem conItem = ConsystemItem(consystem.size(), QString::number(i-1), codelist.at(i), var_init);
            consystem.append(conItem);
        }
    }

    for(int i=0;i<consystem.length();i++)
        qDebug()<<consystem.at(i).pp<<":"<<consystem.at(i).consystem<<":"<<consystem.at(i).code;
    //qDebug()<<"aaaa";
    return consystem;
}

bool ConstraintSystem::conequal(QList<ConsystemItem> consystem_new, QList<ConsystemItem> consystem_old, QList<QString> vars){
    if(consystem_new.length()!=consystem_old.length()){
        qDebug()<<"error:consystems length are not equal!";
        return false;
    }
    else{
        bool flag = true;
        for(int i=0;i<consystem_new.length();i++){
            for(int v=0;v<vars.length();v++){
                float new_upper = Arithmetic::op_upper(consystem_new.at(i).var_interval[vars.at(v)]);
                float new_lower = Arithmetic::op_lower(consystem_new.at(i).var_interval[vars.at(v)]);
                float old_upper = Arithmetic::op_upper(consystem_old.at(i).var_interval[vars.at(v)]);
                float old_lower = Arithmetic::op_lower(consystem_old.at(i).var_interval[vars.at(v)]);
                if(new_upper==old_upper&&new_lower==old_lower){
                    // do nothing;
                }
                else{
                    flag = false;
                }
            }
        }
        return flag;
    }

}
