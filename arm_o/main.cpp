#include "mainwindow.h"

#include <QDebug>
#include <QApplication>
#include "xlsxdocument.h"
#include <QSqlDatabase>
#include <iostream>
#include "denter.h"

QSqlDatabase db;
QString currentUserName;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    std::cout<<"Start ARM-O"<<std::endl;
//    std::cerr<<"Start ARM-O"<<std::endl;
    MainWindow w;
    DEnter dEnter;
    if(dEnter.exec()==QDialog::Accepted)
    {
        w.showMaximized();
        w.initDialogAccess(currentUserName);
        return a.exec();
    }
    else
        exit(0);
}
