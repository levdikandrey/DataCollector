#include "dappointexpert.h"
#include "ui_d_appointexpert.h"
#include "mainwindow.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

extern QSqlDatabase db;
//=========================================================
DAppointExpert::DAppointExpert(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DAppointExpert)
{
    ui->setupUi(this);
    query = new QSqlQuery(db);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    initDialog();
}

//=========================================================
DAppointExpert::~DAppointExpert()
{
    delete ui;
}

//=========================================================
void DAppointExpert::initDialog()
{
    QString sql;
    sql = "select * from \"User\" WHERE \"ID_Group\" = 11 OR \"ID_Group\" = 12 OR \"ID_Group\" = 8;";
//  qDebug()<<"sql ="<<sql;
    if(!query->exec(sql))
    {
        qDebug()<<query->lastError().text();
        QMessageBox::warning(this, tr("Внимание"),query->lastError().text(),tr("Да"));
    }
    else
    {
        while(query->next())
        {
            ui->comboBoxExperts->addItem(query->value(1).toString());
        }
    }
}

//=========================================================
const QString DAppointExpert::getExpertName() const
{
    return ui->comboBoxExperts->currentText();
}

//=========================================================
QString DAppointExpert::getCommentExpert()
{
    return ui->textEdit->toPlainText();
}

//=========================================================
void DAppointExpert::slotApply()
{
    QModelIndexList selectedRows = ((MainWindow*)parent())->getTableCurrentExpert()->selectionModel()->selectedRows();
    if(selectedRows.empty())
    {
        QMessageBox::information(this, tr("Сообщение"),tr("Вы не выбрали задачу!"),tr("Да"));
        return;
    }
    accept();
}

//=========================================================
void DAppointExpert::slotCancel()
{
    reject();
}
