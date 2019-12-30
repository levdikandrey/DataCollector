#include "dgroup.h"
#include "ui_d_group.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QTableWidget>
#include <QDebug>
#include <QMessageBox>

extern QSqlDatabase db;
//=========================================================
DGroup::DGroup(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::D_Group)
{
    ui->setupUi(this);

    m_dAddGroup = new D_AddGroup(this);
    m_dEditGroup = new DEditGroup(this);
    query = new QSqlQuery(db);

    ui->tableWidgetGroup->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->tableWidgetGroup->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->tableWidgetGroup->horizontalHeader()->resizeSection(2, 0);
    connect(ui->tableWidgetGroup, SIGNAL(cellDoubleClicked(int, int)),this, SLOT(slotEdit(int, int)));

    initTableGroup();
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

//=========================================================
DGroup::~DGroup()
{
    delete ui;
    delete query;
}

//=========================================================
void DGroup::initTableGroup()
{
    QString sql="";
    try
    {
        ui->tableWidgetGroup->clearContents();
        ui->tableWidgetGroup->setRowCount(0);

        sql = "SELECT * FROM \"Group\";";

        if(query->exec(sql))
        {
            int row=0;
            while(query->next())
            {
                ui->tableWidgetGroup->setRowCount(row+1);

                QTableWidgetItem *newItem = new QTableWidgetItem();
                QIcon icon;
                icon.addFile(QString::fromUtf8(":/icons/icons/group1.ico"), QSize(), QIcon::Normal, QIcon::Off);
                newItem->setIcon(icon);
                newItem->setText(query->value(1).toString());
                ui->tableWidgetGroup->setItem(row,0, newItem);
                newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);

                QTableWidgetItem *newItem1 = new QTableWidgetItem();
                newItem1->setFlags(newItem1->flags() ^ Qt::ItemIsEditable);
                QIcon icon1;
                icon1.addFile(QString::fromUtf8(":/icons/icons/category.ico"), QSize(), QIcon::Normal, QIcon::Off);
                newItem1->setIcon(icon1);
                newItem1->setText(query->value(2).toString());
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

//=========================================================
void DGroup::slotAdd()
{
    QString sql = "";
    if(m_dAddGroup->exec() == QDialog::Accepted)
    {
        try
        {
            sql = "INSERT INTO \"Group\"(\"GroupName\",\"GroupCategory\") VALUES(\'";
            sql +=  m_dAddGroup->groupName(); sql += "\',\'";
            sql +=  m_dAddGroup->groupCategory(); sql += "\');";
            qDebug()<<"sql="<<sql;
            if(query->exec(sql))
                initTableGroup();
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
void DGroup::slotDelete()
{
    QString sql="";
    QModelIndexList selectedRows = ui->tableWidgetGroup->selectionModel()->selectedRows();
    if(selectedRows.empty())
    {
        QMessageBox::information(this, tr("Сообщение"),tr("Вы не выбрали группу для удаления!"),tr("Да"));
        return;
    }
    else
    {
        if(QMessageBox::warning(this, tr("Внимание"),tr("Вы действительно хотите удалить выбранные группы?"),tr("Да"),tr("Нет")) == 0)
        {
            while (!selectedRows.empty())
            {
                sql = "DELETE FROM \"Group\" WHERE \"ID\"=";sql += ui->tableWidgetGroup->item(selectedRows[0].row(),2)->text(); sql += ";";
                qDebug()<<"sql ="<<sql;
                query->exec(sql);
                ui->tableWidgetGroup->removeRow(selectedRows[0].row());
                selectedRows = ui->tableWidgetGroup->selectionModel()->selectedRows();
            }
        }
    }
}

//=========================================================
void DGroup::slotEdit(int, int)
{
    slotEdit();
}

//=========================================================
void DGroup::slotEdit()
{
    QString sql="";
    QModelIndexList selectedRows = ui->tableWidgetGroup->selectionModel()->selectedRows();
    if(selectedRows.empty())
    {
        QMessageBox::information(this, tr("Сообщение"),tr("Вы не выбрали группу для редактирования!"),tr("Да"));
        return;
    }
    else
    {
        m_dEditGroup->setGroupName(ui->tableWidgetGroup->item(selectedRows[0].row(),0)->text());
        if(m_dEditGroup->exec() == QDialog::Accepted)
        {
            sql = "UPDATE \"Group\" SET \"GroupName\"=\'";
            sql += m_dEditGroup->groupName(); sql += "\',\"GroupCategory\"=\'";
            sql += m_dEditGroup->groupCategory(); sql += "\' WHERE \"ID\"=";
            sql += ui->tableWidgetGroup->item(selectedRows[0].row(),2)->text();
            sql += ";";
            qDebug()<<"sql ="<<sql;
            if(query->exec(sql))
                initTableGroup();
            else
                qDebug()<<query->lastError().text();
        }
    }
}

//=========================================================
void DGroup::slotExit()
{
    reject();
}
