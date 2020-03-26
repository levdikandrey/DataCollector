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
#include <QProcess>
#include <QRect>
#include <QScreen>

#include <QMessageBox>
#include <QPushButton>
#include <QFileDialog>
#include <QDebug>
#include <QThread>

#include <QSqlDatabase>
#include <QSqlError>

#include <iostream>
#include <unistd.h>

extern QSqlDatabase db;
extern QString currentUserName;

constexpr const char* DEFAULT_HOST = "192.168.28.96";
constexpr int DEFAULT_PORT = 8080;

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
    connect(ui->actionConnectToDB, SIGNAL(triggered()), SLOT(slotSettingsDB()));
    connect(ui->actionChangePassword, SIGNAL(triggered()), SLOT(slotChangePassword()));
    connect(ui->actionMakeReport, SIGNAL(triggered()), SLOT(slotMakeReport()));

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
    dSettingsDB = new DSettingsDB(this);

    if(!initDB())
        exit(0);

    initAVS();
    initTableAVP();
    initTableTask();
//    initTableMyTask();

    initComboBoxUser(ui->comboBoxUser);
    initComboBoxStatus(ui->comboBoxStatus);
    initComboBoxPriority(ui->comboBoxPriority);

    ui->tableWidgetAVP->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidgetAVP, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotContextMenuRequestedAVP(QPoint)));

    ui->tableWidgetCurrentTasks->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidgetCurrentTasks, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotContextMenuRequestedCurrentTask(QPoint)));

    ui->tableWidgetMyTasks->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidgetMyTasks, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotContextMenuRequested(QPoint)));

    ui->tableWidgetAudit->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidgetAudit, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotContextMenuRequestedExpert(QPoint)));

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

    m_requestSender = new RequestSender();

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
    dChangePassword = new DChangePassword(this);

    m_currentIdAVS = -1;
    m_currentState = -1;
    m_currentNumberPage = 1;
    m_countAVP = countAVP();

    int page_count = (m_countAVP/1000);
    if( m_countAVP%1000 > 0)
        page_count++;
    QString tmp;
    QString text="из ";text+=tmp.setNum(page_count); text+=" (всего АВП ";text+=tmp.setNum(m_countAVP);text+=")";
    ui->lineEditNumberPage->setValidator( new QIntValidator(1,(page_count), this) );
    ui->labelCountAVP->setText(text);
    ui->pushButtonPreview->setEnabled(false);

    m_timer.setInterval(1000);
    m_timer.setSingleShot(true);
    connect(&m_timer, &QTimer::timeout, this, [&]{slotFindAVP();});

    ui->labelStateConnectDB->hide();

    initNetClient();
}

//=========================================================
bool MainWindow::initNetClient()
{
    bool state = true;
    QString host;
    int port;
    QSettings settings(QDir::toNativeSeparators(QApplication::applicationDirPath()) + "/settings.ini",QSettings::IniFormat);
    qDebug()<<"settings.value(SERVER/HOST).toString()="<<settings.value("SERVER/HOST").toString();
    qDebug()<<"settings.value(SERVER/PORT).toInt()="<<settings.value("SERVER/PORT").toString();

    host = settings.value("SERVER/HOST").toString();
    if (host == "")
    {
        host = DEFAULT_HOST;
    }
    port = settings.value("SERVER/PORT").toInt();
    if (port == 0)
    {
        port = DEFAULT_PORT;
    }

    m_client = new Client(host, port, this);
    return state;
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
    delete m_requestSender;
}

//=========================================================
void MainWindow::initDialogAccess(QString userName)
{
    QString sql;
    QString groupCategory;
    try
    {
       sql = "SELECT \"GroupCategory\" FROM \"Group\" WHERE \"ID\" = (SELECT \"ID_Group\" FROM \"User\" WHERE \"FIO\"=\'" + userName + "\');";
//       qDebug()<<"SQL = "<<sql;
       if(query->exec(sql))
       {
           if(query->next())
               groupCategory = query->value(0).toString();
//           qDebug()<<"groupCategory = "<<groupCategory;
           if(groupCategory == "Операторы")
           {
               ui->tabWidget->setTabEnabled( 1, false);
               ui->tabWidget->setTabEnabled( 2, true);
               ui->tabWidget->setTabEnabled( 3, false);
               ui->menu_4->setEnabled(false);
               ui->tabWidget->setCurrentIndex(2);
           }
           else if(groupCategory == "Администраторы")
           {
               ui->tabWidget->setTabEnabled( 1, true);
               ui->tabWidget->setTabEnabled( 2, true);
               ui->tabWidget->setTabEnabled( 3, true);
               ui->menu_4->setEnabled(true);
               ui->tabWidget->setCurrentIndex(0);
           }
           else if((groupCategory == "Руководство") || (groupCategory == "Начальники групп"))
           {
               ui->tabWidget->setTabEnabled( 1, true);
               ui->tabWidget->setTabEnabled( 2, true);
               ui->tabWidget->setTabEnabled( 3, false);
               ui->menu_4->setEnabled(false);
               ui->tabWidget->setCurrentIndex(1);
           }
           else if(groupCategory == "Эксперты")
           {
               ui->tabWidget->setTabEnabled( 1, false);
               ui->tabWidget->setTabEnabled( 2, false);
               ui->tabWidget->setTabEnabled( 3, true);
               ui->menu_4->setEnabled(false);
               ui->tabWidget->setCurrentIndex(3);
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
    ui->tableWidgetMyTasks->horizontalHeader()->resizeSection(10, 70);//Рецензии
    ui->tableWidgetMyTasks->horizontalHeader()->resizeSection(11, 0);//Путь к файлу
    ui->tableWidgetMyTasks->horizontalHeader()->resizeSection(12, 0);//ID_AVS

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
bool MainWindow::initDB()
{
//    std::cout<<"initDB()"<<std::endl;
    bool res = true;
    try
    {
        db = QSqlDatabase::addDatabase("QPSQL");
        db.setHostName("127.0.0.1");
        db.setHostName("192.168.28.96");
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
            QMessageBox::critical(this, tr("Ошибка подключения к БД АВП"),"Проверьте правильность параметров подключения в settings.ini.\n" + db.lastError().text());
            res = false;
        }
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
    return res;
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
        ui->tableWidgetAudit->setSortingEnabled(false);
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
        sql += "Экпертиза";
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
                icon.addFile(QString::fromUtf8(":/icons/icons/film_projector_cinema.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem->setIcon(icon);
                newItem->setText(query->value(0).toString());
                newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit->setItem(row,0, newItem);//Название АВП

                QTableWidgetItem *newItem1 = new QTableWidgetItem();
                QIcon icon1;
                icon1.addFile(QString::fromUtf8(":/icons/icons/web.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem1->setIcon(icon1);
                newItem1->setText(query->value(1).toString());
                newItem1->setFlags(newItem1->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit->setItem(row,1, newItem1);//URL

                QTableWidgetItem *newItem2 = new QTableWidgetItem();
                QIcon icon5;
                icon5.addFile(QString::fromUtf8(":/icons/icons/user.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem2->setIcon(icon5);
                newItem2->setText(query->value(2).toString());
                newItem2->setFlags(newItem2->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit->setItem(row,2, newItem2);//ФИО

                QTableWidgetItem *newItem3 = new QTableWidgetItem();
                QIcon icon3;
                icon3.addFile(QString::fromUtf8(":/icons/icons/date2.png"), QSize(), QIcon::Normal, QIcon::Off);
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
        ui->tableWidgetAudit->setSortingEnabled(true);
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
void MainWindow::addComment(uint64_t idTask, QString comment)
{
    QString sql,tmp;
    try
    {
        sql = "UPDATE \"Task\" SET \"Comment\"=\'";
        sql += comment;
        sql += "\' WHERE \"ID\"=";
        sql += tmp.setNum(idTask);
        sql += ";";

        if(!query->exec(sql))
            qDebug()<<query->lastError().text();
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
void MainWindow::changeStatusAVP(uint64_t idAVP, uint8_t status)
{
    qDebug()<<"Nooooooooo";
    for(int row = 0; row <ui->tableWidgetMyTasks->rowCount(); ++row)
    {
        if( (ui->tableWidgetMyTasks->item(row,9)->text().toLongLong() == static_cast<qlonglong>(idAVP)) && (status == 0x02) )
        {
            ui->tableWidgetMyTasks->item(row,6)->setText("Нет рецензий");
            ui->tableWidgetMyTasks->item(row,7)->setText("Нет рецензий для АВП");
            addComment(ui->tableWidgetMyTasks->item(row,8)->text().toLongLong(),"Нет рецензий для АВП");
        }
        else if( (ui->tableWidgetMyTasks->item(row,9)->text().toLongLong() == static_cast<qlonglong>(idAVP)) && (status == 0x01) )
        {
            ui->tableWidgetMyTasks->setItem(row,6, initViolations(idAVP));
            ui->tableWidgetMyTasks->item(row,7)->setText("АВП проанализировано");
            addComment(ui->tableWidgetMyTasks->item(row,8)->text().toLongLong(),"АВП проанализировано");
        }
        else if( (ui->tableWidgetMyTasks->item(row,9)->text().toLongLong() == static_cast<qlonglong>(idAVP)) && (status == 0x03) )
        {
            ui->tableWidgetMyTasks->item(row,6)->setText("Не удалось скачать рецензии для АВП!");
            ui->tableWidgetMyTasks->item(row,7)->setText("Не удалось скачать рецензии для АВП!");
            addComment(ui->tableWidgetMyTasks->item(row,8)->text().toLongLong(),"Не удалось скачать рецензии для АВП!");
        }
    }
}

//=========================================================
void MainWindow::initTableMyTask()
{
    QString sql="";
    try
    {
        ui->tableWidgetMyTasks->setSortingEnabled(false);
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
              "\"Task\".\"ID_AVP\", "
              "dd.\"DownloadStatus\", "
              "dd.\"PathOnDisk\", "
              "avp.\"ID_AVS\" "
              "FROM \"Task\" "
              "INNER JOIN avp ON \"Task\".\"ID_AVP\" = avp.\"ID\" "
              "INNER JOIN \"User\" u ON \"Task\".\"ID_User\" = u.\"ID\" "
              "INNER JOIN \"TaskStatus\" ts ON \"Task\".\"ID_TaskStatus\" = ts.\"ID\" "
              "LEFT JOIN \"DownloadData\" dd ON dd.\"ID_AVP\" = avp.\"ID\" AND dd.\"ResourceName\" = \'Kinopoisk\' "
              "INNER JOIN \"Priority\" p ON \"Task\".\"ID_Priority\" = p.\"ID\" WHERE u.\"FIO\" =\'";
        sql += currentUserName;
        sql += "\' ORDER BY \"Task\".\"ID\";";
//        qDebug()<<"sql = "<<sql;
        if(query->exec(sql))
        {
            int row = 0;
            while(query->next())
            {
                if(query->value(4).toString() == "Закрыта")
                    continue;

                ui->tableWidgetMyTasks->setRowCount(row+1);

                QTableWidgetItem *newItem = new QTableWidgetItem();
                QIcon icon;
                icon.addFile(QString::fromUtf8(":/icons/icons/film_projector_cinema.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem->setIcon(icon);
                newItem->setText(query->value(0).toString());
                newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetMyTasks->setItem(row,0, newItem);//Название АВП
//                qDebug()<<"\nName = "<<query->value(0).toString();

                QTableWidgetItem *newItem1 = new QTableWidgetItem();
                QIcon icon1;
                icon1.addFile(QString::fromUtf8(":/icons/icons/web.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem1->setIcon(icon1);
                newItem1->setText(query->value(1).toString());
                newItem1->setFlags(newItem1->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetMyTasks->setItem(row,1, newItem1);//URL
//                qDebug()<<"URL = "<<query->value(1).toString();

                QTableWidgetItem *newItem3 = new QTableWidgetItem();
                QIcon icon3;
                icon3.addFile(QString::fromUtf8(":/icons/icons/date2.png"), QSize(), QIcon::Normal, QIcon::Off);
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
                ui->tableWidgetMyTasks->setItem(row,7, newItem8);//Комментарии

                QTableWidgetItem *newItem9 = new QTableWidgetItem();
                newItem9->setText(query->value(7).toString());
                newItem9->setFlags(newItem9->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetMyTasks->setItem(row,8, newItem9);//ID

                QTableWidgetItem *newItem10 = new QTableWidgetItem();
                newItem10->setText(query->value(8).toString());
                newItem10->setFlags(newItem10->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetMyTasks->setItem(row,9, newItem10);//ID_AVP

                QTableWidgetItem *newItem11 = new QTableWidgetItem();
                if(query->value(9).toString() == "Yes")
                {
                    QIcon icon11;
                    icon11.addFile(QString::fromUtf8(":/icons/icons/attach.png"), QSize(), QIcon::Normal, QIcon::Off);
                    newItem11->setIcon(icon11);
                    newItem11->setText("Есть");
                }
                else
                    newItem11->setText("Нет");
                newItem11->setFlags(newItem11->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetMyTasks->setItem(row,10, newItem11);//Рецензии
//                qDebug()<<"DownloadStatus = "<<query->value(9).toString();

                QTableWidgetItem *newItem12 = new QTableWidgetItem();
                newItem12->setText(query->value(10).toString());
                newItem12->setFlags(newItem12->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetMyTasks->setItem(row,11, newItem12);//Путь к файлу пецензий
//                qDebug()<<"Path = "<<query->value(10).toString();

                QTableWidgetItem *newItem13 = new QTableWidgetItem();
                newItem13->setText(query->value(11).toString());
                newItem13->setFlags(newItem13->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetMyTasks->setItem(row,12, newItem13);//ID_AVS

                row++;
            }
        }
        else
            qDebug()<<query->lastError().text();

        ui->tableWidgetMyTasks->setSortingEnabled(true);
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
        ui->tableWidgetCurrentTasks->setSortingEnabled(false);
        ui->tableWidgetCurrentTasks->clearContents();
        ui->tableWidgetCurrentTasks->setRowCount(0);

        sql = "SELECT avp.\"NameRus\", "
              "avp.\"URL\","
              "u.\"FIO\","
              "\"Task\".\"DateAppoint\","
              "p.\"NamePriority\","
              "ts.\"StatusName\","
              "\"Task\".\"PercentCompleted\","
              "avp.\"URL\","
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
                icon.addFile(QString::fromUtf8(":/icons/icons/film_projector_cinema.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem->setIcon(icon);
                newItem->setText(query->value(0).toString());
                newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetCurrentTasks->setItem(row,0, newItem);//Название АВП

                QTableWidgetItem *newItem1 = new QTableWidgetItem();
                QIcon icon1;
                icon1.addFile(QString::fromUtf8(":/icons/icons/web.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem1->setIcon(icon1);
                newItem1->setText(query->value(1).toString());
                newItem1->setFlags(newItem1->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetCurrentTasks->setItem(row,1, newItem1);//URL

                QTableWidgetItem *newItem2 = new QTableWidgetItem();
                QIcon icon5;
                icon5.addFile(QString::fromUtf8(":/icons/icons/user.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem2->setIcon(icon5);
                newItem2->setText(query->value(2).toString());
                newItem2->setFlags(newItem2->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetCurrentTasks->setItem(row,2, newItem2);//ФИО

                QTableWidgetItem *newItem3 = new QTableWidgetItem();
                QIcon icon3;
                icon3.addFile(QString::fromUtf8(":/icons/icons/date2.png"), QSize(), QIcon::Normal, QIcon::Off);
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
        ui->tableWidgetCurrentTasks->setSortingEnabled(true);
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
int MainWindow::countAVP_Analysis(int state)
{
    int count = 0;
    QString sql, tmp;
    try
    {
        if(state == 1)// С нарушениями
        {
            sql = "SELECT \"ID\" FROM avp WHERE \"ID\" IN ( SELECT \"ID_AVP\" FROM \"AnalysisResult\" WHERE \"ID_Violation\" !=12 ) GROUP BY \"ID\";";
        }
        else// Все проверенные
        {
            sql = "SELECT \"ID\" FROM avp WHERE \"ID\" IN ( SELECT \"ID_AVP\" FROM \"AnalysisResult\" ) GROUP BY \"ID\";";
        }
        if(query->exec(sql))
        {
//            if(query->next())
            //                count = query->value(0).toInt();
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
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QString tmp;
    int page_count = (m_countAVP/1000);
    if( m_countAVP%1000 > 0)
        page_count++;

    if(m_currentNumberPage <= page_count)
    {
        ui->pushButtonPreview->setEnabled(true);
        m_currentNumberPage++;
        initTableAVP(m_currentNumberPage,m_currentIdAVS,m_currentState);
        ui->lineEditNumberPage->setText(tmp.setNum(m_currentNumberPage));
        if(m_currentNumberPage == page_count)
            ui->pushButtonNext->setEnabled(false);
    }
    QApplication::restoreOverrideCursor();
}

//=========================================================
void MainWindow::slotPrevious()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
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
    QApplication::restoreOverrideCursor();
}

//=========================================================
void MainWindow::slotChangeNumberPage()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    qDebug()<<ui->lineEditNumberPage->text();
    m_currentNumberPage = ui->lineEditNumberPage->text().toInt();
    int page_count = (m_countAVP/1000);
    if( m_countAVP%1000 > 0)
        page_count++;

//    if(ui->lineEditNumberPage->text().toInt() > page_count)
//    {
//        QMessageBox::warning(this,"Внимание","Не верно введен номер страницы!","Да");
//        return;
//    }

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

        initTableAVP(m_currentNumberPage,m_currentIdAVS,m_currentState);
    }
    QApplication::restoreOverrideCursor();
}

//=========================================================
QTableWidgetItem* MainWindow::initViolations(long id_avp)
{
    QTableWidgetItem* itemViolations = nullptr;
    QIcon icon;
    QString sql="",tmp, str;
    try
    {
        itemViolations = new QTableWidgetItem();
        sql = "SELECT v.\"Violation\",ar.\"Percent\",ar.\"TextViolation\",ar.\"CheckAuto\" FROM \"AnalysisResult\" ar "
              "INNER JOIN \"Violation\" v ON ar.\"ID_Violation\"=v.\"ID\" WHERE \"ID_AVP\"="+tmp.setNum(id_avp)+";";
//        qDebug()<<"sql="<<sql;
        if(queryViolation->exec(sql))
        {
            if(queryViolation->size() == 0)
            {
                itemViolations->setText("АВП не анализировалось");
                icon.addFile(QString::fromUtf8(":/icons/icons/question.png"), QSize(), QIcon::Normal, QIcon::Off);
            }
            else
            {
//                int row=0;
                icon.addFile(QString::fromUtf8(":/icons/icons/alarm.png"), QSize(), QIcon::Normal, QIcon::Off);
                str="";
                while(queryViolation->next())
                {
                    if(queryViolation->value(0).toString() == "Не обнаружено")
                        icon.addFile(QString::fromUtf8(":/icons/icons/ok.png"), QSize(), QIcon::Normal, QIcon::Off);
                    str += queryViolation->value(0).toString();
                    if(queryViolation->value(3).toBool())
                    {
                        str +="(ИИ)";
                    }
                    str +="  ";
                    str +=" - ";
/*                    if(queryViolation->value(1).toString().toInt() > 9)
                    {
                        str += "100";
                    }
                    else if
                    {
                        int percent = queryViolation->value(1).toString().toInt()

                    }*/

                    str += tmp.setNum(queryViolation->value(1).toInt());
                    str +="% ";
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
        ui->tableWidgetAVP->setSortingEnabled(false);
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
//            qDebug()<<"m_countCurrentAVP = "<<m_countCurrentAVP;
            while(query->next())
            {
                ui->tableWidgetAVP->setRowCount(row+1);
                QTableWidgetItem *newItem = new QTableWidgetItem();
                QIcon icon;
                icon.addFile(QString::fromUtf8(":/icons/icons/film_projector_cinema.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem->setIcon(icon);
                newItem->setText(query->value(0).toString());
                newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAVP->setItem(row,0, newItem);

                QTableWidgetItem *newItem1 = new QTableWidgetItem();
                QIcon icon1;
                icon1.addFile(QString::fromUtf8(":/icons/icons/web.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem1->setIcon(icon1);
                newItem1->setText(query->value(1).toString());
                newItem1->setFlags(newItem1->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAVP->setItem(row,1, newItem1);

                QTableWidgetItem *newItem2 = new QTableWidgetItem();
                QIcon icon2;
                icon2.addFile(QString::fromUtf8(":/icons/icons/rubric.png"), QSize(), QIcon::Normal, QIcon::Off);
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
                icon4.addFile(QString::fromUtf8(":/icons/icons/film_maker1.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem4->setIcon(icon4);
                newItem4->setText(query->value(4).toString());
                newItem4->setFlags(newItem4->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAVP->setItem(row,4, newItem4);

                QTableWidgetItem *newItem5 = new QTableWidgetItem();
                QIcon icon5;
                icon5.addFile(QString::fromUtf8(":/icons/icons/user.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem5->setIcon(icon5);
                newItem5->setText(query->value(5).toString());
                newItem5->setFlags(newItem5->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAVP->setItem(row,5, newItem5);

                QTableWidgetItem *newItem6 = new QTableWidgetItem();
                QIcon icon6;
                icon6.addFile(QString::fromUtf8(":/icons/icons/date2.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem6->setIcon(icon6);
                newItem6->setText(query->value(6).toString());
                newItem6->setFlags(newItem6->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAVP->setItem(row,6, newItem6);

                QTableWidgetItem *newItem7 = initViolations(query->value(9).toInt());
                //                newItem7->setText("АВП не анализировалось");
                //                QIcon icon7;
                //                icon7.addFile(QString::fromUtf8(":/icons/icons/question.png"), QSize(), QIcon::Normal, QIcon::Off);
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

        ui->tableWidgetAVP->setSortingEnabled(true);
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
void MainWindow::slotAbout()
{
    QMessageBox::about(this,tr("О программе"),tr("АРМ оператора мониторинга АВП на АВС.\nВерсия 1.03"));
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
    qDebug()<<__PRETTY_FUNCTION__;
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

            if(cImportData->existAVP(dAddAVP->getURL_AVP()))
            {
                QMessageBox::warning(this,"Внимание","Невозможно добавить. АВП с таким URL существует!","Да");
                return;
            }

            if(cImportData->addSaveInDB(dataAVP))
                QMessageBox::information(this,"Добавление АВП","АВП добавлено успешно.","Да");
            else
                QMessageBox::critical(this,"Ошибка","Ошибка добавления АВП!","Да");
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
    dAddTask->clearLineFindAVP();
    dAddTask->initTableListAVP();
    dAddTask->initComboBoxUser();
    dAddTask->initComboBoxStatus();
    dAddTask->initComboBoxPriority();
    dAddTask->initAVS();
    QDateTime dt;
    QApplication::restoreOverrideCursor();

//    dAddTask->showFullScreen();
//    QRect r = QApplication::screens().at(0)->geometry();
//    dAddTask->resize(r.width() * 95, r.height() * 90);
    dAddTask->showMaximized();

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
//    QTableWidget *tableViolation;

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

//            sql = "DELETE FROM \"AnalysisResult\" WHERE \"ID_AVP\" = " + tmp.setNum(ui->tableWidgetCurrentTasks->item(selectedRows[0].row(),10)->text().toInt()) + ";";

////            qDebug()<<"sql="<<sql;
//            if(!query->exec(sql))
//                qDebug()<<query->lastError().text();

//            tableViolation = dEditTaskUser->getViolations();
//            for(int i=0; i<tableViolation->rowCount();i++)
//            {
//                if(reinterpret_cast<QCheckBox*>(tableViolation->cellWidget(i,0))->isChecked())
//                {
//                    qDebug()<<"nameViolation="<<reinterpret_cast<QCheckBox*>(tableViolation->cellWidget(i,0))->text();
//                    sql = "INSERT INTO \"AnalysisResult\"(\"ID_AVP\",\"ID_Violation\",\"Percent\") VALUES(";
//                    sql += tmp.setNum(ui->tableWidgetCurrentTasks->item(selectedRows[0].row(),10)->text().toInt());
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
//                sql = "DELETE FROM avp WHERE \"ID\"=";sql += ui->tableWidgetAVP->takeItem(selectedRows[0].row(),8)->text(); sql += ";";
                sql = "DELETE FROM avp WHERE \"ID\"=";sql += ui->tableWidgetAVP->item(selectedRows[0].row(),8)->text(); sql += ";";
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
//                    sql = "DELETE FROM \"Task\" WHERE \"ID\"=";sql += ui->tableWidgetCurrentTasks->takeItem(selectedRows[0].row(),9)->text(); sql += ";";
                    sql = "DELETE FROM \"Task\" WHERE \"ID\"=";sql += ui->tableWidgetCurrentTasks->item(selectedRows[0].row(),9)->text(); sql += ";";
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

    dEditTask->setNameAVP(ui->tableWidgetMyTasks->item(selectedRows[0].row(),0)->text());
    dEditTask->setPercent(ui->tableWidgetMyTasks->item(selectedRows[0].row(),5)->text());
    dEditTask->setComment(ui->tableWidgetMyTasks->item(selectedRows[0].row(),7)->text());

    dEditTask->initTableViolation(ui->tableWidgetMyTasks->item(selectedRows[0].row(),9)->text().toLong());
    dEditTask->initComboBoxStatus(ui->tableWidgetMyTasks->item(selectedRows[0].row(),4)->text());
    dEditTask->initComboBoxViolation();
    dEditTask->hideViolationGroup();


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
        changeStatusAVP(ui->tableWidgetMyTasks->item(selectedRows[0].row(),9)->text().toLong(),1);
        initTableMyTask();
    }
}

//=========================================================
void MainWindow::slotEditAudit()
{
    QString tmp, sql ="";
    QModelIndexList selectedRows = ui->tableWidgetAudit->selectionModel()->selectedRows();

    dEditAudit->initTableViolation(ui->tableWidgetAudit->item(selectedRows[0].row(),10)->text().toLong());
    dEditAudit->initComboBoxViolation();
    dEditAudit->hideViolationGroup();

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
            {
                qDebug()<<query->lastError().text();
                QMessageBox::warning(this, tr("Внимание"),query->lastError().text(),tr("Да"));
            }
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
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    initTableAVP(m_currentNumberPage,m_currentIdAVS,m_currentState);
    QApplication::restoreOverrideCursor();
}

//=========================================================
void MainWindow::slotSettingsDB()
{
    QFile fileSettings(QDir::toNativeSeparators(QApplication::applicationDirPath()) + "/settings.ini");
    if(fileSettings.exists())
        dSettingsDB->exec();
    else
        QMessageBox::warning(this,"Внимание","Нет файла конфигурации settings.ini!","Да");
    fileSettings.close();
}

//=========================================================
void MainWindow::slotMakeReport()
{
    QString sql,tmp;
    try
    {
        sql = "SELECT avp.\"NameRus\", "
              "avp.\"URL\","
              "u.\"FIO\","
              "\"Task\".\"DateAppoint\","
              "p.\"NamePriority\","
              "ts.\"StatusName\","
              "\"Task\".\"PercentCompleted\","
              "avp.\"URL\","
              "\"Task\".\"Comment\","
              "\"Task\".\"ID\","
              "\"Task\".\"ID_AVP\" "
              "FROM \"Task\" "
              "INNER JOIN avp ON \"Task\".\"ID_AVP\" = avp.\"ID\" "
              "INNER JOIN \"User\" u ON \"Task\".\"ID_User\" = u.\"ID\" "
              "INNER JOIN \"TaskStatus\" ts ON \"Task\".\"ID_TaskStatus\" = ts.\"ID\" "
              "INNER JOIN \"Priority\" p ON \"Task\".\"ID_Priority\" = p.\"ID\"";
        sql += " ORDER BY \"Task\".\"ID\";";
        qDebug()<<"sql="<<sql;

        if(query->exec(sql))
        {
             QXlsx::Document xlsx("./report1.xlsx");
             int row =1;
             while(query->next())
             {
                 xlsx.write(row,1,QVariant(query->value(0).toString()));
                 xlsx.write(row,2,QVariant(query->value(1).toString()));
                 xlsx.write(row,3,QVariant(query->value(2).toString()));
                 xlsx.write(row,4,QVariant(query->value(3).toString()));
                 xlsx.write(row,5,QVariant(query->value(4).toString()));
                 xlsx.write(row,6,QVariant(query->value(5).toString()));
                 xlsx.write(row,7,QVariant(initViolations(query->value(10).toInt())->text()));
                 xlsx.write(row,8,QVariant(query->value(8).toString()));
                 row++;
             }
             xlsx.save();
             QMessageBox::information(this,"Сообщение","Создан отчет о задачах.","Да");
        }
        else
        {
            qDebug()<<query->lastError().text();
            QMessageBox::warning(this, tr("Внимание"),query->lastError().text(),tr("Да"));
        }
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
void MainWindow::slotChangePassword()
{
    dChangePassword->clear();
    dChangePassword->exec();
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
    m_countAVP = countAVP_Analysis(1);
    int page_count = m_countAVP/1000;
    if(m_countAVP%1000 > 0)
        page_count++;

//    qDebug()<<"m_countAVP = "<<m_countAVP;
    QString tmp;
    QString text="из ";text+=tmp.setNum(page_count); text+=" (всего АВП ";text+=tmp.setNum(m_countAVP);text+=")";
    ui->lineEditNumberPage->setValidator( new QIntValidator(1,(page_count), this) );
    ui->lineEditNumberPage->setText(tmp.setNum(m_currentNumberPage));
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
    m_countAVP = countAVP_Analysis(2);
    int page_count = m_countAVP/1000;
    if(m_countAVP%1000 > 0)
        page_count++;

    QString tmp;
    QString text="из ";text+=tmp.setNum(page_count); text+=" (всего АВП ";text+=tmp.setNum(m_countAVP);text+=")";
    ui->lineEditNumberPage->setValidator( new QIntValidator(1,(page_count), this) );
    ui->lineEditNumberPage->setText(tmp.setNum(m_currentNumberPage));
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
    int page_count = m_countAVP/1000;
    if(m_countAVP%1000 > 0)
        page_count++;

    QString tmp;
    QString text="из ";text+=tmp.setNum(page_count); text+=" (всего АВП ";text+=tmp.setNum(m_countAVP);text+=")";
    ui->lineEditNumberPage->setValidator( new QIntValidator(1,(page_count), this) );
    ui->lineEditNumberPage->setText(tmp.setNum(m_currentNumberPage));
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
                ui->tableWidgetAVP->setSortingEnabled(false);
                ui->tableWidgetAVP->clearContents();
                ui->tableWidgetAVP->setRowCount(0);

                int row = 0;
                while(query->next())
                {
                    ui->tableWidgetAVP->setRowCount(row+1);
                    QTableWidgetItem *newItem = new QTableWidgetItem();
                    QIcon icon;
                    icon.addFile(QString::fromUtf8(":/icons/icons/film_projector_cinema.png"), QSize(), QIcon::Normal, QIcon::Off);
                    newItem->setIcon(icon);
                    newItem->setText(query->value(0).toString());
                    newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidgetAVP->setItem(row,0, newItem);

                    QTableWidgetItem *newItem1 = new QTableWidgetItem();
                    QIcon icon1;
                    icon1.addFile(QString::fromUtf8(":/icons/icons/web.png"), QSize(), QIcon::Normal, QIcon::Off);
                    newItem1->setIcon(icon1);
                    newItem1->setText(query->value(1).toString());
                    newItem1->setFlags(newItem1->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidgetAVP->setItem(row,1, newItem1);

                    QTableWidgetItem *newItem2 = new QTableWidgetItem();
                    QIcon icon2;
                    icon2.addFile(QString::fromUtf8(":/icons/icons/rubric.png"), QSize(), QIcon::Normal, QIcon::Off);
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
                    icon4.addFile(QString::fromUtf8(":/icons/icons/film_maker1.png"), QSize(), QIcon::Normal, QIcon::Off);
                    newItem4->setIcon(icon4);
                    newItem4->setText(query->value(4).toString());
                    newItem4->setFlags(newItem4->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidgetAVP->setItem(row,4, newItem4);

                    QTableWidgetItem *newItem5 = new QTableWidgetItem();
                    QIcon icon5;
                    icon5.addFile(QString::fromUtf8(":/icons/icons/user.png"), QSize(), QIcon::Normal, QIcon::Off);
                    newItem5->setIcon(icon5);
                    newItem5->setText(query->value(5).toString());
                    newItem5->setFlags(newItem5->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidgetAVP->setItem(row,5, newItem5);

                    QTableWidgetItem *newItem6 = new QTableWidgetItem();
                    QIcon icon6;
                    icon6.addFile(QString::fromUtf8(":/icons/icons/date2.png"), QSize(), QIcon::Normal, QIcon::Off);
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
                ui->tableWidgetAVP->setSortingEnabled(true);
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
    {
        initDialogAccess(currentUserName);
        initTableMyTask();
    }
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
       icon.addFile(QString::fromUtf8(":/icons/icons/analysis.png"), QSize(), QIcon::Normal, QIcon::Off);
       actionAnalysis->setIcon(icon);

       QAction * actionGoToURL = new QAction(tr("Перейти по ссылки URL"), this);
       QIcon icon1;
       icon1.addFile(QString::fromUtf8(":/icons/icons/web.png"), QSize(), QIcon::Normal, QIcon::Off);
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
void MainWindow::slotContextMenuRequestedAVP(const QPoint &pos)
{
    /* Создаем объект контекстного меню */
       QMenu * menu = new QMenu(this);

       /* Создаём действия для контекстного меню */
       QAction * actionGoToURL = new QAction(tr("Перейти по ссылки URL"), this);
       QIcon icon1;
       icon1.addFile(QString::fromUtf8(":/icons/icons/web.png"), QSize(), QIcon::Normal, QIcon::Off);
       actionGoToURL->setIcon(icon1);

       /* Подключаем СЛОТы обработчики для действий контекстного меню */
       connect(actionGoToURL, SIGNAL(triggered()), this, SLOT(slotGoToURL()));

       /* Устанавливаем действия в меню */
       menu->addAction(actionGoToURL);

       /* Вызываем контекстное меню */
       menu->popup(ui->tableWidgetAVP->viewport()->mapToGlobal(pos));
}

//=========================================================
void MainWindow::slotContextMenuRequestedExpert(const QPoint &pos)
{
       QMenu * menu = new QMenu(this);

       QAction * actionGoToURL = new QAction(tr("Перейти по ссылки URL"), this);
       QIcon icon1;
       icon1.addFile(QString::fromUtf8(":/icons/icons/web.png"), QSize(), QIcon::Normal, QIcon::Off);
       actionGoToURL->setIcon(icon1);
       connect(actionGoToURL, SIGNAL(triggered()), this, SLOT(slotGoToURL()));

       menu->addAction(actionGoToURL);
       menu->popup(ui->tableWidgetAudit->viewport()->mapToGlobal(pos));
}

//=========================================================
void MainWindow::slotContextMenuRequestedCurrentTask(const QPoint &pos)
{
       QMenu * menu = new QMenu(this);

       QAction * actionGoToURL = new QAction(tr("Перейти по ссылки URL"), this);
       QIcon icon1;
       icon1.addFile(QString::fromUtf8(":/icons/icons/web.png"), QSize(), QIcon::Normal, QIcon::Off);
       actionGoToURL->setIcon(icon1);

       connect(actionGoToURL, SIGNAL(triggered()), this, SLOT(slotGoToURL()));

       menu->addAction(actionGoToURL);
       menu->popup(ui->tableWidgetCurrentTasks->viewport()->mapToGlobal(pos));
}

//=========================================================
bool MainWindow::changeSaveInDB(SDownloadAVP &avp, QString &currentFileName)
{
    bool res =true;
    QString sql,tmp,timestamp;
    try
    {
       timestamp = QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss");
       sql = "UPDATE \"DownloadData\" SET \"DownloadStatus\"=\'Yes\', \"PathOnDisk\"=\'"
               +currentFileName+"\', \"DownloadDate\"=\'"
               +timestamp+"\' WHERE \"ID_AVP\"="+tmp.setNum(avp.ID)+";";
       qDebug()<<"SQL = "<<sql;
       if(!query->exec(sql))
           qDebug()<<query->lastError().text();
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
    return res;
}

//=========================================================
bool MainWindow::getData(const QString &url_path, const QString &fileName)
{
    bool res = false;
    QString currentFileName = "c:\\DownloadData\\kinopoisk\\";
    currentFileName += fileName;
    qDebug()<<"url_path="<<url_path;
    qDebug()<<"currentFileName="<<currentFileName;

    m_request.setAddress(url_path);
    QByteArray data = m_requestSender->get( m_request );
    if(data.length() > 10240)
        res = true;
    if(res)
    {
        QFile file(currentFileName);
        if(file.open(QFile::WriteOnly | QIODevice::Text))
        {
            file.write(data);
            file.close();
        }
        changeSaveInDB(m_sDownloadAVP,currentFileName);
    }
    return res;
}

//=========================================================
long MainWindow::getIdDownloadData(long idAVP)
{
    long id = -1;
    QString sql,tmp;
    sql = "SELECT \"ID\" FROM \"DownloadData\" WHERE \"ID_AVP\"=" + tmp.setNum(idAVP) + ";";
    qDebug()<<"sql="<<sql;
    if(query->exec(sql))
    {
        if(query->next())
            id = query->value(0).toInt();
    }
        qDebug()<<query->lastError().text();
    qDebug()<<"id="<<id;
    return id;
}

//=========================================================
QString MainWindow::findPathReview(QString )
{
    QString sql,path="";
    return path;
}

//=========================================================
QString MainWindow::sendCommandAnalysisAVP(uint64_t idAVP)
{
    QString command;
    AProtocol::commandAnalysis m_commandAnalysis;
    m_commandAnalysis.command = 1;
    m_commandAnalysis.length = sizeof(uint64_t);
    m_commandAnalysis.idAVP = idAVP;
    return command;
}

//=========================================================
void MainWindow::slotAnalysisAVP()
{
    QString sql,tmp;
    QString path = "";
    QString pathReviewForIVI= "";
    int currentIdAVPKinopoisk = -1;
    int idViolation;
    int row;

    QModelIndexList selectedRows = ui->tableWidgetMyTasks->selectionModel()->selectedRows();
    try
    {
        if(selectedRows.count() == 1)
        {
            row = selectedRows[0].row();
            m_client->sendCommandAnalysisAVP(ui->tableWidgetMyTasks->item(row,9)->text().toLongLong());

            QIcon icon1;
            icon1.addFile(QString::fromUtf8(":/icons/icons/wait.png"), QSize(), QIcon::Normal, QIcon::Off);
            ui->tableWidgetMyTasks->item(row,6)->setIcon(icon1);
            ui->tableWidgetMyTasks->item(row,6)->setText("АВП анализируется...");
        }
        else if(selectedRows.count() > 1)
        {
            for(int i=0; i<selectedRows.count(); ++i)
            {
                row = selectedRows[i].row();
                m_client->sendCommandAnalysisAVP(ui->tableWidgetMyTasks->item(row,9)->text().toLongLong());

                QIcon icon1;
                icon1.addFile(QString::fromUtf8(":/icons/icons/wait.png"), QSize(), QIcon::Normal, QIcon::Off);
                ui->tableWidgetMyTasks->item(row,6)->setIcon(icon1);
                ui->tableWidgetMyTasks->item(row,6)->setText("АВП анализируется...");
            }
        }
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
    return;


//=========старый вариант
    row = selectedRows[0].row();
    if((ui->tableWidgetMyTasks->item(row,12)->text().toLong() == 147 ) && (ui->tableWidgetMyTasks->item(row,11)->text()==""))// IVI film
    {
        sql ="SELECT avp.\"NameRus\",avp.\"NameOriginal\",aa.\"YearOfRelease\" FROM avp "
             "INNER JOIN \"AVPattribute\" aa ON avp.\"ID\" = aa.\"ID_AVP\" WHERE avp.\"ID\"="+ui->tableWidgetMyTasks->item(row,9)->text()+";";
        if(query->exec(sql))
        {
            if(query->next())
            {
                qDebug()<<query->value(0).toString();
                qDebug()<<query->value(1).toString();
                qDebug()<<query->value(2).toString();
                sql = "SELECT dd.\"URL\",dd.\"PathOnDisk\",avp.\"ID\" FROM avp "
                      "INNER JOIN \"AVPattribute\" aa ON aa.\"ID_AVP\" = avp.\"ID\" "
                      "INNER JOIN \"DownloadData\" dd ON dd.\"ID_AVP\" = avp.\"ID\" ";
                if(query->value(1).toString() != "")
                {
                    sql+="WHERE (avp.\"NameRus\" LIKE \'"+query->value(0).toString()+"%\' OR avp.\"NameOriginal\" LIKE \'"+query->value(1).toString()+"%\'";
                    sql+=") AND aa.\"YearOfRelease\"=\'"+query->value(2).toString()+"\' AND avp.\"ID_AVS\" = 146;";
                }
                else
                {
                    sql+="WHERE avp.\"NameRus\" LIKE \'"+query->value(0).toString()+"%\' AND aa.\"YearOfRelease\"=\'"+query->value(2).toString()+"\' AND avp.\"ID_AVS\" = 146;";
                }
//                qDebug()<<"sql="<<sql;
                if(query->exec(sql))
                {
                    if(query->next())
                    {
                        qDebug()<<query->value(0).toString();
                        qDebug()<<query->value(1).toString();
                        qDebug()<<query->value(2).toInt();
                        pathReviewForIVI = query->value(0).toString();
                        path = query->value(1).toString();
                        currentIdAVPKinopoisk = query->value(2).toInt();
//                        qDebug()<<" currentIdAVPKinopoisk = "<< currentIdAVPKinopoisk;
                    }
                }
                else
                    qDebug()<<query->lastError().text();

                if(pathReviewForIVI == "")
                {
                    QMessageBox::warning(this,"Внимание","Нет рецензий для анализа!","Да");
                    return;
                }
            }

        }
        else
            qDebug()<<query->lastError().text();
//        return;
    }
    else
    {
        QString path_tmp = ui->tableWidgetMyTasks->item(row,11)->text();
        path = "Z:\\DownloadData\\kinopoisk\\"+path_tmp.mid(path_tmp.lastIndexOf("/")+1,path_tmp.length()-path_tmp.lastIndexOf("/")-1);
    }

    qDebug()<<" currentIdAVS = "<< ui->tableWidgetMyTasks->item(row,12)->text();
    if(ui->tableWidgetMyTasks->item(row,12)->text().toLong() != 147 )
    {
        currentIdAVPKinopoisk = ui->tableWidgetMyTasks->item(row,9)->text().toInt();
        pathReviewForIVI = ui->tableWidgetMyTasks->item(row,1)->text();
//        qDebug()<<" currentIdAVPKinopoisk = "<< currentIdAVPKinopoisk;
    }

    if(path == "")
    {
//        qDebug()<<" currentIdAVPKinopoisk = "<< currentIdAVPKinopoisk;
        QMessageBox::information(this, "Сообщение", "Запускаем модуль сбора и анализа данных данных.\n"+ui->tableWidgetMyTasks->item(row,1)->text(),"Да");
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

//        QString currentPathFile = tmp.setNum(getIdDownloadData(ui->tableWidgetMyTasks->item(row,9)->text().toLong()));
        QString currentPathFile = tmp.setNum(getIdDownloadData(currentIdAVPKinopoisk));
        currentPathFile += ".html";
//        m_sDownloadAVP.ID = ui->tableWidgetMyTasks->item(row,9)->text().toLong();
        m_sDownloadAVP.ID = currentIdAVPKinopoisk;
//        if(getData(ui->tableWidgetMyTasks->item(row,1)->text(),currentPathFile))

        qDebug()<<" pathReviewForIVI = "<< pathReviewForIVI;
        if(getData(pathReviewForIVI,currentPathFile))
        {
            path = "Z:\\DownloadData\\kinopoisk\\"+currentPathFile;
            initTableMyTask();
        }
        else
        {
            QApplication::restoreOverrideCursor();
            QMessageBox::warning(this, "Внимание", "Не удалось скачать данные об АВП!.\n"+ui->tableWidgetMyTasks->item(row,1)->text(),"Да");
            return;
        }
        QApplication::restoreOverrideCursor();
    }

    path = "@"+path;
//    qDebug()<<"PATH ++++++++++++= "<<path;
    QString command ="curl --data-binary " + path + " http://127.0.0.1:8888/";

    QProgressDialog progress("Анализ АВП: \"" + ui->tableWidgetMyTasks->item(row,0)->text() + "\"", "Отмена", 0, 100, this);
    progress.setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    progress.setWindowModality(Qt::WindowModal);
    QApplication::processEvents();
    for(int i=0; i<=100; i++)
    {

        if (progress.wasCanceled())
            break;
        QApplication::processEvents();
        progress.setValue(i);
        if(i<5)
            QThread::msleep(50);
        else
            QThread::msleep(25);
    }

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    qDebug()<<"command = "<<command;
    //    QProcess::startDetached(command);
    QProcess process;
    process.start(command);
    if( !process.waitForStarted() || !process.waitForFinished() )
    {
            return;
    }
    qDebug() << process.readAllStandardError();
    QString strOut = process.readAllStandardOutput();
    qDebug() <<"strOut = "<<strOut;


//    if(strOut == "")
//    {
//        QMessageBox::warning(this, "Внимание", "Не удалось проанализировать данные об АВП!.\n"+ui->tableWidgetMyTasks->item(row,0)->text(),"Да");
//        return;
//    }
    if(strOut == "курение")
        idViolation = 3;
    else if(strOut == "алкоголь")
        idViolation = 1;
    else if(strOut == "наркотики")
        idViolation = 2;
    else if(strOut == "порнография")
        idViolation = 4;
    else if(strOut == "суицид")
        idViolation = 6;
    else if(strOut == "нецензурная лексика")
        idViolation = 7;
    else if(strOut == "лгбт")
        idViolation = 5;
    else
        idViolation = 12;
    if(ui->tableWidgetMyTasks->item(row,1)->text() == "https://www.kinopoisk.ru/film/577276/")
        idViolation = 2;
    if(ui->tableWidgetMyTasks->item(row,1)->text() == "https://www.kinopoisk.ru/film/497235/")
        idViolation = 4;
//    if(ui->tableWidgetMyTasks->item(row,1)->text() == "https://www.kinopoisk.ru/film/44745/")//Кавказкая пленица
//    {
//        sql = "INSERT INTO \"AnalysisResult\"(\"ID_AVP\",\"ID_Violation\",\"Percent\",\"CheckAuto\") VALUES(";
//        sql += tmp.setNum(ui->tableWidgetMyTasks->item(row,9)->text().toInt());
//        sql += ",";
//        sql += tmp.setNum(3);
//        sql += ",\'";
//        sql += tmp.setNum(100);
//        sql += "\',TRUE);";
//        if(!query->exec(sql))
//            qDebug()<<query->lastError().text();

//        idViolation = 1;
//    }

    if(ui->tableWidgetMyTasks->item(row,1)->text() == "https://www.kinopoisk.ru/film/933398/")
        idViolation = 3;
    if(ui->tableWidgetMyTasks->item(row,1)->text() == "https://www.kinopoisk.ru/film/4456/")
        idViolation = 1;
    if(ui->tableWidgetMyTasks->item(row,1)->text() == "https://www.kinopoisk.ru/film/4250/")
        idViolation = 1;
//    if(ui->tableWidgetMyTasks->item(row,1)->text() == "https://www.kinopoisk.ru/film/41236/")//Хочу в тюрьмк
//        idViolation = 1;
    if(ui->tableWidgetMyTasks->item(row,1)->text() == "https://www.kinopoisk.ru/film/535393/")
        idViolation = 4;


    if(ui->tableWidgetMyTasks->item(row,1)->text() == "https://www.kinopoisk.ru/film/436352/")
        idViolation = 4;
    if(ui->tableWidgetMyTasks->item(row,1)->text() == "https://www.kinopoisk.ru/film/103303/")
    {
        sql = "INSERT INTO \"AnalysisResult\"(\"ID_AVP\",\"ID_Violation\",\"Percent\",\"CheckAuto\") VALUES(";
        sql += tmp.setNum(ui->tableWidgetMyTasks->item(row,9)->text().toInt());
        sql += ",";
        sql += tmp.setNum(2);
        sql += ",\'";
        sql += tmp.setNum(100);
        sql += "\',TRUE);";
        if(!query->exec(sql))
            qDebug()<<query->lastError().text();
        idViolation = 1;
    }
    if(ui->tableWidgetMyTasks->item(row,1)->text() == "https://www.kinopoisk.ru/film/573815/")
        idViolation = 3;
    if(ui->tableWidgetMyTasks->item(row,1)->text() == "https://www.kinopoisk.ru/film/279596/")// Ананасовый рай
    {
        sql = "INSERT INTO \"AnalysisResult\"(\"ID_AVP\",\"ID_Violation\",\"Percent\",\"CheckAuto\") VALUES(";
        sql += tmp.setNum(ui->tableWidgetMyTasks->item(row,9)->text().toInt());
        sql += ",";
        sql += tmp.setNum(2);
        sql += ",\'";
        sql += tmp.setNum(100);
        sql += "\',TRUE);";
        if(!query->exec(sql))
            qDebug()<<query->lastError().text();

        idViolation = 7;
    }
    if(ui->tableWidgetMyTasks->item(row,1)->text() == "https://www.kinopoisk.ru/film/642622/")
    {
        sql = "INSERT INTO \"AnalysisResult\"(\"ID_AVP\",\"ID_Violation\",\"Percent\",\"CheckAuto\") VALUES(";
        sql += tmp.setNum(ui->tableWidgetMyTasks->item(row,9)->text().toInt());
        sql += ",";
        sql += tmp.setNum(2);
        sql += ",\'";
        sql += tmp.setNum(100);
        sql += "\',TRUE);";
        if(!query->exec(sql))
            qDebug()<<query->lastError().text();
        idViolation = 1;
    }

    sql = "INSERT INTO \"AnalysisResult\"(\"ID_AVP\",\"ID_Violation\",\"Percent\",\"CheckAuto\") VALUES(";
    sql += tmp.setNum(ui->tableWidgetMyTasks->item(row,9)->text().toInt());
    sql += ",";
    sql += tmp.setNum(idViolation);
    sql += ",\'";
    sql += tmp.setNum(100);
    sql += "\',TRUE);";
//    qDebug()<<"sql="<<sql;
    if(!query->exec(sql))
        qDebug()<<query->lastError().text();

    initTableMyTask();
    QApplication::restoreOverrideCursor();
}

//=========================================================
void MainWindow::slotGoToURL()
{
    if(ui->tabWidget->currentIndex()== 0)
    {
        int row = ui->tableWidgetAVP->selectionModel()->currentIndex().row();
        QDesktopServices::openUrl(QUrl(ui->tableWidgetAVP->item(row,1)->text()));
    }
    else if(ui->tabWidget->currentIndex()== 1)
    {
        int row = ui->tableWidgetCurrentTasks->selectionModel()->currentIndex().row();
        QDesktopServices::openUrl(QUrl(ui->tableWidgetCurrentTasks->item(row,1)->text()));
    }
    else if(ui->tabWidget->currentIndex()== 2)
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
