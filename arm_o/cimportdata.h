#ifndef CIMPORTDATA_H
#define CIMPORTDATA_H

#include <QObject>
#include <QDateTime>
#include <QSqlQuery>

#include "xlsxdocument.h"

/**
 * @brief The SDataAVP struct - структура для хранения данных об АВП
 */
struct SDataAVP
{
   QString avsName;
   QString avsURL;
   QString avpNameRus;
   QString avpNameOriginal;
   QString avpURL;
   QString rubric;
   QString filmMaker;
   QString yearOfRelease;
   QDateTime dateSaveInDB;
   QString age;
   QString duration;
   QString userSaveInDB;
   QString urlKinopoisk;
   QString urlIMDB;
   QString avpForm;
};

/**
 * @brief The CImportData class - класс потока. Реализует функции разбора и записи данных в БД АВП их excel-файла.
 */
class CImportData : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief CImportData - конструктор класса
     * @param parent - указатель на родителя
     */
    explicit CImportData(QObject *parent = nullptr);
    /**
     * @brief ~CImportData - деструктор класса
     */
    ~CImportData();
    /**
     * @brief decode
     * @return
     */
    QString decode(QString );
    /**
     * @brief m_sDataAVP - объект структуры данных АВП
     */
    SDataAVP m_sDataAVP;
    bool addSaveInDB(SDataAVP &sDataAVP);

signals:
    /**
     * @brief resultReady - функция для возврата значения в вызывающий поток
     * @param result - возращаемое значение
     */
    void resultReady(const QString &result);

public slots:
    /**
     * @brief doWork - функция выполнения потока
     */
    void doWork(const QString &);

private:
    /**
     * @brief xlsx - объект для хранения разобранных данных из файла excel
     */
    QXlsx::Document xlsx;

    bool existAVS(QString &url,QString name = "");
    bool existAVP(QString &url);
    bool existUser(QString &fio);

    bool addAVP(SDataAVP &sDataAVP);
    bool addUser(QString &fio);

    QString findIdAVS(QString &url,QString name = "");
    QString findIdAVP(QString &url);
    QString findIdUser(QString &fio);

    QSqlQuery *query;
};

#endif // CIMPORTDATA_H
