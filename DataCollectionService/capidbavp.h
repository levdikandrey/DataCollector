#ifndef CAPIDBAVP_H
#define CAPIDBAVP_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QList>

namespace APIDB
{
    /**
    * @brief The SListFilePath struct - структура для хранения путей к файлам на диске
    */
    struct SListFilePath
    {
      QString path;
      QString url;
      QString yearOfRelease;
      QString filmMaker;
    };

    /**
     * @brief The SAVPattribute struct - структура для хранения аттрибутов АВС
     */
    struct SAVPattribute
    {
        long id;
        QString nameAVS;
        QString urlAVP;
        QString rubric;
        QString filmMaker;
        QString yearOfRelease;
        QString nameRus;
        QString nameOriginal;
        QString resourceName;
        QString downloadStatus;
        QString age;
        QString duration;
        QString dateSaveInDB;
        QString downloadDate;
        QString pathOnDisk;
    };

    /**
    * @brief The CAPIdbAVP class - класс реализации программного интерфеса для работы с БД АВП
    */
    class CAPIdbAVP: public QObject
    {
        Q_OBJECT

    public:
        /**
        * @brief CAPIdbAVP - конструктор класса
        * @param parent - указатель на родителя
        */
        explicit CAPIdbAVP(QObject *parent = nullptr);
        /**
        * @brief setDB - функция установки соединения с БД
        * @param db - объект для работы с БД АВП
        */
        void setDB(const QSqlDatabase &db);
        /**
        * @brief getFilePath - функция возвращает путь к файлу на диске
        * @param nameAVP - назавание АВП
        * @return - путь к файлу
        */
        QList<SListFilePath> getFilePath(QString nameAVP);
        /**
        * @brief getAttributeAVP
        * @param nameAVP
        * @return
         */
        QList<SAVPattribute> getAttributeAVP(QString nameAVP);

    private:
        QSqlDatabase m_db;
        QSqlQuery *m_query;
    };
}

#endif // CAPIDBAVP_H
