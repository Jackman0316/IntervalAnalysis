#ifndef LEXICALANALYSIS_H
#define LEXICALANALYSIS_H
#include <QString>
#include "mainwindow.h"
#include <QTableWidget>
class LexicalAnalysis
{
public:
    LexicalAnalysis();
    int judge(QString pickone);
    int GO(int row, int col, QString pickone);
    int keyword(QString str);
    void output();
    QList<QString> codelist;
    void wrong(int num=0);

    short form[22][19]={{1,0,20,2,3,0,0,7,0,0,0,0,0,11,0,0,0,0,17},
                         {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,5,4,0,0,0,0,0,0,0,0,0,0,0,0},
                         {0,0,0,0,6,5,4,0,0,0,0,0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0,8,0,9,0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0,0,8,0,0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0},
                         {0,11,0,0,0,0,0,0,0,0,0,0,0,0,12,14,0,0,0},
                         {0,13,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                         {0,13,0,0,0,0,0,0,0,0,0,0,0,0,0,14,0,0,0},
                         {0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,15,0},
                         {0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                         {0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                         {18,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,19},
                         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                         {0,0,21,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
    int final[22]={0,1,1,0,0,0,1,1,1,0,1,1,0,1,0,0,1,0,0,1,1,1};
    int state=0;
    int symbol=0;
    QString cur="";
    QString con;
    int hang=0;
    QString keyword_all[26]={
        "main","if","then","while","do","static","defualt","do",
        "int","double","struct","break","else","long","swtich",
        "case","typedf","char","return","const","float","short",
        "continue","for","void","sizeof"};

    QTableWidget* tableWidget_error;
    QTableWidget* tableWidget_lexical;
};

#endif // LEXICALANALYSIS_H
