#include "dedittaskuser.h"
#include "ui_d_edittaskusers.h"

#include <QMessageBox>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QCheckBox>
#include <QSpinBox>

extern QSqlDatabase db;
//=========================================================
DEditTaskUser::DEditTaskUser(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::D_EditTaskUser)
{
    ui->setupUi(this);
    query = new QSqlQuery(db);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->tableWidgetViolation->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->tableWidgetViolation->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
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

        sql = "SELECT \"StatusName\",\"ID\" FROM \"TaskStatus\";";

        if(query->exec(sql))
        {
            while(query->next())
            {
                if((query->value(1).toInt() != 5) && (query->value(1).toInt() != 6))
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
void DEditTaskUser::initTableViolation(long id_avp)
{
    QString sql="",tmp, str;
    ui->tableWidgetViolation->horizontalHeader()->resizeSection(1, 0);

    ui->tableWidgetViolation->clearContents();
    ui->tableWidgetViolation->setRowCount(0);

    try
    {

        sql = "SELECT \"Violation\" FROM \"Violation\";";

        if(query->exec(sql))
        {
            int row = 0;
            while(query->next())
            {
                ui->tableWidgetViolation->setRowCount(row+1);

                QCheckBox *cbItem = new QCheckBox(this);
                cbItem->setText(query->value(0).toString());
                ui->tableWidgetViolation->setCellWidget(row,0, cbItem);//Название нарушения

                QSpinBox *sbItem = new QSpinBox(this);
                sbItem->setMaximum(100);
                ui->tableWidgetViolation->setCellWidget(row,1, sbItem);//Процент обнаружения

                row++;
            }
        }
        else
            qDebug()<<query->lastError().text();

        sql = "SELECT v.\"Violation\",ar.\"Percent\",ar.\"TextViolation\" FROM \"AnalysisResult\" ar "
              "INNER JOIN \"Violation\" v ON ar.\"ID_Violation\"=v.\"ID\" WHERE \"ID_AVP\"="+tmp.setNum(id_avp)+";";

//        qDebug()<<"sql = "<<sql;
        if(query->exec(sql))
        {
            int row = 0;
            while(query->next())
            {
                for(int i=0; i<ui->tableWidgetViolation->rowCount();i++)
                {
                    if(reinterpret_cast<QCheckBox*>(ui->tableWidgetViolation->cellWidget(i,0))->text() == query->value(0).toString())
                    {
                        reinterpret_cast<QCheckBox*>(ui->tableWidgetViolation->cellWidget(i,0))->setChecked(true);
                        reinterpret_cast<QSpinBox*>(ui->tableWidgetViolation->cellWidget(i,1))->setValue(query->value(1).toString().toInt());
                    }
                }
                row++;
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
QTableWidget* DEditTaskUser::getViolations() const
{
    return ui->tableWidgetViolation;
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

