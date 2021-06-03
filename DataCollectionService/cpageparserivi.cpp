#include "cpageparserivi.h"
#include <QDateTime>
#include <QDir>

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
void CPageParserIVI::doWorkOkooTV(const QString &startDir)
{
}

//=========================================================
void CPageParserIVI::doWorkMegogo(const QString &startDir)
{
    qDebug()<<__PRETTY_FUNCTION__;
    qDebug()<<"startDir="<<startDir;
    m_nameAVS = 2;
    colAVP = 0;
//    parsePageMegogo("z:\\DownloadData\\megogo.ru\\ru\\view\\106211-sheri\\tab_comments");
    QStringList allFiles = getDirFiles( startDir );
    for (int i = 0; i < allFiles.size(); ++i)
    {
        parsePageMegogo(allFiles.at(i));
    }
    qDebug()<<"colAVP="<<colAVP;
}

//=========================================================
void CPageParserIVI::doWork(const QString &startDir)
{
    m_nameAVS = 1;
    colAVP = 0;
    QStringList allFiles = getDirFiles( startDir );
    for (int i = 0; i < allFiles.size(); ++i)
        parsePage(allFiles.at(i));
    qDebug()<<"colAVP="<<colAVP;
}

//=========================================================
bool CPageParserIVI::existsFilmMakerInSaveDb(const QString &url)
{
    bool res = true;
    QString sql;
    try
    {
        sql = "SELECT aa.\"FilmMaker\" FROM avp INNER JOIN \"AVPattribute\" aa ON aa.\"ID_AVP\" = avp.\"ID\" WHERE avp.\"URL\"=\'";
        sql += url; sql += "\';";
//        qDebug()<<"sql="<<sql;
        if(m_query->exec(sql))
        {
            if(m_query->next())
            {
//                qDebug()<<"FilmMaker :"<<m_query->value(0).toString();
                if(m_query->value(0).toString()=="")
                   res = false;
            }
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
bool CPageParserIVI::updateSaveInDB()
{
    qDebug()<<"\n"<<__PRETTY_FUNCTION__;
    qDebug()<<"m_URL = "<<m_URL;
    qDebug()<<"m_nameRus = "<<m_nameRus;
    qDebug()<<"m_nameOriginal = "<<m_nameOriginal;
    qDebug()<<"m_yearOfRelease = "<<m_yearOfRelease;
    qDebug()<<"m_age = "<<m_age;
    qDebug()<<"m_duration = "<<m_duration;
    qDebug()<<"m_country = "<<m_country;
    qDebug()<<"m_rubric = "<<m_rubric;
    qDebug()<<"m_filmMaker = "<<m_filmMaker;

    bool res=true;
    QString sql="";
    QString tmp;
    try
    {
        if(existsFilmMakerInSaveDb(m_URL))
        {
            qDebug()<<"FilMaker exists in save DB AVP: "<<m_nameRus;
            return false;
        }

        sql="UPDATE avp SET \"NameRus\" = E\'";
        sql += decode(m_nameRus);sql+="\', \"NameOriginal\" = E\'";
        sql += decode(m_nameOriginal); sql+="\' WHERE \"URL\" = \'";
        sql += m_URL; sql+="\';";
        qDebug()<<sql;
        if(!m_query->exec(sql))
        {
            qDebug()<<sql;
            qDebug()<<m_query->lastError().text();
            res = false;
        }
        else
        {
            res = true;
            sql = "UPDATE \"AVPattribute\" SET \"YearOfRelease\"=\'";
            sql += m_yearOfRelease; sql +="\',\"Age\" =\'";
            sql += m_age; sql+="\',\"Duration\"=\'";
            sql += m_duration; sql +="\',\"FilmMaker\"=\'";
            sql += m_filmMaker; sql +="\',\"Country\"=\'";
            sql += m_country; sql +="\',\"Rubric\" = \'";
            sql += m_rubric; sql +="\' WHERE \"ID_AVP\" =(SELECT \"ID\" FROM avp WHERE \"URL\" = \'";
            sql += m_URL; sql +="\');";
            qDebug()<<sql;
            if(!m_query->exec(sql))
            {
                qDebug()<<sql;
                qDebug()<<m_query->lastError().text();
                res = false;
            }
            else
            {
                colAVP++;
                qDebug()<<"Update in DB OK AVP: "<<m_nameRus<<"   colAVP="<<colAVP<<"\n";
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
            qDebug()<<"";
//           res = updateSaveInDB();
           return res;
        }

        sql="INSERT INTO avp (\"NameRus\",\"URL\",\"ID_AVS\",\"NameOriginal\",\"URL_kinopoisk\",\"URL_imdb\") VALUES (E\'";
        sql += decode(m_nameRus);sql+="\',\'";
        sql += m_URL; sql+="\',";
        if(m_nameAVS == 1)
        {
            sql += findIdAVS("https://www.ivi.ru/","IVI");
        }
        else if(m_nameAVS == 2)
        {
            sql += findIdAVS("https://megogo.ru/ru","Megogo");
        }
        sql+=",E\'";
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
            sql += m_filmMaker;
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
                qDebug()<<"Save in DB OK AVP: "<<m_nameRus<<"   colAVP="<<colAVP<<"\n";
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
void CPageParserIVI::parsePageExt1(const QString &fileName)
{
    qDebug()<<"\nPAGE_EXT="<<fileName;
    QFile file(fileName);
    QString s_FindFilmMaker ="<div class=\"fixedSlimPosterBlock__title\">";
    QString s_FindFilmMakerSecond = "<div class=\"fixedSlimPosterBlock__secondTitle\">";
    if(file.open(QFile::ReadOnly | QIODevice::Text))
    {
        while (!file.atEnd())
        {
            QString line = file.readLine();
            if(line.indexOf(s_FindFilmMaker) != -1) //FilmMaker
            {
//                qDebug()<<"line = "<<line;
                int index = line.indexOf(s_FindFilmMaker);
                QString s_tmp = line.mid(index+s_FindFilmMaker.length(),line.length()-index- s_FindFilmMaker.length());
//                qDebug()<<"s_tmp = "<<s_tmp;
                if(s_tmp.indexOf("</div>") != -1)
                {
                    m_filmMaker = s_tmp.mid(0,s_tmp.indexOf("</div>"));
//                    qDebug()<<"m_filmMaker = "<< m_filmMaker;
                }
                if(line.indexOf(s_FindFilmMakerSecond) != -1)
                {
                    index = line.indexOf(s_FindFilmMakerSecond);
                    s_tmp = line.mid(index+s_FindFilmMakerSecond.length(),line.length()-index- s_FindFilmMakerSecond.length());
//                qDebug()<<"s_tmp_second = "<<s_tmp;
                    if(s_tmp.indexOf("</div>") != -1)
                    {
                        m_filmMaker += " ";
                        m_filmMaker += s_tmp.mid(0,s_tmp.indexOf("</div>"));
                        qDebug()<<"m_filmMaker1 = "<< m_filmMaker;
                    }
                }
            }
            if((m_nameOriginal !="") && (m_filmMaker != ""))
            {
                break;
            }
        }
        file.close();
    }
    else
    {
        qDebug()<<"Не могу открыть файл:"<<fileName;
    }
}

//=========================================================
void CPageParserIVI::parsePageExt(const QString &fileName)
{
    //    qDebug()<<"\nPAGE_EXT="<<fileName;
    QFile file(fileName);
    QString s_FingNameOriginal = "orig_title";
    QString s_FindFilmMaker ="<div class=\"slimPosterBlock__title\">";
    QString s_FindFilmMakerSecond ="<div class=\"slimPosterBlock__secondTitle\">";
    if(file.open(QFile::ReadOnly | QIODevice::Text))
    {
        while (!file.atEnd())
        {
            QString line = file.readLine();
            if(line.indexOf(s_FingNameOriginal) != -1) // NameOriginal
            {
                int index = line.indexOf(s_FingNameOriginal);
                QString s_tmp = line.mid(index+s_FingNameOriginal.length()+5,line.length()-index- s_FingNameOriginal.length()-5);
                if(s_tmp.indexOf(",") != -1)
                {
                    m_nameOriginal = s_tmp.mid(0,s_tmp.indexOf(",")-2);
//                    qDebug()<<"m_nameOriginal = "<< m_nameOriginal;
                }
            }
            else if(line.indexOf(s_FindFilmMaker) != -1) //FilmMaker
            {
//                qDebug()<<"line = "<<line;
                int index = line.indexOf(s_FindFilmMaker);
                QString s_tmp = line.mid(index+s_FindFilmMaker.length(),line.length()-index- s_FindFilmMaker.length());
//                qDebug()<<"s_tmp = "<<s_tmp;
                if(s_tmp.indexOf("</div>") != -1)
                {
                    m_filmMaker = s_tmp.mid(0,s_tmp.indexOf("</div>"));
//                    qDebug()<<"m_filmMaker = "<< m_filmMaker;
                }
                if(line.indexOf(s_FindFilmMakerSecond) != -1)
                {
                    index = line.indexOf(s_FindFilmMakerSecond);
                    s_tmp = line.mid(index+s_FindFilmMakerSecond.length(),line.length()-index- s_FindFilmMakerSecond.length());
//                qDebug()<<"s_tmp_second = "<<s_tmp;
                    if(s_tmp.indexOf("</div>") != -1)
                    {
                        m_filmMaker += " ";
                        m_filmMaker += s_tmp.mid(0,s_tmp.indexOf("</div>"));
//                    qDebug()<<"m_filmMaker1 = "<< m_filmMaker;
                    }
                }
            }
            if((m_nameOriginal !="") && (m_filmMaker != ""))
            {
                break;
            }
        }
        file.close();
    }
    else
    {
        qDebug()<<"Не могу открыть файл:"<<fileName;
    }
}

//=========================================================
void CPageParserIVI::parsePageMegogo(const QString &page)
{
    qDebug()<<__PRETTY_FUNCTION__;
    QString tmp, person;
    QFile file(page);
    QString s_findNameRus = "<meta property=\"og:title\" content=\"";
    QString s_findURL = "<meta property=\"og:url\" content=\"";
    QString s_findDuration ="<meta property=\"video:duration\" content=\"";
    QString s_findCountry ="<meta property=\"ya:ovs:country\" content=\"";
    QString s_findGenre = "<meta property=\"ya:ovs:genre\" content=\"";
    QString s_findAge = "<meta property=\"og:restrictions:age\" content=\"";
    QString s_findYear ="<span class=\"video-year\">";
    QString s_findYearTitle ="<title>";


    QString s_findFilmMaker = "<meta property=\"ya:ovs:person\" content=\""; //  <meta property="ya:ovs:person:role" content="Executive producer" />
    QString s_filmMaker = "<meta property=\"ya:ovs:person:role\" content=\"Executive producer\" />";

    if(file.open(QFile::ReadOnly | QIODevice::Text))
    {
        while (!file.atEnd())
        {
            QString line = file.readLine();
            if(line.indexOf(s_findNameRus) != -1) // Title
            {
                int index = line.indexOf(s_findNameRus);
                m_nameRus = line.mid(index+s_findNameRus.length(),line.indexOf("/>")-index-s_findNameRus.length()-2);
                qDebug()<<"m_nameRus = "<<m_nameRus;
            }
            else if(line.indexOf(s_findURL) != -1) // URL
            {
                int index = line.indexOf(s_findURL);
                m_URL = line.mid(index+s_findURL.length(),line.indexOf("/>")-index-s_findURL.length()-2);
                qDebug()<<"m_URL = "<<m_URL;
            }
            else if(line.indexOf(s_findDuration) != -1) // Duration
            {
                int index = line.indexOf(s_findDuration);
                m_duration = tmp.setNum((line.mid(index+s_findDuration.length(),line.indexOf("/>")-index-s_findDuration.length()-2).toInt())/60);
                qDebug()<<"m_duration = "<<m_duration;
            }
            else if(line.indexOf(s_findCountry) != -1) // Country
            {
                int index = line.indexOf(s_findCountry);
                m_country = line.mid(index+s_findCountry.length(),line.indexOf("/>")-index-s_findCountry.length()-2);
                qDebug()<<"m_country = "<<m_country;
            }
            else if(line.indexOf(s_findGenre) != -1) // Genre
            {
                int index = line.indexOf(s_findGenre);
                m_rubric = line.mid(index+s_findGenre.length(),line.indexOf("/>")-index-s_findGenre.length()-2);
                qDebug()<<"m_rubric = "<<m_rubric;
            }
            else if(line.indexOf(s_findAge) != -1) // Age
            {
                int index = line.indexOf(s_findAge);
                m_age = line.mid(index+s_findAge.length(),line.indexOf("/>")-index-s_findAge.length()-2) + "+";
                qDebug()<<"m_age = "<<m_age;
            }
            else if(line.indexOf(s_findFilmMaker) != -1) // current person
            {
                int index = line.indexOf(s_findFilmMaker);
                person = line.mid(index+s_findFilmMaker.length(),line.indexOf("/>")-index-s_findFilmMaker.length()-2);
//                qDebug()<<"m_age = "<<m_age;
            }
            else if(line.indexOf(s_filmMaker) != -1) // filmMaker
            {
                m_filmMaker = person;
                qDebug()<<"m_filmMaker = "<<m_filmMaker;
            }
            else if(line.indexOf(s_findYearTitle) != -1) // yearTitle
            {
                line = file.readLine();
                m_yearOfRelease = line.mid(line.indexOf(",")+2, 4);
                qDebug()<<"m_yaerOfRelease = "<<m_yearOfRelease;
            }
        }
        file.close();
        addSaveInDB();
    }
    else
        qDebug()<<"Не могу открыть файл:"<<page;
}

//=========================================================
void CPageParserIVI::parsePage(const QString &page)
{
//    qDebug()<<"\nPAGE="<<page;
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
//                    m_URL = "https://www.ivi.ru"+line.mid(index+s_findURL.length(),line.indexOf("\" class=\"item-content-wrapper js-collection-content")-s_findURL.length()-index);
                    m_URL = line.mid(index+s_findURL.length(),line.indexOf("\" class=\"item-content-wrapper js-collection-content")-s_findURL.length()-index);
                    qDebug()<<"URL = "<<m_URL;
//                    QString fileName="d:\\DownloadData\\ivi\\www.ivi.ru\\watch\\";
                    QString fileName="z:\\DownloadData";
                    fileName += line.mid(index+s_findURL.length()+7,line.indexOf("\" class=\"item-content-wrapper js-collection-content")-s_findURL.length()-index-7);
                    if(QDir(fileName).exists())
                    {
                        fileName += "\\person";
                        parsePageExt(fileName);
                    }
                    else
                        parsePageExt1(fileName);
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
                int iter = 0;
                while( line.indexOf("</div>") == -1 )
                {
                    if(iter >10)
                        break;
                    line = file.readLine();
                    s_tmp += line;
                    iter++;
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
//            if((m_URL !="") && (m_nameRus != "") && (m_yearOfRelease !="") && (m_nameOriginal !=""))
            if((m_URL !="") && (m_nameRus != "") && (m_yearOfRelease !=""))
            {
//                qDebug()<<"addSaveInDB";
                addSaveInDB();
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
        file.close();
//        col_file++;
//        if( col_file == 10)
//            exit(0);
    }
    else
        qDebug()<<"Не могу открыть файл:"<<page;
}

//=========================================================
QStringList CPageParserIVI::getDirFiles( const QString& dirName  )
{
//    qDebug()<<__PRETTY_FUNCTION__;
//    int i =0;

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
    dirList.removeAll( "animation" );
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
//    qDebug()<<__PRETTY_FUNCTION__<<"____END";
    return fileNames;
}
