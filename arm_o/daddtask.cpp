#include "daddtask.h"
#include "ui_d_addtask.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QTableWidget>
#include <QDebug>
#include <QMessageBox>

extern QSqlDatabase db;
//=========================================================
DAddTask::DAddTask(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::D_AddTask)
{
    ui->setupUi(this);
    query = new QSqlQuery(db);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->resizeSection(3, 0);
}

//=========================================================
DAddTask::~DAddTask()
{
    delete query;
    delete ui;
}

//=========================================================
void DAddTask::slotAdd()
{
    QModelIndexList selectedRows = ui->tableWidget->selectionModel()->selectedRows();
    if(selectedRows.empty())
    {
        QMessageBox::information(this, tr("Сообщение"),tr("Вы не выбрали АВП для добавления!"),tr("Да"));
        return;
    }
    else
    {
       for( int i=0; i<selectedRows.count();++i)
       {
//            qDebug()<<ui->tableWidget->item(selectedRows[i].row(),0)->text();
            ui->listWidgetJobTasks->addItem(ui->tableWidget->item(selectedRows[i].row(),0)->text());
       }
    }
}

//=========================================================
void DAddTask::slotDelete()
{
   for(int i=0; i<ui->listWidgetJobTasks->count();++i)
   {
       if(ui->listWidgetJobTasks->item(i)->isSelected())
            ui->listWidgetJobTasks->takeItem(i);
   }
}

//=========================================================
void DAddTask::initComboBoxUser()
{
    QString sql="";
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
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
        qDebug()<<query->lastError().text();
    }
}

//=========================================================
void DAddTask::initTableListAVP()
{
    QString sql="";
    try
    {
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);

        sql = "SELECT \"NameRus\",\"URL\",\"ID\" FROM \"avp\";";

        if(query->exec(sql))
        {
            int row=0;
            while(query->next())
            {
                ui->tableWidget->setRowCount(row+1);

                QTableWidgetItem *newItem = new QTableWidgetItem();
                QIcon icon;
                icon.addFile(QString::fromUtf8(":/icons/icons/film_projector_cinema.ico"), QSize(), QIcon::Normal, QIcon::Off);
                newItem->setIcon(icon);
                newItem->setText(query->value(0).toString());
                newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
                ui->tableWidget->setItem(row,0, newItem);

                QTableWidgetItem *newItem1 = new QTableWidgetItem();
                QIcon icon1;
                icon1.addFile(QString::fromUtf8(":/icons/icons/web.ico"), QSize(), QIcon::Normal, QIcon::Off);
                newItem1->setIcon(icon1);
                newItem1->setText(query->value(1).toString());
                newItem1->setFlags(newItem1->flags() ^ Qt::ItemIsEditable);
                ui->tableWidget->setItem(row,1, newItem1);

                QTableWidgetItem *newItem2 = new QTableWidgetItem();
                QIcon icon2;
                icon2.addFile(QString::fromUtf8(":/icons/icons/question.ico"), QSize(), QIcon::Normal, QIcon::Off);
                newItem2->setIcon(icon2);
                newItem2->setText("АВП не анализировалось");
                newItem2->setFlags(newItem2->flags() ^ Qt::ItemIsEditable);
                ui->tableWidget->setItem(row, 2, newItem2);

                QTableWidgetItem *newItem3 = new QTableWidgetItem();
                newItem3->setText(query->value(2).toString());
                ui->tableWidget->setItem(row,3, newItem3);

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

//=========================================================
void DAddTask::slotApply()
{
    accept();
}

//=========================================================
void DAddTask::slotCancel()
{
    ui->listWidgetJobTasks->clear();
    ui->comboBoxUser->setCurrentIndex(0);
    ui->comboBox_2->setCurrentIndex(0);
    ui->comboBox_3->setCurrentIndex(0);
}

//=========================================================
void DAddTask::slotExit()
{
    reject();
}
