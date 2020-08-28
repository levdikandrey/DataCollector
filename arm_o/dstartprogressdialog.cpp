#include "dstartprogressdialog.h"

#include <QDebug>
#include <QThread>
#include <QCoreApplication>
#include <unistd.h>

//=========================================================
DStartProgressDialog::DStartProgressDialog(QObject *parent): QObject(parent)
{
}

//=========================================================
DStartProgressDialog::~DStartProgressDialog()
{
}

//=========================================================
//void DStartProgressDialog::resultReadyStartProgressDialog(const QString &)
//{
//}

//=========================================================
void DStartProgressDialog::doWork(const QString &)
{
    qDebug()<<__PRETTY_FUNCTION__;
//    QProgressDialog progress();
//    progress.setWindowModality(Qt::WindowModal);
//    QProgressDialog pd("Загрузка АРМ-О","Прервать загрузку",0,0,this,Qt::WindowFlags());
    QProgressDialog *pd = new QProgressDialog();
    pd->setWindowModality(Qt::WindowModal);
    pd->setRange(0,100);
//    pd->show();
    int i=0;
    qDebug()<<__PRETTY_FUNCTION__<<"1";
    for(int i=0; i<100; i++)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"2";
        pd->setValue(i);
        QCoreApplication::processEvents();
//        i++;
        sleep(1);
    }
}
