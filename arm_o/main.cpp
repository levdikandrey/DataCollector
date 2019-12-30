#include "mainwindow.h"

#include <QDebug>
#include <QApplication>
#include "xlsxdocument.h"
#include <QSqlDatabase>
#include "denter.h"

QSqlDatabase db;
QString currentUserName;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    DEnter dEnter;
    if(dEnter.exec()==QDialog::Accepted)
    {
        w.showMaximized();
        return a.exec();
    }
    else
        exit(0);
}
