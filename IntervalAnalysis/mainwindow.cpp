#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "arithmetic.h"
#include "varinterval.h"
#include <iostream>
#include "lexicalanalysis.h"
#include <QDebug>
#include "constraintsystem.h"
#include "intervalanalysis.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget_old->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_error->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_old->verticalHeader()->setVisible(false);
    ui->tableWidget_interval->horizontalHeader()->setVisible(false);
    ui->tableWidget_interval->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    LexicalAnalysis* m_LexicalAnalysis;
    m_LexicalAnalysis = new LexicalAnalysis();
    m_LexicalAnalysis->tableWidget_lexical = ui->tableWidget_old;
    m_LexicalAnalysis->tableWidget_error = ui->tableWidget_error;
    ui->tableWidget_interval->setColumnCount(0);
    ui->tableWidget_old->setRowCount(0);
    ui->tableWidget_error->setRowCount(0);

    QString pickone="";
    QString text = ui->textEdit->toPlainText();
    for (int i = 0 ; i<text.length();i++)
        {
            pickone = text.at(i);

            if(i+1<text.length())
            pickone = pickone+text.at(i+1);


           int x =  m_LexicalAnalysis->judge(pickone);
           m_LexicalAnalysis->GO(m_LexicalAnalysis->state,x,pickone);
        }
    m_LexicalAnalysis->cur="";
//    for(int i=0; i<m_LexicalAnalysis->codelist.length();i++)
//       qDebug()<<m_LexicalAnalysis->codelist.at(i)<<endl;

    ConstraintSystem consystem;
    consystem.codelist = m_LexicalAnalysis->codelist;
    consystem.extract_var();
    qDebug()<<"ccc";
    consystem.extract_consystem();

   // qDebug()<<"bbb";

   // ui->tableWidget_interval->setColumnCount(2);
    IntervalAnalysis * go = new IntervalAnalysis();
    go->tableWidget_interval = ui->tableWidget_interval;
    go->consystem_old = consystem.consystem;

    go->vars = consystem.vars;
    go->Widening();

//    IntervalAnalysis  go;
//    go.tableWidget_interval = ui->tableWidget_interval;
//    go.consystem_old = consystem.consystem;
//    go.vars = consystem.vars;
//    go.Widening();

}

void MainWindow::on_pushButton_test_clicked()
{
   //  ui->tableWidget_interval->setColumnCount(2);
        Arithmetic ari;
        VarInterval a =  Arithmetic::op_range2interval(0,0);

        VarInterval b = Arithmetic::op_range2interval(1,2);
        //b.lINF=true;
        VarInterval x = Arithmetic::op_union(a,b);
        cout<<Arithmetic::op_lower(x)<<","<<Arithmetic::op_upper(x)<<endl;

}

void MainWindow::on_pushButton_lexical_clicked()
{
    LexicalAnalysis* m_LexicalAnalysis;
    m_LexicalAnalysis = new LexicalAnalysis();
    m_LexicalAnalysis->tableWidget_lexical = ui->tableWidget_old;
    m_LexicalAnalysis->tableWidget_error = ui->tableWidget_error;

    ui->tableWidget_old->setRowCount(0);
    ui->tableWidget_error->setRowCount(0);

    QString pickone="";
    QString text = ui->textEdit->toPlainText();
    for (int i = 0 ; i<text.length();i++)
        {
            pickone = text.at(i);

            if(i+1<text.length())
            pickone = pickone+text.at(i+1);


           int x =  m_LexicalAnalysis->judge(pickone);
           m_LexicalAnalysis->GO(m_LexicalAnalysis->state,x,pickone);
        }
    m_LexicalAnalysis->cur="";

}




void MainWindow::on_pushButton_narrowing_clicked()
{
    LexicalAnalysis* m_LexicalAnalysis;
    m_LexicalAnalysis = new LexicalAnalysis();
    m_LexicalAnalysis->tableWidget_lexical = ui->tableWidget_old;
    m_LexicalAnalysis->tableWidget_error = ui->tableWidget_error;
    ui->tableWidget_interval->setColumnCount(0);
    ui->tableWidget_old->setRowCount(0);
    ui->tableWidget_error->setRowCount(0);
    QString pickone="";
    QString text = ui->textEdit->toPlainText();
    for (int i = 0 ; i<text.length();i++)
        {
            pickone = text.at(i);

            if(i+1<text.length())
            pickone = pickone+text.at(i+1);


           int x =  m_LexicalAnalysis->judge(pickone);
           m_LexicalAnalysis->GO(m_LexicalAnalysis->state,x,pickone);
        }
    m_LexicalAnalysis->cur="";
//    for(int i=0; i<m_LexicalAnalysis->codelist.length();i++)
//       qDebug()<<m_LexicalAnalysis->codelist.at(i)<<endl;
    ConstraintSystem consystem;
    consystem.codelist = m_LexicalAnalysis->codelist;
    consystem.extract_var();
    consystem.extract_consystem();

    IntervalAnalysis * go = new IntervalAnalysis();
    go->tableWidget_interval = ui->tableWidget_interval;
    go->consystem_old = consystem.consystem;

    go->vars = consystem.vars;
    go->Widening();
    go->Narrowing();

//    IntervalAnalysis go = IntervalAnalysis();
//    go.consystem_old = consystem.consystem;
//    go.vars = consystem.vars;
//    //qDebug()<<"-------WIDENING-------";
//    go.Widening();
//    //qDebug()<<"-------NARROWING-------";
//    go.Narrowing();
}

void MainWindow::on_pushButton_plain_clicked()
{

    LexicalAnalysis* m_LexicalAnalysis;
    m_LexicalAnalysis = new LexicalAnalysis();
    m_LexicalAnalysis->tableWidget_lexical = ui->tableWidget_old;
    m_LexicalAnalysis->tableWidget_error = ui->tableWidget_error;
   // ui->tableWidget_interval->setRowCount(0);
    ui->tableWidget_interval->setColumnCount(0);
    ui->tableWidget_old->setRowCount(0);
    ui->tableWidget_error->setRowCount(0);

    QString pickone="";
    QString text = ui->textEdit->toPlainText();
    for (int i = 0 ; i<text.length();i++)
        {
            pickone = text.at(i);

            if(i+1<text.length())
            pickone = pickone+text.at(i+1);


           int x =  m_LexicalAnalysis->judge(pickone);
           m_LexicalAnalysis->GO(m_LexicalAnalysis->state,x,pickone);
        }
    m_LexicalAnalysis->cur="";
//    for(int i=0; i<m_LexicalAnalysis->codelist.length();i++)
//       qDebug()<<m_LexicalAnalysis->codelist.at(i)<<endl;

    ConstraintSystem consystem;
    consystem.codelist = m_LexicalAnalysis->codelist;
    consystem.extract_var();
    qDebug()<<"ccc";
    consystem.extract_consystem();

   // qDebug()<<"bbb";

   // ui->tableWidget_interval->setColumnCount(2);
    IntervalAnalysis * go = new IntervalAnalysis();
    go->tableWidget_interval = ui->tableWidget_interval;
    go->consystem_old = consystem.consystem;

    go->vars = consystem.vars;
    go->Plain();
}
