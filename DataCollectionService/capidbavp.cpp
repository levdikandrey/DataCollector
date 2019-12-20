#include "capidbavp.h"
#include <QDebug>

namespace APIDB
{
    //=========================================================
    CAPIdbAVP::CAPIdbAVP(QObject *parent): QObject(parent)
    {
    }

    //=========================================================
    void CAPIdbAVP::setDB(const QSqlDatabase &db)
    {
        m_db = db;
        m_query = new QSqlQuery(db);
    }

    //=========================================================
    QList<SAVPattribute> CAPIdbAVP::getAttributeAVP(QString nameAVP)
    {
        QList<SAVPattribute> listAttributeAVP;
        SAVPattribute attributeAVP;
        QString sql;
        try
        {
            sql = "SELECT avp.\"ID\",avp.\"NameRus\",avp.\"NameOriginal\",avp.\"URL\","
                  "avs.\"NameAVS\","
                  "aa.\"Rubric\",aa.\"FilmMaker\",aa.\"YearOfRelease\",aa.\"Age\",aa.\"Duration\",aa.\"DateSaveInDB\","
                  "dld.\"PathOnDisk\",dld.\"ResourceName\",dld.\"DownloadStatus\",dld.\"DownloadDate\" FROM avp "
                  "INNER JOIN avs ON avp.\"ID_AVS\" = avs.\"ID\" "
                  "INNER JOIN \"DownloadData\" dld ON avp.\"ID\" = dld.\"ID_AVP\" "
                  "INNER JOIN \"AVPattribute\" aa ON avp.\"ID\" = aa.\"ID_AVP\" "
                  "WHERE \"NameRus\"=\'";
                   sql += nameAVP;
                   sql += "\';";

            qDebug()<<"sql="<<sql;
            if(m_query->exec(sql))
            {
                while(m_query->next())
                {
                    attributeAVP.id = m_query->value(0).toInt();
                    attributeAVP.nameRus = m_query->value(1).toString();
                    attributeAVP.nameOriginal = m_query->value(2).toString();
                    attributeAVP.urlAVP = m_query->value(3).toString();
                    attributeAVP.nameAVS = m_query->value(4).toString();
                    attributeAVP.rubric = m_query->value(5).toString();
                    attributeAVP.filmMaker = m_query->value(6).toString();
                    attributeAVP.yearOfRelease = m_query->value(7).toString();
                    attributeAVP.age = m_query->value(8).toString();
                    attributeAVP.duration = m_query->value(9).toString();
                    attributeAVP.dateSaveInDB = m_query->value(10).toString();
                    attributeAVP.pathOnDisk = m_query->value(11).toString();
                    attributeAVP.resourceName = m_query->value(12).toString();
                    attributeAVP.downloadStatus = m_query->value(13).toString();
                    attributeAVP.downloadDate = m_query->value(14).toString();
                    listAttributeAVP.push_back(attributeAVP);
                }
            }
            else
                qDebug()<<m_query->lastError().text();

        }
        catch(std::exception &e)
        {
             qDebug()<<e.what();
        }
        return listAttributeAVP;
    }

    //=========================================================
    QList<SListFilePath> CAPIdbAVP::getFilePath(QString nameAVP)
    {
        QList<SListFilePath> listFP;
        SListFilePath sListFilePath;
        QString sql;
        try
        {
            sql = "SELECT dld.\"URL\",dld.\"PathOnDisk\",aa.\"YearOfRelease\",aa.\"FilmMaker\" FROM \"DownloadData\" dld "
                  "INNER JOIN \"AVPattribute\" aa ON aa.\"ID_AVP\"=dld.\"ID_AVP\"  "
                  "WHERE dld.\"ID_AVP\" IN (SELECT \"ID\" FROM avp WHERE \"NameRus\"='";
                   sql += nameAVP;
                   sql += "\');";

            qDebug()<<"sql="<<sql;
            if(m_query->exec(sql))
            {
                while(m_query->next())
                {
                    sListFilePath.url = m_query->value(0).toString();
                    sListFilePath.path = m_query->value(1).toString();
                    sListFilePath.yearOfRelease = m_query->value(2).toString();
                    sListFilePath.filmMaker = m_query->value(3).toString();
                    listFP.push_back(sListFilePath);
                }
            }
            else
                qDebug()<<m_query->lastError().text();

        }
        catch(std::exception &e)
        {
             qDebug()<<e.what();
        }
        return listFP;
    }
}
