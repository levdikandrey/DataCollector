#include "dinfoavp.h"
#include "ui_d_infoavp.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QTableWidget>
#include <QDebug>
#include <QMessageBox>
#include <QDateTime>

extern QSqlDatabase db;
//=========================================================
DInfoAVP::DInfoAVP(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DInfoAVP)
{
    ui->setupUi(this);
    query = new QSqlQuery(db);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    initDialog();
}

//=========================================================
DInfoAVP::~DInfoAVP()
{
    delete ui;
}

//=========================================================
void DInfoAVP::initDialog()
{
    ui->tableWidget->horizontalHeader()->resizeSection(0, 180);//Дата
    ui->tableWidget->horizontalHeader()->resizeSection(1, 200);//ФИО
    ui->tableWidget->horizontalHeader()->resizeSection(2, 150);//Роль
    ui->tableWidget->horizontalHeader()->resizeSection(3, 500);//Действие
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Stretch);//Комментарий
}

//=========================================================
void DInfoAVP::initTable(long id_avp)
{

    ui->tableWidget->setSortingEnabled(false);
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    QString sql,tmp;
        try
        {
            sql="select jj.\"DateEvent\", u.\"FIO\",gr.\"GroupName\",jj.\"Info\",ts.\"Comment\",jj.\"NameAVP\" FROM \"JournalJobAVP\" jj "
                "INNER JOIN \"User\" u ON jj.\"ID_User\" = u.\"ID\" "
                "INNER JOIN \"Group\" gr ON u.\"ID_Group\" = gr.\"ID\" "
                "INNER JOIN \"Task\" ts ON jj.\"ID_AVP\" = ts.\"ID_AVP\" "
                "WHERE jj.\"ID_AVP\"=";
            sql += tmp.setNum(id_avp); sql += ";";
            qDebug()<<"sql="<<sql;
            if(!query->exec(sql))
            {
               qDebug()<<query->lastError().text();
                QMessageBox::warning(this, tr("Внимание"),query->lastError().text(),tr("Да"));
            }
            else
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
                    newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidget->setItem(row,0, newItem);//время

                    QTableWidgetItem *newItem1 = new QTableWidgetItem();
                    QIcon icon1;
                    icon1.addFile(QString::fromUtf8(":/icons/icons/user.png"), QSize(), QIcon::Normal, QIcon::Off);
                    newItem1->setIcon(icon1);
                    newItem1->setText(query->value(1).toString());
                    newItem1->setFlags(newItem1->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidget->setItem(row,1, newItem1);//ФИО

                    QTableWidgetItem *newItem2 = new QTableWidgetItem();
                    newItem2->setText(query->value(2).toString());
                    newItem2->setFlags(newItem1->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidget->setItem(row,2, newItem2);//Роль

                    QString testInfo = query->value(3).toString()+": "+query->value(5).toString();
                    QTableWidgetItem *newItem3 = new QTableWidgetItem();
                    newItem3->setText(testInfo);
                    newItem3->setFlags(newItem1->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidget->setItem(row,3, newItem3);//Инфо

                    QTableWidgetItem *newItem4 = new QTableWidgetItem();
                    newItem4->setText(query->value(4).toString());
                    newItem4->setFlags(newItem1->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidget->setItem(row,4, newItem4);//Комментарий
                    row++;
                }

            }
        }
        catch(std::exception &e)
        {
            qDebug()<<e.what();
            qDebug()<<query->lastError().text();
            QMessageBox::warning(this, tr("Внимание"),query->lastError().text(),tr("Да"));
        }

}
