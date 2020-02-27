#include "threadreadqueue.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QProcess>

#include <deque>

#include <cstdint>
#include <iostream>
#include <unistd.h>

extern std::deque<uint64_t> listAVP;
extern QSqlDatabase db;
//=========================================================
ThreadReadQueue::ThreadReadQueue()
{
    query = new QSqlQuery(db);
}

//=========================================================
void ThreadReadQueue::doWork()
{
    analysisAVP(92116);
    return;
    //    qDebug()<<__PRETTY_FUNCTION__;
    while(true)
    {
        while(!listAVP.empty())
        {
            idAVP = listAVP.front();
            qDebug()<<"ThreadReadQueue: idAVP = "<<idAVP;
            listAVP.pop_front();
            analysisAVP(idAVP);
        }
        usleep(5000);
    }
}

//=========================================================
void ThreadReadQueue::analysisAVP(uint64_t idAVP)
{
    qDebug()<<__PRETTY_FUNCTION__;
    QString sql, tmp;
    QString pathOnDisk = "";
    QString command;
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
//        QProcess process;
//        process.start(command);
//        if( !process.waitForStarted() || !process.waitForFinished() )
//        {
//                return;
//        }
//        qDebug() << process.readAllStandardError();
//        QString strOut = process.readAllStandardOutput();
        QString strOut = "Для файла: /mnt/Data/DownloadData/kinopoisk/100001.html Найдено по темам: {'ЛГБТ': 4, 'Порнография': 5, 'Суицид': 1}";
        qDebug() <<"strOut = "<<strOut;
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}
