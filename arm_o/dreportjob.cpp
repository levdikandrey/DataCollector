#include "dreportjob.h"
#include "ui_d_reportjob.h"
#include "mainwindow.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QTableWidget>
#include <QDebug>
#include <QMessageBox>
#include <QDesktopServices>

extern QSqlDatabase db;
//=========================================================
DReportJob::DReportJob(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DReportJob)
{
    ui->setupUi(this);
    query = new QSqlQuery(db);
    initDialog();
    reinterpret_cast<MainWindow*>(parent)->initComboBoxUser(ui->comboBoxUser);
    reinterpret_cast<MainWindow*>(parent)->initComboBoxStatus(ui->comboBoxStatus);
    initTableReportJob();
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

//=========================================================
DReportJob::~DReportJob()
{
    delete ui;
}

//=========================================================
void DReportJob::initDialog()
{
    ui->tableWidget->horizontalHeader()->resizeSection(0, 200);//Название АВП
    ui->tableWidget->horizontalHeader()->resizeSection(1, 300);//URL
    ui->tableWidget->horizontalHeader()->resizeSection(2, 150);//ФИО
    ui->tableWidget->horizontalHeader()->resizeSection(3, 110);//Дата назначения
    ui->tableWidget->horizontalHeader()->resizeSection(4, 100);//Приоритет
    ui->tableWidget->horizontalHeader()->resizeSection(5, 150);//Статус
    ui->tableWidget->horizontalHeader()->resizeSection(6, 350);//Нарушения
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Stretch);//Комментарии
}

//=========================================================
void DReportJob::initTableReportJob()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QString sql="";
    bool filterCheck = false;
    try
    {
        ui->tableWidget->setSortingEnabled(false);
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);

        sql = "SELECT avp.\"NameRus\", "
              "avp.\"URL\","
              "u.\"FIO\","
              "\"Task\".\"DateAppoint\","
              "p.\"NamePriority\","
              "ts.\"StatusName\","
              "\"Task\".\"Comment\","
              "\"Task\".\"ID\","
              "\"Task\".\"ID_AVP\" "
              "FROM \"Task\" "
              "INNER JOIN avp ON \"Task\".\"ID_AVP\" = avp.\"ID\" "
              "INNER JOIN \"User\" u ON \"Task\".\"ID_User\" = u.\"ID\" "
              "INNER JOIN \"TaskStatus\" ts ON \"Task\".\"ID_TaskStatus\" = ts.\"ID\" "
              "INNER JOIN \"Priority\" p ON \"Task\".\"ID_Priority\" = p.\"ID\"";
            if(ui->groupBoxUser->isChecked())
            {
                sql +=" WHERE u.\"FIO\" = \'";sql += ui->comboBoxUser->currentText(); sql += "\'";
                filterCheck = true;
            }
            if(ui->groupBoxStatus->isChecked())
            {
                if(!filterCheck)
                {
                    sql +=" WHERE ts.\"StatusName\" = \'";sql += ui->comboBoxStatus->currentText(); sql += "\'";
                }
                else
                {
                    sql +=" AND ts.\"StatusName\" = \'";sql += ui->comboBoxStatus->currentText(); sql += "\'";
                }
                filterCheck = true;
            }
            if(ui->groupBoxDate->isChecked())
            {
                if(!filterCheck)
                {
                    sql +=" WHERE (\"Task\".\"DateAppoint\" >='"; sql += ui->dateEditDateBegin->date().toString("yyyy-MM-dd");
                    sql += "\'::date AND \"Task\".\"DateAppoint\" <=\'";sql += ui->dateEditDateEnd->date().toString("yyyy-MM-dd");  sql += "\'::date + \'1 day\'::interval)";
                }
                else
                {
                    sql +=" AND (\"Task\".\"DateAppoint\" >='"; sql += ui->dateEditDateBegin->date().toString("yyyy-MM-dd");
                    sql += "\'::date AND \"Task\".\"DateAppoint\" <=\'";sql += ui->dateEditDateEnd->date().toString("yyyy-MM-dd");  sql += "\'::date + \'1 day\'::interval)";
                }
                filterCheck = true;
            }
        sql += " ORDER BY \"Task\".\"ID\";";
//        qDebug()<<"sql = "<<sql;

        if(query->exec(sql))
        {
            int row = 0;
            while(query->next())
            {
                ui->tableWidget->setRowCount(row+1);

                QTableWidgetItem *newItem = new QTableWidgetItem();
                QIcon icon;
                icon.addFile(QString::fromUtf8(":/icons/icons/film_projector_cinema.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem->setIcon(icon);
                newItem->setText(query->value(0).toString());
                newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
                ui->tableWidget->setItem(row,0, newItem);//Название АВП

                QTableWidgetItem *newItem1 = new QTableWidgetItem();
                QIcon icon1;
                icon1.addFile(QString::fromUtf8(":/icons/icons/web.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem1->setIcon(icon1);
                newItem1->setText(query->value(1).toString());
                newItem1->setFlags(newItem1->flags() ^ Qt::ItemIsEditable);
                ui->tableWidget->setItem(row,1, newItem1);//URL

                QTableWidgetItem *newItem2 = new QTableWidgetItem();
                QIcon icon5;
                icon5.addFile(QString::fromUtf8(":/icons/icons/user.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem2->setIcon(icon5);
                newItem2->setText(query->value(2).toString());
                newItem2->setFlags(newItem2->flags() ^ Qt::ItemIsEditable);
                ui->tableWidget->setItem(row,2, newItem2);//ФИО

                QTableWidgetItem *newItem3 = new QTableWidgetItem();
                QIcon icon3;
                icon3.addFile(QString::fromUtf8(":/icons/icons/date2.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem3->setIcon(icon3);
                newItem3->setText(query->value(3).toDate().toString("yyyy-MM-dd"));
                newItem3->setFlags(newItem3->flags() ^ Qt::ItemIsEditable);
                ui->tableWidget->setItem(row,3, newItem3);//Дата назначения задачи

                QTableWidgetItem *newItem4 = new QTableWidgetItem();
                newItem4->setText(query->value(4).toString());
                newItem4->setFlags(newItem4->flags() ^ Qt::ItemIsEditable);
                ui->tableWidget->setItem(row,4, newItem4);//Приоритет

                QTableWidgetItem *newItem5 = new QTableWidgetItem();
                newItem5->setText(query->value(5).toString());
                newItem5->setFlags(newItem5->flags() ^ Qt::ItemIsEditable);
                ui->tableWidget->setItem(row,5, newItem5);//Статус

                QTableWidgetItem *newItem7 = reinterpret_cast<MainWindow*>(parent())->initViolations(query->value(8).toInt());
                ui->tableWidget->setItem(row,6, newItem7);//Нарушения

                QTableWidgetItem *newItem8 = new QTableWidgetItem();
                newItem8->setText(query->value(6).toString());
                newItem8->setFlags(newItem8->flags() ^ Qt::ItemIsEditable);
                ui->tableWidget->setItem(row,7, newItem8);//Комментарии

//                QTableWidgetItem *newItem9 = new QTableWidgetItem();
//                newItem9->setText(query->value(9).toString());
//                newItem9->setFlags(newItem9->flags() ^ Qt::ItemIsEditable);
//                ui->tableWidgetCurrentTasks->setItem(row,10, newItem9);//ID

//                QTableWidgetItem *newItem10 = new QTableWidgetItem();
//                newItem10->setText(query->value(10).toString());
//                newItem10->setFlags(newItem10->flags() ^ Qt::ItemIsEditable);
//                ui->tableWidgetCurrentTasks->setItem(row,11, newItem10);//ID_AVP

//                QDate currentDT = QDateTime::currentDateTime().date();
//                QDate realizationDT = query->value(11).toDate();
//                if((currentDT > realizationDT) && (query->value(5).toString()!="Закрыта"))
//                    setColorRowTable(ui->tableWidgetCurrentTasks,row,0xff,0xc0,0xcb);
                row++;
            }
        }
        else
            qDebug()<<query->lastError().text();
        ui->tableWidget->setSortingEnabled(true);
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
        qDebug()<<query->lastError().text();
    }
    QApplication::restoreOverrideCursor();
}

//=========================================================
void DReportJob::slotMakeReport()
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
                     xlsx.write(i+1,5,QVariant(ui->tableWidget->item(i,4)->text()));
                     xlsx.write(i+1,6,QVariant(ui->tableWidget->item(i,5)->text()));
                     xlsx.write(i+1,7,QVariant(ui->tableWidget->item(i,6)->text()));
                     xlsx.write(i+1,8,QVariant(ui->tableWidget->item(i,7)->text()));
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
void DReportJob::slotReview()
{
    initTableReportJob();
}

//=========================================================
void DReportJob::slotExit()
{
    accept();
}
