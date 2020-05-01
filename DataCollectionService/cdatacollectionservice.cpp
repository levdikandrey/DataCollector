#include "cdatacollectionservice.h"

#include <QMutex>
#include <QThread>
#include <QDebug>

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
                        qDebug()<<"i ============ "<< i <<"\n";
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
void CDataCollectionService::run()
{
    qDebug()<<"CDataCollectionService::run getpid()="<<getpid();
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
      operate("d:\\DownloadData\\ivi\\www.ivi.ru"); //Разбор IVI сайта
//    testExistsFilmInDB();
//    importDataIVI_InFile();
//    countValidFilmsInFileAchiveKinopoisk("C:\\Users\\leaa\\Documents\\kp_content");
//====END test

//    m_cAPIdbAVP->setDB(db);
//    QList<APIDB::SAVPattribute> listPath = m_cAPIdbAVP->getAttributeAVP("Пятница");
//    for(auto &item: listPath)
//    {
//        qDebug()<<item.id<<" "<<item.urlAVP<<" "<<item.filmMaker<<" "<<item.yearOfRelease;
//    }


}

