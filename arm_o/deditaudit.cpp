#include "deditaudit.h"
#include "ui_d_editaudit.h"

#include <QMessageBox>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QCheckBox>
#include <QFile>
#include <QProcess>

extern QSqlDatabase db;
//=========================================================
DEditAudit::DEditAudit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DEditAudit)
{
    ui->setupUi(this);
    query = new QSqlQuery(db);
    query_data = new QSqlQuery(db);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->tableWidgetViolation->horizontalHeader()->resizeSection(0, 150);
//    ui->tableWidgetViolation->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->tableWidgetViolation->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->tableWidgetViolation->horizontalHeader()->resizeSection(2, 90);
    ui->tableWidgetViolation->horizontalHeader()->resizeSection(3, 100);
    ui->tableWidgetViolation->horizontalHeader()->resizeSection(4, 100);
    ui->tableWidgetViolation->horizontalHeader()->resizeSection(5, 0);
}

//=========================================================
DEditAudit::~DEditAudit()
{
    delete ui;
}

//=========================================================
void DEditAudit::initTableViolation(long id_avp)
{
    QString sql="",tmp, str;

    ui->tableWidgetViolation->clearContents();
    ui->tableWidgetViolation->setRowCount(0);
    m_idAVP = id_avp;

    try
    {
        sql = "SELECT v.\"Violation\",ar.\"Percent\",ar.\"TextViolation\",ar.\"ID\",c.\"Data\" FROM \"AnalysisResult\" ar "
              "INNER JOIN \"Violation\" v ON ar.\"ID_Violation\"=v.\"ID\" LEFT JOIN \"Content\" c ON ar.\"ID\" = c.\"ID_AR\" WHERE ar.\"ID_AVP\"="+tmp.setNum(id_avp)+";";
        if(query->exec(sql))
        {
            int row = 0;
            while(query->next())
            {
//                if(query->value(0).toString()!="Не обнаружено")
//                {
                    ui->tableWidgetViolation->setRowCount(row+1);

                    QTableWidgetItem *newItem = new QTableWidgetItem();
                    newItem->setText(query->value(0).toString());
                    newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidgetViolation->setItem(row,0, newItem);//Название нарушения

                    QTableWidgetItem *newItem1 = new QTableWidgetItem();
                    newItem1->setText(query->value(2).toString());
                    newItem1->setFlags(newItem1->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidgetViolation->setItem(row,1, newItem1);//Описание нарушения

                    QIcon icon1;
                    QPushButton *pbItem = new QPushButton(this);
                    connect(pbItem, SIGNAL(clicked()),this,SLOT(slotViewPicture()));
                    if(query->value(4).isNull())
                        icon1.addFile(QString::fromUtf8(":/icons/icons/unspin.ico"), QSize(), QIcon::Normal, QIcon::Off);
                    else
                        icon1.addFile(QString::fromUtf8(":/icons/icons/attach.ico"), QSize(), QIcon::Normal, QIcon::Off);

                    pbItem->setIcon(icon1);
                    connect(pbItem, SIGNAL(clicked()),this,SLOT(slotViewPicture()));
                    ui->tableWidgetViolation->setCellWidget(row,2, pbItem);//Снимок экрана

                    QCheckBox *cbItem = new QCheckBox(this);
                    connect(cbItem, SIGNAL(stateChanged(int)),this,SLOT(slotStateChanged(int)));
                    ui->tableWidgetViolation->setCellWidget(row,3, cbItem);

                    QCheckBox *cbItem1 = new QCheckBox(this);
                    connect(cbItem1, SIGNAL(stateChanged(int)),this,SLOT(slotStateChanged(int)));
                    ui->tableWidgetViolation->setCellWidget(row,4, cbItem1);

                    QTableWidgetItem *newItem3 = new QTableWidgetItem();
                    newItem3->setText(query->value(3).toString());
                    newItem3->setFlags(newItem3->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidgetViolation->setItem(row,5, newItem3);//ID
                    row++;
//                }
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
void DEditAudit::slotTextChanged()
{
    ui->pushButtonApply->setEnabled(true);
    ui->pushButtonCancel->setEnabled(true);
}

//=========================================================
void DEditAudit::slotStateChanged(int)
{
    ui->pushButtonApply->setEnabled(true);
    ui->pushButtonCancel->setEnabled(true);
    QModelIndexList selectedRows = ui->tableWidgetViolation->selectionModel()->selectedRows();
    int row = selectedRows[0].row();
    int col = ui->tableWidgetViolation->currentColumn();
//    qDebug()<<"row = "<< row;
//    qDebug()<<"col = "<< col;
    QCheckBox *cb1;
    QObject* obj=QObject::sender();
    if (QCheckBox *cb=qobject_cast<QCheckBox *>(obj))
    {
        if(col == 3)
            cb1 = reinterpret_cast<QCheckBox *>(ui->tableWidgetViolation->cellWidget(row,4));
        else if(col == 4)
            cb1 = reinterpret_cast<QCheckBox *>(ui->tableWidgetViolation->cellWidget(row,3));
//        qDebug()<<"cb1 = "<<cb1;
        if(cb->isChecked())
        {
            cb1->setCheckState(Qt::Unchecked);
        }
//        else
//            cb1->setCheckState(Qt::Checked);
    }
}

//=========================================================
void DEditAudit::slotViewPicture()
{
    QByteArray data;
    QModelIndexList selectedRows = ui->tableWidgetViolation->selectionModel()->selectedRows();
    QString sql;
    sql = "SELECT \"Data\" FROM \"Content\" WHERE \"ID_AR\" = ";
    sql += ui->tableWidgetViolation->item(selectedRows[0].row(),5)->text(); sql += ";";

    if(query->exec(sql))
    {
        if(query->next())
            data = query->value(0).toByteArray();

        QFile file("./tmp.jpg");
        if(file.open(QFile::WriteOnly))
        {
            file.write(data);
            qDebug()<<"data.size()="<<data.size();
            file.close();
        }
        else
            qDebug()<<"Не могу открыть файл: ./tmp.jpg";
//        QDesktopServices::openUrl(QUrl("./tmp.jpg"));
        if(data.size() == 0)
        {
            QMessageBox::warning(this, tr("Внимание"),tr("Нет прикрепленного изображения!"),tr("Да"));
        }
        else
        QProcess::startDetached("mspaint ./tmp.jpg");
    }
    else
    {
        QMessageBox::warning(this, tr("Внимание"),query->lastError().text(),tr("Да"));
        qDebug()<<query->lastError().text();
    }
}

//=========================================================
const QString DEditAudit::getComment() const
{
    return m_strChecked + ui->textEdit->toPlainText();
}

//=========================================================
void DEditAudit::slotApply()
{
    QString sql;
    m_strChecked ="";
    for(int row=0; row<ui->tableWidgetViolation->rowCount(); row++)
    {
        m_strChecked += ui->tableWidgetViolation->item(row,0)->text();
        m_strChecked += " - ";
        if(reinterpret_cast<QCheckBox *>(ui->tableWidgetViolation->cellWidget(row,3))->isChecked())
        {
           sql = "UPDATE \"AnalysisResult\" SET \"CheckExpert\"=\'true\'";
           m_strChecked += " подтверждено. ";
        }
        else
        {
            sql = "UPDATE \"AnalysisResult\" SET \"CheckExpert\"=\'false\'";
            m_strChecked += " отклонено. ";
        }
        sql += " WHERE \"ID\"=";
        sql += ui->tableWidgetViolation->item(row,5)->text();
        sql += ";";

        qDebug()<<"sql="<<sql;
        if(!query->exec(sql))
            qDebug()<<query->lastError().text();
    }
    accept();
}

//=========================================================
void DEditAudit::slotCancel()
{
    for(int row=0; row<ui->tableWidgetViolation->rowCount(); row++)
    {
        reinterpret_cast<QCheckBox *>(ui->tableWidgetViolation->cellWidget(row,3))->setCheckState(Qt::Unchecked);
        reinterpret_cast<QCheckBox *>(ui->tableWidgetViolation->cellWidget(row,4))->setCheckState(Qt::Unchecked);
    }
    ui->textEdit->clear();
    ui->groupBox_2->setChecked(Qt::Unchecked);
    ui->pushButtonApply->setEnabled(false);
    ui->pushButtonCancel->setEnabled(false);
}

//=========================================================
void DEditAudit::slotExit()
{
    reject();
}

