#include "mainwindow.h"

#include <QDebug>
#include <QApplication>
#include <QSqlDatabase>
#include <QSettings>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QMessageBox>

#include <iostream>

#include "xlsxdocument.h"
#include "denter.h"

QSqlDatabase db;
QString currentUserName;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile fileSettings(QDir::toNativeSeparators(QApplication::applicationDirPath()) + "/settings.ini");
    if(!fileSettings.exists())
    {
        QMessageBox::warning(0,"Внимание","Нет файла конфигурации settings.ini!","Да");
        fileSettings.close();
        exit(0);
    }
    else
    {
       fileSettings.close();
       QSettings settings(QDir::toNativeSeparators(QApplication::applicationDirPath()) + "/settings.ini",QSettings::IniFormat);
//       qDebug()<<"settings.value(USER/NAME).toString()="<<settings.value("USER/NAME").toString();
       currentUserName = settings.value("USER/NAME").toString();
    }

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
