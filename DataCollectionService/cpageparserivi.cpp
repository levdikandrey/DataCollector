#include "cpageparserivi.h"
#include <QDateTime>

//=========================================================
CPageParserIVI::CPageParserIVI(QObject *parent): QObject(parent)
{
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

//=========================================================
void CPageParserIVI::setDB(const QSqlDatabase &db)
{
    m_db = db;
    m_query = new QSqlQuery(db);
}

//=========================================================
void CPageParserIVI::doWork(const QString &startDir)
{
    colAVP = 0;
    QStringList allFiles = getDirFiles( startDir );
    for (int i = 0; i < allFiles.size(); ++i)
        parsePage(allFiles.at(i));
    qDebug()<<"colAVP="<<colAVP;
}

//=========================================================
bool CPageParserIVI::existsSaveInDb(const QString &url)
{
    bool res = false;
    QString sql;
    try
    {
        sql = "SELECT * FROM avp WHERE \"URL\"=\'";
        sql += url; sql += "\';";
//        qDebug()<<"sql="<<sql;
        if(m_query->exec(sql))
        {
            if(m_query->next())
                res = true;
        }
        else
            qDebug()<<m_query->lastError().text();
    }
    catch(std::exception &e)
    {
         qDebug()<<e.what();
    }
    return res;
}

//=========================================================
QString CPageParserIVI::decode(QString str)
{
    QString newString="";
    for(int i=0; i<str.length();++i)
    {
        if((str[i]=="'") || (str[i]=="\"")|| (str[i]=="'"))
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
QString CPageParserIVI::findIdUser(QString fio)
{
    QString id_user="";
    QString sql="";
    sql = "SELECT \"ID\" FROM \"User\" WHERE \"FIO\"=\'"+fio+"\';";
    if(!m_query->exec(sql))
    {
        qDebug()<<m_query->lastError().text();
        id_user = "0";
    }
    else
    {
        if(m_query->next())
        {
            id_user = m_query->value(0).toString();
        }
    }
    return id_user;
}

//=========================================================
QString CPageParserIVI::findIdAVS(QString url, QString name)
{
    QString id_avs="";
    QString sql="";
    sql = "SELECT \"ID\" FROM avs WHERE \"URL\"=\'"+url+"\' AND \"NameAVS\"=\'"+name+"\';";
    if(!m_query->exec(sql))
    {
        qDebug()<<m_query->lastError().text();
        id_avs = "0";
    }
    else
    {
        if(m_query->next())
        {
            id_avs = m_query->value(0).toString();
        }
    }
    return id_avs;
}

//=========================================================
QString CPageParserIVI::findIdAVP(QString &url)
{
    QString id_avp="";
    QString sql="";
    sql = "SELECT \"ID\" FROM avp WHERE \"URL\"=\'"+url+"\';";
    if(!m_query->exec(sql))
    {
        qDebug()<<m_query->lastError().text();
        id_avp = "0";
    }
    else
    {
        if(m_query->next())
        {
            id_avp = m_query->value(0).toString();
        }
    }
    return id_avp;
}

//=========================================================
bool CPageParserIVI::addSaveInDB()
{
    bool res=true;
    QString sql="";
    QString tmp;
    try
    {
        if(existsSaveInDb(m_URL))
        {
            qDebug()<<"AVP exists in DB AVP: "<<m_nameRus;
           return res = false;
        }

        sql="INSERT INTO avp (\"NameRus\",\"URL\",\"ID_AVS\",\"NameOriginal\",\"URL_kinopoisk\",\"URL_imdb\") VALUES (E\'";
        sql += decode(m_nameRus);sql+="\',\'";
        sql += m_URL; sql+="\',";
        sql += findIdAVS("https://www.ivi.ru/","IVI");sql+=",E\'";
        sql += decode(m_nameOriginal); sql+="\',\'";
//    sql += m_sDataAVP.urlKinopoisk;
        sql+="\',\'";
//    sql += m_sDataAVP.urlIMDB;
        sql+="\');";
//        qDebug()<<sql;
        if(!m_query->exec(sql))
        {
            qDebug()<<sql;
            qDebug()<<m_query->lastError().text();
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
            sql += m_rubric;sql+="\',\'";
//        sql += m_sDataAVP.filmMaker;
            sql+="\',\'";
            sql += m_yearOfRelease;sql+="\',\'";
            sql += m_age;sql+="\',\'";
            sql += m_duration;sql+="\',";
            sql += findIdAVP(m_URL) ;sql+=",";
            sql += findIdUser("Левдик А.А."); sql+=",\'";
            sql += m_country;
            sql+="\');";
//            qDebug()<<sql;
            if(!m_query->exec(sql.toStdString().c_str()))
            {
                qDebug()<<sql;
                qDebug()<<m_query->lastError().text();
                res = false;
            }
            else
            {
                colAVP++;
                qDebug()<<"Save in DB OK AVP: "<<m_nameRus<<"   colAVP="<<colAVP;
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

int col_file =0;
//=========================================================
void CPageParserIVI::parsePageExt(const QString &fileName)
{
//    qDebug()<<"\nPAGE_EXT="<<fileName;
    QFile file(fileName);
    QString s_FingNameOriginal = "orig_title";
    if(file.open(QFile::ReadOnly | QIODevice::Text))
    {
        while (!file.atEnd())
        {
            QString line = file.readLine();
            if(line.indexOf(s_FingNameOriginal) != -1) // URL
            {
                int index = line.indexOf(s_FingNameOriginal);
                QString s_tmp = line.mid(index+s_FingNameOriginal.length()+5,line.length()-index- s_FingNameOriginal.length()-5);
                if(s_tmp.indexOf(",") != -1)
                {
                    m_nameOriginal = s_tmp.mid(0,s_tmp.indexOf(",")-2);
//                    qDebug()<<"m_nameOriginal = "<< m_nameOriginal;
                }
            }
        }
        file.close();
    }
    else
    {
//        qDebug()<<"Не могу открыть файл:"<<fileName;
    }
}

//=========================================================
void CPageParserIVI::parsePage(const QString &page)
{
    qDebug()<<"\nPAGE="<<page;
    m_nameRus = "";
    m_nameOriginal = "";
    m_URL = "";
    m_yearOfRelease = "";
    m_rubric = "";
    m_country = "";
    m_age = "";
    m_duration = "";
    m_filmMaker = "";

    QFile file(page);

    QString s_findNameRus = "<span class=\"name\">";
    QString s_findURL = "<a tabindex=\"-1\" href=\"";
    QString s_findAge = "<span class=\"age-limit\" data-value=\"";
    QString s_findYearCountryRubric = "<div class=\"content-properties-row\">";

    if(file.open(QFile::ReadOnly | QIODevice::Text))
    {
        while (!file.atEnd())
        {
            QString line = file.readLine();
            if(line.indexOf(s_findURL) != -1) // URL
            {
                int index = line.indexOf(s_findURL);
                if(line.indexOf("\" class=\"item-content-wrapper js-collection-content") != -1)
                {
                    m_URL = "https://www.ivi.ru"+line.mid(index+s_findURL.length(),line.indexOf("\" class=\"item-content-wrapper js-collection-content")-s_findURL.length()-index);
                    QString fileName="C:\\DownloadData\\ivi\\www.ivi.ru\\watch\\";
                    fileName += line.mid(index+s_findURL.length()+7,line.indexOf("\" class=\"item-content-wrapper js-collection-content")-s_findURL.length()-index-7);
                    fileName += "\\person";
                    parsePageExt(fileName);
//                    qDebug()<<"m_URL = "<<m_URL;
                }
            }
            else if(line.indexOf(s_findAge) != -1) // Age
            {
                int index = line.indexOf(s_findAge);
                if((line.indexOf("</span>") != -1) && (m_URL != ""))
                {
                    m_age = line.mid(index+s_findAge.length(),line.indexOf("</span>")-s_findAge.length()-index-2);
//                    qDebug()<<"m_age = "<<m_age;
                }
            }
            else if(line.indexOf(s_findNameRus) != -1) // NameRus
            {
                int index = line.indexOf(s_findNameRus);
                m_nameRus = line.mid(index+s_findNameRus.length(),line.indexOf("</span>")-s_findNameRus.length()-index);
//                qDebug()<<"m_nameRus = "<<m_nameRus;
            }
            else if(line.indexOf(s_findYearCountryRubric) != -1) // Year Country Rubric
            {
                int index = line.indexOf(s_findYearCountryRubric);
                QString s_tmp = line;
                while( line.indexOf("</div>") == -1 )
                {
                    line = file.readLine();
                    s_tmp += line;
                }
                //                qDebug()<<"s_tmp = "<<s_tmp;
                if((s_tmp.indexOf("минут") != -1)  && (m_URL != "")) // Duration
                {
                    m_duration = s_tmp.mid(index+s_findYearCountryRubric.length(),s_tmp.indexOf("</div>")-s_findYearCountryRubric.length()-index);
//                    qDebug()<<"m_duration = "<<m_duration;
                }
                else if(s_tmp.indexOf("<p>") != -1)
                {
//                    qDebug()<<"s_tmp = "<<s_tmp;
                    m_yearOfRelease = s_tmp.mid(index+s_findYearCountryRubric.length(),s_tmp.indexOf("<p>")-s_findYearCountryRubric.length()-index);
                    QString tmp="";
                    for(int i=0; i<m_yearOfRelease.length();++i)
                    {
                        if( (m_yearOfRelease[i]!=' ') && (m_yearOfRelease[i]!='\n') )
                            tmp += m_yearOfRelease[i];
                    }
                    m_yearOfRelease = tmp;
//                    qDebug()<<"m_yearOfRelease = "<<m_yearOfRelease;

                    int index = s_tmp.indexOf("<p>");
                    int index_last_country = s_tmp.indexOf("</p>");
                    m_country = s_tmp.mid(index+3,index_last_country-index-3);
//                    qDebug()<<"m_country = "<<m_country;

                    s_tmp = s_tmp.mid(index_last_country+4,s_tmp.length()-index_last_country-4);
//                    qDebug()<<"s_tmp_new = "<<s_tmp;
                    index = s_tmp.indexOf("<p>");
                    index_last_country = s_tmp.indexOf("</p>");
                    if( (index !=-1) && (index_last_country != -1))
                    {
                        m_rubric = s_tmp.mid(index+3,index_last_country-index-3);
//                        qDebug()<<"m_rubric = "<<m_rubric;
                    }
                }
            }
            if((m_URL !="") && (m_nameRus != "") && (m_yearOfRelease !="") && (m_nameOriginal !=""))
            {
//                qDebug()<<"addSaveInDB";
                addSaveInDB();
                m_nameRus = "";
                m_nameOriginal = "";
                m_URL = "https://www.ivi.ru";
                m_yearOfRelease = "";
                m_rubric = "";
                m_country = "";
                m_age = "";
                m_duration = "";
                m_filmMaker = "";
            }
        }
        file.close();
//        col_file++;
//        if( col_file == 1)
//            exit(0);
    }
    else
        qDebug()<<"Не могу открыть файл:"<<page;
}

//=========================================================
QStringList CPageParserIVI::getDirFiles( const QString& dirName  )
{
//    qDebug()<<"dirName="<<dirName;
    QDir dir( dirName );
    if ( !dir.exists() )
        qDebug()<<"No such directory"<<dir.dirName();
    QStringList fileNames;
    QStringList fileList = dir.entryList( QDir::Files );
    for ( QStringList::Iterator fit = fileList.begin(); fit != fileList.end(); ++fit )
    {
        fileNames.append( dir.absolutePath() + "/" + *fit );
    }
    QStringList dirList = dir.entryList( QDir::Dirs );
    dirList.removeAll( "." );
    dirList.removeAll( ".." );
    dirList.removeAll( "watch" );
    for ( QStringList::Iterator dit = dirList.begin(); dit != dirList.end(); ++dit )
    {
        QDir curDir = dir;
        curDir.cd( *dit );
        QStringList curList = getDirFiles( curDir.absolutePath() );
        for ( QStringList::Iterator it = curList.begin(); it != curList.end(); ++it )
        {
            fileNames.append( QFileInfo(*it).absoluteFilePath() );
        }
    }
    return fileNames;
}
