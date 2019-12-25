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
    DEnter *dEnter = new DEnter();
    if(dEnter->exec()!=QDialog::Accepted)
    {
        qDebug()<<"DEnter::slotEnter()";
        delete dEnter;
        return a.exec();
    }
    currentUserName = dEnter->currentUserName();
    MainWindow w;
    w.show();
    qDebug() << a.libraryPaths().join(";");
    return a.exec();
}
