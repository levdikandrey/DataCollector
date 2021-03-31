#include "dviolation.h"
#include "ui_d_violation.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QTableWidget>
#include <QDebug>
#include <QMessageBox>

extern QSqlDatabase db;
//=========================================================
DViolation::DViolation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DViolation)
{
    ui->setupUi(this);
    query = new QSqlQuery(db);
    m_dAddViolation = new DAddViolation(this);
//    m_dEditUser = new DEditUser(this);

    ui->tableWidgetViolation->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->tableWidgetViolation->horizontalHeader()->resizeSection(1, 0);
    connect(ui->tableWidgetViolation, SIGNAL(cellDoubleClicked(int, int)),this, SLOT(slotEdit(int, int)));

    initTableViolation();
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

}

//=========================================================
void DViolation::initTableViolation()
{
    QString sql="";
    setWindowTitle("Справочник нарушений");
    ui->tableWidgetViolation->horizontalHeaderItem(0)->setText("Нарушения");
    try
    {
        ui->tableWidgetViolation->clearContents();
        ui->tableWidgetViolation->setRowCount(0);

        sql = "SELECT * FROM \"Violation\";";

        if(query->exec(sql))
        {
            int row=0;
            while(query->next())
            {
                ui->tableWidgetViolation->setRowCount(row+1);

                QTableWidgetItem *newItem = new QTableWidgetItem();
                QIcon icon;
//                icon.addFile(QString::fromUtf8(":/icons/icons/user.ico"), QSize(24,24), QIcon::Normal, QIcon::Off);
//                newItem->setIcon(icon);
                newItem->setText(query->value(1).toString());
                ui->tableWidgetViolation->setItem(row,0, newItem);
                newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);

                QTableWidgetItem *newItem3 = new QTableWidgetItem();
                newItem3->setText(query->value(0).toString());
                ui->tableWidgetViolation->setItem(row,1, newItem3);

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
void DViolation::initTableStatus()
{
    QString sql="";
    setWindowTitle("Справочник статусов");
    ui->tableWidgetViolation->horizontalHeaderItem(0)->setText("Статусы");
    try
    {
        ui->tableWidgetViolation->clearContents();
        ui->tableWidgetViolation->setRowCount(0);

        sql = "SELECT * FROM \"TaskStatus\";";

        if(query->exec(sql))
        {
            int row=0;
            while(query->next())
            {
                ui->tableWidgetViolation->setRowCount(row+1);

                QTableWidgetItem *newItem = new QTableWidgetItem();
                QIcon icon;
//                icon.addFile(QString::fromUtf8(":/icons/icons/user.ico"), QSize(24,24), QIcon::Normal, QIcon::Off);
//                newItem->setIcon(icon);
                newItem->setText(query->value(1).toString());
                ui->tableWidgetViolation->setItem(row,0, newItem);
                newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);

                QTableWidgetItem *newItem3 = new QTableWidgetItem();
                newItem3->setText(query->value(0).toString());
                ui->tableWidgetViolation->setItem(row,1, newItem3);

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
void DViolation::initViolationList()
{
    QString sql="";
    m_violations.clear();
    try
    {
        sql = "SELECT * FROM \"Violation\";";
        if(query->exec(sql))
        {
            while(query->next())
            {
                m_violations.insert(std::make_pair(query->value(0).toInt(),query->value(1).toString()));
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
DViolation::~DViolation()
{
    delete ui;
}

//=========================================================
void DViolation::slotAdd()
{
    QString sql = "",tmp;
    if(m_typeDictionary == 1)
    {
        m_dAddViolation->setMaximumHeight(16777215);
        m_dAddViolation->setTitle("Добавление нарушения");
        m_dAddViolation->showItem();
    }
    else if(m_typeDictionary == 2)
    {
        m_dAddViolation->setMaximumHeight(120);
        m_dAddViolation->setTitle("Добавление статуса");
        m_dAddViolation->hideItem();
    }
    m_dAddViolation->setViolation("");
    if( m_dAddViolation->exec() == QDialog::Accepted )
    {
        try
        {
            if(m_typeDictionary == 1)
            {
                sql = "INSERT INTO \"Violation\"(\"Violation\",\"ViolationType\",\"LawText\") VALUES(\'";
                sql +=  m_dAddViolation->violation();
                sql += "\',";
                sql += tmp.setNum(m_dAddViolation->getType());
                sql += ",\'";
                sql += m_dAddViolation->getLawText();
                sql += "\');";
            }
            else if(m_typeDictionary == 2)
            {
                sql = "INSERT INTO \"TaskStatus\"(\"StatusName\") VALUES(\'";
                sql +=  m_dAddViolation->violation();
                sql += "\');";
            }
            qDebug()<<"sql="<<sql;
            if(query->exec(sql))
            {
                if(m_typeDictionary == 1)
                    initTableViolation();
                else if(m_typeDictionary == 2)
                    initTableStatus();
            }
            else
                qDebug()<<query->lastError().text();
        }
        catch(std::exception &e)
        {
            qDebug()<<e.what();
        }

    }
}

//=========================================================
void DViolation::slotDelete()
{
    QString sql="";
    QModelIndexList selectedRows = ui->tableWidgetViolation->selectionModel()->selectedRows();
    if(selectedRows.empty())
    {
        if(m_typeDictionary == 1)
            QMessageBox::information(this, tr("Сообщение"),tr("Вы не выбрали нарушение для удаления!"),tr("Да"));
        else if(m_typeDictionary == 2)
            QMessageBox::information(this, tr("Сообщение"),tr("Вы не выбрали статус для удаления!"),tr("Да"));
        return;
    }
    else
    {
        try
        {
            QString text = "Вы действительно хотите удалить выбранные ";
            if(m_typeDictionary == 1)
                text +=" нарушения?";
            else if(m_typeDictionary == 2)
                text +=" статусы?";
            if(QMessageBox::warning(this, tr("Внимание"),text,tr("Да"),tr("Нет")) == 0)
            {
                while (!selectedRows.empty())
                {
                    if(m_typeDictionary == 1)
                    {
                        sql = "DELETE FROM \"Violation\" WHERE \"ID\"=";sql += ui->tableWidgetViolation->item(selectedRows[0].row(),1)->text(); sql += ";";
                    }
                    else if(m_typeDictionary == 2)
                    {
                        sql = "DELETE FROM \"TaskStatus\" WHERE \"ID\"=";sql += ui->tableWidgetViolation->item(selectedRows[0].row(),1)->text(); sql += ";";
                    }
                    qDebug()<<"sql ="<<sql;
                    query->exec(sql);
                    ui->tableWidgetViolation->removeRow(selectedRows[0].row());
                    selectedRows = ui->tableWidgetViolation->selectionModel()->selectedRows();
                }
            }
        }
        catch(std::exception &e)
        {
            QMessageBox::warning(this, tr("Внимание"),query->lastError().text(),tr("Да"));
            qDebug()<<e.what();
        }

    }

}

//=========================================================
void DViolation::slotEdit()
{
    QString sql="";
    QString tmp;
    QModelIndexList selectedRows = ui->tableWidgetViolation->selectionModel()->selectedRows();
    if(selectedRows.empty())
    {
        if(m_typeDictionary == 1)
            QMessageBox::information(this, tr("Сообщение"),tr("Вы не выбрали нарушение для редактирования!"),tr("Да"));
        else if(m_typeDictionary == 2)
            QMessageBox::information(this, tr("Сообщение"),tr("Вы не выбрали статус для редактирования!"),tr("Да"));
        return;
    }
    else
    {
        m_dAddViolation->setViolation(ui->tableWidgetViolation->item(selectedRows[0].row(),0)->text());
        if(m_typeDictionary == 1)
        {
            m_dAddViolation->setMaximumHeight(16777215);
            m_dAddViolation->setTitle("Редактирование нарушения");
            m_dAddViolation->showItemEdit();
        }
        else if(m_typeDictionary == 2)
        {
            m_dAddViolation->setMaximumHeight(120);
            m_dAddViolation->setTitle("Редактирование статуса");
            m_dAddViolation->hideItem();
        }

        if(m_dAddViolation->exec() == QDialog::Accepted)
        {
            if(m_typeDictionary == 1)
            {
                sql = "UPDATE \"Violation\" SET \"Violation\"=\'";
                sql += m_dAddViolation->violation();
                sql += "\',\"LawText\"=\'";
                sql += m_dAddViolation->getLawText();
                sql += "\' WHERE \"ID\"=";
                sql += ui->tableWidgetViolation->item(selectedRows[0].row(),1)->text();
                sql += ";";
            }
            else if(m_typeDictionary == 2)
            {
                sql = "UPDATE \"TaskStatus\" SET \"StatusName\"=\'";
                sql += m_dAddViolation->violation(); sql += "\' WHERE \"ID\"=";
                sql += ui->tableWidgetViolation->item(selectedRows[0].row(),1)->text();
                sql += ";";
            }
            qDebug()<<"sql ="<<sql;
            if(query->exec(sql))
            {
                if(m_typeDictionary == 1)
                    initTableViolation();
                else if(m_typeDictionary == 2)
                    initTableStatus();
            }
            else
            {
                qDebug()<<query->lastError().text();
                QMessageBox::warning(this, tr("Внимание"),query->lastError().text(),tr("Да"));
            }
        }
    }

}

//=========================================================
void DViolation::slotEdit(int, int)
{
    slotEdit();
}

//=========================================================
void DViolation::slotExit()
{
    reject();
}
