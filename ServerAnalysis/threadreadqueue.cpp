#include "threadreadqueue.h"

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QProcess>
#include <QFile>
#include <QDateTime>

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
    m_requestSender = new RequestSender();
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
uint64_t ThreadReadQueue::idViolation(QString nameViolation) const
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
uint64_t ThreadReadQueue::idAVS(uint64_t idAVP) const
{
    QString sql,tmp;
    uint64_t idAVS = -1;
    sql = "SELECT \"ID_AVS\" FROM avp WHERE \"ID\" = " +tmp.setNum(idAVP) + ";";
    qDebug()<<"sql = "<<sql;
    try
    {
        if(query->exec(sql))
        {
            if(query->next())
                idAVS = query->value(0).toLongLong();
        }
        else
            qDebug()<<query->lastError().text();
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
    return idAVS;
}

//=========================================================
QString ThreadReadQueue::getPathOnDisk(uint64_t idAVP) const
{
    QString path = "";
    QString sql,tmp;
    sql = "SELECT \"PathOnDisk\" FROM \"DownloadData\" WHERE \"ID_AVP\" = " +tmp.setNum(idAVP) + ";";
    qDebug()<<"sql = "<<sql;
    try
    {
        if(query->exec(sql))
        {
            if(query->next())
                path = query->value(0).toString();
        }
        else
            qDebug()<<query->lastError().text();
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
    return path;
}

//=========================================================
QString ThreadReadQueue::getURL(uint64_t idAVP) const
{
    QString url = "";
    QString sql,tmp;
    sql = "SELECT \"URL\" FROM \"DownloadData\" WHERE \"ID_AVP\" = " +tmp.setNum(idAVP) + ";";
    qDebug()<<"sql = "<<sql;
    try
    {
        if(query->exec(sql))
        {
            if(query->next())
                url = query->value(0).toString();
        }
        else
            qDebug()<<query->lastError().text();
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
    return url;
}

//=========================================================
uint64_t ThreadReadQueue::getIdDownloadData(uint64_t idAVP) const
{
    uint64_t id = -1;
    QString sql,tmp;
    sql = "SELECT \"ID\" FROM \"DownloadData\" WHERE \"ID_AVP\"=" + tmp.setNum(idAVP) + ";";
    qDebug()<<"sql="<<sql;
    if(query->exec(sql))
    {
        if(query->next())
            id = query->value(0).toInt();
    }
        qDebug()<<query->lastError().text();
    qDebug()<<"id="<<id;
    return id;
}

//=========================================================
void ThreadReadQueue::analysisAVP(uint64_t idAVP)
{
    qDebug()<<__PRETTY_FUNCTION__<<" idAVP="<<idAVP;
    QString sql, tmp;
    QString pathOnDisk = "";
    QString command;
    QString violation;
    QString percentViolation;
    QString pathReviewForIVI= "";
    int currentIdAVPKinopoisk = -1;

    try
    {
        m_idAVP = idAVP;
        m_idAVS = idAVS(idAVP);
        pathOnDisk = getPathOnDisk(idAVP);

        if( (( m_idAVS == 147 ) || ( m_idAVS == 149 ) || ( m_idAVS == 151 ) || ( m_idAVS == 150 ) || ( m_idAVS == 152 )  || ( m_idAVS == 153 ))  && ( pathOnDisk  == "") )// IVI film
        {
            qDebug()<<"****"<<sql;
            sql ="SELECT avp.\"NameRus\",avp.\"NameOriginal\",aa.\"YearOfRelease\" FROM avp "
                 "INNER JOIN \"AVPattribute\" aa ON avp.\"ID\" = aa.\"ID_AVP\" WHERE avp.\"ID\"="+tmp.setNum(idAVP)+";";
            qDebug()<<"sql="<<sql;
            if(query->exec(sql))
            {
                if(query->next())
                {
                    qDebug()<<query->value(0).toString();
                    qDebug()<<query->value(1).toString();
                    qDebug()<<query->value(2).toString();
                    sql = "SELECT dd.\"URL\",dd.\"PathOnDisk\",avp.\"ID\" FROM avp "
                          "INNER JOIN \"AVPattribute\" aa ON aa.\"ID_AVP\" = avp.\"ID\" "
                          "INNER JOIN \"DownloadData\" dd ON dd.\"ID_AVP\" = avp.\"ID\" ";
                    if(query->value(1).toString() != "")
                    {
                        sql+="WHERE (avp.\"NameRus\" LIKE \'"+query->value(0).toString()+"%\' OR avp.\"NameOriginal\" LIKE \'"+query->value(1).toString()+"%\'";
                        sql+=") AND aa.\"YearOfRelease\"=\'"+query->value(2).toString()+"\' AND avp.\"ID_AVS\" = 146;";
                    }
                    else
                    {
                        sql+="WHERE avp.\"NameRus\" LIKE \'"+query->value(0).toString()+"%\' AND aa.\"YearOfRelease\"=\'"+query->value(2).toString()+"\' AND avp.\"ID_AVS\" = 146;";
                    }
                    qDebug()<<"sql="<<sql;
                    if(query->exec(sql))
                    {
                        if(query->next())
                        {
                            qDebug()<<query->value(0).toString();
                            qDebug()<<query->value(1).toString();
                            qDebug()<<query->value(2).toInt();
                            pathReviewForIVI = query->value(0).toString();
                            pathOnDisk = query->value(1).toString();
                            currentIdAVPKinopoisk = query->value(2).toInt();
                            qDebug()<<" currentIdAVPKinopoisk = "<< currentIdAVPKinopoisk;
                        }
                    }
                    else
                        qDebug()<<query->lastError().text();

                    if(pathReviewForIVI == "")
                    {
                        qDebug()<<"Нет рецензий для анализа!";
                        m_sCommand.answerState = 0x02;
                        emit sendAnswerAnalysisAVP(m_sCommand);
                        return;
                    }
                }

            }
            else
                qDebug()<<query->lastError().text();
        }

        if( m_idAVS != 147 )
        {
            currentIdAVPKinopoisk = idAVP;
            pathReviewForIVI = getURL(idAVP);
        }

        if(pathOnDisk == "")
        {
            qDebug()<<"Запускаем модуль сбора и анализа данных данных. "<< pathReviewForIVI;
            QString currentPathFile = tmp.setNum(getIdDownloadData(currentIdAVPKinopoisk));
            currentPathFile += ".html";

            qDebug()<<" pathReviewForIVI = "<< pathReviewForIVI;
            if(getData(pathReviewForIVI,currentPathFile))
            {
                pathOnDisk = "/mnt/Data/DownloadData/kinopoisk/"+currentPathFile;
            }
            else
            {
                qDebug()<<"Не удалось скачать данные об АВП! "<< pathReviewForIVI;
                m_sCommand.answerState = 0x03;
                emit sendAnswerAnalysisAVP(m_sCommand);
                return;
            }
        }

/*        sql = "SELECT dd.\"PathOnDisk\",avp.\"ID_AVS\" FROM \"DownloadData\" dd INNER JOIN avp ON avp.\"ID\" = dd.\"ID_AVP\" WHERE \"ResourceName\" ='Kinopoisk' AND \"ID_AVP\" = ";
        sql += tmp.setNum(idAVP); sql += ";";
        qDebug()<<"sql = "<<sql;
        if(query->exec(sql))
        {
            if(query->next())
                pathOnDisk = query->value(0).toString();
        }
        else
            qDebug()<<query->lastError().text();*/

        qDebug()<<"pathOnDisk ="<<pathOnDisk;
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

            strOut = strOut.mid(strOut.lastIndexOf("{")+1,strOut.length()-strOut.lastIndexOf("{")-3);
            QStringList listViolation = strOut.split(", ");

            for(int i=0; i<listViolation.size();++i)
            {
                violation = listViolation[i].mid(1,listViolation[i].indexOf(":")-2);
                if(listViolation[i].indexOf(":") != -1)
                    percentViolation = listViolation[i].mid(listViolation[i].indexOf(":")+3,listViolation[i].length()-listViolation[i].indexOf(":")-5);
                else
                    percentViolation = "0";

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
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
}

//=========================================================
bool ThreadReadQueue::getData(const QString &url_path, const QString &fileName)
{
    bool res = false;
    QString currentFileName = "/mnt/Data/DownloadData/kinopoisk/";
    currentFileName += fileName;
    qDebug()<<"url_path="<<url_path;
    qDebug()<<"currentFileName="<<currentFileName;

    m_request.setAddress(url_path);
    QByteArray data = m_requestSender->get( m_request );
    if(data.length() > 10240)
        res = true;
    if(res)
    {
        QFile file(currentFileName);
        if(file.open(QFile::WriteOnly | QIODevice::Text))
        {
            file.write(data);
            file.close();
        }
        changeSaveInDB(m_idAVP,currentFileName);
    }
    return res;
}

//=========================================================
bool ThreadReadQueue::changeSaveInDB(uint64_t idAVP, QString &currentFileName)
{
    bool res =true;
    QString sql,tmp,timestamp;
    try
    {
       timestamp = QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss");
       sql = "UPDATE \"DownloadData\" SET \"DownloadStatus\"=\'Yes\', \"PathOnDisk\"=\'"
               +currentFileName+"\', \"DownloadDate\"=\'"
               +timestamp+"\' WHERE \"ID_AVP\"="+tmp.setNum(idAVP)+";";
       qDebug()<<"SQL = "<<sql;
       if(!query->exec(sql))
           qDebug()<<query->lastError().text();
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
    return res;
}
