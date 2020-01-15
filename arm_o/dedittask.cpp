#include "dedittask.h"
#include "ui_d_edittask.h"

#include <QMessageBox>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>

extern QSqlDatabase db;
//=========================================================
DEditTask::DEditTask(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::D_EditTask)
{
    ui->setupUi(this);
    query = new QSqlQuery(db);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

//=========================================================
DEditTask::~DEditTask()
{
    delete query;
    delete ui;
}

//=========================================================
void DEditTask::initComboBoxStatus(QString currentStatus)
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
void DEditTask::setNameAVP(QString nameAVP)
{
    ui->labelNameAVP->setText(nameAVP);
}

//=========================================================
void DEditTask::setPercent(QString percent)
{
    QString value;
    value = percent.mid(0,percent.length()-1);
    m_percent = percent;
    ui->spinBoxPercent->setValue(value.toInt());
}

//=========================================================
void DEditTask::setComment(QString comment)
{
    m_comment = comment;
    ui->textEditComment->setText(comment);
}

//=========================================================
const QString DEditTask::getStatus() const
{
    return ui->comboBoxStatus->currentText();
}

//=========================================================
const QString DEditTask::getPercent() const
{
    QString tmp;
    return tmp.setNum(ui->spinBoxPercent->value());
}

//=========================================================
const QString DEditTask::getComment() const
{
    return ui->textEditComment->toPlainText();
}

//=========================================================
void DEditTask::slotApply()
{
    accept();
}

//=========================================================
void DEditTask::slotCancel()
{
    ui->comboBoxStatus->setCurrentText(m_status);
    ui->spinBoxPercent->setValue(m_percent.toInt());
    ui->textEditComment->setText(m_comment);
}

//=========================================================
void DEditTask::slotExit()
{
    reject();
}

