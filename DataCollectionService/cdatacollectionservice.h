#ifndef CDATACOLLECTIONSERVICE_H
#define CDATACOLLECTIONSERVICE_H

#include <QObject>
#include <QThread>
#include <QFile>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <deque>

#include "cdownloader.h"
#include "requestsender.h"
#include "cpageparserivi.h"
#include "capidbavp.h"


class CDataCollectionService;
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

//=========================================================
class CDataCollectionServiceDestroyer
{
private:
    CDataCollectionService* p_instance;
public:
    ~CDataCollectionServiceDestroyer();
    void initialize( CDataCollectionService* p );
};

//=========================================================
class CDataCollectionService: public QObject
{
    Q_OBJECT

private:
    static CDataCollectionService* p_instance;
    static CDataCollectionServiceDestroyer destroyer;

    CDownLoader *m_downloader;
    RequestSender *m_requestSender;
    CPageParserIVI *m_pageParserIVI;
    APIDB::CAPIdbAVP *m_cAPIdbAVP;

    Request m_request;

    QThread sendRequestThread;
    QThread parsingPagesIVIThread;

    QString currentFileName;

    std::deque<SDownloadAVP> m_listAVP_IMDB;
    SDownloadAVP m_sDownloadAVP;
    QString sql;
    QSqlDatabase db;
    QSqlQuery *query;
    QSqlQuery *query1;
    /**
     * @brief initDB - функция установки соединения с базой данных
     */
    void initDB();
    void makeListAVP_IMDB();
    bool changeSaveInDB(SDownloadAVP &avp);
    void downlodPageIMDB();
    int getMaxID();
    QString analysisURL(const QString &url);
    QString decode(QString );

    //================ utils for test
    void testExistsFilmInDB();
    void importDataIVI_InFile();
    void countValidFilmsInFileAchiveKinopoisk(QString fileName);
    void renamePathView();
    void makeDB_IMBD();
    void makeDB_IMBD_AKAS();
    void makeRefIMBD_ForAVP();
    void remakeColumnYearOfRelease();
    //=============== end list test utils

private slots:
    void readFile();

public slots:
    /**
     * @brief handleResults - результат работы потока разбора страниц ivi.ru
     */
    void handleResults(const QString &);

signals:
     void operate(const QString &);

protected:
    CDataCollectionService(QObject *parent = nullptr);
    CDataCollectionService( const CDataCollectionService& );
    CDataCollectionService& operator=( CDataCollectionService& );
    ~CDataCollectionService();
    friend class CDataCollectionServiceDestroyer;

public:
     static CDataCollectionService& getInstance();
    // Метод инициализации запроса на получение данных
     bool getData(const QString &url, const QString &fileName);
     void run();

};

#endif // CDATACOLLECTIONSERVICE_H
