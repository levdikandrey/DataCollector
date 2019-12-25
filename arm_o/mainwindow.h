#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QSqlQuery>

#include "cimportdata.h"
#include "cimportdatakinopoisk.h"
#include "dimportdataoutexcel.h"
#include "daddavp.h"
#include "daddtask.h"
#include "dedittask.h"
#include "dedittaskuser.h"
#include "dgroup.h"
#include "duser.h"
#include "dviolation.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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
    void initTableAVP();
    /**
     * @brief initTableTask - функция заполнения таблицы "Текущие задачи"
     */
    void initTableTask();
    /**
     * @brief initAVS - функция заполнения "Выберите АВС"
     */
    void initAVS();

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
    void slotAddTask();
    void slotEditTask();
    void slotDeleteTask();
    void slotEditMyTask();
    void slotReload();
    void slotSettingsDB();
    void slotClearDB();
    void slotGroup();
    void slotUser();
    void slotViolation();

public slots:
    /**
     * @brief handleResults
     */
    void handleResults(const QString &);
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
    std::map<int,QString>listAVS;
    DAddAVP *dAddAVP;
    DAddTask *dAddTask;
    DEditTask *dEditTask;
    DEditTaskUser *dEditTaskUser;
    DGroup *dGroup;
    DUser *dUser;
    DViolation *dViolation;
    int progress;
    int stepProgress;

    void initDialog();
};
#endif // MAINWINDOW_H
