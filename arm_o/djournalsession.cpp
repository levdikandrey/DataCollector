#include "djournalsession.h"
#include "ui_d_journalsession.h"
#include "mainwindow.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QTableWidget>
#include <QDebug>
#include <QMessageBox>

extern QSqlDatabase db;
//=========================================================
DJournalSession::DJournalSession(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DJournalSession)
{
    ui->setupUi(this);
    query = new QSqlQuery(db);
    initDialog();
    reinterpret_cast<MainWindow*>(parent)->initComboBoxUser(ui->comboBoxUser);
    initTableJournalSession();
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

//=========================================================
DJournalSession::~DJournalSession()
{
    delete ui;
}

//=========================================================
void DJournalSession::initDialog()
{
    ui->tableWidget->horizontalHeader()->resizeSection(0, 200);//Date
    ui->tableWidget->horizontalHeader()->resizeSection(1, 250);//FIO
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);//Session
}

//=========================================================
void DJournalSession::slotExit()
{
    accept();
}

//=========================================================
void DJournalSession::initTableJournalSession()
{
    QString sql="";
    try
    {
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);

        sql = "SELECT ss.\"SessionDate\",u.\"FIO\",ss.\"Info\" FROM \"Session\" ss INNER JOIN \"User\" u ON ss.\"ID_User\" = u.\"ID\";";

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
                newItem3->setText(query->value(2).toString());
                ui->tableWidget->setItem(row,2, newItem3);

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
void DJournalSession::slotReview()
{
}
