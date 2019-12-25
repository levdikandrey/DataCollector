#include "duser.h"
#include "ui_d_user.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QTableWidget>
#include <QDebug>
#include <QMessageBox>

extern QSqlDatabase db;
//=========================================================
DUser::DUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DUser)
{
    ui->setupUi(this);
    query = new QSqlQuery(db);
    m_dAddUser = new DAddUser(this);
    m_dEditUser = new DEditUser(this);

    ui->tableWidgetUser->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->tableWidgetUser->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->tableWidgetUser->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
    ui->tableWidgetUser->horizontalHeader()->resizeSection(3, 0);
    connect(ui->tableWidgetUser, SIGNAL(cellDoubleClicked(int, int)),this, SLOT(slotEdit(int, int)));

//    initTableUser();
}

//=========================================================
DUser::~DUser()
{
    delete ui;
    delete query;
}

//=========================================================
void DUser::initGroupList()
{
    QString sql="";
    m_groups.clear();
    try
    {
        sql = "SELECT * FROM \"Group\";";
        if(query->exec(sql))
        {
            while(query->next())
            {
                m_groups.insert(std::make_pair(query->value(0).toInt(),query->value(1).toString()));
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
void DUser::initTableUser()
{
    QString sql="";
    try
    {
        ui->tableWidgetUser->clearContents();
        ui->tableWidgetUser->setRowCount(0);

        sql = "SELECT \"User\".\"ID\",\"User\".\"FIO\",\"User\".\"Position\",\"Group\".\"GroupName\" FROM \"User\" LEFT JOIN \"Group\" ON \"User\".\"ID_Group\" = \"Group\".\"ID\";";

        if(query->exec(sql))
        {
            int row=0;
            while(query->next())
            {
                ui->tableWidgetUser->setRowCount(row+1);

                QTableWidgetItem *newItem = new QTableWidgetItem();
                QIcon icon;
                icon.addFile(QString::fromUtf8(":/icons/icons/user.ico"), QSize(24,24), QIcon::Normal, QIcon::Off);
                newItem->setIcon(icon);
                newItem->setText(query->value(1).toString());
                ui->tableWidgetUser->setItem(row,0, newItem);
                newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);

                QTableWidgetItem *newItem1 = new QTableWidgetItem();
                newItem1->setFlags(newItem1->flags() ^ Qt::ItemIsEditable);
                QIcon icon1;
                icon1.addFile(QString::fromUtf8(":/icons/icons/badge.ico"), QSize(24,24), QIcon::Normal, QIcon::Off);
                newItem1->setIcon(icon1);
                newItem1->setText(query->value(2).toString());
                ui->tableWidgetUser->setItem(row,1, newItem1);

                QTableWidgetItem *newItem2 = new QTableWidgetItem();
                newItem1->setFlags(newItem2->flags() ^ Qt::ItemIsEditable);
                QIcon icon2;
                icon2.addFile(QString::fromUtf8(":/icons/icons/group1.ico"), QSize(24,24), QIcon::Normal, QIcon::Off);
                newItem2->setIcon(icon2);
                newItem2->setText(query->value(3).toString());
                ui->tableWidgetUser->setItem(row,2, newItem2);

                QTableWidgetItem *newItem3 = new QTableWidgetItem();
                newItem3->setText(query->value(0).toString());
                ui->tableWidgetUser->setItem(row,3, newItem3);

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
void DUser::slotAdd()
{
    QString sql = "";
    QString tmp;
    initGroupList();
    m_dAddUser->addGroup(m_groups);
    if(m_dAddUser->exec() == QDialog::Accepted)
    {
        try
        {
            sql = "INSERT INTO \"User\"(\"FIO\",\"Position\",\"ID_Group\") VALUES(\'";
            sql +=  m_dAddUser->userName(); sql += "\',\'";
            sql +=  m_dAddUser->range(); sql += "\',";
            sql +=  tmp.setNum(m_dAddUser->idGroup()); sql += ");";
            qDebug()<<"sql="<<sql;
            if(query->exec(sql))
                initTableUser();
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
void DUser::slotDelete()
{
    QString sql="";
    QModelIndexList selectedRows = ui->tableWidgetUser->selectionModel()->selectedRows();
    if(selectedRows.empty())
    {
        QMessageBox::information(this, tr("Сообщение"),tr("Вы не выбрали пользователя для удаления!"),tr("Да"));
        return;
    }
    else
    {
        if(QMessageBox::warning(this, tr("Внимание"),tr("Вы действительно хотите удалить выбранных пользователей?"),tr("Да"),tr("Нет")) == 0)
        {
            while (!selectedRows.empty())
            {
                sql = "DELETE FROM \"User\" WHERE \"ID\"=";sql += ui->tableWidgetUser->item(selectedRows[0].row(),3)->text(); sql += ";";
                qDebug()<<"sql ="<<sql;
                query->exec(sql);
                ui->tableWidgetUser->removeRow(selectedRows[0].row());
                selectedRows = ui->tableWidgetUser->selectionModel()->selectedRows();
            }
        }
    }
}

//=========================================================
void DUser::slotEdit()
{
    QString sql="";
    QString tmp;
    QModelIndexList selectedRows = ui->tableWidgetUser->selectionModel()->selectedRows();
    if(selectedRows.empty())
    {
        QMessageBox::information(this, tr("Сообщение"),tr("Вы не выбрали группу для редактирования!"),tr("Да"));
        return;
    }
    else
    {
        initGroupList();
        m_dEditUser->setUserName(ui->tableWidgetUser->item(selectedRows[0].row(),0)->text());
        m_dEditUser->setPosition(ui->tableWidgetUser->item(selectedRows[0].row(),1)->text());
        m_dEditUser->addGroup(m_groups,ui->tableWidgetUser->item(selectedRows[0].row(),2)->text());

        if(m_dEditUser->exec() == QDialog::Accepted)
        {
            sql = "UPDATE \"User\" SET \"FIO\"=\'";
            sql += m_dEditUser->userName(); sql += "\',\"Position\"=\'";
            sql += m_dEditUser->position(); sql += "\', \"ID_Group\"=";
            sql += tmp.setNum(m_dEditUser->idGroup()); sql += " WHERE \"ID\"=";
            sql += ui->tableWidgetUser->item(selectedRows[0].row(),3)->text();
            sql += ";";
            qDebug()<<"sql ="<<sql;
            if(query->exec(sql))
                initTableUser();
            else
                qDebug()<<query->lastError().text();
        }
    }
}

//=========================================================
void DUser::slotEdit(int, int)
{
    slotEdit();
}

//=========================================================
void DUser::slotExit()
{
    reject();
}

