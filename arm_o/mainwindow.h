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
     * @brief initAVS - функция заполнения "Выберите АВС"
     */
    void initAVS();
    /**
     * @brief initViolations - функция заполнения нарушений
     * @param id_avp - идентификатор АВП
     * @return - указатель на объект ячейки в таблице
     */
    QTableWidgetItem* initViolations(long id_avp);
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
     * @brief slotNext
     */
    void slotNext();
    /**
     * @brief slotPrevious
     */
    void slotPrevious();
    /**
     * @brief slotChangeNumberPage
     */
    void slotChangeNumberPage();
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
     * @brief slotFindAVP
     */
    void slotFindAVP();
    /**
     * @brief slotTextChanged
     */
    void slotTextChanged(const QString&);
    /**
     * @brief slotFilterApply
     */
    void slotFilterApply();
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
    void slotContextMenuRequestedCurrentTask(const QPoint &pos);

    void slotChangePassword();
    void slotMakeReport();

    void slotJournalSession();
    void slotJournalJob();

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
    void operate(const QString &);
    void operateKinopoisk(const QString &);

private:
    Ui::MainWindow *ui;
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

    Client *m_client;

    RequestSender *m_requestSender;
    Request m_request;
    SDownloadAVP m_sDownloadAVP;

    int progress;
    int stepProgress;
    int m_countAVP;
    int m_currentNumberPage;
    int m_countCurrentAVP;
    int m_currentIdAVS;
    int m_currentState;

    QTimer m_timer;

    void initDialog();
    int countAVP(long idAVS = -1);
    int countAVP_Analysis(int state,long idAVS);
    long idAVS(QString nameAVS);
    long getIdAVP(QString nameAVP);
    int getIdUser(QString userName);
    int getIdTaskStatus(QString taskStatus);
    int getIdPriority(QString namePriority);

//    void initComboBoxUser(QComboBox *comboBox);
    void initComboBoxStatus(QComboBox *comboBox);
    void initComboBoxPriority(QComboBox *comboBox);

    // Метод инициализации запроса на получение данных
    bool getData(const QString &url_path, const QString &fileName);
    bool changeSaveInDB(SDownloadAVP &avp, QString &currentFileName);
    long getIdDownloadData(long idAVP);

    QString findPathReview(QString url);

    bool initNetClient();
    void addComment(uint64_t idTask, QString comment);
    void setColorRowTable(QTableWidget *table, int row, int r, int g, int b);

public:
    QString sendCommandAnalysisAVP(uint64_t idAVP);
    void initComboBoxUser(QComboBox *comboBox);
};
#endif // MAINWINDOW_H
