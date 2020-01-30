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
#include "daddtask.h"
#include "dedittask.h"
#include "dedittaskuser.h"
#include "deditaudit.h"
#include "dgroup.h"
#include "duser.h"
#include "dviolation.h"
#include "denter.h"
#include "requestsender.h"

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
    void initDB();
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
     * @brief initTableMyAudit
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
     * @brief slotEditAudit
     */
    void slotEditAudit(int, int);
    /**
     * @brief slotDeleteTask - удаляет одину или несколько задач из таблицы и БД
     */
    void slotDeleteTask();
    /**
     * @brief slotEditMyTask
     */
    void slotEditMyTask();
    /**
     * @brief slotEditAudit
     */
    void slotEditAudit();
    /**
     * @brief slotReload
     */
    void slotReload();
    /**
     * @brief slotSettingsDB
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
     * @brief slotSelectAVS
     * @param avs
     */
    void slotSelectAVS(QString);
    /**
     * @brief slotStateChanged
     */
    void slotStateChanged(int);
    void slotRBViolation(bool);
    void slotRBChecked(bool);
    void slotRBAll(bool);
    void slotFindAVP();
    void slotTextChanged(const QString&);
    void slotFilterApply();
    void slotCurrentChanged(int);
    void slotChangeUser();
    void slotAnalysisAVP();
    void slotGoToURL();
    void slotStatus();
    void slotContextMenuRequested(const QPoint &pos);

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
    CImportData *cImportData;
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
    long idAVS(QString nameAVS);
    long getIdAVP(QString nameAVP);
    int getIdUser(QString userName);
    int getIdTaskStatus(QString taskStatus);
    int getIdPriority(QString namePriority);

    void initComboBoxUser(QComboBox *comboBox);
    void initComboBoxStatus(QComboBox *comboBox);
    void initComboBoxPriority(QComboBox *comboBox);

    // Метод инициализации запроса на получение данных
    bool getData(const QString &url_path, const QString &fileName);
    bool changeSaveInDB(SDownloadAVP &avp, QString &currentFileName);
};
#endif // MAINWINDOW_H
