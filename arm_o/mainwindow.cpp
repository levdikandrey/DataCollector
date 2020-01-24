#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "daddavp.h"
#include "cimportdata.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QSize>
#include <QScreen>
#include <QCheckBox>
#include <QSpinBox>
#include <QMenu>
#include <QDesktopServices>
#include <QProgressDialog>

#include <QMessageBox>
#include <QPushButton>
#include <QFileDialog>
#include <QDebug>
#include <QThread>

#include <QSqlDatabase>
#include <QSqlError>

extern QSqlDatabase db;
extern QString currentUserName;
//DEnter *dEnter1;
//=========================================================
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->actionClearDB->setEnabled(false);
    initDialog();

    connect(ui->actionExit, SIGNAL(triggered()), SLOT(slotExit()));
    connect(ui->actionAbout, SIGNAL(triggered()), SLOT(slotAbout()));
    connect(ui->actionExcel, SIGNAL(triggered()), SLOT(slotImportData()));
    connect(ui->actionKinopoisk, SIGNAL(triggered()), SLOT(slotImportKinopoisk()));
    connect(ui->actionAddAVP, SIGNAL(triggered()), SLOT(slotAddAVP()));
    connect(ui->actionClearDB, SIGNAL(triggered()), SLOT(slotClearDB()));
    connect(ui->actionGroup, SIGNAL(triggered()), SLOT(slotGroup()));
    connect(ui->actionUser, SIGNAL(triggered()), SLOT(slotUser()));
    connect(ui->actionChangeUser, SIGNAL(triggered()), SLOT(slotChangeUser()));
    connect(ui->actionViolation, SIGNAL(triggered()), SLOT(slotViolation()));
    connect(ui->actionStatus, SIGNAL(triggered()), SLOT(slotStatus()));

    connect(ui->radioButtonViolation,SIGNAL(toggled(bool)), SLOT(slotRBViolation(bool)));
    connect(ui->radioButtonChecked,SIGNAL(toggled(bool)), SLOT(slotRBChecked(bool)));
    connect(ui->radioButtonAll,SIGNAL(toggled(bool)), SLOT(slotRBAll(bool)));
    connect(ui->lineEditFindString,SIGNAL(returnPressed()),SLOT(slotFindAVP()));
    connect(ui->lineEditFindString,SIGNAL(textChanged(const QString&)),SLOT(slotTextChanged(const QString&)));

    connect(ui->tableWidgetCurrentTasks, SIGNAL(cellDoubleClicked(int, int)),this, SLOT(slotEditTask(int, int)));
    connect(ui->tableWidgetMyTasks, SIGNAL(cellDoubleClicked(int, int)),this, SLOT(slotEditMyTask(int, int)));
    connect(ui->tableWidgetAudit, SIGNAL(cellDoubleClicked(int, int)),this, SLOT(slotEditAudit(int, int)));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)),this, SLOT(slotCurrentChanged(int)));

    query = nullptr;
    queryViolation = nullptr;


    initDB();
    initAVS();
    initTableAVP();
    initTableTask();
//    initTableMyTask();

    initComboBoxUser(ui->comboBoxUser);
    initComboBoxStatus(ui->comboBoxStatus);
    initComboBoxPriority(ui->comboBoxPriority);

    ui->tableWidgetMyTasks->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidgetMyTasks, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotContextMenuRequested(QPoint)));

    ui->tableWidgetAudit->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidgetAudit, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotContextMenuRequested(QPoint)));

    cImportData = new CImportData();
    cImportData->moveToThread(&importDataThread);
    connect(&importDataThread, &QThread::finished, cImportData, &QObject::deleteLater);
    connect(this, &MainWindow::operate,cImportData, &CImportData::doWork);
    connect(cImportData, &CImportData::resultReady, this, &MainWindow::handleResults);
    importDataThread.start();

    cImportDataKinopoisk = new CImportDataKinopoisk();
    cImportDataKinopoisk->moveToThread(&importDataThreadKinopoisk);
    connect(&importDataThreadKinopoisk, &QThread::finished, cImportDataKinopoisk, &QObject::deleteLater);
    connect(this, &MainWindow::operateKinopoisk,cImportDataKinopoisk, &CImportDataKinopoisk::doWork);
    connect(cImportDataKinopoisk, &CImportDataKinopoisk::resultReady, this, &MainWindow::handleResultsKinopoisk);
    importDataThreadKinopoisk.start();


    dImportDataOutExcel = new DImportDataOutExcel(this);
    dImportDataOutExcel->setModal(false);

    dAddAVP = new DAddAVP(this);
    dAddAVP->setModal(false);

    dAddTask = new DAddTask(this);
    dEditTask = new DEditTask(this);
    dEditTaskUser = new DEditTaskUser(this);
    dGroup = new DGroup(this);
    dUser = new DUser(this);
    dViolation = new DViolation(this);
    dEnter1 = new DEnter();
    dEditAudit = new DEditAudit(this);

    m_currentIdAVS = -1;
    m_currentState = -1;
    m_currentNumberPage = 1;
    m_countAVP = countAVP();
    QString tmp;
    QString text="из ";text+=tmp.setNum(m_countAVP/1000); text+=" (всего АВП ";text+=tmp.setNum(m_countAVP);text+=")";
    ui->lineEditNumberPage->setValidator( new QIntValidator(1,(m_countAVP/1000), this) );
    ui->labelCountAVP->setText(text);
    ui->pushButtonPreview->setEnabled(false);

    m_timer.setInterval(1000);
    m_timer.setSingleShot(true);
    connect(&m_timer, &QTimer::timeout, this, [&]
            {
                slotFindAVP();
            });
}

//=========================================================
MainWindow::~MainWindow()
{
//    qDebug()<<"~MainWindow()";
    importDataThread.quit();
    importDataThread.terminate();
    importDataThreadKinopoisk.quit();
    importDataThreadKinopoisk.terminate();
    delete cImportData;

    delete ui;
    delete query;
}

//=========================================================
void MainWindow::initDialog()
{
//    ui->tableWidgetAVP->verticalHeader()->hide();

//    ui->tableWidgetAVP->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
//    ui->tableWidgetAVP->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->tableWidgetAVP->horizontalHeader()->resizeSection(0, 250);
    ui->tableWidgetAVP->horizontalHeader()->resizeSection(1, 300);
    ui->tableWidgetAVP->horizontalHeader()->resizeSection(2, 130);
    ui->tableWidgetAVP->horizontalHeader()->resizeSection(3, 80);
    //    ui->tableWidgetAVP->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Stretch);
    ui->tableWidgetAVP->horizontalHeader()->resizeSection(4, 250);
    ui->tableWidgetAVP->horizontalHeader()->resizeSection(5, 150);
    ui->tableWidgetAVP->horizontalHeader()->resizeSection(6, 130);
//    ui->tableWidgetAVP->horizontalHeader()->resizeSection(7, 250);
    ui->tableWidgetAVP->horizontalHeader()->setSectionResizeMode(7,QHeaderView::Stretch);
    ui->tableWidgetAVP->horizontalHeader()->resizeSection(8, 0);

    ui->tableWidgetCurrentTasks->horizontalHeader()->resizeSection(0, 200);//Название АВП
    ui->tableWidgetCurrentTasks->horizontalHeader()->resizeSection(1, 300);//URL
    ui->tableWidgetCurrentTasks->horizontalHeader()->resizeSection(2, 150);//ФИО
    ui->tableWidgetCurrentTasks->horizontalHeader()->resizeSection(3, 110);//Дата назначения
    ui->tableWidgetCurrentTasks->horizontalHeader()->resizeSection(4, 100);//Приоритет
    ui->tableWidgetCurrentTasks->horizontalHeader()->resizeSection(5, 150);//Статус
    ui->tableWidgetCurrentTasks->horizontalHeader()->resizeSection(6, 80);//%
    ui->tableWidgetCurrentTasks->horizontalHeader()->resizeSection(7, 250);//Нарушения
    ui->tableWidgetCurrentTasks->horizontalHeader()->setSectionResizeMode(8, QHeaderView::Stretch);//Комментарии
    ui->tableWidgetCurrentTasks->horizontalHeader()->resizeSection(9, 0);//ИД
    ui->tableWidgetCurrentTasks->horizontalHeader()->resizeSection(10, 0);//ИД_AVP

    ui->tableWidgetMyTasks->horizontalHeader()->resizeSection(0,250);//Название АВП
    ui->tableWidgetMyTasks->horizontalHeader()->resizeSection(1,300);//URL
    ui->tableWidgetMyTasks->horizontalHeader()->resizeSection(2,110);//Дата назначения
    ui->tableWidgetMyTasks->horizontalHeader()->resizeSection(3,100);//Приоритет
    ui->tableWidgetMyTasks->horizontalHeader()->resizeSection(4,150);//Статус
    ui->tableWidgetMyTasks->horizontalHeader()->resizeSection(5,80);//%
    ui->tableWidgetMyTasks->horizontalHeader()->resizeSection(6,300);//Нарушения
    ui->tableWidgetMyTasks->horizontalHeader()->setSectionResizeMode(7,QHeaderView::Stretch);//Комментарии
    ui->tableWidgetMyTasks->horizontalHeader()->resizeSection(8, 0);//ID
    ui->tableWidgetMyTasks->horizontalHeader()->resizeSection(9, 0);//ID_AVP

    ui->tableWidgetAudit->horizontalHeader()->resizeSection(0,250);//Название АВП
    ui->tableWidgetAudit->horizontalHeader()->resizeSection(1,300);//URL
    ui->tableWidgetAudit->horizontalHeader()->resizeSection(2, 150);//ФИО
    ui->tableWidgetAudit->horizontalHeader()->resizeSection(3,110);//Дата назначения
    ui->tableWidgetAudit->horizontalHeader()->resizeSection(4,100);//Приоритет
    ui->tableWidgetAudit->horizontalHeader()->resizeSection(5,150);//Статус
    ui->tableWidgetAudit->horizontalHeader()->resizeSection(6,80);//%
    ui->tableWidgetAudit->horizontalHeader()->resizeSection(7,300);//
    ui->tableWidgetAudit->horizontalHeader()->setSectionResizeMode(8,QHeaderView::Stretch);//Комментарии
    ui->tableWidgetAudit->horizontalHeader()->resizeSection(9, 0);//ID
    ui->tableWidgetAudit->horizontalHeader()->resizeSection(10, 0);//ID_AVP
}

//=========================================================
void MainWindow::initDB()
{
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("avpDB");
    db.setUserName("postgres");
    db.setPassword("postgres");
    bool ok = db.open();
    if(ok)
    {
        QPalette palette;
        QBrush brush1(QColor(85, 255, 127, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);

        ui->labelStateConnectDB->setText("Установлено соединение с БД");
        ui->labelStateConnectDB->setPalette(palette);
        ui->labelStateConnectDB->setAutoFillBackground(true);

        query = new QSqlQuery(db);
        queryViolation = new QSqlQuery(db);
    }
    else
    {
        QMessageBox::critical(this, tr("Ошибка подключения к БД АВП!"), db.lastError().text());
    }
}

//=========================================================
void MainWindow::initAVS()
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
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
void MainWindow::initComboBoxUser(QComboBox *comboBox)
{
    QString sql="";
    comboBox->clear();
    try
    {
        sql = "SELECT * FROM \"User\";";
        if(query->exec(sql))
        {
            while(query->next())
            {
                comboBox->addItem(query->value(1).toString());
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
void MainWindow::initComboBoxStatus(QComboBox *comboBox)
{
    QString sql="";
    comboBox->clear();
    try
    {
        sql = "SELECT * FROM \"TaskStatus\";";
        if(query->exec(sql))
        {
            while(query->next())
            {
                comboBox->addItem(query->value(1).toString());
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
void MainWindow::initComboBoxPriority(QComboBox *comboBox)
{
    QString sql="";
    comboBox->clear();
    try
    {
        sql = "SELECT * FROM \"Priority\";";
        if(query->exec(sql))
        {
            while(query->next())
            {
                comboBox->addItem(query->value(1).toString());
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
void MainWindow::initTableAudit()
{
    QString sql="";
    try
    {
        ui->tableWidgetAudit->clearContents();
        ui->tableWidgetAudit->setRowCount(0);

        sql = "SELECT avp.\"NameRus\", "
              "avp.\"URL\","
              "u.\"FIO\","
              "\"Task\".\"DateAppoint\","
              "p.\"NamePriority\","
              "ts.\"StatusName\","
              "\"Task\".\"PercentCompleted\","
              "\"Task\".\"Comment\","
              "\"Task\".\"ID\","
              "\"Task\".\"ID_AVP\" "
              "FROM \"Task\" "
              "INNER JOIN avp ON \"Task\".\"ID_AVP\" = avp.\"ID\" "
              "INNER JOIN \"User\" u ON \"Task\".\"ID_User\" = u.\"ID\" "
              "INNER JOIN \"TaskStatus\" ts ON \"Task\".\"ID_TaskStatus\" = ts.\"ID\" "
              "INNER JOIN \"Priority\" p ON \"Task\".\"ID_Priority\" = p.\"ID\" WHERE ts.\"StatusName\" =\'";
        sql += "Проверена оператором";
        sql += "\' ORDER BY \"Task\".\"ID\";";
//        qDebug()<<"sql = "<<sql;
        if(query->exec(sql))
        {
            int row = 0;
            while(query->next())
            {
                ui->tableWidgetAudit->setRowCount(row+1);

                QTableWidgetItem *newItem = new QTableWidgetItem();
                QIcon icon;
                icon.addFile(QString::fromUtf8(":/icons/icons/film_projector_cinema.ico"), QSize(), QIcon::Normal, QIcon::Off);
                newItem->setIcon(icon);
                newItem->setText(query->value(0).toString());
                newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit->setItem(row,0, newItem);//Название АВП

                QTableWidgetItem *newItem1 = new QTableWidgetItem();
                QIcon icon1;
                icon1.addFile(QString::fromUtf8(":/icons/icons/web.ico"), QSize(), QIcon::Normal, QIcon::Off);
                newItem1->setIcon(icon1);
                newItem1->setText(query->value(1).toString());
                newItem1->setFlags(newItem1->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit->setItem(row,1, newItem1);//URL

                QTableWidgetItem *newItem2 = new QTableWidgetItem();
                QIcon icon5;
                icon5.addFile(QString::fromUtf8(":/icons/icons/user.ico"), QSize(), QIcon::Normal, QIcon::Off);
                newItem2->setIcon(icon5);
                newItem2->setText(query->value(2).toString());
                newItem2->setFlags(newItem2->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit->setItem(row,2, newItem2);//ФИО

                QTableWidgetItem *newItem3 = new QTableWidgetItem();
                QIcon icon3;
                icon3.addFile(QString::fromUtf8(":/icons/icons/date2.ico"), QSize(), QIcon::Normal, QIcon::Off);
                newItem3->setIcon(icon3);
                newItem3->setText(query->value(3).toDate().toString("yyyy-MM-dd"));
                newItem3->setFlags(newItem3->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit->setItem(row,3, newItem3);//Дата назначения задачи

                QTableWidgetItem *newItem4 = new QTableWidgetItem();
                newItem4->setText(query->value(4).toString());
                newItem4->setFlags(newItem4->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit->setItem(row,4, newItem4);//Приоритет

                QTableWidgetItem *newItem5 = new QTableWidgetItem();
                newItem5->setText(query->value(5).toString());
                newItem5->setFlags(newItem5->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit->setItem(row,5, newItem5);//Статус

                QTableWidgetItem *newItem6 = new QTableWidgetItem();
                newItem6->setText(query->value(6).toString()+"%");
                newItem6->setFlags(newItem6->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit->setItem(row,6, newItem6);//Процент завершенности


                QTableWidgetItem *newItem7 = initViolations(query->value(9).toInt());
                ui->tableWidgetAudit->setItem(row,7, newItem7);//Нарушения

                QTableWidgetItem *newItem8 = new QTableWidgetItem();
                newItem8->setText(query->value(7).toString());
                newItem8->setFlags(newItem8->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit->setItem(row,8, newItem8);

                QTableWidgetItem *newItem9 = new QTableWidgetItem();
                newItem9->setText(query->value(8).toString());
                newItem9->setFlags(newItem9->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit->setItem(row,9, newItem9);

                QTableWidgetItem *newItem10 = new QTableWidgetItem();
                newItem10->setText(query->value(9).toString());
                newItem10->setFlags(newItem10->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit->setItem(row,10, newItem10);

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
void MainWindow::initTableMyTask()
{
    QString sql="";
    try
    {
        ui->tableWidgetMyTasks->clearContents();
        ui->tableWidgetMyTasks->setRowCount(0);

        sql = "SELECT avp.\"NameRus\", "
              "avp.\"URL\","
              "\"Task\".\"DateAppoint\","
              "p.\"NamePriority\","
              "ts.\"StatusName\","
              "\"Task\".\"PercentCompleted\","
              "\"Task\".\"Comment\","
              "\"Task\".\"ID\","
              "\"Task\".\"ID_AVP\" "
              "FROM \"Task\" "
              "INNER JOIN avp ON \"Task\".\"ID_AVP\" = avp.\"ID\" "
              "INNER JOIN \"User\" u ON \"Task\".\"ID_User\" = u.\"ID\" "
              "INNER JOIN \"TaskStatus\" ts ON \"Task\".\"ID_TaskStatus\" = ts.\"ID\" "
              "INNER JOIN \"Priority\" p ON \"Task\".\"ID_Priority\" = p.\"ID\" WHERE u.\"FIO\" =\'";
        sql += currentUserName;
        sql += "\' ORDER BY \"Task\".\"ID\";";
//        qDebug()<<"sql = "<<sql;
        if(query->exec(sql))
        {
            int row = 0;
            while(query->next())
            {
                ui->tableWidgetMyTasks->setRowCount(row+1);

                QTableWidgetItem *newItem = new QTableWidgetItem();
                QIcon icon;
                icon.addFile(QString::fromUtf8(":/icons/icons/film_projector_cinema.ico"), QSize(), QIcon::Normal, QIcon::Off);
                newItem->setIcon(icon);
                newItem->setText(query->value(0).toString());
                newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetMyTasks->setItem(row,0, newItem);//Название АВП

                QTableWidgetItem *newItem1 = new QTableWidgetItem();
                QIcon icon1;
                icon1.addFile(QString::fromUtf8(":/icons/icons/web.ico"), QSize(), QIcon::Normal, QIcon::Off);
                newItem1->setIcon(icon1);
                newItem1->setText(query->value(1).toString());
                newItem1->setFlags(newItem1->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetMyTasks->setItem(row,1, newItem1);//URL

                QTableWidgetItem *newItem3 = new QTableWidgetItem();
                QIcon icon3;
                icon3.addFile(QString::fromUtf8(":/icons/icons/date2.ico"), QSize(), QIcon::Normal, QIcon::Off);
                newItem3->setIcon(icon3);
                newItem3->setText(query->value(2).toDate().toString("yyyy-MM-dd"));
                newItem3->setFlags(newItem3->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetMyTasks->setItem(row,2, newItem3);//Дата назначения задачи

                QTableWidgetItem *newItem4 = new QTableWidgetItem();
                newItem4->setText(query->value(3).toString());
                newItem4->setFlags(newItem4->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetMyTasks->setItem(row,3, newItem4);//Приоритет

                QTableWidgetItem *newItem5 = new QTableWidgetItem();
                newItem5->setText(query->value(4).toString());
                newItem5->setFlags(newItem5->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetMyTasks->setItem(row,4, newItem5);//Статус

                QTableWidgetItem *newItem6 = new QTableWidgetItem();
                newItem6->setText(query->value(5).toString()+"%");
                newItem6->setFlags(newItem6->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetMyTasks->setItem(row,5, newItem6);//Процент завершенности


                QTableWidgetItem *newItem7 = initViolations(query->value(8).toInt());
                ui->tableWidgetMyTasks->setItem(row,6, newItem7);//Нарушения

                QTableWidgetItem *newItem8 = new QTableWidgetItem();
                newItem8->setText(query->value(6).toString());
                newItem8->setFlags(newItem8->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetMyTasks->setItem(row,7, newItem8);

                QTableWidgetItem *newItem9 = new QTableWidgetItem();
                newItem9->setText(query->value(7).toString());
                newItem9->setFlags(newItem9->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetMyTasks->setItem(row,8, newItem9);

                QTableWidgetItem *newItem10 = new QTableWidgetItem();
                newItem10->setText(query->value(8).toString());
                newItem10->setFlags(newItem10->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetMyTasks->setItem(row,9, newItem10);

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
void MainWindow::initTableTask(bool)
{
    QString sql="";
    bool filterCheck = false;
    try
    {
        ui->tableWidgetCurrentTasks->clearContents();
        ui->tableWidgetCurrentTasks->setRowCount(0);

        sql = "SELECT avp.\"NameRus\", "
              "avp.\"URL\","
              "u.\"FIO\","
              "\"Task\".\"DateAppoint\","
              "p.\"NamePriority\","
              "ts.\"StatusName\","
              "\"Task\".\"PercentCompleted\","
              "avp.\"ViolationMask\","
              "\"Task\".\"Comment\","
              "\"Task\".\"ID\","
              "\"Task\".\"ID_AVP\" "
              "FROM \"Task\" "
              "INNER JOIN avp ON \"Task\".\"ID_AVP\" = avp.\"ID\" "
              "INNER JOIN \"User\" u ON \"Task\".\"ID_User\" = u.\"ID\" "
              "INNER JOIN \"TaskStatus\" ts ON \"Task\".\"ID_TaskStatus\" = ts.\"ID\" "
              "INNER JOIN \"Priority\" p ON \"Task\".\"ID_Priority\" = p.\"ID\"";
//        if(filter)
//        {
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
            if(ui->groupBoxPriority->isChecked())
            {
                if(!filterCheck)
                {
                    sql +=" WHERE p.\"NamePriority\" = \'";sql += ui->comboBoxPriority->currentText(); sql += "\'";
                }
                else
                {
                    sql +=" AND p.\"NamePriority\" = \'";sql += ui->comboBoxPriority->currentText(); sql += "\'";
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
//        }
        sql += " ORDER BY \"Task\".\"ID\";";
//        qDebug()<<"sql = "<<sql;

        if(query->exec(sql))
        {
            int row = 0;
            while(query->next())
            {
                ui->tableWidgetCurrentTasks->setRowCount(row+1);

                QTableWidgetItem *newItem = new QTableWidgetItem();
                QIcon icon;
                icon.addFile(QString::fromUtf8(":/icons/icons/film_projector_cinema.ico"), QSize(), QIcon::Normal, QIcon::Off);
                newItem->setIcon(icon);
                newItem->setText(query->value(0).toString());
                newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetCurrentTasks->setItem(row,0, newItem);//Название АВП

                QTableWidgetItem *newItem1 = new QTableWidgetItem();
                QIcon icon1;
                icon1.addFile(QString::fromUtf8(":/icons/icons/web.ico"), QSize(), QIcon::Normal, QIcon::Off);
                newItem1->setIcon(icon1);
                newItem1->setText(query->value(1).toString());
                newItem1->setFlags(newItem1->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetCurrentTasks->setItem(row,1, newItem1);//URL

                QTableWidgetItem *newItem2 = new QTableWidgetItem();
                QIcon icon5;
                icon5.addFile(QString::fromUtf8(":/icons/icons/user.ico"), QSize(), QIcon::Normal, QIcon::Off);
                newItem2->setIcon(icon5);
                newItem2->setText(query->value(2).toString());
                newItem2->setFlags(newItem2->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetCurrentTasks->setItem(row,2, newItem2);//ФИО

                QTableWidgetItem *newItem3 = new QTableWidgetItem();
                QIcon icon3;
                icon3.addFile(QString::fromUtf8(":/icons/icons/date2.ico"), QSize(), QIcon::Normal, QIcon::Off);
                newItem3->setIcon(icon3);
                newItem3->setText(query->value(3).toDate().toString("yyyy-MM-dd"));
                newItem3->setFlags(newItem3->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetCurrentTasks->setItem(row,3, newItem3);//Дата назначения задачи

                QTableWidgetItem *newItem4 = new QTableWidgetItem();
                newItem4->setText(query->value(4).toString());
                newItem4->setFlags(newItem4->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetCurrentTasks->setItem(row,4, newItem4);//Приоритет

                QTableWidgetItem *newItem5 = new QTableWidgetItem();
                newItem5->setText(query->value(5).toString());
                newItem5->setFlags(newItem5->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetCurrentTasks->setItem(row,5, newItem5);//Статус

                QTableWidgetItem *newItem6 = new QTableWidgetItem();
                newItem6->setText(query->value(6).toString()+"%");
                newItem6->setFlags(newItem6->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetCurrentTasks->setItem(row,6, newItem6);//Процент завершенности


                QTableWidgetItem *newItem7 = initViolations(query->value(10).toInt());
                ui->tableWidgetCurrentTasks->setItem(row,7, newItem7);//Нарушения

                QTableWidgetItem *newItem8 = new QTableWidgetItem();
                newItem8->setText(query->value(8).toString());
                newItem8->setFlags(newItem8->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetCurrentTasks->setItem(row,8, newItem8);

                QTableWidgetItem *newItem9 = new QTableWidgetItem();
                newItem9->setText(query->value(9).toString());
                newItem9->setFlags(newItem9->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetCurrentTasks->setItem(row,9, newItem9);

                QTableWidgetItem *newItem10 = new QTableWidgetItem();
                newItem10->setText(query->value(10).toString());
                newItem10->setFlags(newItem10->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetCurrentTasks->setItem(row,10, newItem10);

                row++;
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
int MainWindow::countAVP(long idAVS)
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
long MainWindow::idAVS(QString nameAVS)
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

//=========================================================
long MainWindow::getIdAVP(QString )
{
    long id = -1;
    QString sql;
    return id;
}

//=========================================================
int MainWindow::getIdUser(QString userName)
{
    int id = -1;
    QString sql;
    try
    {
        sql = "SELECT \"ID\" FROM \"User\" WHERE \"FIO\"=\'"+ userName +"\';";
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

//=========================================================
int MainWindow::getIdTaskStatus(QString taskStatus)
{
    int id = -1;
    QString sql;
    try
    {
        sql = "SELECT \"ID\" FROM \"TaskStatus\" WHERE \"StatusName\"=\'"+ taskStatus +"\';";
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

//=========================================================
int MainWindow::getIdPriority(QString namePriority)
{
    int id = -1;
    QString sql;
    try
    {
        sql = "SELECT \"ID\" FROM \"Priority\" WHERE \"NamePriority\"=\'"+ namePriority +"\';";
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

//=========================================================
void MainWindow::slotStateChanged(int state)
{
    if(state == Qt::Unchecked)
    {
    }
    else if(state == Qt::Checked)
    {
    }
}

//=========================================================
void MainWindow::slotSelectAVS(QString nameAVS)
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QString avs;
    long id = -1;
    if(nameAVS == "Все")
    {
        m_currentIdAVS = -1;
        initTableAVP(1,m_currentIdAVS,m_currentState);
    }
    else
    {
        avs = nameAVS.mid(0,nameAVS.indexOf('(')-1);
        id = idAVS(avs);
        m_currentIdAVS = id;
        initTableAVP(1,m_currentIdAVS,m_currentState);
    }

    m_currentNumberPage = 1;

    if(m_currentState == -1)
        m_countAVP = countAVP(m_currentIdAVS);
    else
        m_countAVP = m_countCurrentAVP;

    QString tmp;
    QString text="из ";text+=tmp.setNum(m_countAVP/1000); text+=" (всего АВП ";text+=tmp.setNum(m_countAVP);text+=")";
    ui->lineEditNumberPage->setValidator( new QIntValidator(1,(m_countAVP/1000), this) );
    ui->labelCountAVP->setText(text);
    ui->pushButtonPreview->setEnabled(false);
    QApplication::restoreOverrideCursor();
}

//=========================================================
void MainWindow::slotNext()
{
    QString tmp;
    if(m_currentNumberPage<=(m_countAVP/1000))
    {
        ui->pushButtonPreview->setEnabled(true);
        m_currentNumberPage++;
        initTableAVP(m_currentNumberPage,m_currentIdAVS,m_currentState);
        ui->lineEditNumberPage->setText(tmp.setNum(m_currentNumberPage));
        if(m_currentNumberPage == (m_countAVP/1000))
            ui->pushButtonNext->setEnabled(false);
    }
}

//=========================================================
void MainWindow::slotPrevious()
{
    QString tmp;
    if(m_currentNumberPage>1)
    {
        ui->pushButtonNext->setEnabled(true);
        m_currentNumberPage--;
        initTableAVP(m_currentNumberPage,m_currentIdAVS,m_currentState);
//        initTableAVP(m_currentNumberPage);
        ui->lineEditNumberPage->setText(tmp.setNum(m_currentNumberPage));
        if(m_currentNumberPage == 1)
           ui->pushButtonPreview->setEnabled(false);
    }
}

//=========================================================
void MainWindow::slotChangeNumberPage()
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

        initTableAVP(m_currentNumberPage,m_currentIdAVS,m_currentState);
//        initTableAVP(m_currentNumberPage);
    }
}

//=========================================================
QTableWidgetItem* MainWindow::initViolations(long id_avp)
{
    QTableWidgetItem* itemViolations;
    QIcon icon;
    QString sql="",tmp, str;
    try
    {
        itemViolations = new QTableWidgetItem();
        sql = "SELECT v.\"Violation\",ar.\"Percent\",ar.\"TextViolation\" FROM \"AnalysisResult\" ar "
              "INNER JOIN \"Violation\" v ON ar.\"ID_Violation\"=v.\"ID\" WHERE \"ID_AVP\"="+tmp.setNum(id_avp)+";";
//        qDebug()<<"sql="<<sql;
        if(queryViolation->exec(sql))
        {
            if(queryViolation->size() == 0)
            {
                itemViolations->setText("АВП не анализировалось");
                icon.addFile(QString::fromUtf8(":/icons/icons/question.ico"), QSize(), QIcon::Normal, QIcon::Off);
            }
            else
            {
//                int row=0;
                icon.addFile(QString::fromUtf8(":/icons/icons/alarm.ico"), QSize(), QIcon::Normal, QIcon::Off);
                str="";
                while(queryViolation->next())
                {
                    if(queryViolation->value(0).toString() == "Не обнаружено")
                        icon.addFile(QString::fromUtf8(":/icons/icons/ok.ico"), QSize(), QIcon::Normal, QIcon::Off);
                    str += queryViolation->value(0).toString();
                    str +=" - ";
                    str += queryViolation->value(1).toString();str +="% ";
//                    row++;
                }
                itemViolations->setText(str);
            }
            itemViolations->setIcon(icon);
            itemViolations->setFlags(itemViolations->flags() ^ Qt::ItemIsEditable);
        }
        else
            qDebug()<<"Error: "<<queryViolation->lastError().text();
    }
    catch(std::exception &e)
    {
        qDebug()<<"initViolations: "<<e.what();
    }
    return itemViolations;
}

//=========================================================
void MainWindow::initTableAVP(int numberPage, long idAVS, int state)
{
    QString sql="",tmp;
    try
    {
        ui->tableWidgetAVP->clearContents();
        ui->tableWidgetAVP->setRowCount(0);

        sql = "SELECT "
              "avp.\"NameRus\","
              "avp.\"URL\","
              "aa.\"Rubric\","
              "aa.\"YearOfRelease\","
              "aa.\"FilmMaker\","
              "u.\"FIO\","
              "aa.\"DateSaveInDB\","
              "avp.\"NameOriginal\","
              "avs.\"NameAVS\","
              "avp.\"ID\" FROM avp "
              "INNER JOIN avs ON avp.\"ID_AVS\" = avs.\"ID\" "
              "INNER JOIN \"AVPattribute\" aa ON aa.\"ID_AVP\" = avp.\"ID\" "
              "INNER JOIN \"User\" u ON u.\"ID\" = aa.\"ID_User\"";
              if( idAVS != -1)
              {
                  sql+=" WHERE \"ID_AVS\"="; sql+=tmp.setNum(idAVS);
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
              }
              sql+=" ORDER BY avp.\"ID\" LIMIT 1000 OFFSET ";
        sql+=tmp.setNum((numberPage-1)*1000);sql+=";";

        if(query->exec(sql))
        {
            int row=0;
            m_countCurrentAVP = query->size();
            while(query->next())
            {
                ui->tableWidgetAVP->setRowCount(row+1);
                QTableWidgetItem *newItem = new QTableWidgetItem();
                QIcon icon;
                icon.addFile(QString::fromUtf8(":/icons/icons/film_projector_cinema.ico"), QSize(), QIcon::Normal, QIcon::Off);
                newItem->setIcon(icon);
                newItem->setText(query->value(0).toString());
                newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAVP->setItem(row,0, newItem);

                QTableWidgetItem *newItem1 = new QTableWidgetItem();
                QIcon icon1;
                icon1.addFile(QString::fromUtf8(":/icons/icons/web.ico"), QSize(), QIcon::Normal, QIcon::Off);
                newItem1->setIcon(icon1);
                newItem1->setText(query->value(1).toString());
                newItem1->setFlags(newItem1->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAVP->setItem(row,1, newItem1);

                QTableWidgetItem *newItem2 = new QTableWidgetItem();
                QIcon icon2;
                icon2.addFile(QString::fromUtf8(":/icons/icons/rubric.ico"), QSize(), QIcon::Normal, QIcon::Off);
                newItem2->setIcon(icon2);
                newItem2->setText(query->value(2).toString());
                newItem2->setFlags(newItem2->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAVP->setItem(row,2, newItem2);

                QTableWidgetItem *newItem3 = new QTableWidgetItem();
                newItem3->setText(query->value(3).toString());
                newItem3->setFlags(newItem3->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAVP->setItem(row,3, newItem3);

                QTableWidgetItem *newItem4 = new QTableWidgetItem();
                QIcon icon4;
                icon4.addFile(QString::fromUtf8(":/icons/icons/film_maker1.ico"), QSize(), QIcon::Normal, QIcon::Off);
                newItem4->setIcon(icon4);
                newItem4->setText(query->value(4).toString());
                newItem4->setFlags(newItem4->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAVP->setItem(row,4, newItem4);

                QTableWidgetItem *newItem5 = new QTableWidgetItem();
                QIcon icon5;
                icon5.addFile(QString::fromUtf8(":/icons/icons/user.ico"), QSize(), QIcon::Normal, QIcon::Off);
                newItem5->setIcon(icon5);
                newItem5->setText(query->value(5).toString());
                newItem5->setFlags(newItem5->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAVP->setItem(row,5, newItem5);

                QTableWidgetItem *newItem6 = new QTableWidgetItem();
                QIcon icon6;
                icon6.addFile(QString::fromUtf8(":/icons/icons/date2.ico"), QSize(), QIcon::Normal, QIcon::Off);
                newItem6->setIcon(icon6);
                newItem6->setText(query->value(6).toString());
                newItem6->setFlags(newItem6->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAVP->setItem(row,6, newItem6);

                QTableWidgetItem *newItem7 = initViolations(query->value(9).toInt());
                //                newItem7->setText("АВП не анализировалось");
                //                QIcon icon7;
                //                icon7.addFile(QString::fromUtf8(":/icons/icons/question.ico"), QSize(), QIcon::Normal, QIcon::Off);
                //                newItem7->setIcon(icon7);
                //                newItem7->setFlags(newItem7->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAVP->setItem(row,7, newItem7);

                QTableWidgetItem *newItem8 = new QTableWidgetItem();
                newItem8->setText(query->value(9).toString());
                newItem8->setFlags(newItem8->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAVP->setItem(row,8, newItem8);

                row++;
//                if(row==100)
//                    break;
            }
        }
        else            
            qDebug()<<": "<<query->lastError().text();
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
void MainWindow::slotAbout()
{
    QMessageBox::about(this,tr("О программе"),tr("АРМ оператора мониторинга АВП на АВС.\nВерсия 1.0"));
}

//=========================================================
void MainWindow::slotImportData()
{
    QString fileName ="";
    fileName = QFileDialog::getOpenFileName(this,
                       QString::fromUtf8("Открыть файл"),
                       QDir::currentPath(),
                       "Excel (*.xlsx *.xls);;All files (*.*)");

    if(fileName != "")
    {
        progress = 0;
        dImportDataOutExcel->show();
        dImportDataOutExcel->clearTextInWindow();
        dImportDataOutExcel->setText("Начало разбора файла...");
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        emit operate(fileName);
    }
}

//=========================================================
void MainWindow::slotImportKinopoisk()
{
    QString fileName ="";
    fileName = QFileDialog::getOpenFileName(this,
                       QString::fromUtf8("Открыть файл"),
                       QDir::currentPath(),
                       "All files (*.*);;Text (*.txt)");

    if(fileName != "")
    {
        stepProgress = 0;
        progress = 0;
        dImportDataOutExcel->show();
        dImportDataOutExcel->clearTextInWindow();
        dImportDataOutExcel->setText("Начало разбора файла...");
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        emit operateKinopoisk(fileName);
    }
}

//=========================================================
void MainWindow::handleResults(const QString &result)
{
    if(result=="END")
    {
        dImportDataOutExcel->setProgress(100);
        initTableAVP();
        initAVS();
        QApplication::restoreOverrideCursor();
    }
    else
    {
        dImportDataOutExcel->setText(result);
        progress += 4;
        if(progress<100)
            dImportDataOutExcel->setProgress(progress);
    }
}

//=========================================================
void MainWindow::handleResultsKinopoisk(const QString &result)
{
    if(result=="END")
    {
        dImportDataOutExcel->setProgress(100);
        initTableAVP();
        initAVS();
        QApplication::restoreOverrideCursor();
    }
    else
    {
        dImportDataOutExcel->setText(result);
        stepProgress ++;
        if(stepProgress == 100)
        {
            progress += 1;
            if(progress<100)
                dImportDataOutExcel->setProgress(progress);
            stepProgress = 0;
        }
    }
}

//=========================================================
void MainWindow::slotAddAVP()
{
    QString tmp, timestamp, sql ="";
    SDataAVP dataAVP;
    dAddAVP->initAVS();
    if(dAddAVP->exec() == QDialog::Accepted)
    {
        try
        {
            dataAVP.avsName = dAddAVP->getNameAVS();
            dataAVP.avpNameRus = dAddAVP->getNameAVP();
            dataAVP.avpNameOriginal = dAddAVP->getNameOriginal();
            dataAVP.avsURL = dAddAVP->getURL_AVS();
            dataAVP.avpURL = dAddAVP->getURL_AVP();
            dataAVP.avpForm = dAddAVP->getFormAVP();
            dataAVP.rubric = dAddAVP->getRubricAVP();
            dataAVP.filmMaker = dAddAVP->getFilmMaker();
            dataAVP.age = dAddAVP->getAge();
            dataAVP.yearOfRelease = dAddAVP->getYearOfRelease();
            dataAVP.duration = dAddAVP->getDuration();
            dataAVP.dateSaveInDB = QDateTime::currentDateTime();
            dataAVP.userSaveInDB = currentUserName;
            if(cImportData->addSaveInDB(dataAVP))
                QMessageBox::information(this,"Добавление АВП","АВП добавлено успешно.","Да");
            else
                QMessageBox::critical(this,"Ошибка","Ошибка добавлено АВП!","Да");
        }
        catch(std::exception &e)
        {
            qDebug()<<e.what();
        }
        initTableAVP();
    }
}

//=========================================================
void MainWindow::slotAddTask()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    QString tmp, timestamp, sql ="";
    std::map<long,QString> listAVP;
    dAddTask->initTableListAVP();
    dAddTask->initComboBoxUser();
    dAddTask->initComboBoxStatus();
    dAddTask->initComboBoxPriority();
    dAddTask->initAVS();
    QDateTime dt;
    QApplication::restoreOverrideCursor();

    if(dAddTask->exec() == QDialog::Accepted)
    {
        try
        {
            listAVP = dAddTask->getListAVP();
            for(auto idAVP : listAVP)
            {
                sql = "INSERT INTO \"Task\"(\"ID_User\",\"ID_TaskStatus\",\"ID_Priority\",\"ID_AVP\",\"PercentCompleted\",\"DateAppoint\",\"Comment\") VALUES(";
                sql += tmp.setNum(getIdUser(dAddTask->getUserFIO()));sql += ",";
                sql += tmp.setNum(getIdTaskStatus(dAddTask->getStatusName()));sql += ",";
                sql += tmp.setNum(getIdPriority(dAddTask->getNamePriority()));sql += ",";
                sql += tmp.setNum(idAVP.first); sql += ",0,\'";
                timestamp = QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss");
                sql += timestamp;sql += "\',\'Требуется анализ\');";
//                qDebug()<<"sql="<<sql;
                if(!query->exec(sql))
                    qDebug()<<query->lastError().text();
            }
            initTableTask(true);
        }
        catch(std::exception &e)
        {
            qDebug()<<e.what();
            qDebug()<<query->lastError().text();
            QMessageBox::warning(this, tr("Внимание"),query->lastError().text(),tr("Да"));
        }
    }
}

//=========================================================
void MainWindow::slotEditTask()
{
    QString tmp, timestamp, sql ="";
    QTableWidget *tableViolation;

    QModelIndexList selectedRows = ui->tableWidgetCurrentTasks->selectionModel()->selectedRows();

    dEditTaskUser->initComboBoxUser(ui->tableWidgetCurrentTasks->item(selectedRows[0].row(),2)->text());
    dEditTaskUser->initComboBoxPriority(ui->tableWidgetCurrentTasks->item(selectedRows[0].row(),4)->text());
    dEditTaskUser->initComboBoxStatus(ui->tableWidgetCurrentTasks->item(selectedRows[0].row(),5)->text());
    dEditTaskUser->initTableViolation(ui->tableWidgetCurrentTasks->item(selectedRows[0].row(),10)->text().toLong());

    dEditTaskUser->setNameAVP(ui->tableWidgetCurrentTasks->item(selectedRows[0].row(),0)->text());
    dEditTaskUser->setPercent(ui->tableWidgetCurrentTasks->item(selectedRows[0].row(),6)->text());
    dEditTaskUser->setComment(ui->tableWidgetCurrentTasks->item(selectedRows[0].row(),8)->text());

    if(dEditTaskUser->exec() == QDialog::Accepted)
    {
        try
        {
            sql = "UPDATE \"Task\" SET \"ID_User\"=";
            sql += tmp.setNum(getIdUser(dEditTaskUser->getNameUser()));
            sql += ",\"ID_TaskStatus\"=";
            sql += tmp.setNum(getIdTaskStatus(dEditTaskUser->getStatus()));
            sql += ",\"ID_Priority\"=";
            sql += tmp.setNum(getIdPriority(dEditTaskUser->getPriority()));
            sql += ",\"PercentCompleted\"=";
            sql += dEditTaskUser->getPercent();
            sql += ",\"Comment\"=\'";
            sql += dEditTaskUser->getComment();
            sql += "\' WHERE \"ID\"=";
            sql += ui->tableWidgetCurrentTasks->item(selectedRows[0].row(),9)->text();
            sql += ";";

//            qDebug()<<"sql="<<sql;
            if(!query->exec(sql))
                qDebug()<<query->lastError().text();

            sql = "DELETE FROM \"AnalysisResult\" WHERE \"ID_AVP\" = " + tmp.setNum(ui->tableWidgetCurrentTasks->item(selectedRows[0].row(),10)->text().toInt()) + ";";

//            qDebug()<<"sql="<<sql;
            if(!query->exec(sql))
                qDebug()<<query->lastError().text();

            tableViolation = dEditTaskUser->getViolations();
            for(int i=0; i<tableViolation->rowCount();i++)
            {
                if(reinterpret_cast<QCheckBox*>(tableViolation->cellWidget(i,0))->isChecked())
                {
                    qDebug()<<"nameViolation="<<reinterpret_cast<QCheckBox*>(tableViolation->cellWidget(i,0))->text();
                    sql = "INSERT INTO \"AnalysisResult\"(\"ID_AVP\",\"ID_Violation\",\"Percent\") VALUES(";
                    sql += tmp.setNum(ui->tableWidgetCurrentTasks->item(selectedRows[0].row(),10)->text().toInt());
                    sql += ",";
                    sql += tmp.setNum(dEditTask->idViolation(reinterpret_cast<QCheckBox*>(tableViolation->cellWidget(i,0))->text()));
                    sql += ",\'";
                    sql += tmp.setNum(reinterpret_cast<QSpinBox*>(tableViolation->cellWidget(i,1))->value());
                    sql += "\');";
//                    qDebug()<<"sql="<<sql;
                    if(!query->exec(sql))
                        qDebug()<<query->lastError().text();
                }
            }

            initTableTask(true);
        }
        catch(std::exception &e)
        {
            qDebug()<<e.what();
            qDebug()<<query->lastError().text();
            QMessageBox::warning(this, tr("Внимание"),query->lastError().text(),tr("Да"));
        }
    }

}

//=========================================================
void MainWindow::slotEditMyTask(int, int)
{
    slotEditMyTask();
}

//=========================================================
void MainWindow::slotEditTask(int, int)
{
    slotEditTask();
}

//=========================================================
void MainWindow::slotEditAudit(int, int)
{
    slotEditAudit();
}

//=========================================================
void MainWindow::slotDeleteAVP()
{
    QString sql="";
    QModelIndexList selectedRows = ui->tableWidgetAVP->selectionModel()->selectedRows();
    if(selectedRows.empty())
    {
        QMessageBox::information(this, tr("Сообщение"),tr("Вы не выбрали АВП для удаления!"),tr("Да"));
        return;
    }
    else
    {
        if(QMessageBox::warning(this, tr("Внимание"),tr("Вы действительно хотите удалить выбранные АВП?"),tr("Да"),tr("Нет")) == 0)
        {
            while (!selectedRows.empty())
            {
                sql = "DELETE FROM avp WHERE \"ID\"=";sql += ui->tableWidgetAVP->takeItem(selectedRows[0].row(),8)->text(); sql += ";";
//                qDebug()<<"sql ="<<sql;
                query->exec(sql);
                ui->tableWidgetAVP->removeRow(selectedRows[0].row());
                selectedRows = ui->tableWidgetAVP->selectionModel()->selectedRows();
            }
        }
    }
}

//=========================================================
void MainWindow::slotDeleteTask()
{
    QString sql="";
    QModelIndexList selectedRows = ui->tableWidgetCurrentTasks->selectionModel()->selectedRows();
    if(selectedRows.empty())
    {
        QMessageBox::information(this, tr("Сообщение"),tr("Вы не выбрали задачу для удаления!"),tr("Да"));
        return;
    }
    else
    {
        try
        {
            if(QMessageBox::warning(this, tr("Внимание"),tr("Вы действительно хотите удалить выбранные задачи:?"),tr("Да"),tr("Нет")) == 0)
            {
                while (!selectedRows.empty())
                {
                    sql = "DELETE FROM \"Task\" WHERE \"ID\"=";sql += ui->tableWidgetCurrentTasks->takeItem(selectedRows[0].row(),9)->text(); sql += ";";
//                    qDebug()<<"sql ="<<sql;
                    if(query->exec(sql))
                    {
                        ui->tableWidgetCurrentTasks->removeRow(selectedRows[0].row());
                        selectedRows = ui->tableWidgetCurrentTasks->selectionModel()->selectedRows();
                    }
                    else
                        qDebug()<<query->lastError().text();
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
}

//=========================================================
void MainWindow::slotEditMyTask()
{
    QString tmp, timestamp, sql ="";
//    QTableWidget *tableViolation;
    QModelIndexList selectedRows = ui->tableWidgetMyTasks->selectionModel()->selectedRows();

    dEditTask->initTableViolation(ui->tableWidgetMyTasks->item(selectedRows[0].row(),9)->text().toLong());
    dEditTask->initComboBoxStatus(ui->tableWidgetMyTasks->item(selectedRows[0].row(),4)->text());
    dEditTask->initComboBoxViolation();
    dEditTask->hideViolationGroup();

    dEditTask->setNameAVP(ui->tableWidgetMyTasks->item(selectedRows[0].row(),0)->text());
    dEditTask->setPercent(ui->tableWidgetMyTasks->item(selectedRows[0].row(),5)->text());
    dEditTask->setComment(ui->tableWidgetMyTasks->item(selectedRows[0].row(),7)->text());

    if(dEditTask->exec() == QDialog::Accepted)
    {
        try
        {
            sql = "UPDATE \"Task\" SET \"ID_TaskStatus\"=";
            sql += tmp.setNum(getIdTaskStatus(dEditTask->getStatus()));
            sql += ",\"PercentCompleted\"=";
            sql += dEditTask->getPercent();
            sql += ",\"Comment\"=\'";
            sql += dEditTask->getComment();
            sql += "\' WHERE \"ID\"=";
            sql += ui->tableWidgetMyTasks->item(selectedRows[0].row(),8)->text();
            sql += ";";

//            qDebug()<<"sql="<<sql;
            if(!query->exec(sql))
                qDebug()<<query->lastError().text();

//            sql = "DELETE FROM \"AnalysisResult\" WHERE \"ID_AVP\" = " + tmp.setNum(ui->tableWidgetMyTasks->item(selectedRows[0].row(),9)->text().toInt()) + ";";

////            qDebug()<<"sql="<<sql;
//            if(!query->exec(sql))
//                qDebug()<<query->lastError().text();

//            tableViolation = dEditTask->getViolations();
//            for(int i=0; i<tableViolation->rowCount();i++)
//            {
//                if(reinterpret_cast<QCheckBox*>(tableViolation->cellWidget(i,0))->isChecked())
//                {
//                    qDebug()<<"nameViolation="<<reinterpret_cast<QCheckBox*>(tableViolation->cellWidget(i,0))->text();
//                    sql = "INSERT INTO \"AnalysisResult\"(\"ID_AVP\",\"ID_Violation\",\"Percent\") VALUES(";
//                    sql += tmp.setNum(ui->tableWidgetMyTasks->item(selectedRows[0].row(),9)->text().toInt());
//                    sql += ",";
//                    sql += tmp.setNum(dEditTask->idViolation(reinterpret_cast<QCheckBox*>(tableViolation->cellWidget(i,0))->text()));
//                    sql += ",\'";
//                    sql += tmp.setNum(reinterpret_cast<QSpinBox*>(tableViolation->cellWidget(i,1))->value());
//                    sql += "\');";
////                    qDebug()<<"sql="<<sql;
//                    if(!query->exec(sql))
//                        qDebug()<<query->lastError().text();
//                }
//            }

            initTableTask(true);
        }
        catch(std::exception &e)
        {
            qDebug()<<e.what();
            qDebug()<<query->lastError().text();
            QMessageBox::warning(this, tr("Внимание"),query->lastError().text(),tr("Да"));
        }
        initTableMyTask();
    }
}

//=========================================================
void MainWindow::slotEditAudit()
{
    QString tmp, sql ="";
    QModelIndexList selectedRows = ui->tableWidgetAudit->selectionModel()->selectedRows();

    dEditAudit->initTableViolation(ui->tableWidgetAudit->item(selectedRows[0].row(),10)->text().toLong());
    if(dEditAudit->exec() == QDialog::Accepted)
    {
        try
        {
            sql = "UPDATE \"Task\" SET \"ID_TaskStatus\"=";
            sql += tmp.setNum(getIdTaskStatus("Одобрена экспертом"));
            sql += ",\"Comment\"=\'";
            tmp = "Эксперт: " + dEditAudit->getComment();
            sql += tmp;
            sql += "\' WHERE \"ID\"=";
            sql += ui->tableWidgetAudit->item(selectedRows[0].row(),9)->text();
            sql += ";";

            qDebug()<<"sql="<<sql;
            if(!query->exec(sql))
                qDebug()<<query->lastError().text();


        }
        catch(std::exception &e)
        {
            qDebug()<<e.what();
            qDebug()<<query->lastError().text();
            QMessageBox::warning(this, tr("Внимание"),query->lastError().text(),tr("Да"));
        }
        initTableAudit();
    }
}

//=========================================================
void MainWindow::slotReload()
{
    initTableAVP();
}

//=========================================================
void MainWindow::slotSettingsDB()
{
}

//=========================================================
void MainWindow::slotGroup()
{
    dGroup->exec();
}

//=========================================================
void MainWindow::slotUser()
{
    dUser->initTableUser();
    dUser->exec();
}

//=========================================================
void MainWindow::slotViolation()
{
    dViolation->m_typeDictionary = 1;
    dViolation->initTableViolation();
    dViolation->exec();
}

//=========================================================
void MainWindow::slotStatus()
{
    dViolation->m_typeDictionary = 2;
    dViolation->initTableStatus();
    dViolation->exec();
}

//=========================================================
void MainWindow::slotRBViolation(bool state)
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    if(state)
       initTableAVP(1,m_currentIdAVS, 1);

    m_currentState = 1;
    m_currentNumberPage = 1;
    m_countAVP = m_countCurrentAVP;
    QString tmp;
    QString text="из ";text+=tmp.setNum(m_countAVP/1000); text+=" (всего АВП ";text+=tmp.setNum(m_countAVP);text+=")";
    ui->lineEditNumberPage->setValidator( new QIntValidator(1,(m_countAVP/1000), this) );
    ui->labelCountAVP->setText(text);
    ui->pushButtonPreview->setEnabled(false);
    QApplication::restoreOverrideCursor();
}

//=========================================================
void MainWindow::slotRBChecked(bool state)
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    if(state)
       initTableAVP(1, m_currentIdAVS, 2);

    m_currentState = 2;
    m_currentNumberPage = 1;
    m_countAVP = m_countCurrentAVP;
    QString tmp;
    QString text="из ";text+=tmp.setNum(m_countAVP/1000); text+=" (всего АВП ";text+=tmp.setNum(m_countAVP);text+=")";
    ui->lineEditNumberPage->setValidator( new QIntValidator(1,(m_countAVP/1000), this) );
    ui->labelCountAVP->setText(text);
    ui->pushButtonPreview->setEnabled(false);
    QApplication::restoreOverrideCursor();
}

//=========================================================
void MainWindow::slotRBAll(bool state)
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    if(state)
       initTableAVP(1, m_currentIdAVS, -1);

    m_currentState = -1;
    m_currentNumberPage = 1;
    m_countAVP = countAVP(m_currentIdAVS);
    QString tmp;
    QString text="из ";text+=tmp.setNum(m_countAVP/1000); text+=" (всего АВП ";text+=tmp.setNum(m_countAVP);text+=")";
    ui->lineEditNumberPage->setValidator( new QIntValidator(1,(m_countAVP/1000), this) );
    ui->labelCountAVP->setText(text);
    ui->pushButtonPreview->setEnabled(false);
    QApplication::restoreOverrideCursor();
}

//=========================================================
void MainWindow::slotFilterApply()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    initTableTask(true);
    QApplication::restoreOverrideCursor();
}

//=========================================================
void MainWindow::slotTextChanged(const QString &text)
{
//    if(text.length()==0)
//        initTableAVP();
    if(text.length()>1)
        m_timer.start();

}

//=========================================================
void MainWindow::slotFindAVP()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QString sql="";
    try
    {
        sql = "SELECT "
              "avp.\"NameRus\","
              "avp.\"URL\","
              "aa.\"Rubric\","
              "aa.\"YearOfRelease\","
              "aa.\"FilmMaker\","
              "u.\"FIO\","
              "aa.\"DateSaveInDB\","
              "avp.\"NameOriginal\","
              "avs.\"NameAVS\","
              "avp.\"ID\" FROM avp "
              "INNER JOIN avs ON avp.\"ID_AVS\" = avs.\"ID\" "
              "INNER JOIN \"AVPattribute\" aa ON aa.\"ID_AVP\" = avp.\"ID\" "
              "INNER JOIN \"User\" u ON u.\"ID\" = aa.\"ID_User\"";
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
                ui->tableWidgetAVP->clearContents();
                ui->tableWidgetAVP->setRowCount(0);

                int row = 0;
                while(query->next())
                {
                    ui->tableWidgetAVP->setRowCount(row+1);
                    QTableWidgetItem *newItem = new QTableWidgetItem();
                    QIcon icon;
                    icon.addFile(QString::fromUtf8(":/icons/icons/film_projector_cinema.ico"), QSize(), QIcon::Normal, QIcon::Off);
                    newItem->setIcon(icon);
                    newItem->setText(query->value(0).toString());
                    newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidgetAVP->setItem(row,0, newItem);

                    QTableWidgetItem *newItem1 = new QTableWidgetItem();
                    QIcon icon1;
                    icon1.addFile(QString::fromUtf8(":/icons/icons/web.ico"), QSize(), QIcon::Normal, QIcon::Off);
                    newItem1->setIcon(icon1);
                    newItem1->setText(query->value(1).toString());
                    newItem1->setFlags(newItem1->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidgetAVP->setItem(row,1, newItem1);

                    QTableWidgetItem *newItem2 = new QTableWidgetItem();
                    QIcon icon2;
                    icon2.addFile(QString::fromUtf8(":/icons/icons/rubric.ico"), QSize(), QIcon::Normal, QIcon::Off);
                    newItem2->setIcon(icon2);
                    newItem2->setText(query->value(2).toString());
                    newItem2->setFlags(newItem2->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidgetAVP->setItem(row,2, newItem2);

                    QTableWidgetItem *newItem3 = new QTableWidgetItem();
                    newItem3->setText(query->value(3).toString());
                    newItem3->setFlags(newItem3->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidgetAVP->setItem(row,3, newItem3);

                    QTableWidgetItem *newItem4 = new QTableWidgetItem();
                    QIcon icon4;
                    icon4.addFile(QString::fromUtf8(":/icons/icons/film_maker1.ico"), QSize(), QIcon::Normal, QIcon::Off);
                    newItem4->setIcon(icon4);
                    newItem4->setText(query->value(4).toString());
                    newItem4->setFlags(newItem4->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidgetAVP->setItem(row,4, newItem4);

                    QTableWidgetItem *newItem5 = new QTableWidgetItem();
                    QIcon icon5;
                    icon5.addFile(QString::fromUtf8(":/icons/icons/user.ico"), QSize(), QIcon::Normal, QIcon::Off);
                    newItem5->setIcon(icon5);
                    newItem5->setText(query->value(5).toString());
                    newItem5->setFlags(newItem5->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidgetAVP->setItem(row,5, newItem5);

                    QTableWidgetItem *newItem6 = new QTableWidgetItem();
                    QIcon icon6;
                    icon6.addFile(QString::fromUtf8(":/icons/icons/date2.ico"), QSize(), QIcon::Normal, QIcon::Off);
                    newItem6->setIcon(icon6);
                    newItem6->setText(query->value(6).toString());
                    newItem6->setFlags(newItem6->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidgetAVP->setItem(row,6, newItem6);

                    QTableWidgetItem *newItem7 = initViolations(query->value(9).toInt());
                    ui->tableWidgetAVP->setItem(row,7, newItem7);

                    QTableWidgetItem *newItem8 = new QTableWidgetItem();
                    newItem8->setText(query->value(9).toString());
                    newItem8->setFlags(newItem8->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidgetAVP->setItem(row,8, newItem8);
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
void MainWindow::slotCurrentChanged(int index)
{
    if(index == 0 )
    {
        ui->pushButtonReload->show();
    }
    else if(index == 1 )
    {
        ui->pushButtonReload->hide();
        initTableTask();
    }
    else if(index == 2 )
    {
        ui->pushButtonReload->hide();
        initTableMyTask();
    }
    else if(index == 3 )
    {
        ui->pushButtonReload->hide();
        initTableAudit();
    }
}

//=========================================================
void MainWindow::slotChangeUser()
{
    dEnter1->initUserComBoBox();
    if(dEnter1->exec()== QDialog::Accepted)
        initTableMyTask();
}

//=========================================================
void MainWindow::slotClearDB()
{
    if(QMessageBox::warning(this, tr("Внимание"),tr("Вы действительно хотите очистить базу данных АВП?"),tr("Да"),tr("Нет")) == 0)
    {
//        query->exec("DELETE FROM avs;");
        initTableAVP();
        initAVS();
    }
}

//=========================================================
void MainWindow::slotContextMenuRequested(const QPoint &pos)
{
    /* Создаем объект контекстного меню */
       QMenu * menu = new QMenu(this);

       /* Создаём действия для контекстного меню */
       QAction * actionAnalysis = new QAction(tr("Анализ АВП"), this);
       QIcon icon;
       icon.addFile(QString::fromUtf8(":/icons/icons/analysis.ico"), QSize(), QIcon::Normal, QIcon::Off);
       actionAnalysis->setIcon(icon);

       QAction * actionGoToURL = new QAction(tr("Перейти по ссылки URL"), this);
       QIcon icon1;
       icon1.addFile(QString::fromUtf8(":/icons/icons/web.ico"), QSize(), QIcon::Normal, QIcon::Off);
       actionGoToURL->setIcon(icon1);

       /* Подключаем СЛОТы обработчики для действий контекстного меню */
       connect(actionAnalysis, SIGNAL(triggered()), this, SLOT(slotAnalysisAVP()));
       connect(actionGoToURL, SIGNAL(triggered()), this, SLOT(slotGoToURL()));

       /* Устанавливаем действия в меню */
       menu->addAction(actionAnalysis);
       menu->addAction(actionGoToURL);

       /* Вызываем контекстное меню */
       menu->popup(ui->tableWidgetMyTasks->viewport()->mapToGlobal(pos));
}

//=========================================================
void MainWindow::slotAnalysisAVP()
{
    QString sql,tmp;
    int row = ui->tableWidgetMyTasks->selectionModel()->currentIndex().row();
    QProgressDialog progress("Анализ АВП...", "Отмена", 0, 100, this);
    progress.setWindowModality(Qt::WindowModal);
    for(int i=0; i<=100; i++)
    {
        progress.setValue(i);
        QThread::msleep(50);
    }

    sql = "INSERT INTO \"AnalysisResult\"(\"ID_AVP\",\"ID_Violation\",\"Percent\") VALUES(";
    sql += tmp.setNum(ui->tableWidgetMyTasks->item(row,9)->text().toInt());
    sql += ",";
    sql += tmp.setNum(12);
    sql += ",\'";
    sql += tmp.setNum(0);
    sql += "\');";
//    qDebug()<<"sql="<<sql;
    if(!query->exec(sql))
        qDebug()<<query->lastError().text();

    initTableMyTask();
}

//=========================================================
void MainWindow::slotGoToURL()
{
    if(ui->tabWidget->currentIndex()== 2)
    {
        int row = ui->tableWidgetMyTasks->selectionModel()->currentIndex().row();
        QDesktopServices::openUrl(QUrl(ui->tableWidgetMyTasks->item(row,1)->text()));
    }
    else if(ui->tabWidget->currentIndex()== 3)
    {
        int row = ui->tableWidgetAudit->selectionModel()->currentIndex().row();
        QDesktopServices::openUrl(QUrl(ui->tableWidgetAudit->item(row,1)->text()));
    }
}

//=========================================================
void MainWindow::slotExit()
{
    if(QMessageBox::information(this, tr("Сообщение"),tr("Вы действительно хотите выйти из приложения?"),tr("Да"),tr("Нет"))==0)
        close();
}
