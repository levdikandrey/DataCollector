#include "daddtask.h"
#include "ui_d_addtask.h"
#include "mainwindow.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QTableWidget>
#include <QDebug>
#include <QMessageBox>
#include <QPushButton>
#include <QCheckBox>
#include <QObject>

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
    ui->tableWidget->horizontalHeader()->resizeSection(4, 80);//Возраст
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(5,QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->resizeSection(6, 0);

    m_flagNotJob = true;
    m_currentIdAVS = -1;
    m_currentNumberPage = 1;
    m_currentState = -1;
    m_countAVP = countAVPNotJob();
    int page_count = (m_countAVP/1000);
    if( m_countAVP%1000 > 0)
        page_count++;
    QString tmp;
    QString text="из ";text+=tmp.setNum(page_count); text+=" (всего АВП ";text+=tmp.setNum(m_countAVP);text+=")";
    ui->lineEditNumberPage->setValidator( new QIntValidator(1,(page_count), this) );
    ui->labelCountAVP->setText(text);
    ui->pushButtonPreview->setEnabled(false);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags()|Qt::WindowMaximizeButtonHint);

    ui->pushButtonPreview->setEnabled(false);

    connect(ui->radioButtonViolation,SIGNAL(toggled(bool)), SLOT(slotRBViolation(bool)));
    connect(ui->radioButtonChecked,SIGNAL(toggled(bool)), SLOT(slotRBChecked(bool)));
    connect(ui->radioButtonAll,SIGNAL(toggled(bool)), SLOT(slotRBAll(bool)));
    connect(ui->checkBoxFlagNotJob,SIGNAL(toggled(bool)), SLOT(slotRBAllNotJob(bool)));

    connect(ui->lineEditFindString,SIGNAL(textChanged(const QString&)),SLOT(slotTextChanged(const QString&)));
    connect(ui->lineEditFindString,SIGNAL(returnPressed()),SLOT(slotFindAVP()));

    disconnect(ui->pushButton_4, SIGNAL(pressed()),this, SLOT(slotNext()));
    disconnect(ui->pushButtonPreview, SIGNAL(pressed()),this, SLOT(slotPrevious()));

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
const QDateTime DAddTask::getDateRealization() const
{
    return ui->dateEditRealization->dateTime();
}

//=========================================================
bool DAddTask::isExistAVP(long idAVS)
{
    bool res = false;
    for(auto it = listAVP.begin(); it!=listAVP.end();it++)
    {
//        qDebug()<<"(*it).first="<<(*it).first<<" idAVS="<<idAVS;
        if((*it).first == idAVS)
            res =true;
    }
    return res;
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
        try
        {
            for( int i=0; i<selectedRows.count();++i)
            {
     //            qDebug()<<ui->tableWidget->item(selectedRows[i].row(),0)->text();
                 if(!isExistAVP(ui->tableWidget->item(selectedRows[i].row(),6)->text().toLong()))
                 {
                     ui->listWidgetJobTasks->addItem(ui->tableWidget->item(selectedRows[i].row(),0)->text());
                     listAVP.insert(std::make_pair(ui->tableWidget->item(selectedRows[i].row(),6)->text().toLong(),ui->tableWidget->item(selectedRows[i].row(),0)->text()));
                 }
            }
            //            for(auto item: listAVP)
            //                qDebug()<<"==>"<<item.first<<" "<<item.second;
        }
        catch(std::exception &e)
        {
            qDebug()<<e.what();
        }
    }
}

//=========================================================
void DAddTask::slotDelete()
{
   for(int i=0; i<ui->listWidgetJobTasks->count();)
   {
       if(ui->listWidgetJobTasks->item(i)->isSelected())
       {
//           qDebug()<<"\n===========Выбранный элемент = "<<ui->listWidgetJobTasks->item(i)->text();
            for(auto it = listAVP.begin(); it!=listAVP.end();it++)
            {
//                qDebug()<<"LIST_AVP="<<(*it).second<<"   ITEM="<<ui->listWidgetJobTasks->item(i)->text();
                if((*it).second == ui->listWidgetJobTasks->item(i)->text())
                {
//                    qDebug()<<"Delete AVP ="<<(*it).second;
                    it = listAVP.erase(it);
//                    for(auto item: listAVP)
//                       qDebug()<<"==>"<<item.first<<" "<<item.second;
                    break;
                }
            }
            ui->listWidgetJobTasks->takeItem(i);
       }
       else
           i++;
   }
//   for(auto item: listAVP)
//       qDebug()<<"<=="<<item.first<<" "<<item.second;
}

//=========================================================
void DAddTask::clearLineFindAVP()
{
    ui->lineEditFindString->clear();
}

//=========================================================
void DAddTask::initAVS()
{
    QString sql="";
    QString itemAVS;
    try
    {
       ui->comboBoxAVS->clear();
       ui->comboBoxAVS->addItem("Все");
       sql="SELECT \"ID\",\"URL\",\"NameAVS\" FROM avs;";
       if(query->exec(sql))
       {
           while(query->next())
           {
               itemAVS = query->value(2).toString();itemAVS +=" (";
               itemAVS += query->value(1).toString();itemAVS +=" )";
               ui->comboBoxAVS->addItem(itemAVS);
           }
       }
       else
           qDebug()<<query->lastError().text();
       m_currentIdAVS = -1;
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
void DAddTask::initComboBoxUser()
{
    QString sql="";
    try
    {
        ui->comboBoxUser->clear();

        sql = "SELECT \"ID\",\"FIO\" FROM \"User\" WHERE \"FlagJob\" = \'true\';";

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
    ui->dateEditRealization->setDate(QDate::currentDate().addDays(7));
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

        sql = "SELECT \"StatusName\",\"ID\" FROM \"TaskStatus\" WHERE \"StatusName\"='Назначена';";

        if(query->exec(sql))
        {
            while(query->next())
            {
                if((query->value(1).toInt()!= 5) && (query->value(1).toInt() != 6))
                {
                    ui->comboBoxStatus->addItem(query->value(0).toString());
                }

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
int DAddTask::countAVP(long idAVS)
{
    int count = 0;
    QString sql, tmp;
    try
    {
        if(idAVS == -1)
        {
            sql = "SELECT COUNT(*) FROM avp;";
        }
        else
        {
            sql = "SELECT COUNT(*) FROM avp WHERE \"ID_AVS\"="+ tmp.setNum(idAVS)+ ";";
        }
        if(query->exec(sql))
        {
            if(query->next())
                count = query->value(0).toInt();
        }
        else
            qDebug()<<query->lastError().text();
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
    return count;
}

//=========================================================
int DAddTask::countAVPNotJob(long idAVS)
{
    int count = 0;
    QString sql, tmp;
    try
    {
        if(idAVS == -1)
        {
            sql = "SELECT COUNT(*) FROM avp WHERE \"ID\" NOT IN (SELECT \"ID_AVP\" FROM \"Task\");";
        }
        else
        {
            sql = "SELECT COUNT(*) FROM avp WHERE \"ID\" NOT IN (SELECT \"ID_AVP\" FROM \"Task\") AND \"ID_AVS\"="+ tmp.setNum(idAVS)+ ";";
        }
        if(query->exec(sql))
        {
            if(query->next())
                count = query->value(0).toInt();
        }
        else
            qDebug()<<query->lastError().text();
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
    return count;
}


//=========================================================
void DAddTask::slotPrevious()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QString tmp;
    if(m_currentNumberPage>1)
    {
        ui->pushButtonNext->setEnabled(true);
        m_currentNumberPage--;
        initTableListAVP(m_currentNumberPage,m_currentIdAVS, m_currentState);
        ui->lineEditNumberPage->setText(tmp.setNum(m_currentNumberPage));
        if(m_currentNumberPage == 1)
           ui->pushButtonPreview->setEnabled(false);
    }
    QApplication::restoreOverrideCursor();
}

//=========================================================
void DAddTask::slotNext()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QString tmp;
    int page_count = (m_countAVP/1000);
    if( m_countAVP%1000 > 0)
        page_count++;

    if(m_currentNumberPage <= page_count)
    {
        ui->pushButtonPreview->setEnabled(true);
        m_currentNumberPage++;
        initTableListAVP(m_currentNumberPage,m_currentIdAVS,m_currentState);
        ui->lineEditNumberPage->setText(tmp.setNum(m_currentNumberPage));
        if(m_currentNumberPage == page_count)
            ui->pushButtonNext->setEnabled(false);
    }
    QApplication::restoreOverrideCursor();
}

//=========================================================
void DAddTask::slotChangeNumberPage()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    qDebug()<<ui->lineEditNumberPage->text();
    m_currentNumberPage = ui->lineEditNumberPage->text().toInt();
    int page_count = (m_countAVP/1000);
    if( m_countAVP%1000 > 0)
        page_count++;

    if(m_currentNumberPage<=page_count)
    {
        if( (m_currentNumberPage > 1) && (m_currentNumberPage < page_count) )
        {
            ui->pushButtonPreview->setEnabled(true);
            ui->pushButtonNext->setEnabled(true);
        }
        else if(m_currentNumberPage == 1)
        {
            ui->pushButtonPreview->setEnabled(false);
            ui->pushButtonNext->setEnabled(true);
        }
        else if(m_currentNumberPage == page_count)
        {
            ui->pushButtonNext->setEnabled(false);
            ui->pushButtonPreview->setEnabled(true);
        }
        ui->lineEditFindString->setFocus();
        initTableListAVP(m_currentNumberPage,m_currentIdAVS,m_currentState);
    }
    QApplication::restoreOverrideCursor();
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
              "avp.\"ID\", "
              "dd.\"DownloadStatus\" "
              "FROM \"avp\" "
              "INNER JOIN \"AVPattribute\" aa ON aa.\"ID_AVP\" = avp.\"ID\" "
              "LEFT JOIN \"DownloadData\" dd ON dd.\"ID_AVP\" = avp.\"ID\" AND dd.\"ResourceName\" =\'Kinopoisk\' ";

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

                    QTableWidgetItem *newItem4 = ((MainWindow*)parent())->initViolations(query->value(4).toInt(),1);
                    ui->tableWidget->setItem(row, 4, newItem4);

                    QTableWidgetItem *newItem5 = new QTableWidgetItem();
                    newItem5->setText(query->value(4).toString());
                    ui->tableWidget->setItem(row,5, newItem5);

                    QTableWidgetItem *newItem11 = new QTableWidgetItem();
                    if(query->value(5).toString() == "Yes")
                    {
                        QIcon icon11;
                        icon11.addFile(QString::fromUtf8(":/icons/icons/attach.ico"), QSize(), QIcon::Normal, QIcon::Off);
                        newItem11->setIcon(icon11);
                        newItem11->setText("Есть");
                    }
                    else
                        newItem11->setText("Нет");
                    newItem11->setFlags(newItem11->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidget->setItem(row,6, newItem11);//Рецензии
//                    qDebug()<<"DownloadStatus = "<<query->value(5).toString();

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
int DAddTask::countAVP_Analysis(int state,long idAVS)
{
//    qDebug()<<__PRETTY_FUNCTION__;
    int count = 0;
    QString sql, tmp;
    try
    {
        if((state == 1) && (idAVS != -1))// С нарушениями
        {
            sql = "SELECT \"ID\" FROM avp WHERE \"ID_AVS\"="+ tmp.setNum(idAVS)+ "AND \"ID\" IN ( SELECT \"ID_AVP\" FROM \"AnalysisResult\" WHERE \"ID_Violation\" !=12 )";
        }
        if((state == 1) && (idAVS == -1))// С нарушениями
        {
            sql = "SELECT \"ID\" FROM avp WHERE \"ID\" IN ( SELECT \"ID_AVP\" FROM \"AnalysisResult\" WHERE \"ID_Violation\" !=12 )";
        }
        else if((state == 2) && (idAVS != -1))// Все проверенные
        {
            sql = "SELECT \"ID\" FROM avp WHERE \"ID_AVS\"="+ tmp.setNum(idAVS)+ " AND \"ID\" IN ( SELECT \"ID_AVP\" FROM \"AnalysisResult\" )";
        }
        else if((state == 2) && (idAVS == -1))// Все проверенные
        {
            sql = "SELECT \"ID\" FROM avp WHERE \"ID\" IN ( SELECT \"ID_AVP\" FROM \"AnalysisResult\" )";
        }

        if(m_flagNotJob)
            sql+=" AND avp.\"ID\" NOT IN (SELECT \"ID_AVP\" FROM \"Task\")";

        sql+=" GROUP BY \"ID\";";
//        qDebug()<<"sql="<<sql;
        if(query->exec(sql))
        {
            count = query->size();
        }
        else
            qDebug()<<query->lastError().text();
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
    return count;
}

//=========================================================
void DAddTask::slotRBViolation(bool state)
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    if(state)
       initTableListAVP(1, m_currentIdAVS, 1);

    m_currentState = 1;
    m_currentNumberPage = 1;
    initSelectorPage(m_currentNumberPage,m_currentState);
    QApplication::restoreOverrideCursor();
}

//=========================================================
void DAddTask::slotRBChecked(bool state)
{
    qDebug()<<__PRETTY_FUNCTION__;
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    if(state)
       initTableListAVP(1, m_currentIdAVS, 2);

    m_currentState = 2;
    m_currentNumberPage = 1;
    initSelectorPage(m_currentNumberPage,m_currentState);
    QApplication::restoreOverrideCursor();
}

//=========================================================
void DAddTask::slotRBAll(bool state)
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    if(state)
        initTableListAVP(1, m_currentIdAVS, m_currentState);

    m_currentState = -1;
    m_currentNumberPage = 1;
    initSelectorPage(m_currentNumberPage, m_currentState);
    QApplication::restoreOverrideCursor();
}

//=========================================================
void DAddTask::slotRBAllNotJob(bool state)
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    m_flagNotJob = state;
    m_currentNumberPage = 1;

    initTableListAVP(m_currentNumberPage, m_currentIdAVS, m_currentState);
    initSelectorPage(m_currentNumberPage, m_currentState);
    QApplication::restoreOverrideCursor();
}

//=========================================================
void DAddTask::initSelectorPage(int numberPage, int state)
{
//    qDebug()<<__PRETTY_FUNCTION__;
    m_currentNumberPage = numberPage;
    if((state == -1) && (m_flagNotJob))
       m_countAVP = countAVPNotJob(m_currentIdAVS);
    else if((state == -1) && (!m_flagNotJob))
       m_countAVP = countAVP(m_currentIdAVS);
    else if(state == 1)
       m_countAVP = countAVP_Analysis(1,m_currentIdAVS);
    else if(state == 2)
       m_countAVP = countAVP_Analysis(2,m_currentIdAVS);

//    qDebug()<<"m_countAVP = "<<m_countAVP;
    int page_count = m_countAVP/1000;
    if(m_countAVP%1000 > 0)
        page_count++;

    QString tmp;
    QString text="из ";text+=tmp.setNum(page_count); text+=" (всего АВП ";text+=tmp.setNum(m_countAVP);text+=")";
    ui->lineEditNumberPage->setValidator( new QIntValidator(1,(page_count), this) );
    ui->lineEditNumberPage->setText(tmp.setNum(m_currentNumberPage));
    ui->labelCountAVP->setText(text);
    ui->pushButtonPreview->setEnabled(false);
}

//=========================================================
void DAddTask::initStateAVP()
{
    ui->radioButtonAll->setChecked(true);
}

//=========================================================
void DAddTask::initTableListAVP(int numberPage, long idAVS, int state)
{
    qDebug()<<__PRETTY_FUNCTION__;
    QString sql="",tmp;
    try
    {
        ui->tableWidget->setSortingEnabled(false);
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);
        listAVP.clear();
        ui->listWidgetJobTasks->clear();

        sql = "SELECT avp.\"NameRus\","
              "avp.\"URL\","
              "aa.\"YearOfRelease\","
              "aa.\"FilmMaker\","
              "avp.\"ID\", "
              "dd.\"DownloadStatus\", "
              "aa.\"Age\" "
              "FROM \"avp\" "
              "LEFT JOIN \"DownloadData\" dd ON dd.\"ID_AVP\" = avp.\"ID\" AND dd.\"ResourceName\" =\'Kinopoisk\'"
              "INNER JOIN \"AVPattribute\" aa ON aa.\"ID_AVP\" = avp.\"ID\"";
        if( idAVS != -1)
        {
            sql+=" WHERE \"ID_AVS\"="; sql+=tmp.setNum(idAVS);
//            if(m_flagNotJob)
//                sql+=" AND avp.\"ID\" NOT IN (SELECT \"ID_AVP\" FROM \"Task\")";
        }

        if( state == 1)
        {
            if( idAVS != -1)
            {
                sql+=" AND avp.\"ID\" IN (SELECT \"ID_AVP\" FROM \"AnalysisResult\" WHERE \"ID_Violation\"!=12)";
            }
            else
            {
                sql+=" WHERE avp.\"ID\" IN (SELECT \"ID_AVP\" FROM \"AnalysisResult\" WHERE \"ID_Violation\"!=12)";
            }
            if(m_flagNotJob)
                sql+=" AND avp.\"ID\" NOT IN (SELECT \"ID_AVP\" FROM \"Task\")";
        }
        else if( state == 2)
        {
            if( idAVS != -1)
            {
                sql+=" AND avp.\"ID\" IN (SELECT \"ID_AVP\" FROM \"AnalysisResult\")";
            }
            else
            {
                sql+=" WHERE avp.\"ID\" IN (SELECT \"ID_AVP\" FROM \"AnalysisResult\")";
            }
            if(m_flagNotJob)
                sql+=" AND avp.\"ID\" NOT IN (SELECT \"ID_AVP\" FROM \"Task\")";
        }
        else if( state == -1)
        {
            if(m_flagNotJob)
//                sql+=" WHERE avp.\"ID\" NOT IN (SELECT \"ID_AVP\" FROM \"Task\")";
            sql+=" AND avp.\"ID\" NOT IN (SELECT \"ID_AVP\" FROM \"Task\")";
        }

        sql+=" ORDER BY avp.\"ID\" LIMIT 1000 OFFSET ";
        sql+=tmp.setNum((numberPage-1)*1000);sql+=";";
        qDebug()<<"SQL = "<<sql;

        if(query->exec(sql))
        {
            int row=0;
            m_countCurrentAVP = query->size();
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

                QTableWidgetItem *newItem3 = new QTableWidgetItem();// режиссер
                QIcon icon4;
                icon4.addFile(QString::fromUtf8(":/icons/icons/film_maker1.ico"), QSize(), QIcon::Normal, QIcon::Off);
                newItem3->setIcon(icon4);
                newItem3->setText(query->value(3).toString());
                newItem3->setFlags(newItem3->flags() ^ Qt::ItemIsEditable);
                ui->tableWidget->setItem(row,3, newItem3);

                QTableWidgetItem *newItem21 = new QTableWidgetItem(); // возраст
                newItem21->setText(query->value(6).toString());
                newItem21->setFlags(newItem21->flags() ^ Qt::ItemIsEditable);
                ui->tableWidget->setItem(row,4, newItem21);

                QTableWidgetItem *newItem4 = ((MainWindow*)parent())->initViolations(query->value(4).toInt(),1);
                ui->tableWidget->setItem(row, 5, newItem4);

                QTableWidgetItem *newItem5 = new QTableWidgetItem();
                newItem5->setText(query->value(4).toString());
                ui->tableWidget->setItem(row,6, newItem5);

                QTableWidgetItem *newItem11 = new QTableWidgetItem();
                if(query->value(5).toString() == "Yes")
                {
                    QIcon icon11;
                    icon11.addFile(QString::fromUtf8(":/icons/icons/attach.ico"), QSize(), QIcon::Normal, QIcon::Off);
                    newItem11->setIcon(icon11);
                    newItem11->setText("Есть");
                }
                else
                    newItem11->setText("Нет");
                newItem11->setFlags(newItem11->flags() ^ Qt::ItemIsEditable);
                ui->tableWidget->setItem(row,7, newItem11);//Рецензии

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
    }

}

//=========================================================
long DAddTask::idAVS(QString nameAVS)
{
    long id = -1;
    QString sql;
    try
    {
        sql = "SELECT \"ID\" FROM avs WHERE \"NameAVS\"=\'"+ nameAVS +"\';";
        if(query->exec(sql))
        {
            if(query->next())
                id = query->value(0).toInt();
        }
        else
            qDebug()<<query->lastError().text();
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
    return id;
}
\
//=========================================================
void DAddTask::slotSelectAVS(QString nameAVS)
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    QString avs;
    long id = -1;
    if(nameAVS == "Все")
    {
        m_currentIdAVS = -1;
        initTableListAVP(1,m_currentIdAVS,m_currentState);
    }
    else
    {
        avs = nameAVS.mid(0,nameAVS.indexOf('(')-1);
        id = idAVS(avs);
        m_currentIdAVS = id;
        initTableListAVP(1,m_currentIdAVS,m_currentState);
    }

    m_currentNumberPage = 1;

    if(m_currentState == -1)
        m_countAVP = countAVP(m_currentIdAVS);
    else if(m_currentState == 1)
        m_countAVP = countAVP_Analysis(1,m_currentIdAVS);
    else if(m_currentState == 2)
        m_countAVP = countAVP_Analysis(2,m_currentIdAVS);
    else if(m_currentState == 3)
        m_countAVP = countAVPNotJob(m_currentIdAVS);

    QString tmp;
    QString text="из ";text+=tmp.setNum(m_countAVP/1000); text+=" (всего АВП ";text+=tmp.setNum(m_countAVP);text+=")";
    ui->lineEditNumberPage->setValidator( new QIntValidator(1,(m_countAVP/1000), this) );
    ui->lineEditNumberPage->setText(tmp.setNum(m_currentNumberPage));
    ui->labelCountAVP->setText(text);
    ui->pushButtonPreview->setEnabled(false);
    QApplication::restoreOverrideCursor();
}

//=========================================================
void DAddTask::slotApply()
{
    if(QMessageBox::warning(this,"Внимание","Вы точно  уверены в дате выполнения?","Да","Нет")==0)
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
