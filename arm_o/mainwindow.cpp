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
    qDebug()<<__PRETTY_FUNCTION__;
    ui->setupUi(this);

//    dStartProgressDialog = new DStartProgressDialog();
//    dStartProgressDialog->moveToThread(&startProgressDialogThread);
//    connect(&startProgressDialogThread, &QThread::finished, dStartProgressDialog, &QObject::deleteLater);
//    connect(this, &MainWindow::operateStartProgressDialog,dStartProgressDialog, &DStartProgressDialog::doWork);
////    connect(dStartProgressDialog, &DStartProgressDialog::resultReady, this, &MainWindow::handleResults);
//    startProgressDialogThread.start();
//    emit operateStartProgressDialog("");

    ui->toolButtonExcel->hide();
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
    connect(ui->actionReportAVP, SIGNAL(triggered()), SLOT(slotMakeReport()));
    connect(ui->actionReportAllStatistics, SIGNAL(triggered()), SLOT(slotMakeReportAll()));
    connect(ui->actionJournalSession, SIGNAL(triggered()), SLOT(slotJournalSession()));
    connect(ui->actionJournalJob, SIGNAL(triggered()), SLOT(slotJournalJob()));

    connect(ui->radioButtonViolation,SIGNAL(toggled(bool)), SLOT(slotRBViolation(bool)));
    connect(ui->radioButtonChecked,SIGNAL(toggled(bool)), SLOT(slotRBChecked(bool)));
    connect(ui->radioButtonAll,SIGNAL(toggled(bool)), SLOT(slotRBAll(bool)));
    connect(ui->lineEditFindString,SIGNAL(returnPressed()),SLOT(slotFindAVP()));
    connect(ui->lineEditFindString,SIGNAL(textChanged(const QString&)),SLOT(slotTextChanged(const QString&)));

    connect(ui->lineEditFindString_2,SIGNAL(returnPressed()),SLOT(slotFindArchiveAVP()));
    connect(ui->lineEditFindString_2,SIGNAL(textChanged(const QString&)),SLOT(slotTextChanged1(const QString&)));

    connect(ui->tableWidgetAVP, SIGNAL(cellDoubleClicked(int, int)),this, SLOT(slotEditAVP(int, int)));
    connect(ui->tableWidgetCurrentTasks, SIGNAL(cellDoubleClicked(int, int)),this, SLOT(slotEditTask(int, int)));
    connect(ui->tableWidgetMyTasks, SIGNAL(cellDoubleClicked(int, int)),this, SLOT(slotEditMyTask(int, int)));
    connect(ui->tableWidgetAudit, SIGNAL(cellDoubleClicked(int, int)),this, SLOT(slotEditAudit(int, int)));
    connect(ui->tableWidgetAudit_2, SIGNAL(cellDoubleClicked(int, int)),this, SLOT(slotEditAuditRKN(int, int)));
    connect(ui->tableWidgetAudit_4, SIGNAL(cellDoubleClicked(int, int)),this, SLOT(slotEditCurrentAudit(int, int)));
    connect(ui->tableWidgetAuditArchive, SIGNAL(cellDoubleClicked(int, int)),this, SLOT(slotPreviewAchive(int, int)));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)),this, SLOT(slotCurrentChanged(int)));

    query = nullptr;
    queryViolation = nullptr;
    dSettingsDB = new DSettingsDB(this);

    if(!initDB())
        exit(0);

    initAVS();
    initTableAVP();
//    initTableTask();
//    initTableMyTask();

    initComboBoxStatusArchive(ui->comboBoxStatusAchive);
    initComboBoxUser(ui->comboBoxUser);
    initComboBoxStatus(ui->comboBoxStatus);
    initComboBoxPriority(ui->comboBoxPriority);
    initComboBoxUser(ui->comboBoxUser_2);
    initComboBoxUser(ui->comboBoxUser_4);
    initComboBoxOperator(ui->comboBoxUser_3);
    initComboBoxExpert(ui->comboBoxExpert);
    initComboBoxPriority(ui->comboBoxPriority_2);
    initComboBoxPriority(ui->comboBoxPriority_3);
    ui->dateEditDateBegin->setDate(QDate::currentDate());
    ui->dateEditDateBegin_3->setDate(QDate::currentDate());
    ui->dateEditDateBegin_4->setDate(QDate::currentDate());
    ui->dateEditDateBegin_2->setDate(QDate::currentDate());
    ui->dateEditDateEnd->setDate(QDate::currentDate());
    ui->dateEditDateEnd_2->setDate(QDate::currentDate());
    ui->dateEditDateEnd_3->setDate(QDate::currentDate());
    ui->dateEditDateEnd_4->setDate(QDate::currentDate());

    ui->tableWidgetAVP->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidgetAVP, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotContextMenuRequestedAVP(QPoint)));

    ui->tableWidgetCurrentTasks->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidgetCurrentTasks, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotContextMenuRequestedCurrentTask(QPoint)));

    ui->tableWidgetMyTasks->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidgetMyTasks, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotContextMenuRequested(QPoint)));

    ui->tableWidgetAudit->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidgetAudit, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotContextMenuRequestedExpert(QPoint)));

    ui->tableWidgetAudit_4->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidgetAudit_4, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotContextMenuRequestedExpert_4(QPoint)));

    ui->tableWidgetAudit_2->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidgetAudit_2, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotContextMenuRequestedExpert_2(QPoint)));

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

//    m_timerStartInitDialog.setInterval(2000);
//    m_timerStartInitDialog.setSingleShot(true);
//    connect(&m_timer, &QTimer::timeout, this, [&]{slotInitDialog();});
//    m_timerStartInitDialog.start();

//     QTimer::singleShot(2000, this, SLOT(slotInitDialog()));

    m_requestSender = new RequestSender();

    dImportDataOutExcel = new DImportDataOutExcel(this);
    dImportDataOutExcel->setModal(false);

    dAddAVP = new DAddAVP(this);
    dAddAVP->setModal(false);

    dEditAVP = new DEditAVP(this);
    dAddTask = new DAddTask(this);
    dEditTask = new DEditTask(this);
    dEditTaskUser = new DEditTaskUser(this);
    dGroup = new DGroup(this);
    dUser = new DUser(this);
    dViolation = new DViolation(this);
    dEnter1 = new DEnter();
    dEditAudit = new DEditAudit(this);
    dChangePassword = new DChangePassword(this);
    dJournalSession = new DJournalSession(this);
    dJournalJobAVP = new DJournalJobAVP(this);
    dReportJob = new DReportJob(this);
    dReportAllStatistics = new DReportAllStatistics(this);
    dAppointExpert = new DAppointExpert(this);
    dPreviewArchive = new DPreviewArchive(this);

    m_currentIdAVS = -1;
    m_currentState = -1;
    m_currentNumberPage = 1;
    m_currentArchiveNumberPage = 1;
    m_countAVP = countAVP();
    m_countArchiveAVP = countArchiveAVP();

    int page_count = (m_countAVP/1000);
    if( m_countAVP%1000 > 0)
        page_count++;
    QString tmp;
    QString text="из ";text+=tmp.setNum(page_count); text+=" (всего АВП ";text+=tmp.setNum(m_countAVP);text+=")";
    ui->lineEditNumberPage->setValidator( new QIntValidator(1,(page_count), this) );
    ui->labelCountAVP->setText(text);
    ui->pushButtonPreview->setEnabled(false);
    if(page_count == 1)
        ui->pushButtonNext->setEnabled(false);

    int pageArchiveCount = (m_countArchiveAVP/1000);
    if( m_countArchiveAVP%1000 > 0)
        pageArchiveCount++;
    text="из ";text+=tmp.setNum(pageArchiveCount); text+=" (всего АВП ";text+=tmp.setNum(m_countArchiveAVP);text+=")";
    ui->lineEditNumberPage_2->setValidator( new QIntValidator(1,(page_count), this) );
    ui->labelCountAVP_2->setText(text);
    ui->pushButtonPreview_2->setEnabled(false);
    if(pageArchiveCount == 1)
        ui->pushButtonNext_2->setEnabled(false);

    m_timer.setInterval(1000);
    m_timer.setSingleShot(true);
    connect(&m_timer, &QTimer::timeout, this, [&]{slotFindAVP();});

    m_timer1.setInterval(1000);
    m_timer1.setSingleShot(true);
    connect(&m_timer1, &QTimer::timeout, this, [&]{slotFindArchiveAVP();});

    ui->labelStateConnectDB->hide();

    initNetClient();
//    startProgressDialogThread.quit();
//    startProgressDialogThread.terminate();
}

//=========================================================
void MainWindow::slotInitDialog()
{
    qDebug()<<__PRETTY_FUNCTION__;
    m_requestSender = new RequestSender();

    dImportDataOutExcel = new DImportDataOutExcel(this);
    dImportDataOutExcel->setModal(false);

    dAddAVP = new DAddAVP(this);
    dAddAVP->setModal(false);

    dEditAVP = new DEditAVP(this);
    dAddTask = new DAddTask(this);
    dEditTask = new DEditTask(this);
    dEditTaskUser = new DEditTaskUser(this);
    dGroup = new DGroup(this);
    dUser = new DUser(this);
    dViolation = new DViolation(this);
    dEnter1 = new DEnter();
    dEditAudit = new DEditAudit(this);
    dChangePassword = new DChangePassword(this);
    dJournalSession = new DJournalSession(this);
    dJournalJobAVP = new DJournalJobAVP(this);
    dReportJob = new DReportJob(this);
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
    delete m_client;
}

//=========================================================
void MainWindow::closeEvent(QCloseEvent *)
{
//    slotExit();
    saveInJournalRecordClose();
}

//=========================================================
void MainWindow::slotAppointExpert()
{
    QString sql,tmp;
    int id_expert;
    if(dAppointExpert->exec() == QDialog::Accepted)
    {
        id_expert = getIdUser(dAppointExpert->getExpertName());
        QModelIndexList selectedRows = ui->tableWidgetAudit_4->selectionModel()->selectedRows();
        while (!selectedRows.empty())
        {
            sql ="UPDATE \"Task\" SET \"ID_Expert\" = ";
            sql += tmp.setNum(id_expert);
            sql += " WHERE \"ID\"=";
            sql += ui->tableWidgetAudit_4->item(selectedRows[0].row(),11)->text();
            sql += ";";
//            qDebug()<<sql;
            if(query->exec(sql))
            {
                addRecordJournalJobAVP(1,"Назначение эксперта: "+ dAppointExpert->getExpertName() + " для задачи АВП", getNameRusAVP(ui->tableWidgetAudit_4->item(selectedRows[0].row(),12)->text().toLong()));
                ui->tableWidgetAudit_4->removeRow(selectedRows[0].row());
                selectedRows = ui->tableWidgetAudit_4->selectionModel()->selectedRows();
            }
            else
            {
                qDebug()<<query->lastError().text();
                break;
            }
        }
        initTableCurrentAudit();
    }
}

//=========================================================
void MainWindow::saveInJournalRecordClose()
{
    QString sql;
    QString fio;

    sql = "INSERT INTO \"Session\"(\"SessionDate\",\"ID_User\",\"Info\") VALUES(\'";
    sql += QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"); sql += "\',";
    sql += fio.setNum(getIdUser(currentUserName)); sql += ",\'Выход из приложения\');";
    //    qDebug()<<sql;
    if(!query->exec(sql))
        qDebug()<<query->lastError().text();
}

//=========================================================
void MainWindow::addRecordJournalJobAVP(int category, QString info,QString nameAVP)
{
//    qDebug()<<__PRETTY_FUNCTION__;
    QString sql;
    QString fio;
    QString tmp;
    sql = "INSERT INTO \"JournalJobAVP\"(\"DateEvent\",\"ID_User\",\"Info\",\"Сategory\",\"NameAVP\") VALUES(\'";
    sql += QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"); sql += "\',";
    sql += fio.setNum(getIdUser(currentUserName)); sql += ",\'"; sql += info; sql +="\',";
    sql += tmp.setNum(category);
    sql += ",\'"; sql+=nameAVP;sql += "\');";
//    qDebug()<<sql;
    if(!query->exec(sql))
        qDebug()<<query->lastError().text();
}

//=========================================================
void MainWindow::initDialogAccess(QString userName)
{
    QString sql;
    QString groupCategory;
    try
    {
//        sql = "SELECT \"GroupCategory\" FROM \"Group\" WHERE \"ID\" = (SELECT \"ID_Group\" FROM \"User\" WHERE \"FIO\"=\'" + userName + "\');";
        sql = "SELECT \"GroupName\" FROM \"Group\" WHERE \"ID\" = (SELECT \"ID_Group\" FROM \"User\" WHERE \"FIO\"=\'" + userName + "\');";
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
               ui->tabWidget->setTabEnabled( 4, false);
               ui->tabWidget->setTabEnabled( 5, false);
               ui->menu_2->setEnabled(false);
               ui->menu_4->setEnabled(false);
               ui->menu_5->setEnabled(false);
               ui->menu_6->setEnabled(false);
               ui->tabWidget->setCurrentIndex(2);
               ui->pushButtonDeleteAVP->setEnabled(false);
           }
           else if(groupCategory == "Администраторы")
           {
               ui->tabWidget->setTabEnabled( 1, true);
               ui->tabWidget->setTabEnabled( 2, true);
               ui->tabWidget->setTabEnabled( 3, true);
               ui->tabWidget->setTabEnabled( 4, true);
               ui->tabWidget->setTabEnabled( 5, true);
               ui->tabWidget->setTabEnabled( 6, true);
               ui->menu_2->setEnabled(true);
               ui->menu_4->setEnabled(true);
               ui->menu_5->setEnabled(true);
               ui->menu_6->setEnabled(true);
               ui->tabWidget->setCurrentIndex(0);
               ui->pushButtonDeleteAVP->setEnabled(true);
           }
           else if((groupCategory == "Руководство") || (groupCategory == "Начальники групп"))
           {
               ui->tabWidget->setTabEnabled( 1, true);
               ui->tabWidget->setTabEnabled( 2, true);
               ui->tabWidget->setTabEnabled( 3, false);
               ui->tabWidget->setTabEnabled( 4, false);
               ui->tabWidget->setTabEnabled( 5, false);
               ui->menu_4->setEnabled(false);
               ui->menu_5->setEnabled(true);
               ui->menu_6->setEnabled(true);
               ui->tabWidget->setCurrentIndex(1);
               ui->pushButtonDeleteAVP->setEnabled(false);
           }
           else if(groupCategory == "Эксперты")
           {
               ui->tabWidget->setTabEnabled( 1, false);
               ui->tabWidget->setTabEnabled( 2, false);
               ui->tabWidget->setTabEnabled( 3, false);
               ui->tabWidget->setTabEnabled( 4, true);
               ui->tabWidget->setTabEnabled( 5, false);
               ui->menu_2->setEnabled(false);
               ui->menu_4->setEnabled(false);
               ui->menu_5->setEnabled(false);
               ui->menu_6->setEnabled(false);
               ui->tabWidget->setCurrentIndex(4);
               ui->pushButtonDeleteAVP->setEnabled(false);
           }
           else if(groupCategory == "Руководитель экспертов")
           {
               ui->tabWidget->setTabEnabled( 1, true);
               ui->tabWidget->setTabEnabled( 2, false);
               ui->tabWidget->setTabEnabled( 3, true);
               ui->tabWidget->setTabEnabled( 4, true);
               ui->tabWidget->setTabEnabled( 5, false);
               ui->menu_2->setEnabled(true);
               ui->menu_4->setEnabled(false);
               ui->menu_5->setEnabled(true);
               ui->menu_6->setEnabled(true);
               ui->tabWidget->setCurrentIndex(3);
               ui->pushButtonDeleteAVP->setEnabled(false);
           }
           else if(groupCategory == "Инспектор РКН")
           {
               ui->tabWidget->setTabEnabled( 1, true);
               ui->tabWidget->setTabEnabled( 2, false);
               ui->tabWidget->setTabEnabled( 3, false);
               ui->tabWidget->setTabEnabled( 4, false);
               ui->tabWidget->setTabEnabled( 5, true);
               ui->menu_2->setEnabled(true);
               ui->menu_4->setEnabled(false);
               ui->menu_5->setEnabled(true);
               ui->menu_6->setEnabled(true);
               ui->tabWidget->setCurrentIndex(5);
               ui->pushButtonDeleteAVP->setEnabled(false);
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

    ui->tableWidgetAVP->horizontalHeader()->resizeSection(0, 250);//Название АВП
    ui->tableWidgetAVP->horizontalHeader()->resizeSection(1, 300);//URL
    ui->tableWidgetAVP->horizontalHeader()->resizeSection(2, 130);//Рубрика
    ui->tableWidgetAVP->horizontalHeader()->resizeSection(3, 80);//Год выпуска
    ui->tableWidgetAVP->horizontalHeader()->resizeSection(4, 250);//Режиссер
    ui->tableWidgetAVP->horizontalHeader()->resizeSection(5, 80);//Возраст
    ui->tableWidgetAVP->horizontalHeader()->resizeSection(6, 150);//ФИО
    ui->tableWidgetAVP->horizontalHeader()->resizeSection(7, 130);//Дата
    ui->tableWidgetAVP->horizontalHeader()->setSectionResizeMode(8,QHeaderView::Stretch);//Нарушения
    ui->tableWidgetAVP->horizontalHeader()->resizeSection(9, 0);//ID

    ui->tableWidgetCurrentTasks->horizontalHeader()->resizeSection(0, 200);//Название АВП
    ui->tableWidgetCurrentTasks->horizontalHeader()->resizeSection(1, 300);//URL
    ui->tableWidgetCurrentTasks->horizontalHeader()->resizeSection(2, 80);//Возраст
    ui->tableWidgetCurrentTasks->horizontalHeader()->resizeSection(3, 150);//ФИО
    ui->tableWidgetCurrentTasks->horizontalHeader()->resizeSection(4, 110);//Дата назначения
    ui->tableWidgetCurrentTasks->horizontalHeader()->resizeSection(5, 110);//Дата выполнения
    ui->tableWidgetCurrentTasks->horizontalHeader()->resizeSection(6, 100);//Приоритет
    ui->tableWidgetCurrentTasks->horizontalHeader()->resizeSection(7, 150);//Статус
    ui->tableWidgetCurrentTasks->horizontalHeader()->resizeSection(8, 80);//%
    ui->tableWidgetCurrentTasks->horizontalHeader()->resizeSection(9, 250);//Нарушения
    ui->tableWidgetCurrentTasks->horizontalHeader()->setSectionResizeMode(10, QHeaderView::Stretch);//Комментарии
    ui->tableWidgetCurrentTasks->horizontalHeader()->resizeSection(11, 0);//ИД
    ui->tableWidgetCurrentTasks->horizontalHeader()->resizeSection(12, 0);//ИД_AVP

    ui->tableWidgetMyTasks->horizontalHeader()->resizeSection(0,250);//Название АВП
    ui->tableWidgetMyTasks->horizontalHeader()->resizeSection(1,300);//URL
    ui->tableWidgetMyTasks->horizontalHeader()->resizeSection(2, 80);//Возраст
    ui->tableWidgetMyTasks->horizontalHeader()->resizeSection(3,110);//Дата назначения
    ui->tableWidgetMyTasks->horizontalHeader()->resizeSection(4, 110);//Дата выполнения
    ui->tableWidgetMyTasks->horizontalHeader()->resizeSection(5,100);//Приоритет
    ui->tableWidgetMyTasks->horizontalHeader()->resizeSection(6,150);//Статус
    ui->tableWidgetMyTasks->horizontalHeader()->resizeSection(7,80);//%
    ui->tableWidgetMyTasks->horizontalHeader()->resizeSection(8,300);//Нарушения
    ui->tableWidgetMyTasks->horizontalHeader()->setSectionResizeMode(9,QHeaderView::Stretch);//Комментарии
    ui->tableWidgetMyTasks->horizontalHeader()->resizeSection(10, 0);//ID
    ui->tableWidgetMyTasks->horizontalHeader()->resizeSection(11, 0);//ID_AVP
    ui->tableWidgetMyTasks->horizontalHeader()->resizeSection(12, 70);//Рецензии
    ui->tableWidgetMyTasks->horizontalHeader()->resizeSection(13, 0);//Путь к файлу
    ui->tableWidgetMyTasks->horizontalHeader()->resizeSection(14, 0);//ID_AVS

    ui->tableWidgetAudit->horizontalHeader()->resizeSection(0,250);//Название АВП
    ui->tableWidgetAudit->horizontalHeader()->resizeSection(1,300);//URL
    ui->tableWidgetAudit->horizontalHeader()->resizeSection(2, 80);//Возраст
    ui->tableWidgetAudit->horizontalHeader()->resizeSection(3, 150);//ФИО
    ui->tableWidgetAudit->horizontalHeader()->resizeSection(4,110);//Дата назначения
    ui->tableWidgetAudit->horizontalHeader()->resizeSection(5,100);//Приоритет
    ui->tableWidgetAudit->horizontalHeader()->resizeSection(6,150);//Статус
    ui->tableWidgetAudit->horizontalHeader()->resizeSection(7,80);//%
    ui->tableWidgetAudit->horizontalHeader()->resizeSection(8,300);//
    ui->tableWidgetAudit->horizontalHeader()->setSectionResizeMode(9,QHeaderView::Stretch);//Комментарии
    ui->tableWidgetAudit->horizontalHeader()->resizeSection(10, 0);//ID
    ui->tableWidgetAudit->horizontalHeader()->resizeSection(11, 0);//ID_AVP

    ui->tableWidgetAudit_4->horizontalHeader()->resizeSection(0,250);//Название АВП
    ui->tableWidgetAudit_4->horizontalHeader()->resizeSection(1,200);//URL
    ui->tableWidgetAudit_4->horizontalHeader()->resizeSection(2, 80);//Возраст
    ui->tableWidgetAudit_4->horizontalHeader()->resizeSection(3, 150);//ФИО
    ui->tableWidgetAudit_4->horizontalHeader()->resizeSection(4, 150);//ФИО Эксперта
    ui->tableWidgetAudit_4->horizontalHeader()->resizeSection(5,110);//Дата назначения
    ui->tableWidgetAudit_4->horizontalHeader()->resizeSection(6,100);//Приоритет
    ui->tableWidgetAudit_4->horizontalHeader()->resizeSection(7,150);//Статус
    ui->tableWidgetAudit_4->horizontalHeader()->resizeSection(8,80);//%
    ui->tableWidgetAudit_4->horizontalHeader()->resizeSection(9,200);//
    ui->tableWidgetAudit_4->horizontalHeader()->setSectionResizeMode(10,QHeaderView::Stretch);//Комментарии
    ui->tableWidgetAudit_4->horizontalHeader()->resizeSection(11, 0);//ID
    ui->tableWidgetAudit_4->horizontalHeader()->resizeSection(12, 0);//ID_AVP

    ui->tableWidgetAudit_2->horizontalHeader()->resizeSection(0,250);//Название АВП
    ui->tableWidgetAudit_2->horizontalHeader()->resizeSection(1,200);//URL
    ui->tableWidgetAudit_2->horizontalHeader()->resizeSection(2, 80);//Возраст
    ui->tableWidgetAudit_2->horizontalHeader()->resizeSection(3, 150);//ФИО
    ui->tableWidgetAudit_2->horizontalHeader()->resizeSection(4, 150);//ФИО Эксперта
    ui->tableWidgetAudit_2->horizontalHeader()->resizeSection(5,110);//Дата назначения
    ui->tableWidgetAudit_2->horizontalHeader()->resizeSection(6,100);//Приоритет
    ui->tableWidgetAudit_2->horizontalHeader()->resizeSection(7,150);//Статус
    ui->tableWidgetAudit_2->horizontalHeader()->resizeSection(8,80);//%
    ui->tableWidgetAudit_2->horizontalHeader()->resizeSection(9,200);//
    ui->tableWidgetAudit_2->horizontalHeader()->setSectionResizeMode(10,QHeaderView::Stretch);//Комментарии
    ui->tableWidgetAudit_2->horizontalHeader()->resizeSection(11, 0);//ID
    ui->tableWidgetAudit_2->horizontalHeader()->resizeSection(12, 0);//ID_AVP

//    ui->tableWidgetAudit_2->horizontalHeader()->resizeSection(0,250);//Название АВП
//    ui->tableWidgetAudit_2->horizontalHeader()->resizeSection(1,300);//URL
//    ui->tableWidgetAudit_2->horizontalHeader()->resizeSection(2, 80);//Возраст
//    ui->tableWidgetAudit_2->horizontalHeader()->resizeSection(3, 150);//ФИО
//    ui->tableWidgetAudit_2->horizontalHeader()->resizeSection(4,110);//Дата назначения
//    ui->tableWidgetAudit_2->horizontalHeader()->resizeSection(5,100);//Приоритет
//    ui->tableWidgetAudit_2->horizontalHeader()->resizeSection(6,150);//Статус
//    ui->tableWidgetAudit_2->horizontalHeader()->resizeSection(7,80);//%
//    ui->tableWidgetAudit_2->horizontalHeader()->resizeSection(8,300);//Нарушения
//    ui->tableWidgetAudit_2->horizontalHeader()->setSectionResizeMode(9,QHeaderView::Stretch);//Комментарии
//    ui->tableWidgetAudit_2->horizontalHeader()->resizeSection(10, 0);//ID
//    ui->tableWidgetAudit_2->horizontalHeader()->resizeSection(11, 0);//ID_AVP

    ui->tableWidgetAuditArchive->horizontalHeader()->resizeSection(0,250);//Название АВП
    ui->tableWidgetAuditArchive->horizontalHeader()->resizeSection(1,300);//URL
    ui->tableWidgetAuditArchive->horizontalHeader()->resizeSection(2, 80);//Возраст
    ui->tableWidgetAuditArchive->horizontalHeader()->resizeSection(3, 150);//ФИО
    ui->tableWidgetAuditArchive->horizontalHeader()->resizeSection(4,150);//ФИО, эксперта
    ui->tableWidgetAuditArchive->horizontalHeader()->resizeSection(5,110);//Дата закрытия задачи
    ui->tableWidgetAuditArchive->horizontalHeader()->resizeSection(6,150);//Статус
    ui->tableWidgetAuditArchive->horizontalHeader()->resizeSection(7,300);//Нарушения
    ui->tableWidgetAuditArchive->horizontalHeader()->setSectionResizeMode(8,QHeaderView::Stretch);//Комментарии
    ui->tableWidgetAuditArchive->horizontalHeader()->resizeSection(9, 0);//ID
    ui->tableWidgetAuditArchive->horizontalHeader()->resizeSection(10, 0);//ID_AVP
}

//=========================================================
bool MainWindow::initDB()
{
//    std::cout<<"initDB()_START"<<std::endl;
    bool res = true;
    try
    {
        QSettings settings(QDir::toNativeSeparators(QApplication::applicationDirPath()) + "/settings.ini",QSettings::IniFormat);
        db = QSqlDatabase::addDatabase("QPSQL");
        QString server = settings.value("MAIN/SERVER").toString(); //        db.setHostName("192.168.28.96");
        if (server == "")
            server = "127.0.0.1";
        db.setHostName(server);

        QString nameDB = settings.value("MAIN/NAME_DB").toString();
        if (nameDB == "")
            nameDB = "avpDB";
        db.setDatabaseName(nameDB);

        QString user = settings.value("MAIN/USER").toString();
        if (user == "")
            user = "postgres";
        db.setUserName(user);

        QString password = settings.value("MAIN/PASSWD").toString();
        if (password == "")
            password = "postgres";
        db.setPassword(password);
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
//    std::cout<<"initDB()_STOP"<<std::endl;
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
void MainWindow::initComboBoxOperator(QComboBox *comboBox)
{
    QString sql="";
    comboBox->clear();
    try
    {
        sql = "SELECT * FROM \"User\" WHERE \"FlagJob\" = \'true\' AND (\"ID_Group\" = 5 OR \"ID_Group\" = 8 OR \"ID_Group\" = 9 OR \"ID_Group\" = 10);";
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
void MainWindow::initComboBoxExpert(QComboBox *comboBox)
{
    QString sql="";
    comboBox->clear();
    try
    {
        sql = "SELECT * FROM \"User\" WHERE \"FlagJob\" = \'true\' AND (\"ID_Group\" = 11 OR \"ID_Group\" = 12 OR \"ID_Group\" = 8);";
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
void MainWindow::initComboBoxUser(QComboBox *comboBox)
{
    QString sql="";
    comboBox->clear();
    try
    {
        sql = "SELECT * FROM \"User\" WHERE \"FlagJob\" = \'true\';";
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
void MainWindow::initComboBoxStatusArchive(QComboBox *comboBox)
{
    QString sql="";
    comboBox->clear();
    try
    {
        sql = "SELECT * FROM \"TaskStatus\" WHERE \"StatusType\"=1;";
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
void MainWindow::initTableArchive(int numberPage )
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QString sql="",tmp;

    try
    {
        ui->tableWidgetAuditArchive->setSortingEnabled(false);
        ui->tableWidgetAuditArchive->clearContents();
        ui->tableWidgetAuditArchive->setRowCount(0);

        sql = "SELECT avp.\"NameRus\", "
              "avp.\"URL\","
              "u.\"FIO\","
              "ue.\"FIO\","
              "ts.\"StatusName\","
              "\"Task\".\"Comment\","
              "\"Task\".\"ID\","
              "\"Task\".\"ID_AVP\", "
              "\"Task\".\"DateClose\", "
              "aa.\"Age\" FROM \"Task\" "
              "INNER JOIN avp ON \"Task\".\"ID_AVP\" = avp.\"ID\" "
              "INNER JOIN \"AVPattribute\" aa ON aa.\"ID_AVP\" = avp.\"ID\" "
              "INNER JOIN \"User\" u ON \"Task\".\"ID_User\" = u.\"ID\" "
              "LEFT JOIN \"User\" ue ON \"Task\".\"ID_Expert\" = ue.\"ID\" "
              "INNER JOIN \"TaskStatus\" ts ON \"Task\".\"ID_TaskStatus\" = ts.\"ID\" "
//              " WHERE (ts.\"StatusName\" =\'";
//        sql += "Закрыта\' OR ts.\"StatusName\" =\'Закрыта с экспертизой\')";// ORDER BY \"Task\".\"ID\";";
        " WHERE ts.\"StatusType\" =1";
        if(ui->groupBoxUser_3->isChecked())
        {
            sql +=" AND u.\"FIO\" = \'";sql += ui->comboBoxUser_3->currentText(); sql += "\'";
        }
        if(ui->groupBoxExpert->isChecked())
        {
            sql +=" AND ue.\"FIO\" = \'";sql += ui->comboBoxExpert->currentText(); sql += "\'";
        }
        if(ui->groupBoxStatusArchive->isChecked())
        {
            sql +=" AND ts.\"StatusName\" = \'";sql += ui->comboBoxStatusAchive->currentText(); sql += "\'";
        }
        if(ui->groupBoxDate_3->isChecked())
        {
            sql +=" AND (\"Task\".\"DateClose\" >='"; sql += ui->dateEditDateBegin_3->date().toString("yyyy-MM-dd");
            sql += "\'::date AND \"Task\".\"DateClose\" <=\'";sql += ui->dateEditDateEnd_3->date().toString("yyyy-MM-dd");  sql += "\'::date + \'1 day\'::interval)";
        }
         sql += " ORDER BY \"Task\".\"ID\" LIMIT 1000 OFFSET ";
        sql += tmp.setNum((numberPage-1)*1000);sql+=";";
//        qDebug()<<"sql = "<<sql;

        if(query->exec(sql))
        {
            int row = 0;
            while(query->next())
            {
                ui->tableWidgetAuditArchive->setRowCount(row+1);

                QTableWidgetItem *newItem = new QTableWidgetItem();
                QIcon icon;
                icon.addFile(QString::fromUtf8(":/icons/icons/film_projector_cinema.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem->setIcon(icon);
                newItem->setText(query->value(0).toString());
                newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAuditArchive->setItem(row,0, newItem);//Название АВП

                QTableWidgetItem *newItem1 = new QTableWidgetItem();
                QIcon icon1;
                icon1.addFile(QString::fromUtf8(":/icons/icons/web.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem1->setIcon(icon1);
                newItem1->setText(query->value(1).toString());
                newItem1->setFlags(newItem1->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAuditArchive->setItem(row,1, newItem1);//URL

                QTableWidgetItem *newItem41 = new QTableWidgetItem();//Возраст
                newItem41->setText(query->value(9).toString());
                newItem41->setFlags(newItem41->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAuditArchive->setItem(row,2, newItem41);

                QTableWidgetItem *newItem2 = new QTableWidgetItem();
                QIcon icon5;
                icon5.addFile(QString::fromUtf8(":/icons/icons/user.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem2->setIcon(icon5);
                newItem2->setText(query->value(2).toString());
                newItem2->setFlags(newItem2->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAuditArchive->setItem(row,3, newItem2);//ФИО

                QTableWidgetItem *newItem3 = new QTableWidgetItem();
                QIcon icon3;
                icon3.addFile(QString::fromUtf8(":/icons/icons/user.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem3->setIcon(icon3);
                newItem3->setText(query->value(3).toString());
                newItem3->setFlags(newItem2->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAuditArchive->setItem(row,4, newItem3);//ФИО, эксперта

                QTableWidgetItem *newItem31 = new QTableWidgetItem();
                QIcon icon31;
                icon31 .addFile(QString::fromUtf8(":/icons/icons/date2.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem31->setIcon(icon31);
                newItem31->setText(query->value(8).toDate().toString("yyyy-MM-dd"));
                newItem31->setFlags(newItem31->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAuditArchive->setItem(row,5, newItem31);//Дата закрытия задачи

                QTableWidgetItem *newItem4 = new QTableWidgetItem();
                newItem4->setText(query->value(4).toString());
                newItem4->setFlags(newItem4->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAuditArchive->setItem(row,6, newItem4);//Статус

                QTableWidgetItem *newItem5 = initViolations(query->value(7).toInt());
                ui->tableWidgetAuditArchive->setItem(row,7, newItem5);//Нарушения

                QTableWidgetItem *newItem6 = new QTableWidgetItem();
                newItem6->setText(query->value(5).toString());
                newItem6->setFlags(newItem6->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAuditArchive->setItem(row,8, newItem6);//Комментарии

                QTableWidgetItem *newItem9 = new QTableWidgetItem();
                newItem9->setText(query->value(6).toString());
                newItem9->setFlags(newItem9->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAuditArchive->setItem(row,9, newItem9);//ID

                QTableWidgetItem *newItem10 = new QTableWidgetItem();
                newItem10->setText(query->value(7).toString());
                newItem10->setFlags(newItem10->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAuditArchive->setItem(row,10, newItem10);//ID_AVP

                row++;
            }
        }
        else
            qDebug()<<query->lastError().text();
        ui->tableWidgetAuditArchive->setSortingEnabled(true);
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
    QApplication::restoreOverrideCursor();
}

//=========================================================
void MainWindow::initTableAuditRKN()
{
    QString sql="";
    try
    {
        ui->tableWidgetAudit_2->setSortingEnabled(false);
        ui->tableWidgetAudit_2->clearContents();
        ui->tableWidgetAudit_2->setRowCount(0);

        sql = "SELECT avp.\"NameRus\","
              "avp.\"URL\","
              "u.\"FIO\","
              "ue.\"FIO\","
              "\"Task\".\"DateAppoint\","
              "p.\"NamePriority\","
              "ts.\"StatusName\","
              "\"Task\".\"PercentCompleted\","
              "\"Task\".\"Comment\","
              "\"Task\".\"ID\","
              "\"Task\".\"ID_AVP\","
              "aa.\"Age\" FROM \"Task\" "
              "INNER JOIN avp ON \"Task\".\"ID_AVP\" = avp.\"ID\" "
              "INNER JOIN \"AVPattribute\" aa ON aa.\"ID_AVP\" = avp.\"ID\" "
              "INNER JOIN \"User\" u ON \"Task\".\"ID_User\" = u.\"ID\" "
              "LEFT JOIN \"User\" ue ON \"Task\".\"ID_Expert\" = ue.\"ID\" "
              "INNER JOIN \"TaskStatus\" ts ON \"Task\".\"ID_TaskStatus\" = ts.\"ID\" "
              "INNER JOIN \"Priority\" p ON \"Task\".\"ID_Priority\" = p.\"ID\" WHERE ts.\"StatusName\" =\'";
        sql += "Экпертиза\'";
        if(ui->groupBoxUser_4->isChecked())
        {
            sql +=" AND ue.\"FIO\" = \'";sql += ui->comboBoxUser_2->currentText(); sql += "\'";
        }
        if(ui->groupBoxPriority_3->isChecked())
        {
            sql +=" AND p.\"NamePriority\" = \'";sql += ui->comboBoxPriority_2->currentText(); sql += "\'";
        }
        if(ui->groupBoxDate_4->isChecked())
        {
            sql +=" AND (\"Task\".\"DateAppoint\" >='"; sql += ui->dateEditDateBegin_4->date().toString("yyyy-MM-dd");
            sql += "\'::date AND \"Task\".\"DateAppoint\" <=\'";sql += ui->dateEditDateEnd_4->date().toString("yyyy-MM-dd");  sql += "\'::date + \'1 day\'::interval)";
        }
        sql += " ORDER BY \"Task\".\"ID\";";
//        qDebug()<<"sql = "<<sql;
        if(query->exec(sql))
        {
            int row = 0;
            while(query->next())
            {
                ui->tableWidgetAudit_2->setRowCount(row+1);

                QTableWidgetItem *newItem = new QTableWidgetItem();
                QIcon icon;
                icon.addFile(QString::fromUtf8(":/icons/icons/film_projector_cinema.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem->setIcon(icon);
                newItem->setText(query->value(0).toString());
                newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit_2->setItem(row,0, newItem);//Название АВП

                QTableWidgetItem *newItem1 = new QTableWidgetItem();
                QIcon icon1;
                icon1.addFile(QString::fromUtf8(":/icons/icons/web.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem1->setIcon(icon1);
                newItem1->setText(query->value(1).toString());
                newItem1->setFlags(newItem1->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit_2->setItem(row,1, newItem1);//URL

                QTableWidgetItem *newItem41 = new QTableWidgetItem();//Возраст
                newItem41->setText(query->value(11).toString());
                newItem41->setFlags(newItem41->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit_2->setItem(row,2, newItem41);

                QTableWidgetItem *newItem2 = new QTableWidgetItem();
                QIcon icon5;
                icon5.addFile(QString::fromUtf8(":/icons/icons/user.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem2->setIcon(icon5);
                newItem2->setText(query->value(2).toString());
                newItem2->setFlags(newItem2->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit_2->setItem(row,3, newItem2);//ФИО

                QTableWidgetItem *newItem21 = new QTableWidgetItem();
                QIcon icon51;
                icon51.addFile(QString::fromUtf8(":/icons/icons/user.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem21->setIcon(icon51);
                if(query->value(3).toString() == "")
                    newItem21->setText("Не назначен");
                else
                    newItem21->setText(query->value(3).toString());
                newItem21->setFlags(newItem2->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit_2->setItem(row,4, newItem21);//ФИО Эксперта

                QTableWidgetItem *newItem3 = new QTableWidgetItem();
                QIcon icon3;
                icon3.addFile(QString::fromUtf8(":/icons/icons/date2.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem3->setIcon(icon3);
                newItem3->setText(query->value(4).toDate().toString("yyyy-MM-dd"));
                newItem3->setFlags(newItem3->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit_2->setItem(row,5, newItem3);//Дата назначения задачи

                QTableWidgetItem *newItem4 = new QTableWidgetItem();
                newItem4->setText(query->value(5).toString());
                newItem4->setFlags(newItem4->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit_2->setItem(row,6, newItem4);//Приоритет

                QTableWidgetItem *newItem5 = new QTableWidgetItem();
                newItem5->setText(query->value(6).toString());
                newItem5->setFlags(newItem5->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit_2->setItem(row,7, newItem5);//Статус

                QTableWidgetItem *newItem6 = new QTableWidgetItem();
                newItem6->setText(query->value(7).toString()+"%");
                newItem6->setFlags(newItem6->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit_2->setItem(row,8, newItem6);//Процент завершенности

                QTableWidgetItem *newItem7 = initViolations(query->value(10).toInt());
                ui->tableWidgetAudit_2->setItem(row,9, newItem7);//Нарушения

                QTableWidgetItem *newItem8 = new QTableWidgetItem();
                newItem8->setText(query->value(8).toString());
                newItem8->setFlags(newItem8->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit_2->setItem(row,10, newItem8);

                QTableWidgetItem *newItem9 = new QTableWidgetItem();
                newItem9->setText(query->value(9).toString());
                newItem9->setFlags(newItem9->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit_2->setItem(row,11, newItem9);

                QTableWidgetItem *newItem10 = new QTableWidgetItem();
                newItem10->setText(query->value(10).toString());
                newItem10->setFlags(newItem10->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit_2->setItem(row,12, newItem10);

                row++;
            }
        }
        else
            qDebug()<<query->lastError().text();
        ui->tableWidgetAudit_2->setSortingEnabled(true);
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
void MainWindow::initTableCurrentAudit()
{
    QString sql="";
//    bool filterCheck = false;
    try
    {
        ui->tableWidgetAudit_4->setSortingEnabled(false);
        ui->tableWidgetAudit_4->clearContents();
        ui->tableWidgetAudit_4->setRowCount(0);

        sql = "SELECT avp.\"NameRus\","
              "avp.\"URL\","
              "u.\"FIO\","
              "ue.\"FIO\","
              "\"Task\".\"DateAppoint\","
              "p.\"NamePriority\","
              "ts.\"StatusName\","
              "\"Task\".\"PercentCompleted\","
              "\"Task\".\"Comment\","
              "\"Task\".\"ID\","
              "\"Task\".\"ID_AVP\","
              "aa.\"Age\" FROM \"Task\" "
              "INNER JOIN avp ON \"Task\".\"ID_AVP\" = avp.\"ID\" "
              "INNER JOIN \"AVPattribute\" aa ON aa.\"ID_AVP\" = avp.\"ID\" "
              "INNER JOIN \"User\" u ON \"Task\".\"ID_User\" = u.\"ID\" "
              "LEFT JOIN \"User\" ue ON \"Task\".\"ID_Expert\" = ue.\"ID\" "
              "INNER JOIN \"TaskStatus\" ts ON \"Task\".\"ID_TaskStatus\" = ts.\"ID\" "
              "INNER JOIN \"Priority\" p ON \"Task\".\"ID_Priority\" = p.\"ID\" WHERE ts.\"StatusName\" =\'";
        sql += "Экпертиза\'";
        if(ui->groupBoxUser_2->isChecked())
        {
            sql +=" AND ue.\"FIO\" = \'";sql += ui->comboBoxUser_2->currentText(); sql += "\'";
//            filterCheck = true;
        }
        if(ui->groupBoxPriority_2->isChecked())
        {
            sql +=" AND p.\"NamePriority\" = \'";sql += ui->comboBoxPriority_2->currentText(); sql += "\'";
//            filterCheck = true;
        }
        if(ui->groupBoxDate_2->isChecked())
        {
            sql +=" AND (\"Task\".\"DateAppoint\" >='"; sql += ui->dateEditDateBegin_2->date().toString("yyyy-MM-dd");
            sql += "\'::date AND \"Task\".\"DateAppoint\" <=\'";sql += ui->dateEditDateEnd_2->date().toString("yyyy-MM-dd");  sql += "\'::date + \'1 day\'::interval)";
//            filterCheck = true;
        }
        sql += " ORDER BY \"Task\".\"ID\";";
//        qDebug()<<"sql = "<<sql;
//        filterCheck = false;
        if(query->exec(sql))
        {
            int row = 0;
            while(query->next())
            {
                ui->tableWidgetAudit_4->setRowCount(row+1);

                QTableWidgetItem *newItem = new QTableWidgetItem();
                QIcon icon;
                icon.addFile(QString::fromUtf8(":/icons/icons/film_projector_cinema.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem->setIcon(icon);
                newItem->setText(query->value(0).toString());
                newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit_4->setItem(row,0, newItem);//Название АВП

                QTableWidgetItem *newItem1 = new QTableWidgetItem();
                QIcon icon1;
                icon1.addFile(QString::fromUtf8(":/icons/icons/web.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem1->setIcon(icon1);
                newItem1->setText(query->value(1).toString());
                newItem1->setFlags(newItem1->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit_4->setItem(row,1, newItem1);//URL

                QTableWidgetItem *newItem41 = new QTableWidgetItem();//Возраст
                newItem41->setText(query->value(11).toString());
                newItem41->setFlags(newItem41->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit_4->setItem(row,2, newItem41);

                QTableWidgetItem *newItem2 = new QTableWidgetItem();
                QIcon icon5;
                icon5.addFile(QString::fromUtf8(":/icons/icons/user.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem2->setIcon(icon5);
                newItem2->setText(query->value(2).toString());
                newItem2->setFlags(newItem2->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit_4->setItem(row,3, newItem2);//ФИО

                QTableWidgetItem *newItem21 = new QTableWidgetItem();
                QIcon icon51;
                icon51.addFile(QString::fromUtf8(":/icons/icons/user.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem21->setIcon(icon51);
                if(query->value(3).toString() == "")
                    newItem21->setText("Не назначен");
                else
                    newItem21->setText(query->value(3).toString());
                newItem21->setFlags(newItem2->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit_4->setItem(row,4, newItem21);//ФИО Эксперта

                QTableWidgetItem *newItem3 = new QTableWidgetItem();
                QIcon icon3;
                icon3.addFile(QString::fromUtf8(":/icons/icons/date2.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem3->setIcon(icon3);
                newItem3->setText(query->value(4).toDate().toString("yyyy-MM-dd"));
                newItem3->setFlags(newItem3->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit_4->setItem(row,5, newItem3);//Дата назначения задачи

                QTableWidgetItem *newItem4 = new QTableWidgetItem();
                newItem4->setText(query->value(5).toString());
                newItem4->setFlags(newItem4->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit_4->setItem(row,6, newItem4);//Приоритет

                QTableWidgetItem *newItem5 = new QTableWidgetItem();
                newItem5->setText(query->value(6).toString());
                newItem5->setFlags(newItem5->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit_4->setItem(row,7, newItem5);//Статус

                QTableWidgetItem *newItem6 = new QTableWidgetItem();
                newItem6->setText(query->value(7).toString()+"%");
                newItem6->setFlags(newItem6->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit_4->setItem(row,8, newItem6);//Процент завершенности


                QTableWidgetItem *newItem7 = initViolations(query->value(10).toInt());
                ui->tableWidgetAudit_4->setItem(row,9, newItem7);//Нарушения

                QTableWidgetItem *newItem8 = new QTableWidgetItem();
                newItem8->setText(query->value(8).toString());
                newItem8->setFlags(newItem8->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit_4->setItem(row,10, newItem8);

                QTableWidgetItem *newItem9 = new QTableWidgetItem();
                newItem9->setText(query->value(9).toString());
                newItem9->setFlags(newItem9->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit_4->setItem(row,11, newItem9);

                QTableWidgetItem *newItem10 = new QTableWidgetItem();
                newItem10->setText(query->value(10).toString());
                newItem10->setFlags(newItem10->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit_4->setItem(row,12, newItem10);

                row++;
            }
        }
        else
            qDebug()<<query->lastError().text();
        ui->tableWidgetAudit_4->setSortingEnabled(true);
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
              "\"Task\".\"ID_AVP\", "
              "aa.\"Age\" FROM \"Task\" "
              "INNER JOIN avp ON \"Task\".\"ID_AVP\" = avp.\"ID\" "
              "INNER JOIN \"AVPattribute\" aa ON aa.\"ID_AVP\" = avp.\"ID\" "
              "INNER JOIN \"User\" u ON \"Task\".\"ID_Expert\" = u.\"ID\" "
              "INNER JOIN \"TaskStatus\" ts ON \"Task\".\"ID_TaskStatus\" = ts.\"ID\" "
              "INNER JOIN \"Priority\" p ON \"Task\".\"ID_Priority\" = p.\"ID\" WHERE ts.\"StatusName\" =\'";
        sql += "Экпертиза\' AND u.\"FIO\" =\'";
        sql += currentUserName;
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

                QTableWidgetItem *newItem41 = new QTableWidgetItem();//Возраст
                newItem41->setText(query->value(10).toString());
                newItem41->setFlags(newItem41->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit->setItem(row,2, newItem41);

                QTableWidgetItem *newItem2 = new QTableWidgetItem();
                QIcon icon5;
                icon5.addFile(QString::fromUtf8(":/icons/icons/user.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem2->setIcon(icon5);
                newItem2->setText(query->value(2).toString());
                newItem2->setFlags(newItem2->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit->setItem(row,3, newItem2);//ФИО

                QTableWidgetItem *newItem3 = new QTableWidgetItem();
                QIcon icon3;
                icon3.addFile(QString::fromUtf8(":/icons/icons/date2.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem3->setIcon(icon3);
                newItem3->setText(query->value(3).toDate().toString("yyyy-MM-dd"));
                newItem3->setFlags(newItem3->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit->setItem(row,4, newItem3);//Дата назначения задачи

                QTableWidgetItem *newItem4 = new QTableWidgetItem();
                newItem4->setText(query->value(4).toString());
                newItem4->setFlags(newItem4->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit->setItem(row,5, newItem4);//Приоритет

                QTableWidgetItem *newItem5 = new QTableWidgetItem();
                newItem5->setText(query->value(5).toString());
                newItem5->setFlags(newItem5->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit->setItem(row,6, newItem5);//Статус

                QTableWidgetItem *newItem6 = new QTableWidgetItem();
                newItem6->setText(query->value(6).toString()+"%");
                newItem6->setFlags(newItem6->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit->setItem(row,7, newItem6);//Процент завершенности


                QTableWidgetItem *newItem7 = initViolations(query->value(9).toInt());
                ui->tableWidgetAudit->setItem(row,8, newItem7);//Нарушения

                QTableWidgetItem *newItem8 = new QTableWidgetItem();
                newItem8->setText(query->value(7).toString());
                newItem8->setFlags(newItem8->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit->setItem(row,9, newItem8);

                QTableWidgetItem *newItem9 = new QTableWidgetItem();
                newItem9->setText(query->value(8).toString());
                newItem9->setFlags(newItem9->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit->setItem(row,10, newItem9);

                QTableWidgetItem *newItem10 = new QTableWidgetItem();
                newItem10->setText(query->value(9).toString());
                newItem10->setFlags(newItem10->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAudit->setItem(row,11, newItem10);

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
        if( (ui->tableWidgetMyTasks->item(row,11)->text().toLongLong() == static_cast<qlonglong>(idAVP)) && (status == 0x02) )
        {
            ui->tableWidgetMyTasks->item(row,8)->setText("Нет рецензий");
            ui->tableWidgetMyTasks->item(row,9)->setText("Нет рецензий для АВП");
            addComment(ui->tableWidgetMyTasks->item(row,10)->text().toLongLong(),"Нет рецензий для АВП");
        }
        else if( (ui->tableWidgetMyTasks->item(row,11)->text().toLongLong() == static_cast<qlonglong>(idAVP)) && (status == 0x01) )
        {
            ui->tableWidgetMyTasks->setItem(row,8, initViolations(idAVP));
            ui->tableWidgetMyTasks->item(row,9)->setText("АВП проанализировано");
            addComment(ui->tableWidgetMyTasks->item(row,10)->text().toLongLong(),"АВП проанализировано");
        }
        else if( (ui->tableWidgetMyTasks->item(row,11)->text().toLongLong() == static_cast<qlonglong>(idAVP)) && (status == 0x03) )
        {
            ui->tableWidgetMyTasks->item(row,8)->setText("Не удалось скачать рецензии для АВП!");
            ui->tableWidgetMyTasks->item(row,9)->setText("Не удалось скачать рецензии для АВП!");
            addComment(ui->tableWidgetMyTasks->item(row,10)->text().toLongLong(),"Не удалось скачать рецензии для АВП!");
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
              "avp.\"ID_AVS\", "
              "\"Task\".\"DateRealization\", "
              "aa.\"Age\" FROM \"Task\" "
              "INNER JOIN avp ON \"Task\".\"ID_AVP\" = avp.\"ID\" "
              "INNER JOIN \"AVPattribute\" aa ON aa.\"ID_AVP\" = avp.\"ID\" "
              "INNER JOIN \"User\" u ON \"Task\".\"ID_User\" = u.\"ID\" "
              "INNER JOIN \"TaskStatus\" ts ON \"Task\".\"ID_TaskStatus\" = ts.\"ID\" "
              "LEFT JOIN \"DownloadData\" dd ON dd.\"ID_AVP\" = avp.\"ID\" AND dd.\"ResourceName\" = \'Kinopoisk\' "
              "INNER JOIN \"Priority\" p ON \"Task\".\"ID_Priority\" = p.\"ID\" WHERE u.\"FIO\" =\'";
        sql += currentUserName;
        sql += "\' AND (ts.\"StatusName\" =\'Новая\' OR ts.\"StatusName\" =\'В работе\' OR ts.\"StatusName\" =\'Проверена оператором\' OR ts.\"StatusName\" =\'Отложенная\') ORDER BY \"Task\".\"ID\";";
        qDebug()<<"sql = "<<sql;
        if(query->exec(sql))
        {
            int row = 0;
            while(query->next())
            {
                qDebug()<<"kkkkk";
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

                QTableWidgetItem *newItem41 = new QTableWidgetItem();//Возраст
                newItem41->setText(query->value(13).toString());
                newItem41->setFlags(newItem41->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetMyTasks->setItem(row,2, newItem41);

                QTableWidgetItem *newItem3 = new QTableWidgetItem();
                QIcon icon3;
                icon3.addFile(QString::fromUtf8(":/icons/icons/date2.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem3->setIcon(icon3);
                newItem3->setText(query->value(2).toDate().toString("yyyy-MM-dd"));
                newItem3->setFlags(newItem3->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetMyTasks->setItem(row,3, newItem3);//Дата назначения задачи

                QTableWidgetItem *newItem31 = new QTableWidgetItem();
                newItem31->setIcon(icon3);
                newItem31->setText(query->value(12).toDate().toString("yyyy-MM-dd"));
                newItem31->setFlags(newItem31->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetMyTasks->setItem(row,4, newItem31);//Дата выполнения задачи

                QTableWidgetItem *newItem4 = new QTableWidgetItem();
                newItem4->setText(query->value(3).toString());
                newItem4->setFlags(newItem4->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetMyTasks->setItem(row,5, newItem4);//Приоритет

                QTableWidgetItem *newItem5 = new QTableWidgetItem();
                newItem5->setText(query->value(4).toString());
                newItem5->setFlags(newItem5->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetMyTasks->setItem(row,6, newItem5);//Статус

                QTableWidgetItem *newItem6 = new QTableWidgetItem();
                newItem6->setText(query->value(5).toString()+"%");
                newItem6->setFlags(newItem6->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetMyTasks->setItem(row,7, newItem6);//Процент завершенности


                QTableWidgetItem *newItem7 = initViolations(query->value(8).toInt());
                ui->tableWidgetMyTasks->setItem(row,8, newItem7);//Нарушения

                QTableWidgetItem *newItem8 = new QTableWidgetItem();
                newItem8->setText(query->value(6).toString());
                newItem8->setFlags(newItem8->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetMyTasks->setItem(row,9, newItem8);//Комментарии

                QTableWidgetItem *newItem9 = new QTableWidgetItem();
                newItem9->setText(query->value(7).toString());
                newItem9->setFlags(newItem9->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetMyTasks->setItem(row,10, newItem9);//ID

                QTableWidgetItem *newItem10 = new QTableWidgetItem();
                newItem10->setText(query->value(8).toString());
                newItem10->setFlags(newItem10->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetMyTasks->setItem(row,11, newItem10);//ID_AVP

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
                ui->tableWidgetMyTasks->setItem(row,12, newItem11);//Рецензии
//                qDebug()<<"DownloadStatus = "<<query->value(9).toString();

                QTableWidgetItem *newItem12 = new QTableWidgetItem();
                newItem12->setText(query->value(10).toString());
                newItem12->setFlags(newItem12->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetMyTasks->setItem(row,13, newItem12);//Путь к файлу пецензий
//                qDebug()<<"Path = "<<query->value(10).toString();

                QTableWidgetItem *newItem13 = new QTableWidgetItem();
                newItem13->setText(query->value(11).toString());
                newItem13->setFlags(newItem13->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetMyTasks->setItem(row,14, newItem13);//ID_AVS

                QDate currentDT = QDateTime::currentDateTime().date();
                QDate realizationDT = query->value(12).toDate();
                if((currentDT > realizationDT) && (query->value(4).toString()!="Закрыта"))
                    setColorRowTable(ui->tableWidgetMyTasks,row,0xff,0xc0,0xcb);
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
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QString sql="";

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
              "\"Task\".\"ID_AVP\", "
              "\"Task\".\"DateRealization\", "
              "aa.\"Age\" FROM \"Task\" "
              "INNER JOIN avp ON \"Task\".\"ID_AVP\" = avp.\"ID\" "
              "INNER JOIN \"AVPattribute\" aa ON aa.\"ID_AVP\" = avp.\"ID\" "
              "INNER JOIN \"User\" u ON \"Task\".\"ID_User\" = u.\"ID\" "
              "INNER JOIN \"TaskStatus\" ts ON \"Task\".\"ID_TaskStatus\" = ts.\"ID\" "
              "INNER JOIN \"Priority\" p ON \"Task\".\"ID_Priority\" = p.\"ID\" WHERE ts.\"StatusName\" !=\'";
        sql += "Закрыта\' AND ts.\"StatusName\" !=\'Закрыта с экспертизой\'";
            if(ui->groupBoxUser->isChecked())
            {
                sql +=" AND u.\"FIO\" = \'";sql += ui->comboBoxUser->currentText(); sql += "\'";
            }
            if(ui->groupBoxStatus->isChecked())
            {
                sql +=" AND ts.\"StatusName\" = \'";sql += ui->comboBoxStatus->currentText(); sql += "\'";
            }
            if(ui->groupBoxPriority->isChecked())
            {
                 sql +=" AND p.\"NamePriority\" = \'";sql += ui->comboBoxPriority->currentText(); sql += "\'";
            }
            if(ui->groupBoxDate->isChecked())
            {
                 sql +=" AND (\"Task\".\"DateAppoint\" >='"; sql += ui->dateEditDateBegin->date().toString("yyyy-MM-dd");
                 sql += "\'::date AND \"Task\".\"DateAppoint\" <=\'";sql += ui->dateEditDateEnd->date().toString("yyyy-MM-dd");  sql += "\'::date + \'1 day\'::interval)";
            }
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
//                newItem->setBackgroundColor(QColor(0xff,0xc0,0xcb));
                ui->tableWidgetCurrentTasks->setItem(row,0, newItem);//Название АВП

                QTableWidgetItem *newItem1 = new QTableWidgetItem();
                QIcon icon1;
                icon1.addFile(QString::fromUtf8(":/icons/icons/web.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem1->setIcon(icon1);
                newItem1->setText(query->value(1).toString());
                newItem1->setFlags(newItem1->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetCurrentTasks->setItem(row,1, newItem1);//URL

                QTableWidgetItem *newItem41 = new QTableWidgetItem();//Возраст
                newItem41->setText(query->value(12).toString());
                newItem41->setFlags(newItem41->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetCurrentTasks->setItem(row,2, newItem41);

                QTableWidgetItem *newItem2 = new QTableWidgetItem();
                QIcon icon5;
                icon5.addFile(QString::fromUtf8(":/icons/icons/user.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem2->setIcon(icon5);
                newItem2->setText(query->value(2).toString());
                newItem2->setFlags(newItem2->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetCurrentTasks->setItem(row,3, newItem2);//ФИО

                QTableWidgetItem *newItem3 = new QTableWidgetItem();
                QIcon icon3;
                icon3.addFile(QString::fromUtf8(":/icons/icons/date2.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem3->setIcon(icon3);
                newItem3->setText(query->value(3).toDate().toString("yyyy-MM-dd"));
                newItem3->setFlags(newItem3->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetCurrentTasks->setItem(row,4, newItem3);//Дата назначения задачи

                QTableWidgetItem *newItem31 = new QTableWidgetItem();
                newItem31->setIcon(icon3);
                newItem31->setText(query->value(11).toDate().toString("yyyy-MM-dd"));
                newItem31->setFlags(newItem31->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetCurrentTasks->setItem(row,5, newItem31);//Дата выполнения задачи

                QTableWidgetItem *newItem4 = new QTableWidgetItem();
                newItem4->setText(query->value(4).toString());
                newItem4->setFlags(newItem4->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetCurrentTasks->setItem(row,6, newItem4);//Приоритет

                QTableWidgetItem *newItem5 = new QTableWidgetItem();
                newItem5->setText(query->value(5).toString());
                newItem5->setFlags(newItem5->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetCurrentTasks->setItem(row,7, newItem5);//Статус

                QTableWidgetItem *newItem6 = new QTableWidgetItem();
                newItem6->setText(query->value(6).toString()+"%");
                newItem6->setFlags(newItem6->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetCurrentTasks->setItem(row,8, newItem6);//Процент завершенности

                QTableWidgetItem *newItem7 = initViolations(query->value(10).toInt());
                ui->tableWidgetCurrentTasks->setItem(row,9, newItem7);//Нарушения

                QTableWidgetItem *newItem8 = new QTableWidgetItem();
                newItem8->setText(query->value(8).toString());
                newItem8->setFlags(newItem8->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetCurrentTasks->setItem(row,10, newItem8);//Комментарии

                QTableWidgetItem *newItem9 = new QTableWidgetItem();
                newItem9->setText(query->value(9).toString());
                newItem9->setFlags(newItem9->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetCurrentTasks->setItem(row,11, newItem9);//ID

                QTableWidgetItem *newItem10 = new QTableWidgetItem();
                newItem10->setText(query->value(10).toString());
                newItem10->setFlags(newItem10->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetCurrentTasks->setItem(row,12, newItem10);//ID_AVP

                QDate currentDT = QDateTime::currentDateTime().date();
                QDate realizationDT = query->value(11).toDate();

                if( (currentDT > realizationDT) && (query->value(5).toString()!="Закрыта") &&
                        (query->value(5).toString()!="Закрыта с экспертизой") &&
                        (query->value(5).toString()!="Проверена оператором") &&
                        (query->value(5).toString()!="Одобрена экспертом") &&
                        (query->value(5).toString()!="Экпертиза"))
                    setColorRowTable(ui->tableWidgetCurrentTasks,row,0xff,0xc0,0xcb);

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
    QApplication::restoreOverrideCursor();
}

//=========================================================
void MainWindow::setColorRowTable(QTableWidget *table, int row, int r, int g, int b)
{
    for( int col = 0; col < table->columnCount(); ++col )
    {
        table->item(row, col)->setBackground(QColor(r,g,b));
    }
}

//=========================================================
int MainWindow::countArchiveAVP()
{
    int count = 0;
    QString sql;
    try
    {
        sql = "SELECT COUNT(*) FROM \"Task\" WHERE \"ID_TaskStatus\" = 4 OR \"ID_TaskStatus\" = 8;";
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
QString MainWindow::getNameRusAVP(uint64_t idAVP)
{
    QString nameRus;
    QString sql,tmp;
    try
    {
        sql ="SELECT \"NameRus\" FROM avp WHERE \"ID\"="+tmp.setNum(idAVP)+";";
        if(query->exec(sql))
        {
            if(query->next())
                nameRus = query->value(0).toString();
        }
        else
            qDebug()<<query->lastError().text();
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
    return nameRus;
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
int MainWindow::countAVP_Analysis(int state,long idAVS)
{
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

        sql+=" GROUP BY \"ID\";";
        qDebug()<<"sql="<<sql;
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
    else if(m_currentState == 1)
        m_countAVP = countAVP_Analysis(1,m_currentIdAVS);
    else if(m_currentState == 2)
        m_countAVP = countAVP_Analysis(2,m_currentIdAVS);

//    else
//        m_countAVP = m_countCurrentAVP;

    QString tmp;
    QString text="из ";text+=tmp.setNum(m_countAVP/1000); text+=" (всего АВП ";text+=tmp.setNum(m_countAVP);text+=")";
    ui->lineEditNumberPage->setValidator( new QIntValidator(1,(m_countAVP/1000), this) );
    ui->lineEditNumberPage->setText(tmp.setNum(m_currentNumberPage));
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
void MainWindow::slotNextArchivePage()
{
    qDebug()<<__PRETTY_FUNCTION__;
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QString tmp;
    int page_count = (m_countArchiveAVP/1000);
    qDebug()<<"page_count = "<<page_count;
    if( m_countArchiveAVP%1000 > 0)
        page_count++;

    if(m_currentArchiveNumberPage <= page_count)
    {
        ui->pushButtonPreview_2->setEnabled(true);
        m_currentArchiveNumberPage++;
        initTableArchive(m_currentArchiveNumberPage);
        ui->lineEditNumberPage_2->setText(tmp.setNum(m_currentArchiveNumberPage));
        if(m_currentArchiveNumberPage == page_count)
            ui->pushButtonNext_2->setEnabled(false);
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
        ui->lineEditNumberPage->setText(tmp.setNum(m_currentNumberPage));
        if(m_currentNumberPage == 1)
           ui->pushButtonPreview->setEnabled(false);
    }
    QApplication::restoreOverrideCursor();
}

//=========================================================
void MainWindow::slotPreviousArchivePage()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QString tmp;
    if(m_currentArchiveNumberPage>1)
    {
        ui->pushButtonNext_2->setEnabled(true);
        m_currentArchiveNumberPage--;
        initTableArchive(m_currentArchiveNumberPage);
        ui->lineEditNumberPage_2->setText(tmp.setNum(m_currentArchiveNumberPage));
        if(m_currentArchiveNumberPage == 1)
           ui->pushButtonPreview_2->setEnabled(false);
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
//        QApplication::restoreOverrideCursor();
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
void MainWindow::slotChangeNumberPageArchive()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    qDebug()<<ui->lineEditNumberPage_2->text();
    m_currentArchiveNumberPage = ui->lineEditNumberPage_2->text().toInt();
    int page_count = (m_countArchiveAVP/1000);
    if( m_countArchiveAVP%1000 > 0)
        page_count++;

    if(ui->lineEditNumberPage_2->text().toInt() > page_count)
    {
        QApplication::restoreOverrideCursor();
        QMessageBox::warning(this,"Внимание","Не верно введен номер страницы!","Да");
        return;
    }

    if(m_currentArchiveNumberPage<=page_count)
    {
        if( (m_currentArchiveNumberPage > 1) && (m_currentArchiveNumberPage < page_count) )
        {
            ui->pushButtonPreview_2->setEnabled(true);
            ui->pushButtonNext_2->setEnabled(true);
        }
        else if(m_currentArchiveNumberPage == 1)
        {
            ui->pushButtonPreview_2->setEnabled(false);
            ui->pushButtonNext_2->setEnabled(true);
        }
        else if(m_currentArchiveNumberPage == page_count)
        {
            ui->pushButtonNext_2->setEnabled(false);
            ui->pushButtonPreview_2->setEnabled(true);
        }

        initTableArchive(m_currentNumberPage);
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
//        sql = "SELECT v.\"Violation\",ar.\"Percent\",ar.\"TextViolation\",ar.\"CheckAuto\" FROM \"AnalysisResult\" ar "
//              "INNER JOIN \"Violation\" v ON ar.\"ID_Violation\"=v.\"ID\" WHERE \"ID_AVP\"="+tmp.setNum(id_avp)+" AND ar.\"CheckAuto\"=\'true\';";

        sql = "SELECT v.\"Violation\",SUM(ar.\"Percent\"),ar.\"CheckAuto\" FROM \"AnalysisResult\" ar "
              "INNER JOIN \"Violation\" v ON ar.\"ID_Violation\"=v.\"ID\" WHERE \"ID_AVP\"="+tmp.setNum(id_avp)+" GROUP BY(v.\"Violation\",ar.\"CheckAuto\");";
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
                icon.addFile(QString::fromUtf8(":/icons/icons/alarm.png"), QSize(), QIcon::Normal, QIcon::Off);
                str="";
                while(queryViolation->next())
                {
                    if(queryViolation->value(0).toString() == "Не обнаружено")
                        icon.addFile(QString::fromUtf8(":/icons/icons/ok.png"), QSize(), QIcon::Normal, QIcon::Off);
                    else
                        icon.addFile(QString::fromUtf8(":/icons/icons/alarm.png"), QSize(), QIcon::Normal, QIcon::Off);
                    str += queryViolation->value(0).toString();
                    if(queryViolation->value(2).toBool())
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
                    if(queryViolation->value(1).toInt()>100)
                    {
                        str += tmp.setNum(100);
                    }
                    else
                    {
                        str += tmp.setNum(queryViolation->value(1).toInt());
                    }
                    str +="% ";
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
              "avp.\"ID\","
              "aa.\"Age\" FROM avp "
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

                QTableWidgetItem *newItem4 = new QTableWidgetItem();//Режиссер
                QIcon icon4;
                icon4.addFile(QString::fromUtf8(":/icons/icons/film_maker1.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem4->setIcon(icon4);
                newItem4->setText(query->value(4).toString());
                newItem4->setFlags(newItem4->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAVP->setItem(row,4, newItem4);

                QTableWidgetItem *newItem41 = new QTableWidgetItem();//Возраст
                newItem41->setText(query->value(10).toString());
                newItem41->setFlags(newItem41->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAVP->setItem(row,5, newItem41);

                QTableWidgetItem *newItem5 = new QTableWidgetItem();
                QIcon icon5;
                icon5.addFile(QString::fromUtf8(":/icons/icons/user.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem5->setIcon(icon5);
                newItem5->setText(query->value(5).toString());
                newItem5->setFlags(newItem5->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAVP->setItem(row,6, newItem5);

                QTableWidgetItem *newItem6 = new QTableWidgetItem();
                QIcon icon6;
                icon6.addFile(QString::fromUtf8(":/icons/icons/date2.png"), QSize(), QIcon::Normal, QIcon::Off);
                newItem6->setIcon(icon6);
                newItem6->setText(query->value(6).toString());
                newItem6->setFlags(newItem6->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAVP->setItem(row,7, newItem6);

                QTableWidgetItem *newItem7 = initViolations(query->value(9).toInt());
                ui->tableWidgetAVP->setItem(row,8, newItem7);

                QTableWidgetItem *newItem8 = new QTableWidgetItem();
                newItem8->setText(query->value(9).toString());
                newItem8->setFlags(newItem8->flags() ^ Qt::ItemIsEditable);
                ui->tableWidgetAVP->setItem(row,9, newItem8);

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
void MainWindow::slotEditAVP(int, int)
{
    slotEditAVP();
}

//=========================================================
void MainWindow::slotEditAVP()
{
    QString tmp, sql;
    QModelIndexList selectedRows = ui->tableWidgetAVP->selectionModel()->selectedRows();
    dEditAVP->initAVS(ui->tableWidgetAVP->item(selectedRows[0].row(),9)->text().toLongLong());

    if(dEditAVP->exec() == QDialog::Accepted)
    {
        try
        {
            sql = "UPDATE \"avp\" SET \"NameRus\"=E\'";
            sql += cImportData->decode(dEditAVP->getNameAVP());
            sql += "\',\"NameOriginal\"=E\'";
            sql += cImportData->decode(dEditAVP->getNameOriginal());
            sql += "\',\"URL\"=\'";
            sql += dEditAVP->getURL_AVP();
            sql += "\' WHERE \"ID\"=";
            sql += ui->tableWidgetAVP->item(selectedRows[0].row(),9)->text();
            sql += ";";
            qDebug()<<"sql="<<sql;

            if(!query->exec(sql))
            {
                qDebug()<<query->lastError().text();
                QMessageBox::warning(this, tr("Внимание"),query->lastError().text(),tr("Да"));
                return;
            }

            sql = "UPDATE \"AVPattribute\" SET \"Rubric\"=\'";
            sql += dEditAVP->getRubricAVP();
            sql += "\',\"FilmMaker\"=E\'";
            sql += cImportData->decode(dEditAVP->getFilmMaker());
            sql += "\',\"Age\"=\'";
            sql += dEditAVP->getAge();
            sql += "\',\"YearOfRelease\"=\'";
            sql += dEditAVP->getYearOfRelease();
            sql += "\',\"Duration\"=\'";
            sql += dEditAVP->getDuration();
            sql += "\' WHERE \"ID_AVP\"=";
            sql += ui->tableWidgetAVP->item(selectedRows[0].row(),9)->text();
            sql += ";";
            qDebug()<<"sql="<<sql;

            if(!query->exec(sql))
            {

                qDebug()<<query->lastError().text();
                QMessageBox::warning(this, tr("Внимание"),query->lastError().text(),tr("Да"));
                return;
            }

            addRecordJournalJobAVP(1,"Редактирование АВП",dEditAVP->getNameAVP());
//            slotReload();
            ui->tableWidgetAVP->item(selectedRows[0].row(),0)->setText(dEditAVP->getNameAVP());
            ui->tableWidgetAVP->item(selectedRows[0].row(),1)->setText(dEditAVP->getURL_AVP());
            ui->tableWidgetAVP->item(selectedRows[0].row(),2)->setText(dEditAVP->getRubricAVP());
            ui->tableWidgetAVP->item(selectedRows[0].row(),3)->setText(dEditAVP->getYearOfRelease());
            ui->tableWidgetAVP->item(selectedRows[0].row(),4)->setText(dEditAVP->getFilmMaker());
            ui->tableWidgetAVP->item(selectedRows[0].row(),5)->setText(dEditAVP->getAge());

        }
        catch(std::exception &e)
        {
            qDebug()<<e.what();
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
            {
                QMessageBox::information(this,"Добавление АВП","АВП добавлено успешно.","Да");
                addRecordJournalJobAVP(1,"Добавление АВП",dAddAVP->getNameAVP());
            }
            else
            {
                QMessageBox::critical(this,"Ошибка","Ошибка добавления АВП!","Да");
                addRecordJournalJobAVP(2,"Ошибка добавления АВП!",dAddAVP->getNameAVP());
            }
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

    QString tmp, timestamp, dateRealization,sql ="";
    std::map<long,QString> listAVP;
    dAddTask->clearLineFindAVP();
    dAddTask->initComboBoxUser();
    dAddTask->initComboBoxStatus();
    dAddTask->initComboBoxPriority();
    dAddTask->initAVS();
    dAddTask->initStateAVP();
    dAddTask->initTableListAVP();

    QDateTime dt;
    QApplication::restoreOverrideCursor();

    dAddTask->showMaximized();

    if(dAddTask->exec() == QDialog::Accepted)
    {
        try
        {
            listAVP = dAddTask->getListAVP();
            for(auto idAVP : listAVP)
            {
                sql = "INSERT INTO \"Task\"(\"ID_User\",\"ID_TaskStatus\",\"ID_Priority\",\"ID_AVP\",\"PercentCompleted\",\"DateAppoint\",\"Comment\",\"DateRealization\") VALUES(";
                sql += tmp.setNum(getIdUser(dAddTask->getUserFIO()));sql += ",";
                sql += tmp.setNum(getIdTaskStatus(dAddTask->getStatusName()));sql += ",";
                sql += tmp.setNum(getIdPriority(dAddTask->getNamePriority()));sql += ",";
                sql += tmp.setNum(idAVP.first); sql += ",0,\'";
                timestamp = QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss");
                sql += timestamp;
                sql += "\',\'Требуется анализ\',\'";
                dateRealization = dAddTask->getDateRealization().toString("yyyy-MM-ddTHH:mm:ss");
                sql += dateRealization;
                sql += "\');";
//                qDebug()<<"sql="<<sql;
                if(!query->exec(sql))
                    qDebug()<<query->lastError().text();
                else
                    addRecordJournalJobAVP(1,"Добавление новой задачи АВП",getNameRusAVP(idAVP.first));
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
    if(selectedRows.empty())
    {
        QMessageBox::information(this, tr("Сообщение"),tr("Вы не выбрали АВП для редактирования!"),tr("Да"));
        return;
    }


    dEditTaskUser->initComboBoxUser(ui->tableWidgetCurrentTasks->item(selectedRows[0].row(),3)->text());
    dEditTaskUser->initComboBoxPriority(ui->tableWidgetCurrentTasks->item(selectedRows[0].row(),6)->text());
    dEditTaskUser->initComboBoxStatus(ui->tableWidgetCurrentTasks->item(selectedRows[0].row(),7)->text());
    dEditTaskUser->initTableViolation(ui->tableWidgetCurrentTasks->item(selectedRows[0].row(),12)->text().toLong());
    dEditTaskUser->initDateRealization(ui->tableWidgetCurrentTasks->item(selectedRows[0].row(),12)->text().toLong());

    dEditTaskUser->setNameAVP(ui->tableWidgetCurrentTasks->item(selectedRows[0].row(),0)->text());
    dEditTaskUser->setPercent(ui->tableWidgetCurrentTasks->item(selectedRows[0].row(),8)->text());
    dEditTaskUser->setComment(ui->tableWidgetCurrentTasks->item(selectedRows[0].row(),10)->text());

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
            sql += "\',\"DateRealization\"=\'";
            sql += dEditTaskUser->getDateRealization();
            sql += "\',\"DateClose\"=\'";
            QDateTime currentDate = QDateTime::currentDateTime();
            sql += currentDate.toString("yyyy-MM-ddTHH:mm:ss");
            sql += "\' WHERE \"ID\"=";
            sql += ui->tableWidgetCurrentTasks->item(selectedRows[0].row(),11)->text();
            sql += ";";

//            qDebug()<<"sql="<<sql;
            if(!query->exec(sql))
                qDebug()<<query->lastError().text();
            else
            {
                QString info;
                info ="Редактирование \"Текущие задачи\" ";
                if(dEditTaskUser->flagEditOperator == true)
                {
                    info += "(поменялся оператор на - ";info += dEditTaskUser->getNameUser(); info +=") ";
                }
                if(dEditTaskUser->flagEditPriority == true)
                {
                    info += "(поменялся приоритет на - ";info += dEditTaskUser->getPriority(); info +=") ";
                }
                if(dEditTaskUser->flagEditStatus == true)
                {
                    info += "(поменялся статус на - ";info += dEditTaskUser->getStatus(); info +=") ";
                }
                if(dEditTaskUser->flagEditPercent == true)
                {
                    info += "(поменялся процент на - ";info += dEditTaskUser->getPercent(); info +=") ";
                }
                if(dEditTaskUser->flagEditDate == true)
                {
                    info += "(поменялась дата на - ";info += dEditTaskUser->getDateRealization(); info +=") ";
                }
                if(dEditTaskUser->flagEditComment == true)
                {
                    info += "(поменялся комментарий)";
                }
                info +=" АВП";
                addRecordJournalJobAVP(1,info, getNameRusAVP(ui->tableWidgetCurrentTasks->item(selectedRows[0].row(),12)->text().toLong()));
                dEditTaskUser->flagEditOperator = false;
                dEditTaskUser->flagEditPriority = false;
                dEditTaskUser->flagEditStatus = false;
                dEditTaskUser->flagEditPercent = false;
                dEditTaskUser->flagEditDate = false;
                dEditTaskUser->flagEditComment = false;
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
void MainWindow::slotEditCurrentAudit(int, int)
{
    slotEditCurrentAudit();
}

//=========================================================
void MainWindow::slotEditAudit(int, int)
{
    slotEditAudit();
}

//=========================================================
void MainWindow::slotEditAuditRKN(int, int)
{
    slotEditAuditRKN();
}

//=========================================================
void MainWindow::slotPreviewAchive(int, int)
{
    QString tmp, timestamp, sql ="";
    QModelIndexList selectedRows = ui->tableWidgetAuditArchive->selectionModel()->selectedRows();

    dPreviewArchive->setNameAVP(ui->tableWidgetAuditArchive->item(selectedRows[0].row(),0)->text());
    dPreviewArchive->setOperatorFIO(ui->tableWidgetAuditArchive->item(selectedRows[0].row(),3)->text());
    dPreviewArchive->setExpertFIO(ui->tableWidgetAuditArchive->item(selectedRows[0].row(),4)->text());
    dPreviewArchive->setComment(ui->tableWidgetAuditArchive->item(selectedRows[0].row(),7)->text());

    dPreviewArchive->initTableViolation(ui->tableWidgetAuditArchive->item(selectedRows[0].row(),10)->text().toLong());

    dPreviewArchive->exec();
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
                sql = "DELETE FROM avp WHERE \"ID\"=";sql += ui->tableWidgetAVP->item(selectedRows[0].row(),9)->text(); sql += ";";
//                qDebug()<<"sql ="<<sql;
                if(query->exec(sql))
                {
                    addRecordJournalJobAVP(3,"Удаление АВП",ui->tableWidgetAVP->item(selectedRows[0].row(),0)->text());
                    ui->tableWidgetAVP->removeRow(selectedRows[0].row());
                    selectedRows = ui->tableWidgetAVP->selectionModel()->selectedRows();
                }
                else
                    qDebug()<<query->lastError().text();
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
                    sql = "DELETE FROM \"Task\" WHERE \"ID\"=";sql += ui->tableWidgetCurrentTasks->item(selectedRows[0].row(),11)->text(); sql += ";";
//                    qDebug()<<"sql ="<<sql;
                    if(query->exec(sql))
                    {
                        addRecordJournalJobAVP(1,"Удаление задачи АВП", getNameRusAVP(ui->tableWidgetCurrentTasks->item(selectedRows[0].row(),12)->text().toLong()));
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
    QModelIndexList selectedRows = ui->tableWidgetMyTasks->selectionModel()->selectedRows();

    dEditTask->setNameAVP(ui->tableWidgetMyTasks->item(selectedRows[0].row(),0)->text());
    dEditTask->setPercent(ui->tableWidgetMyTasks->item(selectedRows[0].row(),7)->text());
    dEditTask->setComment(ui->tableWidgetMyTasks->item(selectedRows[0].row(),9)->text());

    dEditTask->initTableViolation(ui->tableWidgetMyTasks->item(selectedRows[0].row(),11)->text().toLong());
    dEditTask->initComboBoxStatus(ui->tableWidgetMyTasks->item(selectedRows[0].row(),6)->text());
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
            sql += ui->tableWidgetMyTasks->item(selectedRows[0].row(),10)->text();
            sql += ";";

//            qDebug()<<"sql="<<sql;
            if(!query->exec(sql))
                qDebug()<<query->lastError().text();
            else
                addRecordJournalJobAVP(1,"Редактирование \"Мои задачи\" АВП", getNameRusAVP(ui->tableWidgetMyTasks->item(selectedRows[0].row(),11)->text().toLong()));

            initTableTask(true);
        }
        catch(std::exception &e)
        {
            qDebug()<<e.what();
            qDebug()<<query->lastError().text();
            QMessageBox::warning(this, tr("Внимание"),query->lastError().text(),tr("Да"));
        }
        changeStatusAVP(ui->tableWidgetMyTasks->item(selectedRows[0].row(),11)->text().toLong(),1);
        initTableMyTask();
    }
}

//=========================================================
void MainWindow::slotEditCurrentAudit()
{
    QString tmp, sql ="";
    dEditAudit->clear();
    QModelIndexList selectedRows = ui->tableWidgetAudit_4->selectionModel()->selectedRows();
    if(selectedRows.empty())
    {
        QMessageBox::information(this, tr("Сообщение"),tr("Вы не выбрали АВП для редактирования!"),tr("Да"));
        return;
    }

    dEditAudit->setWindowTitle("Экспертиза");
    dEditAudit->setNameAVP(ui->tableWidgetAudit_4->item(selectedRows[0].row(),0)->text());
    dEditAudit->initTableViolation(ui->tableWidgetAudit_4->item(selectedRows[0].row(),12)->text().toLong());
    dEditAudit->initComboBoxViolation();
    dEditAudit->hideViolationGroup();

    if(dEditAudit->exec() == QDialog::Accepted)
    {
        try
        {
            sql = "UPDATE \"Task\" SET \"ID_TaskStatus\"=";
            sql += tmp.setNum(getIdTaskStatus("Одобрена экспертом"));
            sql += ",\"ID_Expert\"=";
            sql += tmp.setNum(getIdUser(currentUserName));
            sql+=",\"Comment\"=\'";
            tmp = "Эксперт: " + currentUserName +" "+ dEditAudit->getComment();
            sql += tmp;
            sql += "\' WHERE \"ID\"=";
            sql += ui->tableWidgetAudit_4->item(selectedRows[0].row(),10)->text();
            sql += ";";

            qDebug()<<"sql="<<sql;
            if(!query->exec(sql))
            {
                qDebug()<<query->lastError().text();
                QMessageBox::warning(this, tr("Внимание"),query->lastError().text(),tr("Да"));
            }
            else
                addRecordJournalJobAVP(1,"Редактирование \"Экспертиза\" АВП", getNameRusAVP(ui->tableWidgetAudit_4->item(selectedRows[0].row(),12)->text().toLong()));
        }
        catch(std::exception &e)
        {
            qDebug()<<e.what();
            qDebug()<<query->lastError().text();
            QMessageBox::warning(this, tr("Внимание"),query->lastError().text(),tr("Да"));
        }
        initTableCurrentAudit();
    }
}

//=========================================================
void MainWindow::slotEditAudit()
{
    QString tmp, sql ="";
    dEditAudit->clear();
    QModelIndexList selectedRows = ui->tableWidgetAudit->selectionModel()->selectedRows();
    if(selectedRows.empty())
    {
        QMessageBox::information(this, tr("Сообщение"),tr("Вы не выбрали АВП для редактирования!"),tr("Да"));
        return;
    }

    dEditAudit->setWindowTitle("Экспертиза");
    dEditAudit->setNameAVP(ui->tableWidgetAudit->item(selectedRows[0].row(),0)->text());
    dEditAudit->initTableViolation(ui->tableWidgetAudit->item(selectedRows[0].row(),11)->text().toLong());
    dEditAudit->initComboBoxViolation();
    dEditAudit->hideViolationGroup();

    if(dEditAudit->exec() == QDialog::Accepted)
    {
        try
        {
            sql = "UPDATE \"Task\" SET \"ID_TaskStatus\"=";
            sql += tmp.setNum(getIdTaskStatus("Одобрена экспертом"));
            sql += ",\"ID_Expert\"=";
            sql += tmp.setNum(getIdUser(currentUserName));
            sql+=",\"Comment\"=\'";
            tmp = "Эксперт: " + currentUserName +" "+ dEditAudit->getComment();
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
            else
                addRecordJournalJobAVP(1,"Редактирование \"Экспертиза\" АВП", getNameRusAVP(ui->tableWidgetAudit->item(selectedRows[0].row(),11)->text().toLong()));
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
void MainWindow::slotEditAuditRKN()
{
    qDebug()<<"slotEditAuditRKN()";
    QString tmp, sql ="";
    dEditAudit->clear();
    QModelIndexList selectedRows = ui->tableWidgetAudit_2->selectionModel()->selectedRows();
    if(selectedRows.empty())
    {
        QMessageBox::information(this, tr("Сообщение"),tr("Вы не выбрали АВП для редактирования!"),tr("Да"));
        return;
    }


    dEditAudit->setWindowTitle("Инспектр РКН");
    dEditAudit->setNameAVP(ui->tableWidgetAudit_2->item(selectedRows[0].row(),0)->text());
    dEditAudit->initTableViolation(ui->tableWidgetAudit_2->item(selectedRows[0].row(),11)->text().toLong());
    dEditAudit->initComboBoxViolation();
    dEditAudit->hideViolationGroup();

    if(dEditAudit->exec() == QDialog::Accepted)
    {
        try
        {
            sql = "UPDATE \"Task\" SET \"ID_TaskStatus\"=";
            sql += tmp.setNum(getIdTaskStatus("Проверена инспектором РКН"));
            sql += ",\"ID_Expert\"=";
            sql += tmp.setNum(getIdUser(currentUserName));
            sql+=",\"Comment\"=\'";
            tmp = "Инспектр РКН: " + currentUserName +" "+ dEditAudit->getComment();
            sql += tmp;
            sql += "\' WHERE \"ID\"=";
            sql += ui->tableWidgetAudit_2->item(selectedRows[0].row(),9)->text();
            sql += ";";

            qDebug()<<"sql="<<sql;
            if(!query->exec(sql))
            {
                qDebug()<<query->lastError().text();
                QMessageBox::warning(this, tr("Внимание"),query->lastError().text(),tr("Да"));
            }
            else
                addRecordJournalJobAVP(1,"Редактирование \"Инспектр РКН\" АВП", getNameRusAVP(ui->tableWidgetAudit_2->item(selectedRows[0].row(),11)->text().toLong()));
        }
        catch(std::exception &e)
        {
            qDebug()<<e.what();
            qDebug()<<query->lastError().text();
            QMessageBox::warning(this, tr("Внимание"),query->lastError().text(),tr("Да"));
        }
        initTableAuditRKN();
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
void MainWindow::slotMakeReportAll()
{
    dReportAllStatistics->showMaximized();
}

//=========================================================
void MainWindow::slotMakeReport()
{
//    dReportJob->initTableReportJob();
    dReportJob->showMaximized();
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
    m_countAVP = countAVP_Analysis(m_currentState,m_currentIdAVS);
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
    m_countAVP = countAVP_Analysis(m_currentState,m_currentIdAVS);
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
void MainWindow::slotFilterApply1()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    initTableCurrentAudit();
    QApplication::restoreOverrideCursor();
}

//=========================================================
void MainWindow::slotFilterApply2()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    initTableArchive();
    QApplication::restoreOverrideCursor();
}

//=========================================================
void MainWindow::slotFilterApply3()
{
    qDebug()<<__PRETTY_FUNCTION__;
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    initTableAuditRKN();
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
void MainWindow::slotTextChanged1(const QString &text)
{
    if(text.length()>1)
        m_timer1.start();
}

//=========================================================
void MainWindow::slotFindArchiveAVP()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QString sql="";
    try
    {
        sql = "SELECT avp.\"NameRus\", "
              "avp.\"URL\","
              "u.\"FIO\","
              "ue.\"FIO\","
              "ts.\"StatusName\","
              "\"Task\".\"Comment\","
              "\"Task\".\"ID\","
              "\"Task\".\"ID_AVP\", "
              "\"Task\".\"DateClose\" "
              "FROM \"Task\" "
              "INNER JOIN avp ON \"Task\".\"ID_AVP\" = avp.\"ID\" "
              "INNER JOIN \"User\" u ON \"Task\".\"ID_User\" = u.\"ID\" "
              "LEFT JOIN \"User\" ue ON \"Task\".\"ID_Expert\" = ue.\"ID\" "
              "INNER JOIN \"TaskStatus\" ts ON \"Task\".\"ID_TaskStatus\" = ts.\"ID\"";
              if(!ui->lineEditFindString_2->text().isEmpty())
                  sql += " WHERE avp.\"NameRus\" LIKE \'"+ui->lineEditFindString_2->text()+"%\';";
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
                ui->tableWidgetAuditArchive->setSortingEnabled(false);
                ui->tableWidgetAuditArchive->clearContents();
                ui->tableWidgetAuditArchive->setRowCount(0);

                int row = 0;
                while(query->next())
                {
                    ui->tableWidgetAuditArchive->setRowCount(row+1);

                    QTableWidgetItem *newItem = new QTableWidgetItem();
                    QIcon icon;
                    icon.addFile(QString::fromUtf8(":/icons/icons/film_projector_cinema.png"), QSize(), QIcon::Normal, QIcon::Off);
                    newItem->setIcon(icon);
                    newItem->setText(query->value(0).toString());
                    newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidgetAuditArchive->setItem(row,0, newItem);//Название АВП

                    QTableWidgetItem *newItem1 = new QTableWidgetItem();
                    QIcon icon1;
                    icon1.addFile(QString::fromUtf8(":/icons/icons/web.png"), QSize(), QIcon::Normal, QIcon::Off);
                    newItem1->setIcon(icon1);
                    newItem1->setText(query->value(1).toString());
                    newItem1->setFlags(newItem1->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidgetAuditArchive->setItem(row,1, newItem1);//URL

                    QTableWidgetItem *newItem2 = new QTableWidgetItem();
                    QIcon icon5;
                    icon5.addFile(QString::fromUtf8(":/icons/icons/user.png"), QSize(), QIcon::Normal, QIcon::Off);
                    newItem2->setIcon(icon5);
                    newItem2->setText(query->value(2).toString());
                    newItem2->setFlags(newItem2->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidgetAuditArchive->setItem(row,2, newItem2);//ФИО

                    QTableWidgetItem *newItem3 = new QTableWidgetItem();
                    QIcon icon3;
                    icon3.addFile(QString::fromUtf8(":/icons/icons/user.png"), QSize(), QIcon::Normal, QIcon::Off);
                    newItem3->setIcon(icon3);
                    newItem3->setText(query->value(3).toString());
                    newItem3->setFlags(newItem2->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidgetAuditArchive->setItem(row,3, newItem3);//ФИО, эксперта

                    QTableWidgetItem *newItem31 = new QTableWidgetItem();
                    QIcon icon31;
                    icon31 .addFile(QString::fromUtf8(":/icons/icons/date2.png"), QSize(), QIcon::Normal, QIcon::Off);
                    newItem31->setIcon(icon31);
                    newItem31->setText(query->value(8).toDate().toString("yyyy-MM-dd"));
                    newItem31->setFlags(newItem31->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidgetAuditArchive->setItem(row,4, newItem31);//Дата закрытия задачи

                    QTableWidgetItem *newItem4 = new QTableWidgetItem();
                    newItem4->setText(query->value(4).toString());
                    newItem4->setFlags(newItem4->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidgetAuditArchive->setItem(row,5, newItem4);//Статус

                    QTableWidgetItem *newItem5 = initViolations(query->value(7).toInt());
                    ui->tableWidgetAuditArchive->setItem(row,6, newItem5);//Нарушения

                    QTableWidgetItem *newItem6 = new QTableWidgetItem();
                    newItem6->setText(query->value(5).toString());
                    newItem6->setFlags(newItem6->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidgetAuditArchive->setItem(row,7, newItem6);//Комментарии

                    QTableWidgetItem *newItem9 = new QTableWidgetItem();
                    newItem9->setText(query->value(6).toString());
                    newItem9->setFlags(newItem9->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidgetAuditArchive->setItem(row,8, newItem9);//ID

                    QTableWidgetItem *newItem10 = new QTableWidgetItem();
                    newItem10->setText(query->value(7).toString());
                    newItem10->setFlags(newItem10->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidgetAuditArchive->setItem(row,9, newItem10);//ID_AVP

                    row++;
                }
                ui->tableWidgetAuditArchive->setSortingEnabled(true);
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
              "avp.\"ID\","
              "aa.\"Age\" FROM avp "
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

                    QTableWidgetItem *newItem4 = new QTableWidgetItem();//Режиссер
                    QIcon icon4;
                    icon4.addFile(QString::fromUtf8(":/icons/icons/film_maker1.png"), QSize(), QIcon::Normal, QIcon::Off);
                    newItem4->setIcon(icon4);
                    newItem4->setText(query->value(4).toString());
                    newItem4->setFlags(newItem4->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidgetAVP->setItem(row,4, newItem4);

                    QTableWidgetItem *newItem41 = new QTableWidgetItem();//Возраст
                    newItem41->setText(query->value(10).toString());
                    newItem41->setFlags(newItem41->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidgetAVP->setItem(row,5, newItem41);

                    QTableWidgetItem *newItem5 = new QTableWidgetItem();
                    QIcon icon5;
                    icon5.addFile(QString::fromUtf8(":/icons/icons/user.png"), QSize(), QIcon::Normal, QIcon::Off);
                    newItem5->setIcon(icon5);
                    newItem5->setText(query->value(5).toString());
                    newItem5->setFlags(newItem5->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidgetAVP->setItem(row,6, newItem5);

                    QTableWidgetItem *newItem6 = new QTableWidgetItem();
                    QIcon icon6;
                    icon6.addFile(QString::fromUtf8(":/icons/icons/date2.png"), QSize(), QIcon::Normal, QIcon::Off);
                    newItem6->setIcon(icon6);
                    newItem6->setText(query->value(6).toString());
                    newItem6->setFlags(newItem6->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidgetAVP->setItem(row,7, newItem6);

                    QTableWidgetItem *newItem7 = initViolations(query->value(9).toInt());
                    ui->tableWidgetAVP->setItem(row,8, newItem7);

                    QTableWidgetItem *newItem8 = new QTableWidgetItem();
                    newItem8->setText(query->value(9).toString());
                    newItem8->setFlags(newItem8->flags() ^ Qt::ItemIsEditable);
                    ui->tableWidgetAVP->setItem(row,9, newItem8);

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
        initTableCurrentAudit();
    }
    else if(index == 4 )
    {
        ui->pushButtonReload->hide();
        initTableAudit();
    }
    else if(index == 5 )
    {
        ui->pushButtonReload->hide();
        initTableAuditRKN();
    }
    else if(index == 6 )
    {
        ui->pushButtonReload->hide();
        initTableArchive();
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
void MainWindow::slotJournalSession()
{
    dJournalSession->initTableJournalSession();
    dJournalSession->exec();
}

//=========================================================
void MainWindow::slotJournalJob()
{
    dJournalJobAVP->initTableJournalSession();
    dJournalJobAVP->exec();
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
void MainWindow::slotContextMenuRequestedExpert_2(const QPoint &pos)
{
    QMenu * menu = new QMenu(this);

    QAction * actionGoToURL = new QAction(tr("Перейти по ссылки URL"), this);
    QIcon icon1;
    icon1.addFile(QString::fromUtf8(":/icons/icons/web.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionGoToURL->setIcon(icon1);
    connect(actionGoToURL, SIGNAL(triggered()), this, SLOT(slotGoToURL()));

    menu->addAction(actionGoToURL);
    menu->popup(ui->tableWidgetAudit_2->viewport()->mapToGlobal(pos));
}

//=========================================================
void MainWindow::slotContextMenuRequestedExpert_4(const QPoint &pos)
{
    QMenu * menu = new QMenu(this);

    QAction * actionGoToURL = new QAction(tr("Перейти по ссылки URL"), this);
    QIcon icon1;
    icon1.addFile(QString::fromUtf8(":/icons/icons/web.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionGoToURL->setIcon(icon1);
    connect(actionGoToURL, SIGNAL(triggered()), this, SLOT(slotGoToURL()));

    menu->addAction(actionGoToURL);
    menu->popup(ui->tableWidgetAudit_4->viewport()->mapToGlobal(pos));
}

//=========================================================
void MainWindow::slotContextMenuRequestedCurrentTask(const QPoint &pos)
{
       QMenu * menu = new QMenu(this);

       QAction * actionGoToURL = new QAction(tr("Перейти по ссылки URL"), this);
       QIcon icon1;
       icon1.addFile(QString::fromUtf8(":/icons/icons/web.png"), QSize(), QIcon::Normal, QIcon::Off);
       actionGoToURL->setIcon(icon1);

       QAction * actionChageStatus = new QAction(tr("Сменить статус АВП на \"Закрыта\""), this);
       QIcon icon2;
       icon2.addFile(QString::fromUtf8(":/icons/icons/button_reload.png"), QSize(), QIcon::Normal, QIcon::Off);
       actionChageStatus->setIcon(icon2);

       connect(actionGoToURL, SIGNAL(triggered()), this, SLOT(slotGoToURL()));
       connect(actionChageStatus, SIGNAL(triggered()), this, SLOT(slotCurrentTaskChageStatus()));

       menu->addAction(actionGoToURL);
       menu->addAction(actionChageStatus);

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
            m_client->sendCommandAnalysisAVP(ui->tableWidgetMyTasks->item(row,10)->text().toLongLong());

            QIcon icon1;
            icon1.addFile(QString::fromUtf8(":/icons/icons/wait.png"), QSize(), QIcon::Normal, QIcon::Off);
            ui->tableWidgetMyTasks->item(row,7)->setIcon(icon1);
            ui->tableWidgetMyTasks->item(row,7)->setText("АВП анализируется...");
        }
        else if(selectedRows.count() > 1)
        {
            for(int i=0; i<selectedRows.count(); ++i)
            {
                row = selectedRows[i].row();
                m_client->sendCommandAnalysisAVP(ui->tableWidgetMyTasks->item(row,10)->text().toLongLong());

                QIcon icon1;
                icon1.addFile(QString::fromUtf8(":/icons/icons/wait.png"), QSize(), QIcon::Normal, QIcon::Off);
                ui->tableWidgetMyTasks->item(row,7)->setIcon(icon1);
                ui->tableWidgetMyTasks->item(row,7)->setText("АВП анализируется...");
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
        int row = ui->tableWidgetAudit_4->selectionModel()->currentIndex().row();
        QDesktopServices::openUrl(QUrl(ui->tableWidgetAudit_4->item(row,1)->text()));
    }
    else if(ui->tabWidget->currentIndex()== 4)
    {
        int row = ui->tableWidgetAudit->selectionModel()->currentIndex().row();
        QDesktopServices::openUrl(QUrl(ui->tableWidgetAudit->item(row,1)->text()));
    }
    else if(ui->tabWidget->currentIndex()== 5)
    {
        int row = ui->tableWidgetAudit_2->selectionModel()->currentIndex().row();
        QDesktopServices::openUrl(QUrl(ui->tableWidgetAudit_2->item(row,1)->text()));
    }
}

//=========================================================
void MainWindow::slotCurrentTaskChageStatus()
{
    qDebug()<<__PRETTY_FUNCTION__;
    QString sql="";
    QModelIndexList selectedRows = ui->tableWidgetCurrentTasks->selectionModel()->selectedRows();
    if(selectedRows.empty())
    {
        QMessageBox::information(this, tr("Сообщение"),tr("Вы не выбрали задачу для смены статуса!"),tr("Да"));
        return;
    }
    else
    {
            try
            {
                if(QMessageBox::warning(this,"Внимание","После закрытия задача уйдет в архив.\nВы действительно хотите поменять статус?","Да","Нет") == 0)
                {
                    while (!selectedRows.empty())
                    {
                        sql = "UPDATE \"Task\" SET \"ID_TaskStatus\" = 4, \"DateClose\" = \'";
                        QDateTime currentDate = QDateTime::currentDateTime();
                        sql += currentDate.toString("yyyy-MM-ddTHH:mm:ss");
                        sql +="\' WHERE \"ID\"=";
                        sql += ui->tableWidgetCurrentTasks->item(selectedRows[0].row(),10)->text();
                        sql += ";";
                        qDebug()<<"sql ="<<sql;
                        if(query->exec(sql))
                        {
                            addRecordJournalJobAVP(1,"Закрытие задачи АВП", getNameRusAVP(ui->tableWidgetCurrentTasks->item(selectedRows[0].row(),11)->text().toLong()));
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
const QTableWidget* MainWindow::getTableCurrentExpert() const
{
    return ui->tableWidgetAudit_4;
}

//=========================================================
void MainWindow::slotExit()
{
    if(QMessageBox::information(this, tr("Сообщение"),tr("Вы действительно хотите выйти из приложения?"),tr("Да"),tr("Нет"))==0)
    {
//        saveInJournalRecordClose();
        close();
    }
}
