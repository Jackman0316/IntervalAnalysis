#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "lexicalanalysis.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private slots:
    void on_pushButton_clicked();

    void on_pushButton_test_clicked();

    void on_pushButton_lexical_clicked();



    void on_pushButton_narrowing_clicked();

    void on_pushButton_plain_clicked();

private:
    //LexicalAnalysis* m_LexicalAnalysis;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
