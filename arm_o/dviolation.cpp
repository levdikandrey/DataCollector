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
    QString sql = "";
    m_dAddViolation->setTitle("Добавление нарушения");
    m_dAddViolation->setViolation("");
    if( m_dAddViolation->exec() == QDialog::Accepted )
    {
        try
        {
            sql = "INSERT INTO \"Violation\"(\"Violation\") VALUES(\'";
            sql +=  m_dAddViolation->violation();
            sql += "\');";
            qDebug()<<"sql="<<sql;
            if(query->exec(sql))
                initTableViolation();
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
void DViolation::slotDelete()
{
    QString sql="";
    QModelIndexList selectedRows = ui->tableWidgetViolation->selectionModel()->selectedRows();
    if(selectedRows.empty())
    {
        QMessageBox::information(this, tr("Сообщение"),tr("Вы не выбрали нарушение для удаления!"),tr("Да"));
        return;
    }
    else
    {
        if(QMessageBox::warning(this, tr("Внимание"),tr("Вы действительно хотите удалить выбранные нарушения?"),tr("Да"),tr("Нет")) == 0)
        {
            while (!selectedRows.empty())
            {
                sql = "DELETE FROM \"Violation\" WHERE \"ID\"=";sql += ui->tableWidgetViolation->item(selectedRows[0].row(),1)->text(); sql += ";";
                qDebug()<<"sql ="<<sql;
                query->exec(sql);
                ui->tableWidgetViolation->removeRow(selectedRows[0].row());
                selectedRows = ui->tableWidgetViolation->selectionModel()->selectedRows();
            }
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
        QMessageBox::information(this, tr("Сообщение"),tr("Вы не выбрали нарушение для редактирования!"),tr("Да"));
        return;
    }
    else
    {
        m_dAddViolation->setViolation(ui->tableWidgetViolation->item(selectedRows[0].row(),0)->text());
        m_dAddViolation->setTitle("Редактирование нарушения");

        if(m_dAddViolation->exec() == QDialog::Accepted)
        {
            sql = "UPDATE \"Violation\" SET \"Violation\"=\'";
            sql += m_dAddViolation->violation(); sql += "\' WHERE \"ID\"=";
            sql += ui->tableWidgetViolation->item(selectedRows[0].row(),1)->text();
            sql += ";";
            qDebug()<<"sql ="<<sql;
            if(query->exec(sql))
                initTableViolation();
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
