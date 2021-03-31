#include "dreportallviolationavp.h"
#include "ui_d_reportallviolationavp.h"
#include "mainwindow.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QTableWidget>
#include <QDebug>
#include <QMessageBox>
#include <QDesktopServices>

extern QSqlDatabase db;
//=========================================================
DReportAllViolationAVP::DReportAllViolationAVP(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DReportAllViolationAVP)
{
    ui->setupUi(this);
    query = new QSqlQuery(db);
    query1 = new QSqlQuery(db);
    initDialog();
    reinterpret_cast<MainWindow*>(parent)->initComboBoxViolation(ui->comboBoxViolation);
    initTableReportViolationAVP();
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

//=========================================================
DReportAllViolationAVP::~DReportAllViolationAVP()
{
    delete ui;
}

//=========================================================
void DReportAllViolationAVP::initDialog()
{
    ui->tableWidget->horizontalHeader()->resizeSection(0, 90);//Дата отчета
    ui->tableWidget->horizontalHeader()->resizeSection(1, 120);//Дата исследования
    ui->tableWidget->horizontalHeader()->resizeSection(2, 150);//Наименование АВС
    ui->tableWidget->horizontalHeader()->resizeSection(3, 250);//Наименование АВП
    ui->tableWidget->horizontalHeader()->resizeSection(4, 200);//Тип нарушения
    ui->tableWidget->horizontalHeader()->resizeSection(5, 240);//Описание нарушения
    ui->tableWidget->horizontalHeader()->resizeSection(6, 200);//Принятые меры
    ui->tableWidget->horizontalHeader()->resizeSection(7, 130);//Время нарушений
    ui->tableWidget->horizontalHeader()->resizeSection(8, 150);//Ссылка на АВП
    ui->tableWidget->horizontalHeader()->resizeSection(9, 210);//Комментарии РКН
//    ui->tableWidget->horizontalHeader()->resizeSection(10, 120);//
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(10, QHeaderView::Stretch);//Статус отправки
}

//=========================================================
void DReportAllViolationAVP::initTableReportViolationAVP()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QString sql="";
    QDate date;
    try
    {
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);

        sql = "SELECT DISTINCT \"Task\".\"ID\","
              "\"Task\".\"ID_AVP\","
              "\"Task\".\"DateClose\","
              "avp.\"NameRus\","
              "v.\"Violation\","
              "avs.\"NameAVS\","
              "ar.\"TimeBegin\","
              "ar.\"TimeEnd\","
              "avp.\"URL\" FROM \"Task\" "
              "INNER JOIN avp ON \"Task\".\"ID_AVP\" = avp.\"ID\" "
              "INNER JOIN avs ON avp.\"ID_AVS\" = avs.\"ID\" "
              "INNER JOIN \"AnalysisResult\" ar ON \"Task\".\"ID_AVP\" = ar.\"ID_AVP\" "
              "INNER JOIN \"TaskStatus\" ts ON \"Task\".\"ID_TaskStatus\" = ts.\"ID\" "
              "INNER JOIN \"Violation\" v ON ar.\"ID_Violation\" = v.\"ID\" "
              "INNER JOIN \"AVPattribute\" aa ON aa.\"ID_AVP\" = avp.\"ID\" "
              "WHERE ts.\"StatusType\" = 1  AND v.\"ViolationType\" = 2";
        if(ui->groupBoxViolation->isChecked())
        {
            sql +=" AND v.\"Violation\" = \'";sql += ui->comboBoxViolation->currentText(); sql += "\'";
        }
        if(ui->groupBoxDate->isChecked())
        {
            sql +=" AND (\"Task\".\"DateClose\" >='"; sql += ui->dateEditDateBegin->date().toString("yyyy-MM-dd");
            sql += "\'::date AND \"Task\".\"DateClose\" <=\'";sql += ui->dateEditDateEnd->date().toString("yyyy-MM-dd");  sql += "\'::date + \'1 day\'::interval)";
        }
         sql += " ORDER BY \"Task\".\"ID\";";

        qDebug()<<"sql = "<<sql;

        if(query->exec(sql))
        {
            int row=0;
            while(query->next())
            {
                ui->tableWidget->setRowCount(row+1);

                QTableWidgetItem *newItem = new QTableWidgetItem();//Дата отчета
                QIcon icon;
                icon.addFile(QString::fromUtf8(":/icons/icons/date2.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem->setIcon(icon);
                newItem->setText(date.currentDate().toString("yyyy-MM-dd"));
                ui->tableWidget->setItem(row,0, newItem);
                newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);

                QTableWidgetItem *newItem1 = new QTableWidgetItem();//Дата исполнения
                QIcon icon1;
                icon1.addFile(QString::fromUtf8(":/icons/icons/date2.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem1->setIcon(icon1);
                newItem1->setText( query->value(2).toDate().toString("yyyy-MM-dd"));
                ui->tableWidget->setItem(row,1, newItem1);
                newItem1->setFlags(newItem->flags() ^ Qt::ItemIsEditable);

                QTableWidgetItem *newItem2 = new QTableWidgetItem();//АВС
                newItem2->setText(query->value(5).toString());
                ui->tableWidget->setItem(row,2, newItem2);
                newItem2->setFlags(newItem->flags() ^ Qt::ItemIsEditable);

                QTableWidgetItem *newItem3 = new QTableWidgetItem();//АВП
                QIcon icon3;
                icon3.addFile(QString::fromUtf8(":/icons/icons/film_projector_cinema.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem3->setIcon(icon3);
                newItem3->setText( query->value(3).toString());
                ui->tableWidget->setItem(row,3, newItem3);
                newItem3->setFlags(newItem->flags() ^ Qt::ItemIsEditable);

                QTableWidgetItem *newItem4 = new QTableWidgetItem();//Тип нарушения
                newItem4->setText(query->value(4).toString());
                ui->tableWidget->setItem(row,4, newItem4);
                newItem4->setFlags(newItem->flags() ^ Qt::ItemIsEditable);

                QTableWidgetItem *newItem5 = new QTableWidgetItem();//Время нарушения
                newItem5->setText(query->value(6).toTime().toString("HH:mm:ss")+"-"+query->value(7).toTime().toString("HH:mm:ss"));
                ui->tableWidget->setItem(row,7, newItem5);
                newItem5->setFlags(newItem->flags() ^ Qt::ItemIsEditable);

                QTableWidgetItem *newItem6 = new QTableWidgetItem();//URL
                QIcon icon6;
                icon6.addFile(QString::fromUtf8(":/icons/icons/web.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem6->setIcon(icon6);
                newItem6->setText( query->value(8).toString());
                ui->tableWidget->setItem(row,8, newItem6);
                newItem6->setFlags(newItem->flags() ^ Qt::ItemIsEditable);

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
void DReportAllViolationAVP::slotMakeReport()
{
    qDebug()<<__PRETTY_FUNCTION__;
    QString nameReport;
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QString sql,tmp;
    try
    {

        //======start
//        QString timeStart, timeEnd;
//        sql="SELECT \"TextViolation\", \"ID\" FROM \"AnalysisResult\" WHERE \"TextViolation\" IS NOT NULL;";
//        if(query->exec(sql))
//        {
//            while(query->next())
//            {
//                tmp = query->value(0).toString();
//                timeStart = tmp.mid(tmp.indexOf("с")+2,8);
//                timeEnd = tmp.mid(tmp.indexOf("по")+3,8);
//                qDebug()<<"timeStart = "<<timeStart;
//                qDebug()<<"timeEnd = "<<timeEnd;
//                sql = "UPDATE \"AnalysisResult\" SET \"TimeBegin\" = \'";
//                sql += timeStart;sql += "\',\"TimeEnd\" = \'";
//                sql += timeEnd;sql += "\' WHERE \"ID\" = ";
//                sql += tmp.setNum(query->value(1).toInt()); sql += ";";
//                qDebug()<<"sql ="<<sql;
//                if(!query1->exec(sql))
//                    qDebug()<<query1->lastError().text();
//            }
//        }
//        else
//        {
//            qDebug()<<query->lastError().text();
//        }

//        return;
        //======end;





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
             xlsx.setColumnWidth(1,13);
             xlsx.setColumnWidth(2,18);
             xlsx.setColumnWidth(3,20);
             xlsx.setColumnWidth(4,30);
             xlsx.setColumnWidth(5,30);
             xlsx.setColumnWidth(6,40);
             xlsx.setColumnWidth(7,30);
             xlsx.setColumnWidth(8,25);
             xlsx.setColumnWidth(9,20);
             xlsx.setColumnWidth(10,30);
             xlsx.setColumnWidth(11,20);
             xlsx.setRowHeight(1,30);

             xlsx.write(1,1,QVariant("Дата отчета"));
             xlsx.write(1,2,QVariant("Дата исследования"));
             xlsx.write(1,3,QVariant("Наименование АВС"));
             xlsx.write(1,4,QVariant("Наименование АВП"));
             xlsx.write(1,5,QVariant("Тип нарушения"));
             xlsx.write(1,6,QVariant("Описание нарушения законодательства"));
             xlsx.write(1,7,QVariant("Принятые меры"));
             xlsx.write(1,8,QVariant("Время нарушения"));
             xlsx.write(1,9,QVariant("Ссылка на АВП"));
             xlsx.write(1,10,QVariant("Комментарий РКН"));
             xlsx.write(1,11,QVariant("Статус отправки"));
             for(int i=1; i<ui->tableWidget->rowCount();++i)
             {
//                     qDebug()<<"i="<<i;
                 xlsx.write(i+1,1,QVariant(ui->tableWidget->item(i,0)->text()));
                 xlsx.write(i+1,2,QVariant(ui->tableWidget->item(i,1)->text()));
                 xlsx.write(i+1,3,QVariant(ui->tableWidget->item(i,2)->text()));
                 xlsx.write(i+1,4,QVariant(ui->tableWidget->item(i,3)->text()));
                 xlsx.write(i+1,5,QVariant(ui->tableWidget->item(i,4)->text()));
                 xlsx.write(i+1,8,QVariant(ui->tableWidget->item(i,7)->text()));
                 xlsx.write(i+1,9,QVariant(ui->tableWidget->item(i,8)->text()));
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
void DReportAllViolationAVP::slotReview()
{
    initTableReportViolationAVP();
}

//=========================================================
void DReportAllViolationAVP::slotExit()
{
    accept();
}

