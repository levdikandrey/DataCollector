#include "denter.h"
#include "ui_d_enter.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QTableWidget>
#include <QDebug>
#include <QMessageBox>

extern QSqlDatabase db;
extern QString currentUserName;
//=========================================================
DEnter::DEnter(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::D_Enter)
{
    ui->setupUi(this);

//    db = QSqlDatabase::addDatabase("QPSQL");
//    db.setHostName("127.0.0.1");
//    db.setDatabaseName("avpDB");
//    db.setUserName("postgres");
//    db.setPassword("postgres");

    query = new QSqlQuery(db);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    initUserComBoBox();
}

//=========================================================
QString DEnter::currentUserName1()
{
    return ui->comboBoxUsers->currentText();
}

//=========================================================
void DEnter::initUserComBoBox()
{
    QString sql="";
    ui->comboBoxUsers->clear();
    try
    {
        sql = "SELECT * FROM \"User\";";
        if(query->exec(sql))
        {
            while(query->next())
            {
                ui->comboBoxUsers->addItem(query->value(1).toString());
            }
        }
        else
            qDebug()<<query->lastError().text();
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
void DEnter::slotEnter()
{
    currentUserName = ui->comboBoxUsers->currentText();
    accept();
}

//=========================================================
void DEnter::slotCancel()
{
    reject();
}
