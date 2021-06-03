#include "daddavs.h"
#include "ui_d_addavs.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QTableWidget>
#include <QDebug>
#include <QMessageBox>


extern QSqlDatabase db;

//=========================================================
DAddAVS::DAddAVS(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DAddAVS)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    query = nullptr;
    query = new QSqlQuery(db);
}

//=========================================================
DAddAVS::~DAddAVS()
{
    delete ui;
}

//=========================================================
void DAddAVS::slotApply()
{
    accept();
}

//=========================================================
QString DAddAVS::getName()
{
    return ui->lineEditName->text();
}

//=========================================================
QString DAddAVS::getURL()
{
    return ui->lineEditURL_AVS->text();
}
