#include "threadreadqueue.h"

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QProcess>

#include <deque>

#include <cstdint>
#include <iostream>
#include <unistd.h>

extern std::deque<SCommand> listAVP;
extern QSqlDatabase db;
//=========================================================
ThreadReadQueue::ThreadReadQueue()
{
    query = new QSqlQuery(db);
}

//=========================================================
void ThreadReadQueue::doWork()
{
    //    qDebug()<<__PRETTY_FUNCTION__;
    while(true)
    {
        while(!listAVP.empty())
        {
            m_sCommand = listAVP.front();
            listAVP.pop_front();
            client = m_sCommand.client;
            analysisAVP(m_sCommand.idAVP);
        }
        usleep(5000);
    }
}

//=========================================================
uint64_t ThreadReadQueue::idViolation(QString nameViolation)
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
void ThreadReadQueue::addAnalysisResult(uint64_t idAVP, uint64_t idViolation, QString percent)
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
//        qDebug()<<"sql = "<<sql;
        if(!query->exec(sql))
            qDebug()<<query->lastError().text();

    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
void ThreadReadQueue::analysisAVP(uint64_t idAVP)
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
//        qDebug()<<"sql = "<<sql;
        if(query->exec(sql))
        {
            if(query->next())
                pathOnDisk = query->value(0).toString();
        }
        else
            qDebug()<<query->lastError().text();

        if(pathOnDisk != "")
        {
            command = "python3.6 /usr/local/module_analysis/t.py " + pathOnDisk;
            qDebug()<<"command = "<<command;

            QProcess process;
            process.start(command);
            if( !process.waitForStarted() || !process.waitForFinished() )
            {
                return;
            }
            qDebug() << process.readAllStandardError();
            QString strOut = process.readAllStandardOutput();

//        qDebug() <<"strOut = "<<strOut;
            strOut = strOut.mid(strOut.lastIndexOf("{")+1,strOut.length()-strOut.lastIndexOf("{")-3);
//        qDebug() <<"strOut = "<<strOut;
            QStringList listViolation = strOut.split(", ");
            for(int i=0; i<listViolation.size();++i)
            {
            //            qDebug()<<"listViolation["<<i<<"] = "<<listViolation[i];
                violation = listViolation[i].mid(1,listViolation[i].indexOf(":")-2);
//            qDebug()<<"violation = "<<violation;
                if(listViolation[i].indexOf(":") != -1)
                    percentViolation = listViolation[i].mid(listViolation[i].indexOf(":")+3,listViolation[i].length()-listViolation[i].indexOf(":")-5);
                else
                    percentViolation = "0";
//            qDebug()<<"percentViolation = "<<percentViolation;
                qDebug()<<"violation["<<i<<"] = "<<violation<<" Percent violation = "<<percentViolation;
                uint64_t idV = idViolation(violation);

                if(idV != static_cast<uint64_t>(-1))
                    addAnalysisResult(idAVP, idV, percentViolation);
            }
            m_sCommand.answerState = 0x01;
        }
        else
        {
            m_sCommand.answerState = 0x02;
        }
        emit sendAnswerAnalysisAVP(m_sCommand);
//          client->sendAnswerAnalysisAVP(m_sCommand.idAVP);
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}
