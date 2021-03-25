#include "denter.h"
#include "ui_d_enter.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QTableWidget>
#include <QDebug>
#include <QMessageBox>
#include <QSettings>
#include <QDir>
#include <QDateTime>

extern QSqlDatabase db;
extern QString currentUserName;
//=========================================================
DEnter::DEnter(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::D_Enter)
{
    ui->setupUi(this);
//    if(query != nullptr)
//    {
//        delete query;
//        query = nullptr;
//    }
    query = new QSqlQuery(db);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    initUserComBoBox();
    ui->comboBoxUsers->model()->sort(0);
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
        sql = "SELECT * FROM \"User\" WHERE \"FlagJob\" = \'true\';";
        if(query->exec(sql))
        {
            while(query->next())
            {
                ui->comboBoxUsers->addItem(query->value(1).toString());
            }
        }
        else
            qDebug()<<query->lastError().text();
        ui->comboBoxUsers->setCurrentText(currentUserName);
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
bool DEnter::checkPassword(QString fio)
{
    bool res = false;
    QString sql;
    sql = "SELECT \"Password\" FROM \"User\" WHERE \"FIO\"=\'";
    sql += fio; sql += "\';";
    if(query->exec(sql))
    {
        if(query->next())
        {
            if(query->value(0).toString() == ui->lineEditPassword->text())
                res = true;
        }
    }
    else
        qDebug()<<query->lastError().text();
    return res;
}

//=========================================================
int DEnter::getIdUser(QString userName)
{
    int id = -1;
    QString sql;
    try
    {
        sql = "SELECT \"ID\" FROM \"User\" WHERE \"FIO\"=\'"+ userName +"\';";
        if(query->exec(sql))
        {
            if(query->next())
                id = query->value(0).toInt();
        }
        else
            qDebug()<<query->lastError().text();
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
    return id;
}

//=========================================================
void DEnter::saveRecord()
{
    QString sql;
    QString fio;

    sql = "INSERT INTO \"Session\"(\"SessionDate\",\"ID_User\",\"Info\") VALUES(\'";
    sql += QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"); sql += "\',";
    sql += fio.setNum(getIdUser(ui->comboBoxUsers->currentText())); sql += ",\'Вход в приложение\');";
//    qDebug()<<sql;
    if(!query->exec(sql))
        qDebug()<<query->lastError().text();
}

//=========================================================
void DEnter::slotEnter()
{
    if(checkPassword(ui->comboBoxUsers->currentText()))
    {
        currentUserName = ui->comboBoxUsers->currentText();
        QSettings settings(QDir::toNativeSeparators(QApplication::applicationDirPath()) + "/settings.ini",QSettings::IniFormat);
//    qDebug()<<"settings.value(USER/NAME).toString()="<<settings.value("USER/NAME").toString();
        settings.setValue("USER/NAME",ui->comboBoxUsers->currentText());
        saveRecord();
        accept();
    }
    else
    {
        QMessageBox::critical(this,"Ошибка...","Не правильный пароль!","Да");
        return;
    }
}

//=========================================================
void DEnter::slotCancel()
{
    reject();
}
