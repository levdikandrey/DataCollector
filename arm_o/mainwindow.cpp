#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "daddavp.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QSize>
#include <QScreen>

#include <QMessageBox>
#include <QPushButton>
#include <QFileDialog>
#include <QDebug>
#include <QThread>

#include <QSqlDatabase>
#include <QSqlError>
#include <QTableWidget>

extern QSqlDatabase db;
//=========================================================
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initDialog();

    connect(ui->actionExit, SIGNAL(triggered()), SLOT(slotExit()));
    connect(ui->actionAbout, SIGNAL(triggered()), SLOT(slotAbout()));
    connect(ui->actionExcel, SIGNAL(triggered()), SLOT(slotImportData()));
    connect(ui->actionKinopoisk, SIGNAL(triggered()), SLOT(slotImportKinopoisk()));
    connect(ui->actionAddAVP, SIGNAL(triggered()), SLOT(slotAddAVP()));
    connect(ui->actionClearDB, SIGNAL(triggered()), SLOT(slotClearDB()));
    connect(ui->actionGroup, SIGNAL(triggered()), SLOT(slotGroup()));
    connect(ui->actionUser, SIGNAL(triggered()), SLOT(slotUser()));
    connect(ui->actionViolation, SIGNAL(triggered()), SLOT(slotViolation()));

    connect(ui->tableWidgetCurrentTasks, SIGNAL(cellDoubleClicked(int, int)),this, SLOT(slotEditTask(int, int)));
    connect(ui->checkBoxAnalyzed, SIGNAL(stateChanged(int)),this, SLOT(slotStateChanged(int))));

    query = nullptr;

    initDB();
    initAVS();
    initTableAVP();

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

    m_currentNumberPage = 1;
    m_countAVP = countAVP();
    QString tmp;
    QString text="из ";text+=tmp.setNum(m_countAVP/1000); text+=" (всего АВП ";text+=tmp.setNum(m_countAVP);text+=")";
    ui->lineEditNumberPage->setValidator( new QIntValidator(1,(m_countAVP/1000), this) );
    ui->labelCountAVP->setText(text);
    ui->pushButtonPreview->setEnabled(false);
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
//    qApp->setStyleSheet("QPushButton{ border-style: outset;border-width: 2px;border-radius: 10px;border-color: beige;font: bold 12px; min-width: 10em; padding: 6px;}");

//    ui->pushButtonExit->setStyleSheet("QPushButton{border: 2px solid #8f8f91; border-radius: 6px; border-color: navy; min-width: 100px; padding: 4px;}");
//    ui->tableWidgetAVP->verticalHeader()->hide();

    ui->tableWidgetAVP->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->tableWidgetAVP->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->tableWidgetAVP->horizontalHeader()->resizeSection(2, 130);
    ui->tableWidgetAVP->horizontalHeader()->resizeSection(3, 80);
    ui->tableWidgetAVP->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Stretch);
    ui->tableWidgetAVP->horizontalHeader()->resizeSection(5, 150);
    ui->tableWidgetAVP->horizontalHeader()->resizeSection(6, 130);
    ui->tableWidgetAVP->horizontalHeader()->resizeSection(7, 180);
    ui->tableWidgetAVP->horizontalHeader()->resizeSection(8, 0);

    ui->tableWidgetCurrentTasks->horizontalHeader()->resizeSection(0, 200);
    ui->tableWidgetCurrentTasks->horizontalHeader()->resizeSection(1, 250);
    ui->tableWidgetCurrentTasks->horizontalHeader()->resizeSection(2, 150);
    ui->tableWidgetCurrentTasks->horizontalHeader()->resizeSection(3, 120);
    ui->tableWidgetCurrentTasks->horizontalHeader()->resizeSection(4, 140);
    ui->tableWidgetCurrentTasks->horizontalHeader()->resizeSection(5, 180);
    ui->tableWidgetCurrentTasks->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);
    ui->tableWidgetCurrentTasks->horizontalHeader()->resizeSection(7, 0);

    ui->tableWidgetMyTasks->horizontalHeader()->resizeSection(0,250);
    ui->tableWidgetMyTasks->horizontalHeader()->resizeSection(1,300);
    ui->tableWidgetMyTasks->horizontalHeader()->resizeSection(2,120);
    ui->tableWidgetMyTasks->horizontalHeader()->resizeSection(3,150);
    ui->tableWidgetMyTasks->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Stretch);
    ui->tableWidgetMyTasks->horizontalHeader()->resizeSection(5, 0);


    //=============================
    ui->tableWidgetCurrentTasks->setRowCount(3);

    QTableWidgetItem *newItem = new QTableWidgetItem();
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/icons/icons/film_projector_cinema.ico"), QSize(), QIcon::Normal, QIcon::Off);
    newItem->setIcon(icon);
    newItem->setText("Фарца");
    ui->tableWidgetCurrentTasks->setItem(0,0, newItem);

    QTableWidgetItem *newItem1 = new QTableWidgetItem();
    QIcon icon1;
    icon1.addFile(QString::fromUtf8(":/icons/icons/web.ico"), QSize(), QIcon::Normal, QIcon::Off);
    newItem1->setIcon(icon1);
    newItem1->setText("https://www.imdb.com/title/tt4567218/?ref_=fn_al_tt_1");
    ui->tableWidgetCurrentTasks->setItem(0,1, newItem1);

    QTableWidgetItem *newItem5 = new QTableWidgetItem();
    QIcon icon5;
    icon5.addFile(QString::fromUtf8(":/icons/icons/user.ico"), QSize(), QIcon::Normal, QIcon::Off);
    newItem5->setIcon(icon5);
    newItem5->setText("Иванов И.И.");
    ui->tableWidgetCurrentTasks->setItem(0,2, newItem5);

    QTableWidgetItem *newItem2 = new QTableWidgetItem();
    newItem2->setText("50%");
    ui->tableWidgetCurrentTasks->setItem(0,3, newItem2);

    QTableWidgetItem *newItem3 = new QTableWidgetItem();
    newItem3->setText("В работе");
    ui->tableWidgetCurrentTasks->setItem(0,4, newItem3);

    QTableWidgetItem *newItem4 = new QTableWidgetItem();
    newItem4->setText("АВП не анализировалось");
    QIcon icon7;
    icon7.addFile(QString::fromUtf8(":/icons/icons/question.ico"), QSize(), QIcon::Normal, QIcon::Off);
    newItem4->setIcon(icon7);
    ui->tableWidgetCurrentTasks->setItem(0,5, newItem4);

    QTableWidgetItem *newItem6 = new QTableWidgetItem();
    newItem6->setText("Требуется анализ");
    ui->tableWidgetCurrentTasks->setItem(0,6, newItem6);


    //-----
    QTableWidgetItem *newItem7 = new QTableWidgetItem();
    newItem7->setIcon(icon);
    newItem7->setText("Мажор");
    ui->tableWidgetCurrentTasks->setItem(1,0, newItem7);

    QTableWidgetItem *newItem8 = new QTableWidgetItem();
    newItem8->setIcon(icon1);
    newItem8->setText("https://www.imdb.com/title/tt4426042/?ref_=fn_al_tt_1");
    ui->tableWidgetCurrentTasks->setItem(1,1, newItem8);

    QTableWidgetItem *newItem9 = new QTableWidgetItem();
    newItem9->setIcon(icon5);
    newItem9->setText("Сидоров А.А.");
    ui->tableWidgetCurrentTasks->setItem(1,2, newItem9);

    QTableWidgetItem *newItem10 = new QTableWidgetItem();
    newItem10->setText("100%");
    ui->tableWidgetCurrentTasks->setItem(1,3, newItem10);

    QTableWidgetItem *newItem11 = new QTableWidgetItem();
    newItem11->setText("Выполнена");
    ui->tableWidgetCurrentTasks->setItem(1,4, newItem11);

    QTableWidgetItem *newItem12 = new QTableWidgetItem();
    newItem12->setText("Наркотики");
    QIcon icon12;
    icon12.addFile(QString::fromUtf8(":/icons/icons/alarm.ico"), QSize(), QIcon::Normal, QIcon::Off);
    newItem12->setIcon(icon12);
    ui->tableWidgetCurrentTasks->setItem(1,5, newItem12);

    QTableWidgetItem *newItem13 = new QTableWidgetItem();
    newItem13->setText("Обнаружены сцены употребления наркотиков!");
    ui->tableWidgetCurrentTasks->setItem(1,6, newItem13);



    //------
    QTableWidgetItem *newItem14 = new QTableWidgetItem();
    newItem14->setIcon(icon);
    newItem14->setText("Крик совы");
    ui->tableWidgetCurrentTasks->setItem(2,0, newItem14);

    QTableWidgetItem *newItem15 = new QTableWidgetItem();
    newItem15->setIcon(icon1);
    newItem15->setText("https://www.imdb.com/title/tt456898/?ref_=fn_al_tt_1");
    ui->tableWidgetCurrentTasks->setItem(2,1, newItem15);

    QTableWidgetItem *newItem16 = new QTableWidgetItem();
    newItem16->setIcon(icon5);
    newItem16->setText("Петров С.С.");
    ui->tableWidgetCurrentTasks->setItem(2,2, newItem16);

    QTableWidgetItem *newItem17 = new QTableWidgetItem();
    newItem17->setText("100%");
    ui->tableWidgetCurrentTasks->setItem(2,3, newItem17);

    QTableWidgetItem *newItem18 = new QTableWidgetItem();
    newItem18->setText("Выполнена");
    ui->tableWidgetCurrentTasks->setItem(2,4, newItem18);

    QTableWidgetItem *newItem19 = new QTableWidgetItem();
    newItem19->setText("Не обнаружено");
    QIcon icon19;
    icon7.addFile(QString::fromUtf8(":/icons/icons/ok.ico"), QSize(), QIcon::Normal, QIcon::Off);
    newItem19->setIcon(icon7);
    ui->tableWidgetCurrentTasks->setItem(2,5, newItem19);

    QTableWidgetItem *newItem20 = new QTableWidgetItem();
    newItem20->setText("Нарушений не обнаружено");
    ui->tableWidgetCurrentTasks->setItem(2,6, newItem20);
//-----------------------------------



    ui->tableWidgetMyTasks->setRowCount(2);

    QTableWidgetItem *newItem21 = new QTableWidgetItem();
    newItem21->setIcon(icon);
    newItem21->setText("Точка отрыва");
    ui->tableWidgetMyTasks->setItem(0,0, newItem21);

    QTableWidgetItem *newItem22 = new QTableWidgetItem();
    newItem22->setIcon(icon1);
    newItem22->setText("https://www.imdb.com/title/tt456866/");
    ui->tableWidgetMyTasks->setItem(0,1, newItem22);

    QTableWidgetItem *newItem23 = new QTableWidgetItem();
    newItem23->setText("100%");
    ui->tableWidgetMyTasks->setItem(0,2, newItem23);

    QTableWidgetItem *newItem24 = new QTableWidgetItem();
    newItem24->setText("Выполнена");
    ui->tableWidgetMyTasks->setItem(0,3, newItem24);

    QTableWidgetItem *newItem25 = new QTableWidgetItem();
    newItem25->setText("Не обнаружено");
    newItem25->setIcon(icon7);
    ui->tableWidgetMyTasks->setItem(0,4, newItem25);



    QTableWidgetItem *newItem26 = new QTableWidgetItem();
    newItem26->setIcon(icon);
    newItem26->setText("Пятница");
    ui->tableWidgetMyTasks->setItem(1,0, newItem26);

    QTableWidgetItem *newItem27 = new QTableWidgetItem();
    newItem27->setIcon(icon1);
    newItem27->setText("https://www.imdb.com/title/tt456234/?ref_=fn_al_tt_1");
    ui->tableWidgetMyTasks->setItem(1,1, newItem27);

    QTableWidgetItem *newItem28 = new QTableWidgetItem();
    newItem28->setText("100%");
    ui->tableWidgetMyTasks->setItem(1,2, newItem28);

    QTableWidgetItem *newItem29 = new QTableWidgetItem();
    newItem29->setText("Выполнена");
    ui->tableWidgetMyTasks->setItem(1,3, newItem29);

    QTableWidgetItem *newItem30 = new QTableWidgetItem();
    newItem30->setText("Алкоголь");
    newItem30->setIcon(icon12);
    ui->tableWidgetMyTasks->setItem(1,4, newItem30);



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
void MainWindow::initTableTask()
{
    QString sql="";
    try
    {
        ui->tableWidgetCurrentTasks->clearContents();
        ui->tableWidgetCurrentTasks->setRowCount(0);

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
              "INNER JOIN \"User\" u ON u.\"ID\" = aa.\"ID_User\";";

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
                ui->tableWidgetCurrentTasks->setItem(row,0, newItem);
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
    QString avs;
    long id = -1;
    if(nameAVS == "Все")
        initTableAVP(1,-1);
    else
    {
        avs = nameAVS.mid(0,nameAVS.indexOf('(')-1);
        id = idAVS(avs);
        initTableAVP(1,id);
    }

    m_currentNumberPage = 1;
    m_countAVP = countAVP(id);
    QString tmp;
    QString text="из ";text+=tmp.setNum(m_countAVP/1000); text+=" (всего АВП ";text+=tmp.setNum(m_countAVP);text+=")";
    ui->lineEditNumberPage->setValidator( new QIntValidator(1,(m_countAVP/1000), this) );
    ui->labelCountAVP->setText(text);
    ui->pushButtonPreview->setEnabled(false);
}

//=========================================================
void MainWindow::slotNext()
{
    QString tmp;
    if(m_currentNumberPage<=(m_countAVP/1000))
    {
        ui->pushButtonPreview->setEnabled(true);
        m_currentNumberPage++;
        initTableAVP(m_currentNumberPage);
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
        initTableAVP(m_currentNumberPage);
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

        initTableAVP(m_currentNumberPage);
    }
}

//=========================================================
void MainWindow::initTableAVP(int numberPage, long idAVS )
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
              sql+=" ORDER BY avp.\"ID\" LIMIT 1000 OFFSET ";
        sql+=tmp.setNum((numberPage-1)*1000);sql+=";";

        if(query->exec(sql))
        {
            int row=0;
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

                QTableWidgetItem *newItem7 = new QTableWidgetItem();
                newItem7->setText("АВП не анализировалось");
                QIcon icon7;
                icon7.addFile(QString::fromUtf8(":/icons/icons/question.ico"), QSize(), QIcon::Normal, QIcon::Off);
                newItem7->setIcon(icon7);
                newItem7->setFlags(newItem7->flags() ^ Qt::ItemIsEditable);
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
            qDebug()<<query->lastError().text();
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
    if(dAddAVP->exec() == QDialog::Accepted)
        initTableAVP();
}

//=========================================================
void MainWindow::slotAddTask()
{
    QString sql ="";
    dAddTask->initComboBoxUser();
    dAddTask->initTableListAVP();
    if(dAddTask->exec() == QDialog::Accepted)
    {
        try
        {
            sql = "INSERT INTO \"Task\"() VALUES();";
//            sql = "INSERT INTO \"User\"(\"FIO\",\"Position\",\"ID_Group\") VALUES(\'";
//            sql +=  m_dAddUser->userName(); sql += "\',\'";
//            sql +=  m_dAddUser->range(); sql += "\',";
//            sql +=  tmp.setNum(m_dAddUser->idGroup()); sql += ");";
//            qDebug()<<"sql="<<sql;
//            if(query->exec(sql))
//                initTableUser();
//            else
//                qDebug()<<query->lastError().text();
        }
        catch(std::exception &e)
        {
            qDebug()<<e.what();
            qDebug()<<query->lastError().text();
        }
    }
}

//=========================================================
void MainWindow::slotEditTask()
{
    dEditTaskUser->exec();
}

//=========================================================
void MainWindow::slotEditTask(int, int)
{
    slotEditTask();
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
    if(QMessageBox::warning(this, tr("Внимание"),tr("Вы действительно хотите удалить выбранные задачи:?"),tr("Да"),tr("Нет")) == 0)
    {

    }
}

//=========================================================
void MainWindow::slotEditMyTask()
{
    dEditTask->exec();
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
    dViolation->initTableViolation();
    dViolation->exec();
}

//=========================================================
void MainWindow::slotClearDB()
{
    if(QMessageBox::warning(this, tr("Внимание"),tr("Вы действительно хотите очистить базу данных АВП?"),tr("Да"),tr("Нет")) == 0)
    {
        query->exec("DELETE FROM avs;");
        initTableAVP();
        initAVS();
    }
}

//=========================================================
void MainWindow::slotExit()
{
    if(QMessageBox::information(this, tr("Сообщение"),tr("Вы действительно хотите выйти из приложения?"),tr("Да"),tr("Нет"))==0)
        close();
}
