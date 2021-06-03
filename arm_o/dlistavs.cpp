#include "dlistavs.h"
#include "ui_d_listavs.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QTableWidget>
#include <QDebug>
#include <QMessageBox>


extern QSqlDatabase db;
//=========================================================
DListAVS::DListAVS(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DListAVS)
{
    ui->setupUi(this);
    query = nullptr;
    query = new QSqlQuery(db);

//    m_cImportData = new CImportData();
    ui->tableWidgetGroup->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->tableWidgetGroup->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->tableWidgetGroup->horizontalHeader()->resizeSection(2, 0);
//    connect(ui->tableWidgetGroup, SIGNAL(cellDoubleClicked(int, int)),this, SLOT(slotEdit(int, int)));

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    initDialog();
    dAddAVS = new DAddAVS(this);
    ui->pushButtonEdit->hide();
}

//=========================================================
void DListAVS::slotAddAVS()
{
    QString sql = "";
    if(dAddAVS->exec() == QDialog::Accepted)
    {
            try
            {
                sql = "INSERT INTO avs(\"NameAVS\",\"URL\") VALUES(\'";
                sql +=  dAddAVS->getName(); sql += "\',\'";
                sql +=  dAddAVS->getURL(); sql += "\');";
                qDebug()<<"sql="<<sql;
                if(query->exec(sql))
                    initDialog();
                else
                    qDebug()<<query->lastError().text();
            }
            catch(std::exception &e)
            {
                qDebug()<<e.what();
                qDebug()<<query->lastError().text();
            }

    }
}

//=========================================================
void DListAVS::slotDeleteAVS()
{
    QString sql="";
    QModelIndexList selectedRows = ui->tableWidgetGroup->selectionModel()->selectedRows();
    if(selectedRows.empty())
    {
        QMessageBox::information(this, tr("Сообщение"),tr("Вы не выбрали АВС для удаления!"),tr("Да"));
        return;
    }
    else
    {
        if(QMessageBox::warning(this, tr("Внимание"),tr("Вы действительно хотите удалить выбранные АВС?"),tr("Да"),tr("Нет")) == 0)
        {
            while (!selectedRows.empty())
            {
                sql = "DELETE FROM avs WHERE \"ID\"=";sql += ui->tableWidgetGroup->item(selectedRows[0].row(),2)->text(); sql += ";";
                qDebug()<<"sql ="<<sql;
                query->exec(sql);
                ui->tableWidgetGroup->removeRow(selectedRows[0].row());
                selectedRows = ui->tableWidgetGroup->selectionModel()->selectedRows();
            }
        }
    }
}

//=========================================================
void DListAVS::slotEditAVS()
{
}

//=========================================================
DListAVS::~DListAVS()
{
    delete ui;
}

//=========================================================
void DListAVS::initDialog()
{
    QString sql="";
    try
    {
        ui->tableWidgetGroup->clearContents();
        ui->tableWidgetGroup->setRowCount(0);

        sql = "SELECT * FROM \"avs\";";

        if(query->exec(sql))
        {
            int row=0;
            while(query->next())
            {
                ui->tableWidgetGroup->setRowCount(row+1);

                QTableWidgetItem *newItem = new QTableWidgetItem();
                QIcon icon;
//                icon.addFile(QString::fromUtf8(":/icons/icons/group1.png"), QSize(), QIcon::Normal, QIcon::Off);
//                newItem->setIcon(icon);
                newItem->setText(query->value(2).toString());
                ui->tableWidgetGroup->setItem(row,0, newItem);
                newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);

                QTableWidgetItem *newItem1 = new QTableWidgetItem();
                newItem1->setFlags(newItem1->flags() ^ Qt::ItemIsEditable);
//                QIcon icon1;
//                icon1.addFile(QString::fromUtf8(":/icons/icons/category.png"), QSize(), QIcon::Normal, QIcon::Off);
//                newItem1->setIcon(icon1);
                newItem1->setText(query->value(1).toString());
                ui->tableWidgetGroup->setItem(row,1, newItem1);

                QTableWidgetItem *newItem3 = new QTableWidgetItem();
                newItem3->setText(query->value(0).toString());
                ui->tableWidgetGroup->setItem(row,2, newItem3);

                row++;
            }
        }
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
        qDebug()<<query->lastError().text();
    }
}
