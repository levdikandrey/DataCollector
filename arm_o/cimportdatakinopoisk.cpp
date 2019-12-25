#include "cimportdatakinopoisk.h"

#include <QFile>
#include <QMessageBox>
#include <QRegExp>
#include <QDebug>
#include <QThread>
#include <QDateTime>

#include <iostream>

extern QSqlDatabase db;
extern QString currentUserName;
//=========================================================
CImportDataKinopoisk::CImportDataKinopoisk(QObject *parent) : QObject(parent)
{
    query = nullptr;
    query = new QSqlQuery(db);
    m_cImportData = nullptr;
    m_cImportData = new CImportData();
}

//=========================================================
CImportDataKinopoisk::~CImportDataKinopoisk()
{
    if(query != nullptr)
        delete query;
    if(m_cImportData != nullptr)
        delete m_cImportData;
}

//=========================================================
void CImportDataKinopoisk::doWork(const QString &fileName)
{
    QString url, title_ru, title_original;
    QString result,tmp;

    int col_Ok = 0;
    bool flag_error = false;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString textMsg = "Не могу открыть файл " + fileName+"!";
        emit resultReady(textMsg);
        return;
    }
    else
    {
//        emit resultReady("Открыл файл!");
        int i=0;
        while (!file.atEnd())
        {
            QString line = file.readLine();
            QStringList list = line.split('\t');

            url = list.at(0).mid(list.at(0).lastIndexOf("=")+1,list.at(0).length()-list.at(0).lastIndexOf("=")-1);
            title_ru = list.at(1).mid(list.at(1).lastIndexOf("=")+1,list.at(1).length()-list.at(1).lastIndexOf("=")-1);
            title_original = list.at(2).mid(list.at(2).lastIndexOf("=")+1,list.at(2).length()-list.at(2).lastIndexOf("=")-1);

            i++;
            qDebug()<<"i="<<i;

            if(title_ru == "" || i<671000)
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
                m_sDataAVP.userSaveInDB = currentUserName;
                m_sDataAVP.rubric = "";
                m_sDataAVP.age = "";
                m_sDataAVP.duration = "";
                m_sDataAVP.filmMaker = "";

                try
                {
                   if(m_cImportData->addSaveInDB(m_sDataAVP))
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
                           emit resultReady(result);
                       }
                   }
                   else
                   {
                       result = "Загрузка данных АВП ";
                       result +=tmp.setNum(i); result +=" ";
                       result += m_sDataAVP.avpURL; result += "........FAIL";
                       flag_error = true;
                       qDebug()<<result;
//                       emit resultReady(result);
                   }
                }
                catch (std::exception &e)
                {
                    qDebug()<<e.what();
                }

            }
//            if(i==5)
//                break;
        }
        file.close();

        result="Запись данных о " + tmp.setNum(col_Ok)+" АВП завершена успешно!";
        emit resultReady(result);
        emit resultReady("END");
        if(flag_error == true)
            emit resultReady("Загрузка данных произошла c ошибками или не полностью!!!\nПосмотрите log-файл!");
    }
}
