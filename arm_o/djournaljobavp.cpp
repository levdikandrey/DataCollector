#include "djournaljobavp.h"
#include "ui_d_journaljobavp.h"
#include "mainwindow.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QTableWidget>
#include <QDebug>
#include <QMessageBox>

extern QSqlDatabase db;
//=========================================================
DJournalJobAVP::DJournalJobAVP(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DJournalJobAVP)
{
    ui->setupUi(this);
    query = new QSqlQuery(db);
    initDialog();
    reinterpret_cast<MainWindow*>(parent)->initComboBoxUser(ui->comboBoxUser);
    initTableJournalSession();
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

//=========================================================
DJournalJobAVP::~DJournalJobAVP()
{
    delete ui;
}

//=========================================================
void DJournalJobAVP::initDialog()
{
    ui->tableWidget->horizontalHeader()->resizeSection(0, 200);//Date
    ui->tableWidget->horizontalHeader()->resizeSection(1, 250);//FIO
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);//Session
    ui->tableWidget->horizontalHeader()->resizeSection(3, 150);//Category
}

//=========================================================
void DJournalJobAVP::initTableJournalSession()
{
    QString sql="";
    try
    {
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);

        sql = "SELECT jj.\"DateEvent\",u.\"FIO\",jj.\"Info\",jj.\"Сategory\",jj.\"NameAVP\" FROM \"JournalJobAVP\" jj INNER JOIN \"User\" u ON jj.\"ID_User\" = u.\"ID\";";

        if(query->exec(sql))
        {
            int row=0;
            while(query->next())
            {
                ui->tableWidget->setRowCount(row+1);

                QTableWidgetItem *newItem = new QTableWidgetItem();
                QIcon icon;
                icon.addFile(QString::fromUtf8(":/icons/icons/date2.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem->setIcon(icon);
                newItem->setText(query->value(0).toDateTime().toString("yyyy-MM-dd HH:mm:ss"));
                ui->tableWidget->setItem(row,0, newItem);
                newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);

                QTableWidgetItem *newItem1 = new QTableWidgetItem();
                newItem1->setFlags(newItem1->flags() ^ Qt::ItemIsEditable);
                QIcon icon1;
                icon1.addFile(QString::fromUtf8(":/icons/icons/user.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem1->setIcon(icon1);
                newItem1->setText(query->value(1).toString());
                ui->tableWidget->setItem(row,1, newItem1);

                QTableWidgetItem *newItem3 = new QTableWidgetItem();
                newItem3->setText(query->value(2).toString()+": "+query->value(4).toString());
                ui->tableWidget->setItem(row,2, newItem3);

                QTableWidgetItem *newItem4 = new QTableWidgetItem();
                newItem4->setText(query->value(3).toString());
                ui->tableWidget->setItem(row,3, newItem4);

                row++;
            }
        }
        else
        {
            qDebug()<<query->lastError().text();
        }
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
void DJournalJobAVP::slotReview()
{
    QString sql="";
    bool filterCheck = false;
    try
    {
        ui->tableWidget->setSortingEnabled(false);
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);

        sql = "SELECT ss.\"DateEvent\",u.\"FIO\",ss.\"Info\",ss.\"Сategory\" FROM \"JournalJobAVP\" ss INNER JOIN \"User\" u ON ss.\"ID_User\" = u.\"ID\"";

        if(ui->groupBoxUser->isChecked())
        {
            sql +=" WHERE u.\"FIO\" = \'";sql += ui->comboBoxUser->currentText(); sql += "\'";
            filterCheck = true;
        }
        if(ui->groupBoxDate->isChecked())
        {
            if(!filterCheck)
            {
                sql +=" WHERE (ss.\"DateEvent\" >='"; sql += ui->dateEditDateBegin->date().toString("yyyy-MM-dd");
                sql += "\'::date AND ss.\"DateEvent\" <=\'";sql += ui->dateEditDateEnd->date().toString("yyyy-MM-dd");  sql += "\'::date + \'1 day\'::interval)";
            }
            else
            {
                sql +=" AND (ss.\"DateEvent\" >='"; sql += ui->dateEditDateBegin->date().toString("yyyy-MM-dd");
                sql += "\'::date AND ss.\"DateEvent\" <=\'";sql += ui->dateEditDateEnd->date().toString("yyyy-MM-dd");  sql += "\'::date + \'1 day\'::interval)";
            }
            filterCheck = true;
        }
        sql += " ORDER BY ss.\"ID\";";
//        qDebug()<<"sql = "<<sql;

        if(query->exec(sql))
        {
            int row = 0;
            while(query->next())
            {
                ui->tableWidget->setRowCount(row+1);

                QTableWidgetItem *newItem = new QTableWidgetItem();
                QIcon icon;
                icon.addFile(QString::fromUtf8(":/icons/icons/date2.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem->setIcon(icon);
                newItem->setText(query->value(0).toDateTime().toString("yyyy-MM-dd HH:mm:ss"));
                ui->tableWidget->setItem(row,0, newItem);
                newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);

                QTableWidgetItem *newItem1 = new QTableWidgetItem();
                newItem1->setFlags(newItem1->flags() ^ Qt::ItemIsEditable);
                QIcon icon1;
                icon1.addFile(QString::fromUtf8(":/icons/icons/user.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem1->setIcon(icon1);
                newItem1->setText(query->value(1).toString());
                ui->tableWidget->setItem(row,1, newItem1);

                QTableWidgetItem *newItem3 = new QTableWidgetItem();
                newItem3->setText(query->value(2).toString());
                ui->tableWidget->setItem(row,2, newItem3);

                QTableWidgetItem *newItem4 = new QTableWidgetItem();
                newItem3->setText(query->value(3).toString());
                ui->tableWidget->setItem(row,3, newItem4);

                row++;
            }
        }
        else
        {
            qDebug()<<query->lastError().text();
        }
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
void DJournalJobAVP::slotExit()
{
    accept();
}



