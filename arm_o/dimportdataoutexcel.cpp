#include "dimportdataoutexcel.h"
#include "ui_d_importdata.h"
#include "mainwindow.h"

#include <QThread>

MainWindow *m_mainWindow;
//=========================================================
DImportDataOutExcel::DImportDataOutExcel(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::D_ImportDataOutExel)
{
    ui->setupUi(this);
    m_mainWindow = reinterpret_cast<MainWindow*>(parent);
}

//=========================================================
DImportDataOutExcel::~DImportDataOutExcel()
{
}

//=========================================================
void DImportDataOutExcel::setText(QString text)
{
    ui->listWidget->addItem(text);
    ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
    ui->listWidget->setFocus();

//    QThread::msleep(5);
//    qApp->processEvents();
}

//=========================================================
void DImportDataOutExcel::setProgress(int progress)
{
    ui->progressBar->setValue(progress);
}

//=========================================================
void DImportDataOutExcel::clearTextInWindow()
{
    ui->listWidget->clear();
}

//=========================================================
void DImportDataOutExcel::slotExit()
{
    close();
}
