#include "threadanalysisavp.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QProcess>

#include <deque>

#include <cstdint>
#include <iostream>
#include <unistd.h>

extern QSqlDatabase db;
//=========================================================
ThreadAnalysisAVP::ThreadAnalysisAVP()
{
    query = new QSqlQuery(db);
    query1 = new QSqlQuery(db);
    m_listNonAnalysisAVP.clear();
}

//=========================================================
void ThreadAnalysisAVP::doWork()
{
    //    qDebug()<<__PRETTY_FUNCTION__;
    initListAVP();
    while(true)
    {
        while(m_listNonAnalysisAVP.size() != 0)
        {
            idAVP =  m_listNonAnalysisAVP.front();
            analysisAVP(idAVP);
            m_listNonAnalysisAVP.pop_front();
            usleep(500);
        }
        usleep(5000);
    }
}

//=========================================================
void ThreadAnalysisAVP::initListAVP()
{
    QString sql;
    try
    {
        sql = "SELECT \"ID\" FROM avp WHERE \"ID\" NOT IN ( SELECT \"ID_AVP\" FROM \"AnalysisResult\") GROUP BY \"ID\" LIMIT 3;";
        if(query->exec(sql))
        {
            while(query->next())
            {
                idAVP = query->value(0).toLongLong();
                m_listNonAnalysisAVP.push_back(idAVP);
            }
        }
        else
            qDebug()<<query->lastError().text();

//        sql = "SELECT \"URL\", string_agg(CAST(\"ID\" AS TEXT), \', \') AS ids, count(\"URL\") AS cnt from avp group by \"URL\" having count(*) > 1;";
//        if(query->exec(sql))
//        {
//            int row = 0;
//            QString tmp;
//            while(query->next())
//            {
//                QString ids = query->value(1).toString();
//                qDebug()<<"row = "<<row++<<" ids = "<<ids;
//                int id1 = ids.split(",")[0].toInt();
//                int id2 = ids.split(",")[1].toInt();
//                qDebug()<<"id1 = "<<id1<<" id2 = "<<id2;
//                if(id1 > id2)
//                {
//                    sql = "DELETE FROM avp WHERE \"ID\" ="; sql += tmp.setNum(id1); sql += ";";
//                }
//                else
//                {
//                    sql = "DELETE FROM avp WHERE \"ID\" ="; sql += tmp.setNum(id2); sql += ";";
//                }
//                qDebug()<<"sql = "<< sql;
//                if(!query1->exec(sql))
//                    qDebug()<<query1->lastError().text();
////                if(row == 3)
////                    break;
//            }
//        }
//        else
//            qDebug()<<query->lastError().text();
//        return;


    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
uint64_t ThreadAnalysisAVP::idViolation(QString nameViolation)
{
    QString sql;
    uint64_t idViolation = -1;
    if(nameViolation.indexOf("Нет") != -1)
        return 12;
    sql = "SELECT \"ID\" FROM \"Violation\" WHERE \"Violation\" = \'" +nameViolation + "\';";
    qDebug()<<"sql = "<<sql;
    try
    {
        if(query->exec(sql))
        {
            if(query->next())
                idViolation = query->value(0).toLongLong();
        }
        else
            qDebug()<<query->lastError().text();
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
    return idViolation;
}

//=========================================================
void ThreadAnalysisAVP::addAnalysisResult(uint64_t idAVP, uint64_t idViolation, QString percent)
{
    QString sql,tmp;
    try
    {
        sql = "INSERT INTO \"AnalysisResult\"(\"ID_AVP\",\"ID_Violation\",\"Percent\",\"CheckAuto\") VALUES(";
        sql += tmp.setNum(idAVP);
        sql += ",";
        sql += tmp.setNum(idViolation);
        sql += ",";
        sql += percent;
        sql += ",TRUE);";
        qDebug()<<"sql = "<<sql;
        if(!query->exec(sql))
            qDebug()<<query->lastError().text();

    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
void ThreadAnalysisAVP::analysisAVP(uint64_t idAVP)
{
    qDebug()<<__PRETTY_FUNCTION__;
    QString sql, tmp;
    QString pathOnDisk = "";
    QString command;
    QString violation;
    QString percentViolation;

    try
    {
        sql = "SELECT \"PathOnDisk\" FROM \"DownloadData\" WHERE \"ResourceName\" ='Kinopoisk' AND \"ID_AVP\" = ";
        sql += tmp.setNum(idAVP); sql += ";";
        qDebug()<<"sql = "<<sql;
        if(query->exec(sql))
        {
            if(query->next())
                pathOnDisk = query->value(0).toString();
        }
        else
            qDebug()<<query->lastError().text();

        command = "python3.6 /usr/local/module_analysis/t.py " + pathOnDisk;
        qDebug()<<"command = "<<command;

        //    QProcess::startDetached(command);
        QProcess process;
        process.start(command);
        if( !process.waitForStarted() || !process.waitForFinished() )
        {
                return;
        }
        qDebug() << process.readAllStandardError();
        QString strOut = process.readAllStandardOutput();

        qDebug() <<"strOut = "<<strOut;
        strOut = strOut.mid(strOut.lastIndexOf("{")+1,strOut.length()-strOut.lastIndexOf("{")-3);
        qDebug() <<"strOut = "<<strOut;
        QStringList listViolation = strOut.split(", ");
        for(int i=0; i<listViolation.size();++i)
        {
            qDebug()<<"listViolation["<<i<<"] = "<<listViolation[i];
            violation = listViolation[i].mid(1,listViolation[i].indexOf(":")-2);
            qDebug()<<"violation = "<<violation;
            if(listViolation[i].indexOf(":") != -1)
                percentViolation = listViolation[i].mid(listViolation[i].indexOf(":")+3,listViolation[i].length()-listViolation[i].indexOf(":")-5);
            else
                percentViolation = "0";
            qDebug()<<"percentViolation = "<<percentViolation;
            uint64_t idV = idViolation(violation);

            if(idV != static_cast<uint64_t>(-1))
                addAnalysisResult(idAVP, idV, percentViolation);
        }
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}
