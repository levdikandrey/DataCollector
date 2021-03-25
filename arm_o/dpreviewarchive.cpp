#include "dpreviewarchive.h"
#include "ui_d_previewarchive.h"

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QFileDialog>
#include <QProcess>
#include <QDesktopServices>
#include <QMessageBox>

extern QSqlDatabase db;
//=========================================================
DPreviewArchive::DPreviewArchive(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DPreviewArchive)
{
    ui->setupUi(this);
    query = new QSqlQuery(db);
    query_data = new QSqlQuery(db);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->tableWidgetViolation->horizontalHeader()->resizeSection(0, 150);
    ui->tableWidgetViolation->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->tableWidgetViolation->horizontalHeader()->resizeSection(2, 90);
    ui->tableWidgetViolation->horizontalHeader()->resizeSection(3, 0);
    ui->label_4->hide();
    ui->labelMakeDate->hide();
}

//=========================================================
DPreviewArchive::~DPreviewArchive()
{
    delete ui;
    delete query;
}

//=========================================================
void DPreviewArchive::slotPreviewAudit()
{
    QByteArray data;
    QString tmp;
    QString sql;
    sql = "SELECT \"Data\",\"FileName\" FROM \"ExpertiseData\" WHERE \"ID_AVP\" = ";
    sql += tmp.setNum(m_idAVP); sql += ";";
//    qDebug()<<"sql = "<<sql;
    if(query->exec(sql))
    {
        if(query->next())
        {
            data = query->value(0).toByteArray();
            QString pathFileName;
            pathFileName = "./"+query->value(1).toString();
            QFile file(pathFileName);
            if(file.open(QFile::WriteOnly))
            {
                file.write(data);
                qDebug()<<"data.size()="<<data.size();
                file.close();
            }
            else
                qDebug()<<"Не могу открыть файл: "+pathFileName;
            if(data.size() != 0)
            {
                QDesktopServices::openUrl(QUrl::fromLocalFile(pathFileName));
            }
        }
        else
            QMessageBox::warning(this, tr("Внимание"),tr("Нет прикрепленной экспертизы!"),tr("Да"));
    }
    else
    {
        QMessageBox::warning(this, tr("Внимание"),query->lastError().text(),tr("Да"));
        qDebug()<<query->lastError().text();
    }
}

//=========================================================
void DPreviewArchive::initTableViolation(long id_avp)
{
    QString sql="",tmp, str;

    ui->tableWidgetViolation->clearContents();
    ui->tableWidgetViolation->setRowCount(0);
    m_idAVP = id_avp;

    try
    {
        sql = "SELECT v.\"Violation\",ar.\"Percent\",ar.\"TextViolation\",ar.\"ID\",c.\"Data\" FROM \"AnalysisResult\" ar "
              "INNER JOIN \"Violation\" v ON ar.\"ID_Violation\"=v.\"ID\" LEFT JOIN \"Content\" c ON ar.\"ID\" = c.\"ID_AR\" WHERE ar.\"ID_AVP\"="+tmp.setNum(id_avp)+" AND ar.\"TextViolation\" IS NOT NULL;";
        if(query->exec(sql))
        {
            int row = 0;
            while(query->next())
            {
                    ui->tableWidgetViolation->setRowCount(row+1);

                    QTableWidgetItem *newItem = new QTableWidgetItem();
                    newItem->setText(query->value(0).toString());
                    newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidgetViolation->setItem(row,0, newItem);//Название нарушения

                    QTableWidgetItem *newItem1 = new QTableWidgetItem();
                    newItem1->setText(query->value(2).toString());
                    newItem1->setFlags(newItem1->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidgetViolation->setItem(row,1, newItem1);//Описание нарушения

                    QIcon icon1;
                    QPushButton *pbItem = new QPushButton(this);
                    connect(pbItem, SIGNAL(clicked()),this,SLOT(slotViewPicture()));
                    if(query->value(4).isNull())
                    {
                        icon1.addFile(QString::fromUtf8(":/icons/icons/unspin.png"), QSize(), QIcon::Normal, QIcon::Off);
                        pbItem->setText("Нет");
                    }
                    else
                        icon1.addFile(QString::fromUtf8(":/icons/icons/attach.png"), QSize(), QIcon::Normal, QIcon::Off);

                    pbItem->setIcon(icon1);
                    ui->tableWidgetViolation->setCellWidget(row,2, pbItem);//Снимок экрана

                    QTableWidgetItem *newItem3 = new QTableWidgetItem();
                    newItem3->setText(query->value(3).toString());
                    newItem3->setFlags(newItem3->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidgetViolation->setItem(row,3, newItem3);//ID
                    row++;
            }

        }
        else
            qDebug()<<query->lastError().text();
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
void DPreviewArchive::slotViewPicture()
{
    QByteArray data;
    QModelIndexList selectedRows = ui->tableWidgetViolation->selectionModel()->selectedRows();
    QString sql;
    sql = "SELECT \"Data\" FROM \"Content\" WHERE \"ID_AR\" = ";
    sql += ui->tableWidgetViolation->item(selectedRows[0].row(),3)->text(); sql += ";";
//    qDebug()<<"sql = "<<sql;
    if(query->exec(sql))
    {
        if(query->next())
            data = query->value(0).toByteArray();

        QFile file("./tmp.jpg");
        if(file.open(QFile::WriteOnly))
        {
            file.write(data);
            qDebug()<<"data.size()="<<data.size();
            file.close();
        }
        else
            qDebug()<<"Не могу открыть файл: ./tmp.jpg";
//        QDesktopServices::openUrl(QUrl("./tmp.jpg"));
        if(data.size() == 0)
        {
            QMessageBox::warning(this, tr("Внимание"),tr("Нет прикрепленного изображения!"),tr("Да"));
        }
        else
        QProcess::startDetached("mspaint ./tmp.jpg");
    }
    else
    {
        QMessageBox::warning(this, tr("Внимание"),query->lastError().text(),tr("Да"));
        qDebug()<<query->lastError().text();
    }
}

//=========================================================
void DPreviewArchive::setOperatorFIO(QString fio)
{
    ui->labelFIO->setText(fio);
}

//=========================================================
void DPreviewArchive::setExpertFIO(QString fio)
{
    ui->labelExpertFIO->setText(fio);
}

//=========================================================
void DPreviewArchive::setNameAVP(QString nameAVP)
{
    ui->labelNameAVP->setText(nameAVP);
}

//=========================================================
void DPreviewArchive::setComment(QString comment)
{
    ui->textEditComment->setText(comment);
}
