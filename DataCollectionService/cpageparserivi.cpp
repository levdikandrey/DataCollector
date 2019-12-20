#include "cpageparserivi.h"

//=========================================================
CPageParserIVI::CPageParserIVI(QObject *parent): QObject(parent)
{
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
bool CPageParserIVI::existsSaveInDb(const QString &avpName)
{
    bool res = false;
    QString sql;
    try
    {
        sql = "SELECT * FROM ivi WHERE \"NameAVP\"=\'";
        sql += avpName; sql += "\';";
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
void CPageParserIVI::addSaveInDB(const QString &avpName)
{
    QString sql;
    try
    {
        if(!existsSaveInDb(avpName))
        {
            sql="INSERT INTO ivi(\"NameAVP\") VALUES(\'";
            sql += avpName; sql += "\');";
//            qDebug()<<"sql="<<sql;
            if(m_query->exec(sql))
            {
                colAVP++;
                qDebug()<<"Save in DB OK AVP: "<<avpName<<"   colAVP="<<colAVP;
            }
            else
                qDebug()<<m_query->lastError().text();
        }
        else
            qDebug()<<"AVP exists in DB AVP: "<<avpName;

    }
    catch(std::exception &e)
    {
         qDebug()<<e.what();
    }
}

//=========================================================
void CPageParserIVI::parsePage(const QString &page)
{
    qDebug()<<"PAGE="<<page;
    QString fileName;
    QFile file(page);
    if(file.open(QFile::ReadOnly | QIODevice::Text))
    {
        while (!file.atEnd())
        {
            QString line = file.readLine();
            int index = line.indexOf("<span class=\"name\">");
            if(index!=-1)
            {
                fileName=line.mid(index+19,line.indexOf("</span>")-19-index);
//                qDebug()<<fileName;
                addSaveInDB(fileName);
            }
        }
        file.close();
    }
    else
        qDebug()<<"Не могу открыть файл:"<<page;
}

//=========================================================
QStringList CPageParserIVI::getDirFiles( const QString& dirName  )
{
    qDebug()<<"dirName="<<dirName;
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
