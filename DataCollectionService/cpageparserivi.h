#ifndef CPAGEPARSERIVI_H
#define CPAGEPARSERIVI_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QDebug>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

/**
 * @brief The CPageParserIVI class - класс поток для разбора страниц ivi.ru
 */
class CPageParserIVI: public QObject
{
    Q_OBJECT

public:
    explicit CPageParserIVI(QObject *parent = nullptr);
    void setDB(const QSqlDatabase &db);
    /**
     * @brief addSaveInDB - функция добавления записи в БД АВП
     * @param avpName - название АВП
     */
    void addSaveInDB(const QString &avpName);

signals:
    /**
     * @brief resultReady - сигнал о выполнениии работы
     * @param result - результат выполнения
     */
    void resultReady(const QString &result);

public slots:
    /**
     * @brief doWork - функция выполнения потока
     * @param dirName - имя директории
     */
    void doWork(const QString &dirName);

private:
    /**
     * @brief getDirFiles - функция возвращает список всех файлов в директории
     * @param dirName - имя директории
     * @return - список файлов
     */
    QStringList getDirFiles(const QString &dirName);
    /**
     * @brief parsePage - функция разбора страницы на наличие записи об АВП
     * @param page - имя страницы
     */
    void parsePage(const QString &page);
    /**
     * @brief existsSaveInDb - функция проверяет существование, такой записи в БД АВП
     * @param avpName - название АВП
     * @return - true - если существует запись
     */
    bool existsSaveInDb(const QString &avpName);

    QSqlDatabase m_db;
    QSqlQuery *m_query;
    int colAVP;
};

#endif // CPAGEPARSERIVI_H
