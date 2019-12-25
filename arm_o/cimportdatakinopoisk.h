#ifndef CIMPORTDATAKINOPOISK_H
#define CIMPORTDATAKINOPOISK_H

#include <QObject>
#include <QDateTime>
#include <QSqlQuery>

#include "cimportdata.h"

/**
 * @brief The CImportDataKinopoisk class - класс потока. Реализует функции разбора и записи данных в БД АВП их файла от kinopoisk.ru.
 */
class CImportDataKinopoisk: public QObject
{
    Q_OBJECT
public:
    /**
     * @brief CImportDataKinopoisk- конструктор класса
     * @param parent - указатель на родителя
     */
    CImportDataKinopoisk(QObject *parent = nullptr);
    /**
     * @brief ~CImportDataKinopoisk - деструктор класса
     */
    ~CImportDataKinopoisk();
    /**
     * @brief m_sDataAVP - объект структуры данных АВП
     */
    SDataAVP m_sDataAVP;

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
    QSqlQuery *query;
    CImportData *m_cImportData;
};

#endif // CIMPORTDATAKINOPOISK_H
