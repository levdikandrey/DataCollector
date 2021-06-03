#ifndef CPAGEPARSERIVI_H
#define CPAGEPARSERIVI_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QDebug>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

/**
 * @brief The SDataAVP struct - структура для хранения данных об АВП
 */
/*struct SDataAVP
{
   QString avsName;
   QString avsURL;
   QString avpNameRus;
   QString avpSeasonNum;
   QString avpTrackNum;
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
   QString country;
   void clear()
   {
       avsName = "";
       avsURL = "";
       avpNameRus = "";
       avpSeasonNum = "";
       avpTrackNum = "";
       avpNameOriginal = "";
       avpURL = "";
       rubric = "";
       filmMaker = "";
       yearOfRelease = "";
       age = "";
       duration = "";
       userSaveInDB = "";
       urlKinopoisk = "";
       urlIMDB = "";
       avpForm = "";
       country = "";
   }
};*/
/**
 * @brief The CPageParserIVI class - класс поток для разбора страниц ivi.ru и megogo.ru
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
    bool addSaveInDB();
    bool updateSaveInDB();

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
    void doWorkMegogo(const QString &dirName);
    void doWorkOkooTV(const QString &dirName);

private:
    /**
     * @brief parsePage - функция разбора страницы на наличие записи об АВП
     * @param page - имя страницы
     */
    void parsePageMegogo(const QString &page);
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
    bool existsSaveInDb(const QString &url);
    /**
     * @brief parsePageExt - функция разбора страницы на наличие дополнительных параметров об АВП
     * @param fileName - имя страницы
     */
    void parsePageExt(const QString &fileName);
    /**
     * @brief parsePageExt1 - функция разбора страницы на наличие дополнительных параметров об АВП
     * @param fileName - имя страницы
     */
    void parsePageExt1(const QString &fileName);
    /**
     * @brief existsFilmMakerInSaveDb
     * @param url - URL АВП
     * @return  - true - если существует запись
     */
    bool existsFilmMakerInSaveDb(const QString &url);
    QString decode(QString str);

    QString findIdAVS(QString url, QString name);
    QString findIdAVP(QString &url);
    QString findIdUser(QString fio);

    QSqlDatabase m_db;
    QSqlQuery *m_query;
    int colAVP;

    QString m_nameRus;
    QString m_nameOriginal;
    QString m_URL;
    QString m_yearOfRelease;
    QString m_rubric;
    QString m_country;
    QString m_age;
    QString m_duration;
    QString m_filmMaker;

    int m_nameAVS;
};

#endif // CPAGEPARSERIVI_H
