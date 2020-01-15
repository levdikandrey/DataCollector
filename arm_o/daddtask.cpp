#include "daddtask.h"
#include "ui_d_addtask.h"
#include "mainwindow.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QTableWidget>
#include <QDebug>
#include <QMessageBox>

extern QSqlDatabase db;
//=========================================================
DAddTask::DAddTask(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::D_AddTask)
{
    ui->setupUi(this);
    query = new QSqlQuery(db);

    ui->tableWidget->horizontalHeader()->resizeSection(0, 200);//Название АВП
    ui->tableWidget->horizontalHeader()->resizeSection(1, 250);//URL
    ui->tableWidget->horizontalHeader()->resizeSection(2, 80);//Год выпуска
    ui->tableWidget->horizontalHeader()->resizeSection(3, 200);//Режиссер
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->resizeSection(5, 0);

    m_currentNumberPage = 1;
    m_countAVP = countAVP();
    QString tmp;
    QString text="из ";text+=tmp.setNum(m_countAVP/1000); text+=" (всего АВП ";text+=tmp.setNum(m_countAVP);text+=")";
    ui->lineEditNumberPage->setValidator( new QIntValidator(0, m_countAVP/1000, this) );
    ui->labelCountAVP->setText(text);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->pushButtonPreview->setEnabled(false);

    connect(ui->lineEditFindString,SIGNAL(textChanged(const QString&)),SLOT(slotTextChanged(const QString&)));

    m_timer.setInterval(1000);
    m_timer.setSingleShot(true);
    connect(&m_timer, &QTimer::timeout, this, [&]
            {
                slotFindAVP();
            });
}

//=========================================================
DAddTask::~DAddTask()
{
    delete query;
    delete ui;
}

//=========================================================
const QString DAddTask::getUserFIO() const
{
    return ui->comboBoxUser->currentText();
}

//=========================================================
const QString DAddTask::getStatusName() const
{
    return ui->comboBoxStatus->currentText();
}

//=========================================================
const QString DAddTask::getNamePriority() const
{
    return ui->comboBoxPriority->currentText();
}

//=========================================================
const std::map<long,QString> DAddTask::getListAVP() const
{
    return listAVP;
}

//=========================================================
void DAddTask::slotAdd()
{
    QModelIndexList selectedRows = ui->tableWidget->selectionModel()->selectedRows();
    if(selectedRows.empty())
    {
        QMessageBox::information(this, tr("Сообщение"),tr("Вы не выбрали АВП для добавления!"),tr("Да"));
        return;
    }
    else
    {
       for( int i=0; i<selectedRows.count();++i)
       {
//            qDebug()<<ui->tableWidget->item(selectedRows[i].row(),0)->text();
            ui->listWidgetJobTasks->addItem(ui->tableWidget->item(selectedRows[i].row(),0)->text());
            listAVP.insert(std::make_pair(ui->tableWidget->item(selectedRows[i].row(),5)->text().toLong(),ui->tableWidget->item(selectedRows[i].row(),0)->text()));
       }
    }
//    for(auto item: listAVP)
//        qDebug()<<"==>"<<item.first<<" "<<item.second;
}

//=========================================================
void DAddTask::slotDelete()
{
   for(int i=0; i<ui->listWidgetJobTasks->count();++i)
   {
       if(ui->listWidgetJobTasks->item(i)->isSelected())
       {
            for(auto it = listAVP.begin(); it!=listAVP.end();it++)
            {
//                qDebug()<<"(*it).second="<<(*it).second<<"ui->listWidgetJobTasks->item(i)->text()="<<ui->listWidgetJobTasks->item(i)->text();
                if((*it).second == ui->listWidgetJobTasks->item(i)->text())
                    it = listAVP.erase(it);
            }
            ui->listWidgetJobTasks->takeItem(i);
       }
   }
//   for(auto item: listAVP)
//       qDebug()<<"<=="<<item.first<<" "<<item.second;
}

//=========================================================
void DAddTask::initComboBoxUser()
{
    QString sql="";
    try
    {
        ui->comboBoxUser->clear();

        sql = "SELECT \"ID\",\"FIO\" FROM \"User\";";

        if(query->exec(sql))
        {
            while(query->next())
            {
                ui->comboBoxUser->addItem(query->value(1).toString());

            }
        }
        else
            qDebug()<<query->lastError().text();
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
        qDebug()<<query->lastError().text();
    }
}

//=========================================================
void DAddTask::initComboBoxPriority()
{
    QString sql="";
    try
    {
        ui->comboBoxPriority->clear();

        sql = "SELECT \"NamePriority\" FROM \"Priority\";";

        if(query->exec(sql))
        {
            while(query->next())
            {
                ui->comboBoxPriority->addItem(query->value(0).toString());

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
void DAddTask::initComboBoxStatus()
{
    QString sql="";
    try
    {
        ui->comboBoxStatus->clear();

        sql = "SELECT \"StatusName\" FROM \"TaskStatus\";";

        if(query->exec(sql))
        {
            while(query->next())
            {
                ui->comboBoxStatus->addItem(query->value(0).toString());

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
int DAddTask::countAVP()
{
    int count = 0;
    QString sql;
    try
    {
        sql = "SELECT COUNT(*) FROM avp;";
        if(query->exec(sql))
        {
            if(query->next())
                count = query->value(0).toInt();
        }
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
        qDebug()<<query->lastError().text();
    }
    return count;
}

//=========================================================
void DAddTask::slotPrevious()
{
    QString tmp;
    if(m_currentNumberPage>1)
    {
        ui->pushButtonNext->setEnabled(true);
        m_currentNumberPage--;
        initTableListAVP(m_currentNumberPage);
        ui->lineEditNumberPage->setText(tmp.setNum(m_currentNumberPage));
        if(m_currentNumberPage == 1)
           ui->pushButtonPreview->setEnabled(false);
    }
}

//=========================================================
void DAddTask::slotNext()
{
    QString tmp;
    if(m_currentNumberPage<=(m_countAVP/1000))
    {
        ui->pushButtonPreview->setEnabled(true);
        m_currentNumberPage++;
        initTableListAVP(m_currentNumberPage);
        ui->lineEditNumberPage->setText(tmp.setNum(m_currentNumberPage));
        if(m_currentNumberPage == (m_countAVP/1000))
            ui->pushButtonNext->setEnabled(false);
    }
}

//=========================================================
void DAddTask::slotChangeNumberPage()
{
    qDebug()<<ui->lineEditNumberPage->text();
    m_currentNumberPage = ui->lineEditNumberPage->text().toInt();

    if(m_currentNumberPage<=(m_countAVP/1000))
    {
        if( (m_currentNumberPage>1) && (m_currentNumberPage<(m_countAVP/1000)) )
        {
            ui->pushButtonPreview->setEnabled(true);
            ui->pushButtonNext->setEnabled(true);
        }
        else if(m_currentNumberPage==1)
        {
            ui->pushButtonPreview->setEnabled(false);
            ui->pushButtonNext->setEnabled(true);
        }
        else if(m_currentNumberPage==(m_countAVP/1000))
        {
            ui->pushButtonNext->setEnabled(false);
            ui->pushButtonPreview->setEnabled(true);
        }

        initTableListAVP(m_currentNumberPage);
    }
}


//=========================================================
void DAddTask::slotFindAVP()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QString sql="",tmp;

    try
    {
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);

        sql = "SELECT avp.\"NameRus\","
              "avp.\"URL\","
              "aa.\"YearOfRelease\","
              "aa.\"FilmMaker\","
              "avp.\"ID\" "
              "FROM \"avp\" "
              "INNER JOIN \"AVPattribute\" aa ON aa.\"ID_AVP\" = avp.\"ID\" ";
        if(!ui->lineEditFindString->text().isEmpty())
            sql += " WHERE avp.\"NameRus\" LIKE \'"+ui->lineEditFindString->text()+"%\';";
        else
            sql += " ORDER BY avp.\"ID\" LIMIT 1000 OFFSET 0;";

        if(query->exec(sql))
        {
            if(query->size()==0)
            {
                QApplication::restoreOverrideCursor();
                QMessageBox::information(this,"Сообщение","Нет такого АВП, удовлетворяющего условиям поиска!","Да");
            }
            else
            {
                ui->tableWidget->clearContents();
                ui->tableWidget->setRowCount(0);

                int row = 0;
                while(query->next())
                {
                    ui->tableWidget->setRowCount(row+1);

                    QTableWidgetItem *newItem = new QTableWidgetItem();
                    QIcon icon;
                    icon.addFile(QString::fromUtf8(":/icons/icons/film_projector_cinema.ico"), QSize(), QIcon::Normal, QIcon::Off);
                    newItem->setIcon(icon);
                    newItem->setText(query->value(0).toString());
                    newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidget->setItem(row,0, newItem);

                    QTableWidgetItem *newItem1 = new QTableWidgetItem();
                    QIcon icon1;
                    icon1.addFile(QString::fromUtf8(":/icons/icons/web.ico"), QSize(), QIcon::Normal, QIcon::Off);
                    newItem1->setIcon(icon1);
                    newItem1->setText(query->value(1).toString());
                    newItem1->setFlags(newItem1->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidget->setItem(row,1, newItem1);

                    QTableWidgetItem *newItem2 = new QTableWidgetItem();
                    newItem2->setText(query->value(2).toString());
                    newItem2->setFlags(newItem2->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidget->setItem(row,2, newItem2);

                    QTableWidgetItem *newItem3 = new QTableWidgetItem();
                    QIcon icon4;
                    icon4.addFile(QString::fromUtf8(":/icons/icons/film_maker1.ico"), QSize(), QIcon::Normal, QIcon::Off);
                    newItem3->setIcon(icon4);
                    newItem3->setText(query->value(3).toString());
                    newItem3->setFlags(newItem3->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidget->setItem(row,3, newItem3);

                    QTableWidgetItem *newItem4 = ((MainWindow*)parent())->initViolations(query->value(4).toInt());
                    ui->tableWidget->setItem(row, 4, newItem4);

                    QTableWidgetItem *newItem5 = new QTableWidgetItem();
                    newItem5->setText(query->value(4).toString());
                    ui->tableWidget->setItem(row,5, newItem5);

                    row++;
                }
            }
        }
        else
            qDebug()<<query->lastError().text();

    }
    catch (std::exception &e)
    {
        qDebug()<<e.what();
    }
    QApplication::restoreOverrideCursor();

}

//=========================================================
void DAddTask::slotTextChanged(const QString &text)
{
    if(text.length()>1)
        m_timer.start();
}

//=========================================================
void DAddTask::initTableListAVP(int numberPage)
{
    QString sql="",tmp;
    try
    {
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);
        listAVP.clear();
        ui->listWidgetJobTasks->clear();

        sql = "SELECT avp.\"NameRus\","
              "avp.\"URL\","
              "aa.\"YearOfRelease\","
              "aa.\"FilmMaker\","
              "avp.\"ID\" "
              "FROM \"avp\" "
              "INNER JOIN \"AVPattribute\" aa ON aa.\"ID_AVP\" = avp.\"ID\" "
              "ORDER BY \"ID\" LIMIT 1000 OFFSET ";
        sql+=tmp.setNum((numberPage-1)*1000);sql+=";";

        if(query->exec(sql))
        {
            int row=0;
            while(query->next())
            {
                ui->tableWidget->setRowCount(row+1);

                QTableWidgetItem *newItem = new QTableWidgetItem();
                QIcon icon;
                icon.addFile(QString::fromUtf8(":/icons/icons/film_projector_cinema.ico"), QSize(), QIcon::Normal, QIcon::Off);
                newItem->setIcon(icon);
                newItem->setText(query->value(0).toString());
                newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
                ui->tableWidget->setItem(row,0, newItem);

                QTableWidgetItem *newItem1 = new QTableWidgetItem();
                QIcon icon1;
                icon1.addFile(QString::fromUtf8(":/icons/icons/web.ico"), QSize(), QIcon::Normal, QIcon::Off);
                newItem1->setIcon(icon1);
                newItem1->setText(query->value(1).toString());
                newItem1->setFlags(newItem1->flags() ^ Qt::ItemIsEditable);
                ui->tableWidget->setItem(row,1, newItem1);

                QTableWidgetItem *newItem2 = new QTableWidgetItem();
                newItem2->setText(query->value(2).toString());
                newItem2->setFlags(newItem2->flags() ^ Qt::ItemIsEditable);
                ui->tableWidget->setItem(row,2, newItem2);

                QTableWidgetItem *newItem3 = new QTableWidgetItem();
                QIcon icon4;
                icon4.addFile(QString::fromUtf8(":/icons/icons/film_maker1.ico"), QSize(), QIcon::Normal, QIcon::Off);
                newItem3->setIcon(icon4);
                newItem3->setText(query->value(3).toString());
                newItem3->setFlags(newItem3->flags() ^ Qt::ItemIsEditable);
                ui->tableWidget->setItem(row,3, newItem3);

                QTableWidgetItem *newItem4 = ((MainWindow*)parent())->initViolations(query->value(4).toInt());
                ui->tableWidget->setItem(row, 4, newItem4);

                QTableWidgetItem *newItem5 = new QTableWidgetItem();
                newItem5->setText(query->value(4).toString());
                ui->tableWidget->setItem(row,5, newItem5);

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
void DAddTask::slotApply()
{
    accept();
}

//=========================================================
void DAddTask::slotCancel()
{
    ui->listWidgetJobTasks->clear();
    ui->comboBoxUser->setCurrentIndex(0);
    ui->comboBoxPriority->setCurrentIndex(0);
    ui->comboBoxStatus->setCurrentIndex(0);
}

//=========================================================
void DAddTask::slotExit()
{
    reject();
}
