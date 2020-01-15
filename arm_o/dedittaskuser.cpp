#include "dedittaskuser.h"
#include "ui_d_edittaskusers.h"

#include <QMessageBox>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>

extern QSqlDatabase db;
//=========================================================
DEditTaskUser::DEditTaskUser(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::D_EditTaskUser)
{
    ui->setupUi(this);
    query = new QSqlQuery(db);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->label_6->hide();
    ui->comboBoxViolation->hide();
}

//=========================================================
DEditTaskUser::~DEditTaskUser()
{
    delete query;
    delete ui;
}

//=========================================================
void DEditTaskUser::initComboBoxUser(QString currentUser)
{
    QString sql="";
    m_user = currentUser;
    try
    {
        ui->comboBoxUser->clear();

        sql = "SELECT \"ID\",\"FIO\" FROM \"User\";";

        if(query->exec(sql))
        {
            while(query->next())
            {
                ui->comboBoxUser->addItem(query->value(1).toString());

            }
        }
        else
            qDebug()<<query->lastError().text();
        ui->comboBoxUser->setCurrentText(currentUser);
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
        qDebug()<<query->lastError().text();
    }
}

//=========================================================
void DEditTaskUser::initComboBoxPriority(QString currentPriority)
{
    QString sql="";
    m_priority = currentPriority;
    try
    {
        ui->comboBoxPriority->clear();

        sql = "SELECT \"NamePriority\" FROM \"Priority\";";

        if(query->exec(sql))
        {
            while(query->next())
            {
                ui->comboBoxPriority->addItem(query->value(0).toString());

            }
        }
        else
            qDebug()<<query->lastError().text();
        ui->comboBoxPriority->setCurrentText(currentPriority);
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
void DEditTaskUser::initComboBoxStatus(QString currentStatus)
{
    QString sql="";
    m_status = currentStatus;
    try
    {
        ui->comboBoxStatus->clear();

        sql = "SELECT \"StatusName\" FROM \"TaskStatus\";";

        if(query->exec(sql))
        {
            while(query->next())
            {
                ui->comboBoxStatus->addItem(query->value(0).toString());

            }
        }
        else
            qDebug()<<query->lastError().text();
        ui->comboBoxStatus->setCurrentText(currentStatus);
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
void DEditTaskUser::initComboBoxViolation(QString currentViolation)
{
    QString sql="";
    m_violation = currentViolation;
    try
    {
        ui->comboBoxViolation->clear();

        sql = "SELECT \"Violation\" FROM \"Violation\";";

        if(query->exec(sql))
        {
            while(query->next())
            {
                ui->comboBoxViolation->addItem(query->value(0).toString());

            }
        }
        else
            qDebug()<<query->lastError().text();
        ui->comboBoxViolation->setEnabled(false);
//        ui->comboBoxViolation->setCurrentText(currentViolation);
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }

}

//=========================================================
void DEditTaskUser::setNameAVP(QString nameAVP)
{
    ui->labelNameAVP->setText(nameAVP);
}

//=========================================================
void DEditTaskUser::setPercent(QString percent)
{
    QString value;
    value = percent.mid(0,percent.length()-1);
    m_percent = percent;
    ui->spinBoxPercent->setValue(value.toInt());
}

//=========================================================
void DEditTaskUser::setComment(QString comment)
{
    m_comment = comment;
    ui->textEditComment->setText(comment);
}

//=========================================================
const QString DEditTaskUser::getNameUser() const
{
    return ui->comboBoxUser->currentText();
}

//=========================================================
const QString DEditTaskUser::getPriority() const
{
    return ui->comboBoxPriority->currentText();
}

//=========================================================
const QString DEditTaskUser::getStatus() const
{
    return ui->comboBoxStatus->currentText();
}

//=========================================================
const QString DEditTaskUser::getPercent() const
{
    QString tmp;
    return tmp.setNum(ui->spinBoxPercent->value());
}

//=========================================================
const QString DEditTaskUser::getComment() const
{
    return ui->textEditComment->toPlainText();
}

//=========================================================
void DEditTaskUser::slotApply()
{
    accept();
}

//=========================================================
void DEditTaskUser::slotCancel()
{
    ui->comboBoxUser->setCurrentText(m_user);
    ui->comboBoxPriority->setCurrentText(m_priority);
    ui->comboBoxStatus->setCurrentText(m_status);
    ui->spinBoxPercent->setValue(m_percent.toInt());
    ui->textEditComment->setText(m_comment);
}

//=========================================================
void DEditTaskUser::slotExit()
{
    reject();
}

