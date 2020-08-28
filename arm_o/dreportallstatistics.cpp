#include "dreportallstatistics.h"
#include "ui_d_reportallstatistics.h"
#include "mainwindow.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QTableWidget>
#include <QDebug>
#include <QMessageBox>
#include <QDesktopServices>

extern QSqlDatabase db;
//=========================================================
DReportAllStatistics::DReportAllStatistics(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DReportAllStatistics)
{
    ui->setupUi(this);
    query = new QSqlQuery(db);
    initDialog();
    reinterpret_cast<MainWindow*>(parent)->initComboBoxUser(ui->comboBoxUser);
    initTableReportAllStatistics();
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

//=========================================================
DReportAllStatistics::~DReportAllStatistics()
{
    delete ui;
}

//=========================================================
void DReportAllStatistics::initDialog()
{
    ui->tableWidget->horizontalHeader()->resizeSection(0, 200);//Date
    ui->tableWidget->horizontalHeader()->resizeSection(1, 250);//FIO
    ui->tableWidget->horizontalHeader()->resizeSection(2, 250);//Name AVP
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);//action
}

//=========================================================
void DReportAllStatistics::initTableReportAllStatistics()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QString sql="";
    bool filterCheck = false;
    try
    {
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);

        sql = "SELECT jj.\"DateEvent\",u.\"FIO\",jj.\"NameAVP\",jj.\"Info\" FROM \"JournalJobAVP\" jj INNER JOIN \"User\" u ON jj.\"ID_User\" = u.\"ID\"";
        if(ui->groupBoxUser->isChecked())
        {
            sql +=" WHERE u.\"FIO\" = \'";sql += ui->comboBoxUser->currentText(); sql += "\'";
            filterCheck = true;
        }
        if(ui->groupBoxDate->isChecked())
        {
            if(!filterCheck)
            {
                sql +=" WHERE (\"jj\".\"DateEvent\" >='"; sql += ui->dateEditDateBegin->date().toString("yyyy-MM-dd");
                sql += "\'::date AND \"jj\".\"DateEvent\" <=\'";sql += ui->dateEditDateEnd->date().toString("yyyy-MM-dd");  sql += "\'::date + \'1 day\'::interval)";
            }
            else
            {
                sql +=" AND (\"jj\".\"DateEvent\" >='"; sql += ui->dateEditDateBegin->date().toString("yyyy-MM-dd");
                sql += "\'::date AND \"jj\".\"DateEvent\" <=\'";sql += ui->dateEditDateEnd->date().toString("yyyy-MM-dd");  sql += "\'::date + \'1 day\'::interval)";
            }
            filterCheck = true;
        }
        sql += " ORDER BY \"jj\".\"ID\";";

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

                QTableWidgetItem *newItem4 = new QTableWidgetItem();
                newItem4->setText(query->value(3).toString()+": "+query->value(2).toString());
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
    QApplication::restoreOverrideCursor();
}

//=========================================================
void DReportAllStatistics::slotMakeReport()
{
    qDebug()<<__PRETTY_FUNCTION__;
    QString nameReport;
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QString sql,tmp;
    try
    {
        QDateTime dt = QDateTime::currentDateTime();
        nameReport = "report_"+tmp.setNum(dt.date().day())+"_";
        nameReport += tmp.setNum(dt.date().month())+"_";
        nameReport += tmp.setNum(dt.date().year())+"_";
        nameReport += tmp.setNum(dt.time().hour())+"_";
        nameReport += tmp.setNum(dt.time().minute())+"_";
        nameReport += tmp.setNum(dt.time().second())+".xlsx";
//            qDebug()<<"nameReport = "<<nameReport;

        QFile file(nameReport);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

             QXlsx::Document xlsx(nameReport);
             qDebug()<<"rowCount="<<ui->tableWidget->rowCount();
             for(int i=0; i<ui->tableWidget->rowCount();++i)
             {
//                     qDebug()<<"i="<<i;
                 xlsx.write(i+1,1,QVariant(ui->tableWidget->item(i,0)->text()));
                 xlsx.write(i+1,2,QVariant(ui->tableWidget->item(i,1)->text()));
                 xlsx.write(i+1,3,QVariant(ui->tableWidget->item(i,2)->text()));
                 xlsx.write(i+1,4,QVariant(ui->tableWidget->item(i,3)->text()));
             }
             xlsx.save();
             QApplication::restoreOverrideCursor();
             QMessageBox::information(this,"Сообщение","Создан отчет о задачах.","Да");
             QDesktopServices::openUrl(QUrl::fromLocalFile(nameReport));
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
        QApplication::restoreOverrideCursor();
    }
    QApplication::restoreOverrideCursor();

}

//=========================================================
void DReportAllStatistics::slotReview()
{
    initTableReportAllStatistics();
}

//=========================================================
void DReportAllStatistics::slotExit()
{
    accept();
}

