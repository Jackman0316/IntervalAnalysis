#include "lexicalanalysis.h"
#include <QString>
#include <iostream>
using namespace std;
LexicalAnalysis::LexicalAnalysis()
{
    codelist.append("");
}

int LexicalAnalysis::judge(QString pickone)
{
    //字母、下划线  以及冲突判断
    if((state==4&&pickone.at(0)!='*')||(state==5&&pickone.at(0)!='/'))
        return 6;
    else if(('A'<=pickone.at(0)&&pickone.at(0)<='D')||('F'<=pickone.at(0)&&pickone.at(0)<='Z')||('g'<=pickone.at(0)&&pickone.at(0)<='w')||
            ('y'<=pickone.at(0)&&pickone.at(0)<='z')||pickone.at(0)=='_')
        return 0;
    else if('a'<=pickone.at(0)&&pickone.at(0)<='f')
    {
        if(state==9)
            return 11;
        else if(state==10)
            return 12;
        else
            return 0;
    }
    else if(pickone.at(0)=='x')
    {
        if(state==7)
            return 10;
        else
            return 0;
    }
    else if(pickone.at(0)=='E')
    {
        if(state==11||state==13)
            return 15;
        else
            return 0;
    }

    //空格
    else if(pickone.at(0)==' '||pickone.at(0)=='\t')
        return 999;//空格 制表符 返回 999
    else if(pickone.at(0)=='\n')
    {

        return 999; //换行符 返回999
    }

    else if(pickone.at(0)>='0'&&pickone.at(0)<='9')
    {
        if(state==0)
        {
            if(pickone.at(0)=='0')
                return 7;
            else if(pickone.at(0)<='9'&&pickone.at(0)>='1')
                return 13;
        }
        else if(state==7&&(pickone.at(0)>='1'&&pickone.at(0)<='7'))
            return 8;
        else if(state==7&&(pickone.at(0)=='0'||pickone.at(0)<='8'||pickone.at(0)<='9'))
            return 777; //此时必然出现8进制数错误 返回777 交给wrong处理
        else if(state==8&&(pickone.at(0)>='0'&&pickone.at(0)<='7'))
            return 9;
        else if(state==9&&(pickone.at(0)>='1'&&pickone.at(0)<='9'))
            return 11;
        else if(state==10&&(pickone.at(0)>='0'&&pickone.at(0)<='9'))
            return 12;
        else
            return 1;

    }


    //运算符以及冲突判断
    else if(                    pickone.at(0)=='='||
            pickone.at(0)=='%'||pickone.at(0)=='!'||pickone.at(0)=='>'||pickone.at(0)=='<'||pickone.at(0)=='^'||
            pickone.at(0)=='|'||pickone.at(0)=='&')
        return 2;
    else if(pickone.at(0)=='/')
    {
        if((pickone.length()==2&&pickone.at(1)=='*')||state==5)
            return 4;
        else
            return 2;
    }
    else if(pickone.at(0)=='*')
    {
        if(state==3||state==4||state==5)
            return 5;
        else
            return 2;
    }
    else if(pickone.at(0)=='+')
    {
        if(state==14)
            return 16;
        else
            return 2;
    }
    else if(pickone.at(0)=='-')
    {
        if(state==14)
            return 17;
        else
            return 2;
    }

    //界符及冲突判断
    else if(pickone.at(0)=='{'||pickone.at(0)=='}'||pickone.at(0)=='['||pickone.at(0)==']'||pickone.at(0)=='('||
            pickone.at(0)==')'||pickone.at(0)==','||pickone.at(0)==';'||pickone.at(0)==':'||pickone.at(0)=='?'||
            pickone.at(0)=='~')
        return 3;
    else if(pickone.at(0)=='.')
    {
        if(cur!="")
            return 14;
        else
            return 3;
    }
    else if(pickone.at(0)=='\'')
        return 18;
    else
        return 888; // 当前字符是个非法字符

}

int LexicalAnalysis::GO(int row, int col, QString pickone)
{
    if(col==888)
    {
        if(final[state]==1)
        {
            output();
        }
        else
        {
            if(cur!="")
            wrong();

        }
        cur=cur+pickone.at(0);
        wrong(1);

    }
    else if(col==777)
    {
        cur=cur+pickone.at(0);
        wrong(2);
    }
    else if(pickone.length()==2)
    {
        if(col!=999)
        {
            if(form[row][col]!=0)
            {
                state=form[row][col];
                cur=cur+pickone.at(0);
            }
            else if(form[row][col]==0)
            {
                if(final[state]==1)
                {
                        output();
                        col = judge(pickone);
                        GO(state,col,pickone);
                }
                else
                {
                   // qDebug()<<"not match1";
                  wrong();
                  col = judge(pickone);
                  GO(state,col,pickone);
                }
            }

        }
        else if(col==999&&cur!="")
        {
            if(final[state]==1)
            {
                output();

            }
            else
            {
                // qDebug()<<"wrong space position1";
                wrong();

            }
            if(pickone.at(0)=='\n'){
                hang++;
                codelist.append("");
            }
        }
        else
            if(pickone.at(0)=='\n'){
                hang++;
                codelist.append("");
            }
    }
    else
    {
        if(col!=999)
        {
            if(form[row][col]!=0)
            {
                state=form[row][col];
                cur=cur+pickone.at(0);
                if(final[state]==1)
                    output();
                else
                {
                  //  qDebug()<<"program havent finished!";
                   wrong();

                }
            }
            else if(form[row][col]==0)
            {
                if(final[state]==1)
                {
                    output();
                    col = judge(pickone);
                    GO(state,col,pickone);

                }
                else
                {
                   // qDebug()<<"not match2";
                  wrong();

                }
            }
        }
        else if(col==999&&cur!="")
        {
            if(final[state]==1)
            {
                output();

            }
            else
            {
               // qDebug()<<"wrong space position2";
                wrong();

            }
            if(pickone.at(0)=='\n'){
                hang++;
                codelist.append("");
            }
        }

    }
}

int LexicalAnalysis::keyword(QString str)
{
    for(int i=0;i<26;i++)
    {
        if(keyword_all[i]==str)
            return 1;

    }
    return 0;
}

void LexicalAnalysis::output()
{
    //cout<<hang<<endl;

    int now = tableWidget_lexical->rowCount();//获取当前行数
    tableWidget_lexical->setRowCount(now+ 1);//添加一行
    switch(state)
    {
    case 1:
        tableWidget_lexical->setItem(now,0,new QTableWidgetItem(cur));
        if(keyword(cur)==1)
        {
            tableWidget_lexical->setItem(now,1,new QTableWidgetItem(QString("<%1,_>").arg(cur)));

            codelist[hang]=codelist[hang]+QString("<%1,_>").arg(cur);
            tableWidget_lexical->setItem(now,2,new QTableWidgetItem("Keyword"));
        }
        else
        {
            tableWidget_lexical->setItem(now,1,new QTableWidgetItem(QString("<IDN,%1>").arg(cur)));
            codelist[hang]=codelist[hang]+QString("<IDN,%1>").arg(cur);
            tableWidget_lexical->setItem(now,2,new QTableWidgetItem("Identifier"));
        }
        break;
    case 2:
        tableWidget_lexical->setItem(now,0,new QTableWidgetItem(cur));
        tableWidget_lexical->setItem(now,1,new QTableWidgetItem(QString("<%1,_>").arg(cur)));
        codelist[hang]=codelist[hang]+QString("<%1,_>").arg(cur);
        tableWidget_lexical->setItem(now,2,new QTableWidgetItem("Boundary"));
        break;
    case 6:
        tableWidget_lexical->setItem(now,0,new QTableWidgetItem("/**/"));
        tableWidget_lexical->setItem(now,1,new QTableWidgetItem(QString("<NOTE,_>")));
        codelist[hang]=codelist[hang]+QString("<NOTE,_>");
        tableWidget_lexical->setItem(now,2,new QTableWidgetItem("Annotation"));
        break;
    case 7:
        tableWidget_lexical->setItem(now,0,new QTableWidgetItem(cur));
        tableWidget_lexical->setItem(now,1,new QTableWidgetItem(QString("<CONST,%1>").arg(cur)));
        codelist[hang]=codelist[hang]+QString("<CONST,%1>").arg(cur);
        tableWidget_lexical->setItem(now,2,new QTableWidgetItem("Const"));
        break;
    case 11:
    case 13:
    case 16:
        tableWidget_lexical->setItem(now,0,new QTableWidgetItem(cur));
        tableWidget_lexical->setItem(now,1,new QTableWidgetItem(QString("<CONST,%1>").arg(cur)));
        codelist[hang]=codelist[hang]+QString("<CONST,%1>").arg(cur);
        tableWidget_lexical->setItem(now,2,new QTableWidgetItem("Const"));
        break;
    case 8:
        tableWidget_lexical->setItem(now,0,new QTableWidgetItem(cur));
        tableWidget_lexical->setItem(now,1,new QTableWidgetItem(QString("<CONST,%1>").arg(cur)));
        codelist[hang]=codelist[hang]+QString("<CONST,%1>").arg(cur);
        tableWidget_lexical->setItem(now,2,new QTableWidgetItem("Octal number"));
        break;
    case 10:
        tableWidget_lexical->setItem(now,0,new QTableWidgetItem(cur));
        tableWidget_lexical->setItem(now,1,new QTableWidgetItem(QString("<CONST,%1>").arg(cur)));
        codelist[hang]=codelist[hang]+QString("<CONST,%1>").arg(cur);
        tableWidget_lexical->setItem(now,2,new QTableWidgetItem("hexadecimal number"));
        break;
    case 19:
        tableWidget_lexical->setItem(now,0,new QTableWidgetItem(cur));
        tableWidget_lexical->setItem(now,1,new QTableWidgetItem(QString("<%1,_>").arg(cur)));
        codelist[hang]=codelist[hang]+QString("<%1,_>").arg(cur);
        tableWidget_lexical->setItem(now,2,new QTableWidgetItem("Char"));
        break;

    case 20:
    case 21:
        tableWidget_lexical->setItem(now,0,new QTableWidgetItem(cur));
        tableWidget_lexical->setItem(now,1,new QTableWidgetItem(QString("<%1,_>").arg(cur)));
        codelist[hang]=codelist[hang]+QString("<%1,_>").arg(cur);
        tableWidget_lexical->setItem(now,2,new QTableWidgetItem("Operator"));
        break;
    }
    tableWidget_lexical->setItem(now,3,new QTableWidgetItem(QString("%1").arg(hang)));
    cur="";
    state=0;
}

void LexicalAnalysis::wrong(int num)
{

    int now = tableWidget_error->rowCount();//获取当前行数
    tableWidget_error->setRowCount(now+1);//添加一行
    if(num==0)
    {
        switch(state)
        {
        case 17:
        case 18:
            tableWidget_error->setItem(now,0,new QTableWidgetItem(cur));
            tableWidget_error->setItem(now,1,new QTableWidgetItem("错误的字符常量"));
            tableWidget_error->setItem(now,2,new QTableWidgetItem(QString("%1").arg(hang)));
             break;
        case 4:
        case 5:
            tableWidget_error->setItem(now,0,new QTableWidgetItem(cur));
            tableWidget_error->setItem(now,1,new QTableWidgetItem("注释未封闭"));
            tableWidget_error->setItem(now,2,new QTableWidgetItem(QString("%1").arg(hang)));
             break;
//        case 7:
//            tableWidget_error->setItem(now,0,new QTableWidgetItem(cur));
//            tableWidget_error->setItem(now,1,new QTableWidgetItem("错误的8进制数"));
//            tableWidget_error->setItem(now,2,new QTableWidgetItem(QString("%1").arg(hang)));
//            break;
        case 9:
            tableWidget_error->setItem(now,0,new QTableWidgetItem(cur));
            tableWidget_error->setItem(now,1,new QTableWidgetItem("错误的16进制数"));
            tableWidget_error->setItem(now,2,new QTableWidgetItem(QString("%1").arg(hang)));
            break;

        case 12:
        case 14:
        case 15:
            tableWidget_error->setItem(now,0,new QTableWidgetItem(cur));
            tableWidget_error->setItem(now,1,new QTableWidgetItem("错误常数"));
            tableWidget_error->setItem(now,2,new QTableWidgetItem(QString("%1").arg(hang)));
            break;

        }
    }
    else if(num==1)
    {
        tableWidget_error->setItem(now,0,new QTableWidgetItem(cur));
        tableWidget_error->setItem(now,1,new QTableWidgetItem("非法字符"));
        tableWidget_error->setItem(now,2,new QTableWidgetItem(QString("%1").arg(hang)));
    }
    else if(num ==2)
    {
        tableWidget_error->setItem(now,0,new QTableWidgetItem(cur));
        tableWidget_error->setItem(now,1,new QTableWidgetItem("错误的8进制数"));
        tableWidget_error->setItem(now,2,new QTableWidgetItem(QString("%1").arg(hang)));
    }
    cur="";
    state=0;
}
