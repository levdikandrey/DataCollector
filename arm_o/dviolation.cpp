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
}

//=========================================================
void DViolation::slotEdit()
{
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
