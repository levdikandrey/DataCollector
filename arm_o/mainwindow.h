#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QSqlQuery>
#include <QTableWidget>
#include <QMutex>
#include <QTimer>
#include <QElapsedTimer>
#include <QComboBox>

#include "cimportdata.h"
#include "cimportdatakinopoisk.h"
#include "dimportdataoutexcel.h"
#include "daddavp.h"
#include "deditavp.h"
#include "daddtask.h"
#include "dedittask.h"
#include "dedittaskuser.h"
#include "deditaudit.h"
#include "dgroup.h"
#include "duser.h"
#include "dviolation.h"
#include "denter.h"
#include "requestsender.h"
#include "dsettingsdb.h"
#include "dchangepassword.h"
#include "djournalsession.h"
#include "djournaljobavp.h"
#include "dreportjob.h"
#include "dreportallstatistics.h"
#include "dappointexpert.h"
#include "dstartprogressdialog.h"
#include "dpreviewarchive.h"
#include "dmanualconnecttestdb.h"

#include "client.h"
#include "aprotocol.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
using namespace Network;

struct SDownloadAVP
{
    int ID;
    QString ResourceName;
    QString URL;
    QString DownloadStatus;
    QString DownloadDate;
    QString PathOnDisk;
    int ID_AVP;
};

/**
*  Класс главного окна программы
*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief MainWindow - конструктор класса
     * @param parent - указатель на родителя
     */
    MainWindow(QWidget *parent = nullptr);
    /**
      Деструктор класса
     */
    ~MainWindow();
    /**
     * @brief initDB - функция установки соединения с базой данных
     */
    bool initDB();
    /**
     * @brief initTableAVP - функция заполнения таблицы "Список АВП"
     */
    void initTableAVP(int numberPage = 1, long idAVS = -1, int state = -1);
    /**
     * @brief initTableTask - функция заполнения таблицы "Текущие задачи"
     */
    void initTableTask(bool filter = false);
    /**
     * @brief initTableMyTask - функция заполнения таблицы "Мои задачи"
     */
    void initTableMyTask();
    /**
     * @brief initTableMyAudit - функция заполнения таблицы "Экспертиза"
     */
    void initTableAudit();
    /**
     * @brief initTableMyAudit - функция заполнения таблицы "Текущие экспертизы"
     */
    void initTableCurrentAudit();
    /**
     * @brief initTableAuditRKN - функция заполнения таблицы "Инспектор РКН"
     */
    void initTableAuditRKN();
    /**
     * @brief initTableArchive - функция заполнения таблицы "Архив"
     */
    void initTableArchive(int numberPage = 1);
    /**
     * @brief initAVS - функция заполнения "Выберите АВС"
     */
    void initAVS();
    /**
     * @brief initViolations - функция заполнения нарушений
     * @param id_avp - идентификатор АВП
     * @return - указатель на объект ячейки в таблице
     */
    QTableWidgetItem* initViolations(long id_avp, int violationType);
    /**
     * @brief initDialogAccess - функция разграничения доступа пользователя к функциям программы.
     * @param userName - имя пользователя.
     */
    void initDialogAccess(QString userName);
    void changeStatusAVP(uint64_t idAVP, uint8_t status);

    CImportData *cImportData;

private slots:
    /**
     * @brief slotImportData - функция импорта данных из Excel
     */
    void slotImportData();
    /**
     * @brief slotImportKinopoisk - функция импорта данных от kinopoiska.ru
     */
    void slotImportKinopoisk();
    /**
     * @brief slotAbout - функция вызывает диалог "О программе"
     */
    void slotAbout();
    /**
     * @brief slotExit - функция выхода из приложения
     */
    void slotExit();
    /**
     * @brief slotAddAVP - добавляет новый АВП в таблицу и в БД
     */
    void slotAddAVP();
    /**
     * @brief slotEditAVP - редактировать в таблице и в БД.
     */
    void slotEditAVP();
    /**
     * @brief slotEditAVP - редактировать в таблице и в БД (при двойном клике на строке записи в таблице).
     */
    void slotEditAVP(int, int);
    /**
     * @brief slotDeleteAVP - удаляет один или несколько АВП из таблицы и БД
     */
    void slotDeleteAVP();
    /**
     * @brief slotAddTask - функция добавляет новые текущие задачи
     */
    void slotAddTask();
    /**
     * @brief slotEditTask - функция вызывает диалог для редактирования параметров "Текущие задачи" в БД АВП
     */
    void slotEditTask();
    /**
     * @brief slotEdit - функция вызывает диалог для редактирования параметров "Текущие задачи" в БД АВП (при двойном клике на строке записи в таблице)
     */
    void slotEditTask(int, int);
    /**
     * @brief slotEdit - функция вызывает диалог для редактирования параметров "Мои задачи" в БД АВП (при двойном клике на строке записи в таблице)
     */
    void slotEditMyTask(int, int);
    /**
     * @brief slotEditAudit - функция вызывает диалог для редактирования параметров "Экспертиза" в БД АВП (при двойном клике на строке записи в таблице)
     */
    void slotEditAudit(int, int);
    /**
     * @brief slotEditAuditRKN - функция вызывает диалог для редактирования параметров "Инспектор РКН" в БД АВП (при двойном клике на строке записи в таблице)
     */
    void slotEditAuditRKN(int, int);
    /**
     * @brief slotEditAuditRKN - функция вызывает диалог для редактирования параметров "Текущие экспертизы" в БД АВП (при двойном клике на строке записи в таблице)
     */
    void slotEditCurrentAudit(int, int);
    /**
     * @brief slotPreviewAchive - функция вызывает диалог для просмотра параметров АВП "Архив" в БД АВП (при двойном клике на строке записи в таблице)
     */
    void slotPreviewAchive(int, int);
    /**
     * @brief slotDeleteTask - удаляет одину или несколько задач из таблицы и БД
     */
    void slotDeleteTask();
    /**
     * @brief slotEditMyTask - функция вызывает диалог для редактирования параметров "Мои задачи" в БД АВП
     */
    void slotEditMyTask();
    /**
     * @brief slotEditAudit - функция вызывает диалог для редактирования параметров "Экспертиза" в БД АВП
     */
    void slotEditAudit();
    /**
     * @brief slotEditAudit - функция вызывает диалог для редактирования параметров "Текущие экспертизы" в БД АВП
     */
    void slotEditCurrentAudit();
    /**
     * @brief slotEditAuditRKN - функция вызывает диалог для редактирования параметров "Инспектор РКН" в БД АВП
     */
    void slotEditAuditRKN();
    /**
     * @brief slotReload - функция перечитывает данные об АВП из БД
     */
    void slotReload();
    /**
     * @brief slotSettingsDB - функция настройки параметров подключения к БД
     */
    void slotSettingsDB();
    /**
     * @brief slotClearDB
     */
    void slotClearDB();
    /**
     * @brief slotGroup
     */
    void slotGroup();
    /**
     * @brief slotUser
     */
    void slotUser();
    /**
     * @brief slotViolation
     */
    void slotViolation();
    /**
     * @brief slotNext - функция отображает следующую страницу с АВП в закладке "Список АВП"
     */
    void slotNext();
    /**
     * @brief slotNextArchivePage - функция отображает следующую страницу с АВП в закладке "Архив"
     */
    void slotNextArchivePage();
    /**
     * @brief slotPrevious - функция отображает предыдущую страницу с АВП в закладке "Список АВП"
     */
    void slotPrevious();
    /**
     * @brief slotPreviousArchivePage - функция отображает предыдущую страницу с АВП в закладке "Архив"
     */
    void slotPreviousArchivePage();
    /**
     * @brief slotChangeNumberPage - функция отображает страницу с АВП в закладке "Список АВП" по номеру  страницы
     */
    void slotChangeNumberPage();
    /**
     * @brief slotChangeNumberPageArchive - функция отображает страницу с АВП в закладке "Архив" по номеру  страницы
     */
    void slotChangeNumberPageArchive();
    /**
     * @brief slotSelectAVS - функция выбора АВС
     * @param avs
     */
    void slotSelectAVS(QString);
    /**
     * @brief slotStateChanged
     */
    void slotStateChanged(int);
    /**
     * @brief slotRBViolation
     */
    void slotRBViolation(bool);
    /**
     * @brief slotRBChecked
     */
    void slotRBChecked(bool);
    /**
     * @brief slotRBAll
     */
    void slotRBAll(bool);
    /**
     * @brief slotFindAVP - функция ищет АВП по названию
     */
    void slotFindAVP();
    void slotFindAVP2();
    void slotFindAVP3();
    void slotFindAVP4();
    void slotFindAVP5();
    void slotFindAVP6();
    void slotFindAVP7();
    /**
     * @brief slotFindArchiveAVP - функция ищет АВП по названию в архиве
     */
    void slotFindArchiveAVP();
    /**
     * @brief slotTextChanged
     */
    void slotTextChanged(const QString&);
    /**
     * @brief slotTextChanged1
     */
    void slotTextChanged1(const QString&);
    void slotTextChanged2(const QString&);
    void slotTextChanged3(const QString&);
    void slotTextChanged4(const QString&);
    void slotTextChanged5(const QString&);
    void slotTextChanged6(const QString&);
    void slotTextChanged7(const QString&);
    /**
     * @brief slotFilterApply - функция применяет фильтры в закладке "Текущие задачи"
     */
    void slotFilterApply();
    /**
     * @brief slotFilterApply1 - функция применяет фильтры в закладке "Текущие экспертизы"
     */
    void slotFilterApply1();
    /**
     * @brief slotFilterApply2 - функция применяет фильтры в закладке "Архив"
     */
    void slotFilterApply2();
    /**
     * @brief slotFilterApply3 - функция применяет фильтры в закладке "Инспектр РКН"
     */
    void slotFilterApply3();
    /**
     * @brief slotFilterApplyAVP - функция применяет фильтры в закладке "Список АВП"
     */
    void slotFilterApplyAVP();
    /**
     * @brief slotFilterApplyMyTask - функция применяет фильтры в закладке "Мои задачи"
     */
    void slotFilterApplyMyTask();
    /**
     * @brief slotFilterApplyExpert - функция применяет фильтры в закладке "Экспертиза"
     */
    void slotFilterApplyExpert();
    /**
     * @brief slotCurrentChanged
     */
    void slotCurrentChanged(int);
    /**
     * @brief slotChangeUser
     */
    void slotChangeUser();
    /**
     * @brief slotAnalysisAVP - функция для анализа выбранных АВП
     */
    void slotAnalysisAVP();
    /**
     * @brief slotGoToURL
     */
    void slotGoToURL();
    /**
     * @brief slotCurrentTaskChageStatus
     */
    void slotCurrentTaskChageStatus();
    /**
     * @brief slotStatus
     */
    void slotStatus();
    /**
     * @brief slotContextMenuRequested - функция создания контекстного меню
     * @param pos
     */
    void slotContextMenuRequested(const QPoint &pos);
    void slotContextMenuRequestedAVP(const QPoint &pos);
    void slotContextMenuRequestedExpert(const QPoint &pos);
    void slotContextMenuRequestedExpert_2(const QPoint &pos);
    void slotContextMenuRequestedExpert_4(const QPoint &pos);
    void slotContextMenuRequestedCurrentTask(const QPoint &pos);

    void slotChangePassword();
    void slotMakeReport();
    void slotMakeReportAll();

    void slotJournalSession();
    void slotJournalJob();
    void slotInitDialog();
    void slotAppointExpert();

    void slotClearFilter();
    void slotClearFilter2();
    void slotClearFilter3();
    void slotClearFilter4();
    void slotClearFilter5();
    void slotClearFilter6();
    void slotClearFilter7();

    void slotManualConnectToDB();

public slots:
    /**
     * @brief handleResults
     */
    void handleResults(const QString &);
    /**
     * @brief handleResultsKinopoisk
     */
    void handleResultsKinopoisk(const QString &);

signals:
    /**
     * @brief operate
     */
    void operateStartProgressDialog(const QString &);
    void operate(const QString &);
    void operateKinopoisk(const QString &);

private:
    Ui::MainWindow *ui;
    DStartProgressDialog *dStartProgressDialog;
    QThread startProgressDialogThread;

    CImportDataKinopoisk  *cImportDataKinopoisk;
    QThread importDataThread;
    QThread importDataThreadKinopoisk;
    DImportDataOutExcel *dImportDataOutExcel;
    QSqlQuery *query;
    QSqlQuery *queryViolation;
    std::map<int,QString>listAVS;
    DAddAVP *dAddAVP;
    DAddTask *dAddTask;
    DEditTask *dEditTask;
    DEditTaskUser *dEditTaskUser;
    DGroup *dGroup;
    DUser *dUser;
    DViolation *dViolation;
    DEnter *dEnter1;
    DEditAudit *dEditAudit;
    DSettingsDB *dSettingsDB;
    DChangePassword *dChangePassword;
    DEditAVP *dEditAVP;
    DJournalSession *dJournalSession;
    DJournalJobAVP *dJournalJobAVP;
    DReportJob *dReportJob;
    DReportAllStatistics *dReportAllStatistics;
    DAppointExpert *dAppointExpert;
    DPreviewArchive *dPreviewArchive;
    DManualConnectTestDB *dManualConnectTestDB;

    Client *m_client;

    RequestSender *m_requestSender;
    Request m_request;
    SDownloadAVP m_sDownloadAVP;

    int progress;
    int stepProgress;
    int m_countAVP;
    int m_countArchiveAVP;
    int m_currentNumberPage;
    int m_currentArchiveNumberPage;
    int m_countCurrentAVP;
    int m_currentIdAVS;
    int m_currentState;

    QTimer m_timer;
    QTimer m_timer1;
    QTimer m_timer2;
    QTimer m_timer3;
    QTimer m_timer4;
    QTimer m_timer5;
    QTimer m_timer6;
    QTimer m_timer7;
    QTimer m_timerStartInitDialog;

    void initDialog();
    int countAVP(long idAVS = -1);
    int countArchiveAVP();
    int countAVP_Analysis(int state,long idAVS);
    long idAVS(QString nameAVS);
    long getIdAVP(QString nameAVP);
    int getIdUser(QString userName);
    int getIdTaskStatus(QString taskStatus);
    int getIdPriority(QString namePriority);
    int getIdAttribute(QString textAttribute);


//    void initComboBoxUser(QComboBox *comboBox);
//    void initComboBoxStatus(QComboBox *comboBox);
//    void initComboBoxPriority(QComboBox *comboBox);

    // Метод инициализации запроса на получение данных
    bool getData(const QString &url_path, const QString &fileName);
    bool changeSaveInDB(SDownloadAVP &avp, QString &currentFileName);
    long getIdDownloadData(long idAVP);

    QString findPathReview(QString url);

    bool initNetClient();
    void addComment(uint64_t idTask, QString comment);
    void setColorRowTable(QTableWidget *table, int row, int r, int g, int b);

    void closeEvent(QCloseEvent *event);
    void saveInJournalRecordClose();

public:
    QString sendCommandAnalysisAVP(uint64_t idAVP);
    void initComboBoxUser(QComboBox *comboBox);
    void initComboBoxExpert(QComboBox *comboBox);
    void initComboBoxOperator(QComboBox *comboBox);
    void initComboBoxStatus(QComboBox *comboBox);
    void initComboBoxAttribute(QComboBox *comboBox);
    void initComboBoxAge(QComboBox *comboBox);
    void initComboBoxViolation(QComboBox *comboBox);
    void initComboBoxStatusArchive(QComboBox *comboBox);

    /**
     * @brief addRecordJournalJobAVP
     * @param category
     * @param info
     */
    void addRecordJournalJobAVP(int category, QString info, QString nameAVP);
    QString getNameRusAVP(uint64_t idAVP);
    const QTableWidget* getTableCurrentExpert() const;
};
#endif // MAINWINDOW_H
