#include "cdatacollectionservice.h"

#include <QMutex>
#include <QThread>
#include <QDebug>
#include <QStringList>

#include <unistd.h>

CDataCollectionService * CDataCollectionService::p_instance = nullptr;
CDataCollectionServiceDestroyer CDataCollectionService::destroyer;

//=========================================================
CDataCollectionServiceDestroyer::~CDataCollectionServiceDestroyer()
{
    delete p_instance;
}

//=========================================================
void CDataCollectionServiceDestroyer::initialize( CDataCollectionService* p )
{
    p_instance = p;
}

//=========================================================
CDataCollectionService::CDataCollectionService(QObject *parent) : QObject(parent)
{
    initDB();
//    makeListAVP_IMDB();

    m_downloader = new CDownLoader();
    connect(m_downloader, &CDownLoader::onReady, this, &CDataCollectionService::readFile);

    m_pageParserIVI = new CPageParserIVI();
    m_pageParserIVI->setDB(db);
    m_pageParserIVI->moveToThread(&parsingPagesIVIThread);
    connect(&parsingPagesIVIThread, &QThread::finished, m_pageParserIVI, &QObject::deleteLater);
    connect(this, &CDataCollectionService::operate, m_pageParserIVI, &CPageParserIVI::doWork);
    connect(this, &CDataCollectionService::operateMegogo, m_pageParserIVI, &CPageParserIVI::doWorkMegogo);
    connect(this, &CDataCollectionService::operateOkkoTV, m_pageParserIVI, &CPageParserIVI::doWorkOkooTV);
    connect(m_pageParserIVI, &CPageParserIVI::resultReady, this, &CDataCollectionService::handleResults);
    parsingPagesIVIThread.start();

    m_requestSender = new RequestSender();
    m_cAPIdbAVP = new APIDB::CAPIdbAVP();
}

//=========================================================
CDataCollectionService::~CDataCollectionService()
{
    sendRequestThread.quit();
    sendRequestThread.terminate();
    parsingPagesIVIThread.quit();
    parsingPagesIVIThread.terminate();
    delete m_downloader;
    delete m_requestSender;
}

//=========================================================
CDataCollectionService& CDataCollectionService::getInstance()
{
    qDebug()<<"getInstance()";
    if(!p_instance)
    {
        qDebug()<<"!getInstance()";
        p_instance = new CDataCollectionService();
        destroyer.initialize( p_instance);
    }
    return *p_instance;
}

//=========================================================
void CDataCollectionService::initDB()
{
    qDebug()<<"initDB()";
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("192.168.28.96");
//    db.setHostName("127.0.0.1");
    db.setDatabaseName("avpDB");
    db.setUserName("postgres");
    db.setPassword("postgres");
    bool ok = db.open();
    if(ok)
    {
        query = new QSqlQuery(db);
        query1 = new QSqlQuery(db);
        qDebug()<<"Успешное подключения к БД АВП!";
    }
    else
    {
        qDebug()<<"Ошибка подключения к БД АВП!";
    }
}

//=========================================================
void CDataCollectionService::readFile()
{
    qDebug()<<"CDataCollectionService::readFile()";
}

//=========================================================
void CDataCollectionService::handleResults(const QString &res)
{
    qDebug()<<"Result="<<res;
}

//=========================================================
bool CDataCollectionService::changeSaveInDB(SDownloadAVP &avp)
{
    bool res =true;
    QString tmp,timestamp;
    try
    {
       timestamp = QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss");
       sql = "UPDATE \"DownloadData\" SET \"DownloadStatus\"=\'Yes\', \"PathOnDisk\"=\'"
               +currentFileName+"\', \"DownloadDate\"=\'"
               +timestamp+"\' WHERE \"ID\"="+tmp.setNum(avp.ID)+";";
//       qDebug()<<"SQL = "<<sql;
       if(query->exec(sql))
       {
           while(query->next())
           {
           }
       }
       else
           qDebug()<<query->lastError().text();
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }

    return res;
}

//=========================================================
bool CDataCollectionService::getData(const QString &url_path, const QString &fileName)
{
    bool res = false;
    currentFileName = "c:\\DownloadData\\imdb\\";
    currentFileName += fileName;
    qDebug()<<"url_path="<<url_path;
    qDebug()<<"currentFileName="<<currentFileName;

    m_request.setAddress(url_path);
    QByteArray data = m_requestSender->get( m_request );
    if(data.length()!=0)
        res = true;
    if(res)
    {
        QFile file(currentFileName);
        if(file.open(QFile::WriteOnly | QIODevice::Text))
        {
            file.write(data);
            file.close();
        }
        changeSaveInDB(m_sDownloadAVP);
    }
    return res;
}

//=========================================================
void CDataCollectionService::makeListAVP_IMDB()
{
    qDebug()<<"makeListAVP_IMDB()";
    m_listAVP_IMDB.clear();
    SDownloadAVP m_sDownloadAVP;
    try
    {
       sql = "SELECT * FROM \"DownloadData\" WHERE \"ResourceName\"=\'IMDB\';";
       qDebug()<<"SQL = "<<sql;
       if(query->exec(sql))
       {
           while(query->next())
           {
               m_sDownloadAVP.ID = query->value(0).toInt();
               m_sDownloadAVP.ResourceName = query->value(1).toString();
               m_sDownloadAVP.URL = query->value(2).toString();
               m_sDownloadAVP.DownloadStatus = query->value(3).toString();
               m_sDownloadAVP.DownloadDate = query->value(4).toString();
               m_sDownloadAVP.PathOnDisk = query->value(5).toString();
               m_sDownloadAVP.ID_AVP = query->value(6).toInt();
//               qDebug()<<"m_sDownloadAVP.ResourceName = "<<m_sDownloadAVP.ResourceName<< " URL = "<<m_sDownloadAVP.URL;
               m_listAVP_IMDB.push_back(m_sDownloadAVP);
           }
       }
       else
           qDebug()<<query->lastError().text();
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
void CDataCollectionService::testExistsFilmInDB()
{
    qDebug()<<"testExistsFilmInDB()";
    QFile file("c:\\Projects\\films.txt");
    QString sql;
    if(file.open(QFile::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"file open";
        int i=0;
        while (!file.atEnd())
        {
            i++;
            QString avpName = file.readLine();
            qDebug()<<" ";
//            m_pageParserIVI->addSaveInDB(avpName.mid(0,avpName.length()-1));
//            if(i==5)
//                break;
        }
        file.close();
    }
    else
        qDebug()<<"Не могу открыть файл";
}

//=========================================================
int CDataCollectionService::getMaxID()
{
    int maxID =0;
    try
    {
       sql = "SELECT COUNT(\"ID\") FROM \"DownloadData\";";
       qDebug()<<"SQL = "<<sql;
       if(query->exec(sql))
       {
           if(query->next())
           {
               maxID = query->value(0).toInt();
           }
       }
       else
           qDebug()<<query->lastError().text();
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
    return maxID+1;
}

//=========================================================
void CDataCollectionService::downlodPageIMDB()
{
    QString tmp;
    QString currentPathFile = "";

    int name_avp = getMaxID();
    for(auto &avp: m_listAVP_IMDB)
    {
        if(avp.DownloadStatus == "No")
        {
            m_sDownloadAVP = avp;
            currentPathFile = tmp.setNum(avp.ID);
            currentPathFile += ".txt";
            qDebug()<<"m_sDownloadAVP.ResourceName = "<<avp.ResourceName<< " URL = "<<avp.URL;
            getData(analysisURL(avp.URL)+"reviews?ref_=tt_urv",currentPathFile);
            name_avp++;
        }
//        if(name_avp==3)
//            break;
    }
}

//=========================================================
QString CDataCollectionService::analysisURL(const QString &url)
{
    QString newURL="";
    if(url[url.length()-1] == "/")
        newURL = url;
    else
        newURL = url+"/";
    return newURL;
}

//=========================================================
void CDataCollectionService::parserKinopoisk(QString fileName)
{
    qDebug()<<__PRETTY_FUNCTION__;
    QString url, title_ru, title_original, year_of_production,directors, genres;
    QString result,tmp;

    int col_Ok = 0;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"Не могу открыть файл " + fileName+"!";
        return;
    }
    else
    {
        int i=0;
        while (!file.atEnd())
        {
            QString line = file.readLine();
            QStringList list = line.split('\t');

            year_of_production = list.at(0).mid(list.at(0).lastIndexOf("=")+1,list.at(0).length()-list.at(0).lastIndexOf("=")-1);
            url = list.at(1).mid(list.at(1).lastIndexOf("=")+1,list.at(1).length()-list.at(1).lastIndexOf("=")-1);
            title_ru = list.at(2).mid(list.at(2).lastIndexOf("=")+1,list.at(2).length()-list.at(2).lastIndexOf("=")-1);
            directors = list.at(3).mid(list.at(3).lastIndexOf("=")+1,list.at(3).length()-list.at(3).lastIndexOf("=")-1);
            title_original = list.at(4).mid(list.at(4).lastIndexOf("=")+1,list.at(4).length()-list.at(4).lastIndexOf("=")-1);
            if(list.size()>=6)
                genres = list.at(5).mid(list.at(5).lastIndexOf("=")+1,list.at(5).length()-list.at(5).lastIndexOf("=")-2);

            i++;
            qDebug()<<"i="<<i;

             if(title_ru == "")
                continue;
            else
            {
                qDebug()<<"i="<<i<<"    "<<url<<"   "<<title_ru;
                m_sDataAVP.urlKinopoisk = url;
                m_sDataAVP.urlIMDB = "";
                m_sDataAVP.avsName = "КиноПоиск Онлайн";
                m_sDataAVP.avsURL = "https://www.kinopoisk.ru/watch/films/";
                m_sDataAVP.dateSaveInDB = QDateTime::currentDateTime();
                m_sDataAVP.avpURL = url;
                m_sDataAVP.avpNameRus = title_ru;
                m_sDataAVP.avpNameOriginal = title_original;
                m_sDataAVP.avpForm = genres;
                qDebug()<<"genres="<<genres;
                m_sDataAVP.age = "";
                m_sDataAVP.duration = "";
                m_sDataAVP.filmMaker = directors;
                m_sDataAVP.yearOfRelease = year_of_production;

                try
                {
                   if(addSaveInDB(m_sDataAVP))
                   {
                       col_Ok++;
                       if(i%100 == 0)
                       {
                           result = QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss");
                           result += " Загрузка данных АВП: ";
                           result +=tmp.setNum(i);
                           result +=" записей. Добавлено ";
                           result +=tmp.setNum(col_Ok);
                           result +=" записей.";
                           result += "........OK";
                           qDebug()<<result;

                       }
                   }
                   else
                   {
                       result = "Загрузка данных АВП ";
                       result +=tmp.setNum(i); result +=" ";
                       result += m_sDataAVP.avpURL; result += "........FAIL";
                       qDebug()<<result;
                   }
                }
                catch (std::exception &e)
                {
                    qDebug()<<e.what();
                }
            }
        }
        file.close();
        result="Запись данных о " + tmp.setNum(col_Ok)+" АВП завершена успешно!";
        qDebug()<<result;
    }
}

//=========================================================
void CDataCollectionService::parserIVI(QString fileName)
{
    operate(fileName); //Разбор IVI сайта
}

//=========================================================
void CDataCollectionService::parserMegogo(QString fileName)
{
    operateMegogo(fileName); //Разбор megogo сайта
}

//=========================================================
void CDataCollectionService::parserPremier(QString fileName)
{
    qDebug()<<__PRETTY_FUNCTION__;
    QString year_tmp;
    QString result,tmp;
    int col_Ok = 0;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<__PRETTY_FUNCTION__<<"1";
        QString textMsg = "Не могу открыть файл " + fileName+"!";
        return;
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"2";
        int i=0;
        while (!file.atEnd())
        {
            i++;
            qDebug()<<"i="<<i;
            m_sDataAVP.clear();
            QString line = file.readLine();
            if(i == 1)
                continue;
            QStringList list = line.split(';');

            m_sDataAVP.age = list.at(0);
            m_sDataAVP.avpForm = list.at(2);
            m_sDataAVP.yearOfRelease = list.at(1);
            m_sDataAVP.filmMaker = list.at(3);
            m_sDataAVP.avpURL = list.at(4);
            m_sDataAVP.avpSeasonNum = list.at(5);
            m_sDataAVP.avpTrackNum = list.at(6);
            m_sDataAVP.avpNameRus = list.at(7);

            m_sDataAVP.avsName = "PREMIER";
            m_sDataAVP.avsURL = "https://premier.one/";
            m_sDataAVP.dateSaveInDB = QDateTime::currentDateTime();


            qDebug()<<"title_ru="<<m_sDataAVP.avpNameRus
                   << " season_num="<< m_sDataAVP.avpSeasonNum
                   <<" track_num="<< m_sDataAVP.avpTrackNum
                   << " url="<< m_sDataAVP.avpURL
                   << " year="<< m_sDataAVP.yearOfRelease
                   << " age="<<m_sDataAVP.age
                   << " genres="<<m_sDataAVP.avpForm;
//            break;
            try
            {
               if(addSaveInDB(m_sDataAVP))
               {
                   col_Ok++;
//                       if(col_Ok%100 == 0)
                   if(i%100 == 0)
                   {
                       result = QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss");
                       result += " Загрузка данных АВП: ";
                    // result +=tmp.setNum(col_Ok);
                       result +=tmp.setNum(i);
                       result +=" записей. Добавлено ";
                       result +=tmp.setNum(col_Ok);
                       result +=" записей.";
                       result += "........OK";
                       qDebug()<<result;
                   }
               }
               else
               {
                   result = "Загрузка данных АВП ";
                   result +=tmp.setNum(i); result +=" ";
                   result += m_sDataAVP.avpURL; result += "........FAIL";
                   qDebug()<<result;
               }
            }
            catch (std::exception &e)
            {
//                qDebug()<<"exception";
                qDebug()<<e.what();
            }

//            if(i == 10)
//                break;
//            qDebug()<<"i = "<<i;
        }
        file.close();
        result="Запись данных о " + tmp.setNum(col_Ok)+" АВП завершена успешно!";
        qDebug()<<result;
    }
}

//=========================================================
void CDataCollectionService::parserOkkoTV(QString fileName)
{
//    qDebug()<<__PRETTY_FUNCTION__;
    operateOkkoTV(fileName); //Разбор megogo сайта

//    QString year_tmp;
//    QString result,tmp;
//    QStringList list;
//    int col_Ok = 0;
//    bool new_save = false;

//    QFile file(fileName);
//    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
//    {
//        QString textMsg = "Не могу открыть файл " + fileName+"!";
//        return;
//    }
//    else
//    {
//        int i=0;
//        while (!file.atEnd())
//        {
//            //            m_sDataAVP.clear();
//            QString line = file.readLine();
////            qDebug()<<"line"<<line;
//            if(line.lastIndexOf("Url")!=-1)
//            {
//                i++;
//                qDebug()<<"i ="<<i;
//                m_sDataAVP.clear();
//                new_save = true;
//                list = line.split(':');
//                m_sDataAVP.avpURL = list.at(1)+list.at(2);
//                m_sDataAVP.avpURL = m_sDataAVP.avpURL.mid(1,m_sDataAVP.avpURL.length()-2);
////                qDebug()<<m_sDataAVP.avpURL;
//            }
//            else if(line.lastIndexOf("Фильм")!=-1)
//            {
//                list = line.split(':');
//                m_sDataAVP.avpNameRus = list.at(1);
//                m_sDataAVP.avpNameRus = m_sDataAVP.avpNameRus.mid(1,m_sDataAVP.avpNameRus.length()-2);
//                if(m_sDataAVP.avpNameRus[0] == 0x00ab)
//                {
////                    qDebug()<<"жопа";
//                    m_sDataAVP.avpNameRus = m_sDataAVP.avpNameRus.mid(1,m_sDataAVP.avpNameRus.lastIndexOf(0x00bb)-1);

//                }
//                qDebug()<<m_sDataAVP.avpNameRus;
//            }
//            else if(line.lastIndexOf("год")!=-1)
//            {
//                list = line.split(':');
//                m_sDataAVP.yearOfRelease = list.at(1);
//                m_sDataAVP.yearOfRelease =m_sDataAVP.yearOfRelease.mid(0,m_sDataAVP.yearOfRelease.length()-1);
//            }
//            else if(line.lastIndexOf("жанр")!=-1)
//            {
//                list = line.split(':');
//                m_sDataAVP.avpForm = list.at(1);
//                m_sDataAVP.avpForm =m_sDataAVP.avpForm.mid(0,m_sDataAVP.avpForm.length()-1);
//            }
//            else if(line.lastIndexOf("режиссёр")!=-1)
//            {
//                list = line.split(':');
//                m_sDataAVP.filmMaker = list.at(1);
//                m_sDataAVP.filmMaker =m_sDataAVP.filmMaker.mid(0,m_sDataAVP.filmMaker.length()-1);
//            }
//            else if(line.lastIndexOf("Возрастное")!=-1)
//            {
//                list = line.split(':');
//                m_sDataAVP.age = list.at(1);
//                m_sDataAVP.age = m_sDataAVP.age.mid(1,3);
//            }
//            else if((line!="") && (line.lastIndexOf("В ролях") == -1))
//            {
//               m_sDataAVP.country = line.mid(0, line.length()-1);
//               list = m_sDataAVP.country.split(0x00b7);
//               m_sDataAVP.country = list.at(0);
//               m_sDataAVP.country = m_sDataAVP.country.mid(0,m_sDataAVP.country.length()-1);
//               if(list.size()>1)
//               {
//                    m_sDataAVP.duration = list.at(1);
//                    m_sDataAVP.duration = m_sDataAVP.duration.mid(1,m_sDataAVP.duration.length()-1);
//                    list = m_sDataAVP.duration.split(' ');
//                    m_sDataAVP.duration = list.at(0);
//               }
//            }

//            if(line.lastIndexOf("В ролях") == -1)
//                continue;

//            m_sDataAVP.avsName = "OKKO";
//            m_sDataAVP.avsURL = "https://okko.tv";
//            m_sDataAVP.dateSaveInDB = QDateTime::currentDateTime();


//            qDebug()<<"title_ru="<<m_sDataAVP.avpNameRus
////                   << " season_num="<< m_sDataAVP.avpSeasonNum
////                   <<" track_num="<< m_sDataAVP.avpTrackNum
//                   << " url="<< m_sDataAVP.avpURL
//                   << " year="<< m_sDataAVP.yearOfRelease
//                   << " age="<<m_sDataAVP.age
//                   << " filmMaker="<<m_sDataAVP.filmMaker
//                   << " country="<<m_sDataAVP.country
//                   << " duration="<<m_sDataAVP.duration
//                   << " genres="<<m_sDataAVP.avpForm;
//            try
//            {
//               if(addSaveInDB(m_sDataAVP))
//               {
//                   col_Ok++;
////                       if(col_Ok%100 == 0)
//                   if(i%100 == 0)
//                   {
//                       result = QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss");
//                       result += " Загрузка данных АВП: ";
//                    // result +=tmp.setNum(col_Ok);
//                       result +=tmp.setNum(i);
//                       result +=" записей. Добавлено ";
//                       result +=tmp.setNum(col_Ok);
//                       result +=" записей.";
//                       result += "........OK";
//                       qDebug()<<result;
//                   }
//               }
//               else
//               {
//                   result = "Загрузка данных АВП ";
//                   result +=tmp.setNum(i); result +=" ";
//                   result += m_sDataAVP.avpURL; result += "........FAIL";
//                   qDebug()<<result;
//               }
//            }
//            catch (std::exception &e)
//            {
//                qDebug()<<e.what();
//            }

////            if(i == 20)
////                break;
////            qDebug()<<"i = "<<i;
//        }
//        file.close();
//        result="Запись данных о " + tmp.setNum(col_Ok)+" АВП завершена успешно!";
//        qDebug()<<result;
//    }
}

//=========================================================
void CDataCollectionService::parserMoreTV(QString fileName)
{
    qDebug()<<__PRETTY_FUNCTION__;
    QString year_tmp;
    QString result,tmp;
    int col_Ok = 0;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString textMsg = "Не могу открыть файл " + fileName+"!";
        return;
    }
    else
    {
        int i=0;
        while (!file.atEnd())
        {
            i++;
            qDebug()<<"i="<<i;
            m_sDataAVP.clear();
            QString line = file.readLine();
//            qDebug()<<"line="<<line;
            if(i == 1)
                continue;
            QStringList list = line.split('\t');
//            qDebug()<<"list.size()="<<list.size();

            m_sDataAVP.avpNameRus = list.at(0).mid(list.at(0).lastIndexOf("=")+1,list.at(0).length()-list.at(0).lastIndexOf("=")-1);
            m_sDataAVP.avpSeasonNum = list.at(1).mid(list.at(1).lastIndexOf("=")+1,list.at(1).length()-list.at(1).lastIndexOf("=")-1);
            m_sDataAVP.avpTrackNum = list.at(2).mid(list.at(2).lastIndexOf("=")+1,list.at(2).length()-list.at(2).lastIndexOf("=")-1);
            m_sDataAVP.avpURL = "https://more.tv" + list.at(3).mid(list.at(3).lastIndexOf("=")+1,list.at(3).length()-list.at(3).lastIndexOf("=")-1);
            year_tmp = list.at(5).mid(list.at(5).lastIndexOf("=")+1,list.at(5).length()-list.at(5).lastIndexOf("=")-1);
            QStringList list_year = year_tmp.split('-');
            m_sDataAVP.yearOfRelease = list_year.at(0);
            m_sDataAVP.age = list.at(6).mid(list.at(6).lastIndexOf("=")+1,list.at(6).length()-list.at(6).lastIndexOf("=")-1) + "+";
            m_sDataAVP.avpForm = list.at(7).mid(list.at(7).lastIndexOf("=")+1,list.at(7).length()-list.at(7).lastIndexOf("=")-2);

            m_sDataAVP.avsName = "more.tv";
            m_sDataAVP.avsURL = "https://more.tv";
            m_sDataAVP.dateSaveInDB = QDateTime::currentDateTime();


            qDebug()<<"title_ru="<<m_sDataAVP.avpNameRus
                   << " season_num="<< m_sDataAVP.avpSeasonNum
                   <<" track_num="<< m_sDataAVP.avpTrackNum
                   << " url="<< m_sDataAVP.avpURL
                   << " year="<< m_sDataAVP.yearOfRelease
                   << " age="<<m_sDataAVP.age
                   << " genres="<<m_sDataAVP.avpForm;
            try
            {
               if(addSaveInDB(m_sDataAVP))
               {
                   col_Ok++;
//                       if(col_Ok%100 == 0)
                   if(i%100 == 0)
                   {
                       result = QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss");
                       result += " Загрузка данных АВП: ";
                    // result +=tmp.setNum(col_Ok);
                       result +=tmp.setNum(i);
                       result +=" записей. Добавлено ";
                       result +=tmp.setNum(col_Ok);
                       result +=" записей.";
                       result += "........OK";
                       qDebug()<<result;
                   }
               }
               else
               {
                   result = "Загрузка данных АВП ";
                   result +=tmp.setNum(i); result +=" ";
                   result += m_sDataAVP.avpURL; result += "........FAIL";
                   qDebug()<<result;
               }
            }
            catch (std::exception &e)
            {
                qDebug()<<e.what();
            }

//            if(i == 10)
//                break;
//            qDebug()<<"i = "<<i;
        }
        file.close();
        result="Запись данных о " + tmp.setNum(col_Ok)+" АВП завершена успешно!";
        qDebug()<<result;
    }
}

//=========================================================
void CDataCollectionService::parserAppleTV(QString fileName)
{
        qDebug()<<"\nfileName="<<fileName;
        m_nameRus = "";
        m_nameOriginal = "";
        m_URL = "";
        m_yearOfRelease = "";
        m_rubric = "";
        m_country = "";
        m_age = "";
        m_duration = "";
        m_filmMaker = "";
        m_sDataAVP.clear();

        QFile file(fileName);

        QString s_findNameRus = "<span data-test-info-title>";
        QString s_findURL = "<link rel=\"canonical\" href=";
        QString s_findAge = "<dd data-test-rating class=\"product-footer__metadata__section__desc typ-caption clr-secondary-text\">";
        QString s_findRubric = "<dd data-test-genre class=\"product-footer__metadata__section__desc typ-caption clr-secondary-text\">";
        QString s_findDuration = "<dd data-test-duration class=\"product-footer__metadata__section__desc typ-caption clr-secondary-text\">";
        QString s_findYearOfRelease = "<dd data-test-releasedate class=\"product-footer__metadata__section__desc typ-caption clr-secondary-text\">";
        QString s_findCountry = "<span class=\"locale-switcher__footer__country-name\" data-test-country-name>";
        QString s_findFilmMaker = "<meta property=\"video:director\" content=";

        if(file.open(QFile::ReadOnly | QIODevice::Text))
        {
            while (!file.atEnd())
            {
                QString line = file.readLine();
                if(line.indexOf(s_findURL) != -1) // URL
                {
                    int index = line.indexOf(s_findURL);
                    if(line.indexOf("id=") != -1)
                    {
                        m_URL = line.mid(index+s_findURL.length()+1,line.indexOf("id=")-s_findURL.length()-index-3);
                        qDebug()<<"URL ="<<m_URL;
                    }
                }
                else if(line.indexOf(s_findAge) != -1) // Age
                {
                    int index = line.indexOf(s_findAge);
                    if((line.indexOf("</dd>") != -1) && (m_URL != ""))
                    {
                        m_age = line.mid(index+s_findAge.length(),line.indexOf("</dd>")-s_findAge.length()-index);
                        qDebug()<<"m_age ="<<m_age;
                    }
                }
                else if(line.indexOf(s_findNameRus) != -1) // NameRus
                {
                    int index = line.indexOf(s_findNameRus);
                    m_nameRus = line.mid(index+s_findNameRus.length(),line.indexOf("</span>")-s_findNameRus.length()-index);
                    qDebug()<<"m_nameRus ="<<m_nameRus;
                }
                else if(line.indexOf(s_findRubric) != -1) // Rubric
                {
                    int index = line.indexOf(s_findRubric);
                    if((line.indexOf("</dd>") != -1))
                    {
                        m_rubric = line.mid(index+s_findRubric.length(),line.indexOf("</dd>")-s_findRubric.length()-index);
                        qDebug()<<"m_rubric ="<<m_rubric;
                    }
                }
                else if(line.indexOf(s_findYearOfRelease) != -1) // YearOfRelease
                {
                    int index = line.indexOf(s_findYearOfRelease);
                    if((line.indexOf("</dd>") != -1))
                    {
                        m_yearOfRelease = line.mid(index+s_findYearOfRelease.length(),line.indexOf("</dd>")-s_findYearOfRelease.length()-index);
                        qDebug()<<"m_yearOfRelease ="<<m_yearOfRelease;
                    }
                }
                else if(line.indexOf(s_findDuration) != -1) // Duration
                {
                    int index = line.indexOf(s_findDuration);
                    if((line.indexOf("</dd>") != -1))
                    {
                        m_duration = line.mid(index+s_findDuration.length(),line.indexOf("</dd>")-s_findDuration.length()-index);
                        qDebug()<<"m_duration ="<<m_duration;
                    }
                }
                else if(line.indexOf(s_findFilmMaker) != -1) // filmMaker
                {
                    int index = line.indexOf(s_findFilmMaker);
                    if((line.indexOf("id=") != -1))
                    {
                        m_filmMaker = line.mid(index+s_findFilmMaker.length()+1,line.indexOf("id=")-s_findFilmMaker.length()-index-3);
                    }
                    qDebug()<<"m_filmMaker ="<<m_filmMaker;
                }

            }
            file.close();
    //        col_file++;
    //        if( col_file == 10)
    //            exit(0);
        }
        else
            qDebug()<<"Не могу открыть файл:"<<fileName;

        m_sDataAVP.avpNameRus = m_nameRus;
        m_sDataAVP.avpURL = m_URL;
        m_sDataAVP.yearOfRelease = m_yearOfRelease;
        m_sDataAVP.age = m_age;
        m_sDataAVP.avpForm = m_rubric;
        m_sDataAVP.filmMaker = m_filmMaker;
        m_sDataAVP.duration = m_duration;

        m_sDataAVP.avsName = "AppleTV";
        m_sDataAVP.avsURL = "https://tv.apple.com";
        m_sDataAVP.dateSaveInDB = QDateTime::currentDateTime();

        if((m_URL !="") && (m_nameRus != "") && (m_yearOfRelease !=""))
        {
            qDebug()<<"addSaveInDB";
            addSaveInDB(m_sDataAVP);
            m_nameRus = "";
            m_nameOriginal = "";
            m_URL = "";
            m_yearOfRelease = "";
            m_rubric = "";
            m_country = "";
            m_age = "";
            m_duration = "";
            m_filmMaker = "";
        }
}

//=========================================================
bool CDataCollectionService::existsSaveInDb(const QString &url)
{
    bool res = false;
    QString sql;
    try
    {
        sql = "SELECT * FROM avp WHERE \"URL\"=\'";
        sql += url; sql += "\';";
        //        qDebug()<<"sql="<<sql;
        if(query->exec(sql))
        {
            if(query->next())
                res = true;
        }
        else
            qDebug()<<query->lastError().text();
    }
    catch(std::exception &e)
    {
         qDebug()<<e.what();
    }
    return res;
}

//=========================================================
QString CDataCollectionService::findIdAVS(QString url, QString name)
{
    QString id_avs="";
    QString sql="";
    sql = "SELECT \"ID\" FROM avs WHERE \"URL\"=\'"+url+"\' AND \"NameAVS\"=\'"+name+"\';";
//    qDebug()<<"sql="<<sql;
    if(!query->exec(sql))
    {
        qDebug()<<query->lastError().text();
        id_avs = "0";
    }
    else
    {
        if(query->next())
        {
            id_avs = query->value(0).toString();
        }
    }
    qDebug()<<"id_avs="<<id_avs;
    return id_avs;
}

//=========================================================
bool CDataCollectionService::addSaveInDB(SDataAVP &sDataAVP)
{
    bool res=true;
    QString sql="";
    QString tmp;
    try
    {
        if(existsSaveInDb(sDataAVP.avpURL))
        {
           return false;
        }
//        sql ="DELETE FROM avp WHERE \"URL\" =\'";
//        sql +=sDataAVP.avpURL;
//        sql+="\';";
//        qDebug()<<sql;
//        query->exec(sql);

        sql="INSERT INTO avp (\"NameRus\",\"URL\",\"ID_AVS\",\"NameOriginal\",\"URL_kinopoisk\",\"URL_imdb\",\"Season_num\",\"Track_num\") VALUES (E\'";
        sql += decode(sDataAVP.avpNameRus);sql+="\',\'";
        sql += sDataAVP.avpURL; sql+="\',";
        sql += findIdAVS(sDataAVP.avsURL,sDataAVP.avsName);
//        sql += "151";
        sql+=",E\'"; sql += decode(sDataAVP.avpNameOriginal); sql+="\',\'";
        sql += sDataAVP.urlKinopoisk;
        sql+="\',\'";
        sql += sDataAVP.urlIMDB;
        sql+="\',E\'";
        sql += decode(sDataAVP.avpSeasonNum);
        sql+="\',E\'";
        sql += decode(sDataAVP.avpTrackNum);
        sql+="\');";
        qDebug()<<sql;

        if(!query->exec(sql))
        {
            qDebug()<<sql;
            qDebug()<<query->lastError().text();
            res = false;
        }
        else
        {
            res = true;
            sql = "INSERT INTO \"AVPattribute\" (\"DateSaveInDB\",\"Rubric\",\"FilmMaker\",\"YearOfRelease\",\"Age\",\"Duration\",\"ID_AVP\",\"ID_User\",\"Country\") VALUES ('";
            sql += tmp.setNum(QDateTime::currentDateTime().date().year());sql += "-";
            sql += tmp.setNum(QDateTime::currentDateTime().date().month());sql += "-";
            sql += tmp.setNum(QDateTime::currentDateTime().date().day());
            sql+="\',\'";
            sql += sDataAVP.avpForm;sql+="\',\'";
            sql += sDataAVP.filmMaker;
            sql+="\',\'";
            sql += sDataAVP.yearOfRelease;sql+="\',\'";
            sql += sDataAVP.age;sql+="\',\'";
            sql += sDataAVP.duration;sql+="\',";
            sql += findIdAVP(sDataAVP.avpURL) ;sql+=",";
            sql += findIdUser("Левдик А.А."); sql+=",\'";
            sql += sDataAVP.country;
            sql+="\');";
            qDebug()<<sql;

            if(!query->exec(sql.toStdString().c_str()))
            {
                qDebug()<<sql;
                qDebug()<<query->lastError().text();
                res = false;
            }
            else
            {
//                colAVP++;
//                qDebug()<<"Save in DB OK AVP: "<<sDataAVP.avpNameRus<<"   colAVP="<<colAVP<<"\n";
                qDebug()<<"Save in DB OK AVP: "<<sDataAVP.avpNameRus<<"\n";
                res = true;
            }
        }
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
    return res;
}

//=========================================================
QString CDataCollectionService::findIdAVP(QString &url)
{
    QString id_avp="";
    QString sql="";
    sql = "SELECT \"ID\" FROM avp WHERE \"URL\"=\'"+url+"\';";
    if(!query->exec(sql))
    {
        qDebug()<<query->lastError().text();
        id_avp = "0";
    }
    else
    {
        if(query->next())
        {
            id_avp = query->value(0).toString();
        }
    }
    return id_avp;
}

//=========================================================
QString CDataCollectionService::findIdUser(QString fio)
{
    QString id_user="";
    QString sql="";
    sql = "SELECT \"ID\" FROM \"User\" WHERE \"FIO\"=\'"+fio+"\';";
    if(!query->exec(sql))
    {
        qDebug()<<query->lastError().text();
        id_user = "0";
    }
    else
    {
        if(query->next())
        {
            id_user = query->value(0).toString();
        }
    }
    return id_user;
}

//=========================================================
//void CDataCollectionService::makeListURL_AppleTV(QString fileName)
//{
//    qDebug()<<__PRETTY_FUNCTION__;
//    qDebug()<<fileName;
//    QFile file(fileName);
//    QFile file_new("D:\\Program\\ap\\list_url.txt");
////    if (!file_new.open(QIODevice::WriteOnly | QIODevice::Text))
//    if (!file_new.open(QIODevice::Append | QIODevice::Text))
//    {
//        QString textMsg = "Не могу открыть файл D:\\Program\\ap1\\list_url.txt!";
//        return;
//    }
//    QTextStream out(&file_new);

//    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
//    {
//        QString textMsg = "Не могу открыть файл " + fileName+"!";
//        return;
//    }
//    else
//    {
//        qDebug()<<__PRETTY_FUNCTION__<<"_1";
//        int i=0;
//        int col=0;
//        while (!file.atEnd())
//        {
//            i++;
//            QString line = file.readLine();

//            if(line.indexOf("\"ru-ru\"") != -1)
//            {
//                if(line.indexOf("href") != -1)
//                {
//                    out << line.mid(line.indexOf("href=")+6, line.length()-line.indexOf("href=")-11)+"\n";
////                    out<< line  ;
//                    col++;
//                }
//            }
////            qDebug()<<"i = "<<i;
//        }
//        qDebug()<<"col = "<<col;
//        file.close();
//        file_new.close();
//    }
//}

//=========================================================
void CDataCollectionService::makeNewFileForKinopoisk(QString fileName)
{
    qDebug()<<__PRETTY_FUNCTION__;
    QString year_of_production;
    QFile file(fileName);
    QFile file_new("D:\\Program\\Kinopoisk\\kp_content.tskv\\kp_content_new.tskv");
    if (!file_new.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QString textMsg = "Не могу открыть файл D:\\Program\\Kinopoisk\\kp_content.tskv\\kp_content_new.tskv!";
        return;
    }
    QTextStream out(&file_new);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString textMsg = "Не могу открыть файл " + fileName+"!";
        return;
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"_1";
        int i=0;
        int col=0;
        while (!file.atEnd())
        {
            i++;
            QString line = file.readLine();
            QStringList list = line.split('\t');

            year_of_production = list.at(0).mid(list.at(0).lastIndexOf("=")+1,list.at(0).length()-list.at(0).lastIndexOf("=")-1);
            //            qDebug()<<"year_of_production = "<<year_of_production;

            if(year_of_production == "2020")
            {
                qDebug()<<"year_of_production = "<<year_of_production;
                out<< line;
                col++;
            }
//            qDebug()<<"i = "<<i;
        }
        qDebug()<<"col = "<<col;
        file.close();
        file_new.close();
    }
}

//=========================================================
void CDataCollectionService::countValidFilmsInFileAchiveKinopoisk(QString fileName)
{
    QString url, title_ru, title_original;
    QFile file("C:\\Users\\leaa\\Documents\\kp_content");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString textMsg = "Не могу открыть файл " + fileName+"!";
        return;
    }
    else
    {
        int i=0;
        int col=0;
        while (!file.atEnd())
        {
            i++;
            QString line = file.readLine();
            QStringList list = line.split('\t');

            url = list.at(0).mid(list.at(0).lastIndexOf("=")+1,list.at(0).length()-list.at(0).lastIndexOf("=")-1);
            title_ru = list.at(1).mid(list.at(1).lastIndexOf("=")+1,list.at(1).length()-list.at(1).lastIndexOf("=")-1);
            title_original = list.at(2).mid(list.at(2).lastIndexOf("=")+1,list.at(2).length()-list.at(2).lastIndexOf("=")-1);
            if(title_ru == "")
            {
                col++;
            }
            qDebug()<<"i = "<<i;
        }
        qDebug()<<"col = "<<col;
    }
}

//=========================================================
void CDataCollectionService::importDataIVI_InFile()
{
    QFile file("C:\\Users\\leaa\\Documents\\films_ivi.txt");
    QString sql;
    try
    {
        if(file.open(QFile::WriteOnly | QIODevice::Text))
        {
            qDebug()<<"file open";
            QTextStream out(&file);
            sql="SELECT * FROM ivi;";
            if(query->exec(sql))
            {
                while(query->next())
                {
                    out<<query->value(1).toString().toUtf8()<<"\n";
                }
                file.close();
                qDebug()<<"file close";
            }
            else
                qDebug()<<query->lastError().text();

        }
        else
            qDebug()<<"Не могу открыть файл";
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
QString CDataCollectionService::decode(QString str)
{
    QString newString="";
    for(int i=0; i<str.length();++i)
    {
        if((str[i]=="'") || (str[i]=="\"") || (str[i]=="\\"))
        {
            newString += "\\";
            newString += str[i];
        }
        else
        {
            newString += str[i];
        }
    }
    return newString;
}

//=========================================================
void CDataCollectionService::remakeColumnYearOfRelease()
{
    qDebug()<<__PRETTY_FUNCTION__;
    QString sql, yearOfRelease, tmp;
    bool ok;
    try
    {
        sql="SELECT aa.\"ID\", aa.\"YearOfRelease\" FROM \"AVPattribute\" aa WHERE aa.\"YearOfRelease\"!=\'\';";
        if(query->exec(sql))
        {
            int i = 0;
            while(query->next())
            {
                yearOfRelease = query->value(1).toString();
                yearOfRelease = yearOfRelease.mid(0,4);
                yearOfRelease.toInt(&ok);
                if(ok == false)
                {
                    i++;
                    qDebug()<<"ID="<<tmp.setNum(query->value(0).toLongLong())<<" yearOfRelease = "<<yearOfRelease<< "i = "<<i;
                    sql = "UPDATE \"AVPattribute\" SET \"YearOfRelease\"=\'\' WHERE \"ID\"=";
                    sql += tmp.setNum(query->value(0).toLongLong()); sql += ";";
                    qDebug()<<"sql = "<<sql;
                    if(query1->exec(sql))
                        qDebug()<<query->lastError().text();
                }
            }
        }
        else
            qDebug()<<query->lastError().text();
        qDebug()<<"End function = "<<sql;
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
void CDataCollectionService::makeRefIMBD_ForAVP()
{
    qDebug()<<__PRETTY_FUNCTION__;
    QString sql,tmp;
    QString nameRus, nameOriginal, yearOfRelease;
    uint64_t avpID;
    QString urlIMDB;

    try
    {
        sql="SELECT avp.\"NameRus\", avp.\"NameOriginal\", aa.\"YearOfRelease\",avp.\"ID\" FROM avp "
            "INNER JOIN \"AVPattribute\" aa ON avp.\"ID\" = aa.\"ID_AVP\" "
            "WHERE avp.\"URL_imdb\"=\'\' AND aa.\"YearOfRelease\"!=\'\' AND avp.\"NameOriginal\" !=\'\' ORDER BY avp.\"ID\";";
//        "WHERE avp.\"URL_imdb\"=\'\' AND aa.\"YearOfRelease\"!=\'\' AND avp.\"NameOriginal\" !=\'\' ORDER BY avp.\"ID\" LIMIT 1000;";
//        qDebug()<<"SQL = "<<sql;
        if(query->exec(sql))
        {
            int i = 0;
            while(query->next())
            {
                nameRus = query->value(0).toString();
                nameOriginal = query->value(1).toString();
                yearOfRelease = query->value(2).toString();
                avpID = query->value(3).toLongLong();
                yearOfRelease = yearOfRelease.mid(0,4);
//                qDebug()<<"nameRus = "<<nameRus;
//                qDebug()<<"nameOriginal = "<<nameOriginal;
//                qDebug()<<"yearOfRelease = "<<yearOfRelease;

                sql = "SELECT \"imdbBasics\".tconst, \"imdbBasics\".\"originalTitle\",\"imdbAkas\".title FROM \"imdbBasics\" "
                      "LEFT JOIN \"imdbAkas\" ON \"imdbBasics\".tconst = \"imdbAkas\".\"titleId\" "
                      "WHERE (\"imdbBasics\".\"originalTitle\" =E\'";
                sql += decode(nameOriginal); sql += "\' OR \"imdbAkas\".title = E\'";
                sql += decode(nameRus); sql += "\') AND \"imdbBasics\".\"startYear\" = \'";
                sql += yearOfRelease; sql +="\';";
//                qDebug()<<"SQL = "<<sql;
                if(query1->exec(sql))
                {
                    if(query1->next())
                    {
                        qDebug()<<"imdbBasics.tconst = "<< query1->value(0).toString();
                        qDebug()<<"imdbBasics.originalTitle = "<< query1->value(1).toString();
                        qDebug()<<"imdbAkas.title = "<< query1->value(2).toString();
                        qDebug()<<"yearOfRelease = "<<yearOfRelease;
                        urlIMDB = "https://www.imdb.com/title/";
                        urlIMDB += query1->value(0).toString();
                        urlIMDB += "/reviews?ref_=tt_urv";
                        qDebug()<<"urlIMDB = "<< urlIMDB;
                        sql = "UPDATE avp SET \"URL_imdb\" = \'";
                        sql += urlIMDB; sql += "\' WHERE \"ID\" = ";
                        sql += tmp.setNum(avpID); sql += ";";
                        qDebug()<<"SQL = "<<sql;
                        if(!query1->exec(sql))
                            qDebug()<<query1->lastError().text();
                        i++;
//                        qDebug()<<"i ============ "<< i <<"\n";
                    }
                }
                else
                    qDebug()<<query1->lastError().text();
//                if(i==1)
//                    break;
            }
        }
        else
            qDebug()<<query->lastError().text();
        qDebug()<<"End function makeRefIMBD_ForAVP()";
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
void CDataCollectionService::makeDB_IMBD_AKAS()
{
    qDebug()<<__PRETTY_FUNCTION__;
    QFile file("D:\\Program\\IMBD\\title.akas.tsv\\data.txt");
    QString sql,tmp;
    QString titleId, ordering, title, region, language, types, attributes, isOriginalTitle;
    QString line;
    QStringList list;

    try
    {
        if(file.open(QFile::ReadOnly | QIODevice::Text))
        {

            int i=0;
            int col=0;
            while (!file.atEnd())
            {
                i++;
                line = file.readLine();
                if( i <= 1 )
                    continue;
                list = line.split('\t');

                titleId = list.at(0);
                ordering = list.at(1);
                title = list.at(2);
                region = list.at(3);
                language = list.at(4);
                types = list.at(5);
                attributes = list.at(6);
                isOriginalTitle = list.at(7);

//                qDebug()<<"titleID = "<<titleId;
//                qDebug()<<"ordering = "<<ordering;
//                qDebug()<<"title = "<<title;
//                qDebug()<<"region = "<<region;
//                qDebug()<<"language = "<<language;
//                qDebug()<<"types = "<<types;
//                qDebug()<<"attributes = "<<attributes;
//                qDebug()<<"isOriginalTitle = "<<isOriginalTitle;
//                qDebug()<<"";

                if(( region == "RU" ) && (types != "alternative"))
                {
                    sql = "INSERT INTO \"imdbAkas\"(\"titleId\",\"ordering\",\"title\",\"types\",\"isOriginalTitle\") VALUES(\'";
                    sql += titleId; sql += "\',";
                    sql += ordering; sql += ",E\'";
                    sql += decode(title); sql += "\',E\'";
                    sql += decode(types); sql += "\',\'";
                    if(isOriginalTitle == "\\N\n")
                        sql += "0";
                    else
                        sql += isOriginalTitle;
                    sql += "\');";

//                    qDebug()<<sql;
                    if(!query->exec(sql))
                    {
                       qDebug()<<query->lastError().text();
                       qDebug()<<sql;
                       qDebug()<<"isOriginalTitle = "<<isOriginalTitle;
//                       break;
                    }
                    col++;
                    if( col%1000 == 0 )
                    {
                        qDebug()<<"col = "<<col<<" i = "<<i;
                    }
                }
            }
            file.close();
        }
        else
            qDebug()<<"Не могу открыть файл";
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
void CDataCollectionService::makeDB_IMBD()
{
    qDebug()<<__PRETTY_FUNCTION__;
    QFile file("D:\\Program\\IMBD\\title.basics.tsv\\data.txt");
    QString sql,path,tmp;
    QString tconst, tvMovie, primaryTitle,originalTitle, isAdult, startYear, endYear, runtimeMinutes, genres;
    QString line;
    QStringList list;

    try
    {
        if(file.open(QFile::ReadOnly | QIODevice::Text))
        {

            int i=0;
            while (!file.atEnd())
            {
                i++;
                line = file.readLine();
                if( i <= 402494 )
                    continue;
                if( i%1000 == 0 )
                {
                    qDebug()<<"i = "<<i;
                }
                list = line.split('\t');
                tconst = list.at(0);
                tvMovie = list.at(1);
                primaryTitle = list.at(2);
                originalTitle = list.at(3);
                isAdult = list.at(4);
                startYear = list.at(5);
                endYear = list.at(6);
                runtimeMinutes = list.at(7);
                genres = list.at(8);

//                qDebug()<<"tconst = "<<tconst;
//                qDebug()<<"tvMovie = "<<tvMovie;
//                qDebug()<<"primaryTitle = "<<primaryTitle;
//                qDebug()<<"originalTitle = "<<originalTitle;
//                qDebug()<<"isAdult = "<<isAdult;
//                qDebug()<<"startYear = "<<startYear;
//                qDebug()<<"endYear = "<<endYear;
//                qDebug()<<"runtimeMinutes = "<<runtimeMinutes;
//                qDebug()<<"genres = "<<genres;
                //                qDebug()<<"";

                sql = "INSERT INTO \"imdbBasics\"(tconst,\"tvMovie\",\"primaryTitle\",\"originalTitle\",\"isAdult\",\"startYear\",\"endYear\",\"runtimeMinutes\",genres) VALUES(\'";
                sql += tconst; sql += "\',\'";
                sql += tvMovie; sql += "\',E\'";
                sql += decode(primaryTitle); sql += "\',E\'";
                sql += decode(originalTitle); sql += "\',\'";
                sql += isAdult; sql += "\',\'";
                sql += startYear; sql += "',\'";
                sql += endYear; sql += "',";
                if(runtimeMinutes == "\\N")
                    sql += "-1";
                else
                    sql += runtimeMinutes;
                sql += ",\'";
                sql += genres; sql += "\');";

                if(!query->exec(sql))
                {
                   qDebug()<<query->lastError().text();
                   qDebug()<<sql;
//                   break;
                }
//                usleep(5);
            }
            file.close();
        }
        else
            qDebug()<<"Не могу открыть файл";
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
void CDataCollectionService::renamePathView()
{
    qDebug()<<"START rename path on view";
    QString sql,path,tmp;
    try
    {
        int i=0;
        sql="SELECT * FROM \"DownloadData\" WHERE \"ResourceName\" = \'IMDB\' AND \"PathOnDisk\"!=\'\';";
        if(query->exec(sql))
        {
            while(query->next())
            {
                path = "/mnt/Data/DownloadData/imdb/";
                tmp = query->value(5).toString();
                qDebug()<<"\nOld path = "<<tmp;
                auto parts = tmp.split('\\');
                qDebug()<<"parts.at(parts.size()) = "<<parts.at(parts.size()-1);
                tmp = parts.at(parts.size()-1);
                tmp = tmp.mid(0,tmp.indexOf('.'));
                tmp +=".html";
                path += tmp;
                qDebug()<<"ID = "<<query->value(0).toInt();
                qDebug()<<"New path = "<<path;

                sql ="UPDATE \"DownloadData\" SET \"PathOnDisk\"=\'"+path+"\' WHERE \"ID\"="+tmp.setNum(query->value(0).toInt())+";";
                qDebug()<<"SQL = "<<sql;
                if(query1->exec(sql))
                {
                    i++;
                }
                else
                    qDebug()<<query1->lastError().text();
//                i++;
//                if(i == 10)
//                    break;
            }
        }
        else
            qDebug()<<query->lastError().text();
        qDebug()<<"Change save ="<<i;
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
void CDataCollectionService::remakeOkko()
{
    QString sql,path,tmp;
    try
    {
        int i=0;
        sql="SELECT \"ID\",\"URL\" FROM avp WHERE \"ID_AVS\" = 152;";
        if(query->exec(sql))
        {
            while(query->next())
            {
                tmp = query->value(1).toString();
                if(tmp.indexOf(":") == -1)
                {
                    qDebug()<<"tmp"<<tmp;
                    path = tmp.mid(0,5)+":"+tmp.mid(5,tmp.length()-5);
                    qDebug()<<"path"<<path;
//                    break;
                }
                sql ="UPDATE avp SET \"URL\"=\'"+path+"\' WHERE \"ID\"="+tmp.setNum(query->value(0).toInt())+";";
                qDebug()<<"SQL = "<<sql;
                if(query1->exec(sql))
                {
                    i++;
                }
                else
                    qDebug()<<query1->lastError().text();
//                if(i==10)
//                    break;
            }
        }
        else
            qDebug()<<query->lastError().text();
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
void CDataCollectionService::run()
{
    qDebug()<<"CDataCollectionService::run getpid()="<<getpid();
    remakeOkko();
//    parserKinopoisk("D:\\Program\\POKAZ\\kinopoisk.tskv");
//    parserMoreTV("D:\\Program\\POKAZ\\MoreTV.txt");
//    parserPremier("D:\\Program\\POKAZ\\Premier.csv");
//    parserOkkoTV("D:\\Program\\POKAZ\\OkkoTV.txt");
//    parserMegogo("D:\\Program\\POKAZ\\megogo.ru\\ru\\view");
//    parserIVI("D:\\Program\\POKAZ\\www.ivi.ru");


    //=====================================IMDB make DB
//    makeDB_IMBD();
//    makeDB_IMBD_AKAS();
//    makeRefIMBD_ForAVP();
//    remakeColumnYearOfRelease();
    //=====================================IMDB end make DB


//    renamePathView();
//    downlodPageIMDB();

//===Test functions===
//    operate("c:\\DownloadData\\ivi\\www.ivi.ru"); //Разбор IVI сайта
//      operate("d:\\DownloadData\\ivi\\www.ivi.ru"); //Разбор IVI сайта
//    testExistsFilmInDB();
//    importDataIVI_InFile();
//    countValidFilmsInFileAchiveKinopoisk("C:\\Users\\leaa\\Documents\\kp_content");

//        makeNewFileForKinopoisk("D:\\Program\\Kinopoisk\\kp_content.tskv\\kp_content.tskv"); //сделать файл для показа


    //======================================AppleTV
//    QString pathFile;
//    QString tmp;
//    for(int i=0; i<=99;i++)
//    {
//        pathFile = "D:\\Program\\ap\\ap\\";
//        pathFile += tmp.setNum(i);
//делает файл со списком URL на страницы переведенные на русский в AppleTV
//        makeListURL_AppleTV(pathFile);
//    };

//    QDir dir( "Z:/DownloadData/AppleTV" );
//    if ( !dir.exists() )
//    {
//        qDebug()<<"No such directory"<<dir.dirName();
//    }
//    else
//    {
//        QStringList fileList = dir.entryList( QDir::Files );
//        for (int i = 0; i < fileList.size(); ++i)
//            parserAppleTV("Z:/DownloadData/AppleTV/"+fileList.at(i));
//    }

//    parserAppleTV("Z:/DownloadData/AppleTV/umc.cmc.1a312xxue8d8qbdatkvhv0mwc");

    //======================================AppleTV END Test

//====END test

//    m_cAPIdbAVP->setDB(db);
//    QList<APIDB::SAVPattribute> listPath = m_cAPIdbAVP->getAttributeAVP("Пятница");
//    for(auto &item: listPath)
//    {
//        qDebug()<<item.id<<" "<<item.urlAVP<<" "<<item.filmMaker<<" "<<item.yearOfRelease;
//    }


}

