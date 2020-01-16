#include "dedittask.h"
#include "ui_d_edittask.h"

#include <QMessageBox>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QCheckBox>
#include <QSpinBox>

extern QSqlDatabase db;
//=========================================================
DEditTask::DEditTask(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::D_EditTask)
{
    ui->setupUi(this);
    query = new QSqlQuery(db);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->tableWidgetViolation->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->tableWidgetViolation->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
}

//=========================================================
DEditTask::~DEditTask()
{
    delete query;
    delete ui;
}

//=========================================================
void DEditTask::initTableViolation(long id_avp)
{
    QString sql="",tmp, str;
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

