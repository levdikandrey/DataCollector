#include "dchangepassword.h"
#include "ui_d_changepassword.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

extern QSqlDatabase db;
extern QString currentUserName;
//=========================================================
DChangePassword::DChangePassword(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DChangePassword)
{
    ui->setupUi(this);
    query = new QSqlQuery(db);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

//=========================================================
DChangePassword::~DChangePassword()
{
    delete ui;
}

//=========================================================
void DChangePassword::clear()
{
    ui->lineEditOldPassword->clear();
    ui->lineEditNewPassword->clear();
    ui->lineEditRetryNewPassword->clear();
     ui->lineEditOldPassword->setFocus();
}

//=========================================================
void DChangePassword::slotApply()
{
    QString sql;
    if(checkPassword(currentUserName))
    {
        if(ui->lineEditNewPassword->text() != ui->lineEditRetryNewPassword->text())
        {
            QMessageBox::warning(this,"Внимание","Вы ввели не правильный пароль для подтверждения!","Да");
            ui->lineEditRetryNewPassword->setFocus();
            return;
        }
        else
        {
            sql = "UPDATE \"User\" SET \"Password\"=\'"; sql += ui->lineEditNewPassword->text();
            sql += "\' WHERE \"FIO\"=\'";
            sql += currentUserName;
            sql += "\';";
//            qDebug()<<"sql ="<<sql;
            if(query->exec(sql))
            {
                QMessageBox::information(this,"Сообщение","Пароль изменен успешно!","Да");
                accept();
            }
            else
            {
                qDebug()<<query->lastError().text();
                QMessageBox::warning(this, tr("Внимание"),query->lastError().text(),tr("Да"));
            }
        }
    }
    else
    {
        QMessageBox::warning(this,"Внимание","Вы ввели не правильный старый пароль!","Да");
        ui->lineEditOldPassword->setFocus();
        return;
    }
}

//=========================================================
bool DChangePassword::checkPassword(QString fio)
{
    bool res = false;
    QString sql;
    sql = "SELECT \"Password\" FROM \"User\" WHERE \"FIO\"=\'";
    sql += fio; sql += "\';";
    if(query->exec(sql))
    {
        if(query->next())
        {
            if(query->value(0).toString() == ui->lineEditOldPassword->text())
                res = true;
        }
    }
    else
        qDebug()<<query->lastError().text();
    return res;
}

//=========================================================
void DChangePassword::slotCancel()
{
    reject();
}
