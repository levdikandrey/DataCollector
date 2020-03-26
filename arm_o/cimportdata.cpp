#include "cimportdata.h"
#include <QDebug>
#include <QThread>
#include <QApplication>

#include <QSqlDatabase>
#include <QSqlError>

#include <QMessageBox>
#include <QWidget>

#include <iostream>

extern QSqlDatabase db;
//=========================================================
CImportData::CImportData(QObject *parent) : QObject(parent)
{
    query = nullptr;
    query = new QSqlQuery(db);
}

//=========================================================
CImportData::~CImportData()
{
    if(query != nullptr)
        delete query;
}

//=========================================================
void CImportData::doWork(const QString &fileName)
{
    QString result,tmp;
    QXlsx::Cell *cell;
    int col_Ok = 0;

    QXlsx::Document xlsx(fileName);
    emit resultReady("Конец разбора файла...\nНачало записи данных в БД АВП...");

    bool flag_error = false;
    int row = 2;
    int row_null = 0;
    cell=xlsx.cellAt(row, 10);
    while((row_null<600))
    {
        if((cell!= Q_NULLPTR) && (cell->value().toString()!=""))
        {
             cell=xlsx.cellAt(row, 1);
             if(cell!= Q_NULLPTR)
             {
//                 qDebug()<<"row 1="<<row<<" "<<cell->value();
                 m_sDataAVP.urlKinopoisk = cell->value().toString();
             }
             else
             {
                 m_sDataAVP.urlKinopoisk = "";
             }

             cell=xlsx.cellAt(row, 2);
             if(cell!= Q_NULLPTR)
             {
//                qDebug()<<"row 2="<<row<<" "<<cell->value();
                 m_sDataAVP.urlIMDB = cell->value().toString();
             }
             else
             {
                 m_sDataAVP.urlIMDB = "";
             }

             cell=xlsx.cellAt(row, 4);
             if(cell!= Q_NULLPTR)
             {
                 if(cell->isDateTime())
                 {
//                    qDebug()<<"row 4 isDateTime="<<row<<" "<<cell->dateTime();
                    m_sDataAVP.dateSaveInDB = cell->dateTime();
                 }
             }

             cell=xlsx.cellAt(row, 5);
             if(cell!= Q_NULLPTR)
             {
//                qDebug()<<"row 5="<<row<<" "<<cell->value();
                m_sDataAVP.avsName = cell->value().toString();
             }

             cell=xlsx.cellAt(row, 6);
             if(cell!= Q_NULLPTR)
             {
//                 qDebug()<<"row 6="<<row<<" "<<cell->value();
                 m_sDataAVP.avsURL = cell->value().toString();
             }

             cell=xlsx.cellAt(row, 7);
             if(cell!= Q_NULLPTR)
             {
//                 qDebug()<<"row 7="<<row<<" "<<cell->value();
                 m_sDataAVP.avpNameRus = cell->value().toString();
             }

             cell=xlsx.cellAt(row, 9);
             if(cell!= Q_NULLPTR)
             {
//                qDebug()<<"row 9="<<row<<" "<<cell->value();
                m_sDataAVP.rubric = cell->value().toString();
             }

             cell=xlsx.cellAt(row, 10);
             if(cell!= Q_NULLPTR)
             {
//                 qDebug()<<"row 10="<<row<<" "<<cell->value();
                 m_sDataAVP.avpURL = cell->value().toString();
             }

             cell=xlsx.cellAt(row, 11);
             if(cell!= Q_NULLPTR)
             {
//                qDebug()<<"row 11="<<row<<" "<<cell->value();
                 m_sDataAVP.avpNameOriginal = cell->value().toString();
             }
             else
                 m_sDataAVP.avpNameOriginal = "";

             cell=xlsx.cellAt(row, 12);
             if(cell!= Q_NULLPTR)
             {
//                 qDebug()<<"row 12="<<row<<" "<<cell->value();
                 m_sDataAVP.age = cell->value().toString();
             }

             cell=xlsx.cellAt(row, 13);
             if(cell!= Q_NULLPTR)
             {
//                 qDebug()<<"row 13="<<row<<" "<<cell->value();
                 m_sDataAVP.filmMaker = cell->value().toString();
             }

             cell=xlsx.cellAt(row, 14);
             if(cell!= Q_NULLPTR)
             {
//                 qDebug()<<"row 14="<<row<<" "<<cell->value();
                 m_sDataAVP.yearOfRelease = cell->value().toString();
             }

             cell=xlsx.cellAt(row, 21);
             if(cell!= Q_NULLPTR)
             {
//                 qDebug()<<"row 21="<<row<<" "<<cell->value();
//                 if()
                 m_sDataAVP.duration = cell->value().toString();
             }

             cell=xlsx.cellAt(row, 24);
             if(cell!= Q_NULLPTR)
             {
//                 qDebug()<<"row 24="<<row<<" "<<cell->value();
                 m_sDataAVP.userSaveInDB = cell->value().toString();
             }

             try
             {
                if(addSaveInDB(m_sDataAVP))
                {
                    col_Ok++;
                    if(col_Ok%100 == 0)
                    {
                        result = "Загрузка данных АВП: ";
                        result +=tmp.setNum(col_Ok);
                        result +=" записей";
//                        result += m_sDataAVP.avpURL;
                        result += "........OK";
                        emit resultReady(result);
                    }
                }
                else
                {
                    result = "Загрузка данных АВП ";
                    result +=tmp.setNum(row); result +=" ";
                    result += m_sDataAVP.avpURL; result += "........FAIL";
                    flag_error = true;
                    emit resultReady(result);
                }
             }
             catch (std::exception &e)
             {
                 qDebug()<<e.what();
             }

         }
         else
         {
            row_null++;
            qDebug()<<"===================="<<row_null;
         }
        qDebug()<<"row0="<<row<<" col_Ok="<<col_Ok<<"row_null="<<row_null;
        row++;
        cell=xlsx.cellAt(row, 10);
        if(cell==Q_NULLPTR)
            qDebug()<<"xlsx.cellAt(row, 10)=Q_NULLPTR row="<<row;
        else
            qDebug()<<"xlsx.cellAt(row, 10)!=Q_NULLPTR row="<<row<<" "<<cell->value().toString();
        qDebug()<<"row1="<<row<<" col_Ok="<<col_Ok<<"row_null="<<row_null;
    }
    result="Запись данных о " + tmp.setNum(col_Ok)+" АВП завершена успешно!";
    emit resultReady(result);
    emit resultReady("END");
    if(flag_error == true)
        emit resultReady("Загрузка данных произошла с ОШИБКАМИ!!!\nПроверьте правильность данных в файле excel!");
}

//=========================================================
bool CImportData::existAVS(QString &url, QString name)
{
    qDebug()<<__PRETTY_FUNCTION__;
    bool res=true;
    QString sql="";

    sql = "SELECT * FROM avs WHERE \"URL\"=\'"+url+"\' AND \"NameAVS\"=\'"+name+"\';";
//    qDebug()<<sql;
    if(!query->exec(sql))
    {
        emit resultReady(query->lastError().text());
        qDebug()<<query->lastError().text();
        res = false;
    }
    else
    {
        if(query->next())
            res = true;
        else
            res = false;
    }
    return res;
}

//=========================================================
bool CImportData::existAVP(const QString &url)
{
    qDebug()<<__PRETTY_FUNCTION__;
    bool res=true;
    QString sql="";
    sql = "SELECT * FROM avp WHERE \"URL\"=\'"+url+"\';";
    qDebug()<<sql;
    if(!query->exec(sql))
    {
        emit resultReady(query->lastError().text());
        res = false;
    }
    else
    {
        if(query->next())
            res = true;
        else
            res = false;
    }
    return res;
}

//=========================================================
bool CImportData::existUser(QString &fio)
{
    qDebug()<<__PRETTY_FUNCTION__;
    bool res=true;
    QString sql="";
    sql = "SELECT * FROM \"User\" WHERE \"FIO\"=\'"+fio+"\';";
//    qDebug()<<sql;
    if(!query->exec(sql))
    {
        emit resultReady(query->lastError().text());
        res = false;
    }
    else
    {
        if(query->next())
            res = true;
        else
            res = false;
    }
    return res;
}

//=========================================================
bool CImportData::addSaveInDB(SDataAVP &m_sDataAVP)
{
    qDebug()<<__PRETTY_FUNCTION__;
//    qDebug()<<"CImportData::addSaveInDB(SDataAVP &m_sDataAVP)="<<m_sDataAVP.avsURL;
    bool res=true;
    QString sql="";
    if(!existAVS(m_sDataAVP.avsURL,m_sDataAVP.avsName))
    {
        sql="INSERT INTO avs(\"URL\",\"NameAVS\")VALUES(\'";
        sql+=m_sDataAVP.avsURL;sql+="\',\'";
        sql+=m_sDataAVP.avsName;sql+="\');";
        qDebug()<<sql;
        if(!query->exec(sql))
        {
            emit resultReady(query->lastError().text());
            qDebug()<<query->lastError().text();
            res = false;
        }
        else
            res = true;
    }
    if(!existUser(m_sDataAVP.userSaveInDB))
        addUser(m_sDataAVP.userSaveInDB);

    if(!existAVP(m_sDataAVP.avpURL))
    {
        if(!addAVP(m_sDataAVP))
            res = false;
    }
    else
        res = false;
    return res;
}

//=========================================================
bool CImportData::addUser(QString &fio)
{
    bool res=true;
    QString sql="";
    QString tmp;
    sql="INSERT INTO \"User\" (\"FIO\") VALUES (\'";
    sql += fio;sql+="\');";
    qDebug()<<sql;
    if(!query->exec(sql))
    {
        emit resultReady(query->lastError().text());
        res = false;
    }
    else
    {
        res = true;
    }
    return res;
}

//=========================================================
QString CImportData::decode(QString str)
{
    QString newString="";
    for(int i=0; i<str.length();++i)
    {
        if((str[i]=="'") || (str[i]=="\"") || (str[i]=="\\"))
        {
//            if(str[i]=="'")
//            {
                newString += "\\";
                newString += str[i];
//            }
//            else if(str[i]=="'")

        }
        else
        {
            newString += str[i];
        }
    }
//    qDebug().noquote() <<"newString="<<newString;
//    std::cout << newString.toStdString() <<std::endl;
    return newString;
}

//=========================================================
bool CImportData::addAVP(SDataAVP &m_sDataAVP)
{
    bool res=true;
    QString sql="";
    QString tmp;
    sql="INSERT INTO avp (\"NameRus\",\"URL\",\"ID_AVS\",\"NameOriginal\",\"URL_kinopoisk\",\"URL_imdb\") VALUES (E\'";
    sql += decode(m_sDataAVP.avpNameRus);sql+="\',\'";
    sql += m_sDataAVP.avpURL; sql+="\',";
    sql += findIdAVS(m_sDataAVP.avsURL,m_sDataAVP.avsName);sql+=",E\'";
    sql += decode(m_sDataAVP.avpNameOriginal); sql+="\',\'";
    sql += m_sDataAVP.urlKinopoisk; sql+="\',\'";
    sql += m_sDataAVP.urlIMDB; sql+="\');";
    qDebug()<<sql;
    if(!query->exec(sql))
    {
        qDebug()<<sql;
        qDebug()<<query->lastError().text();
        emit resultReady(query->lastError().text());
        res = false;
    }
    else
    {
        res = true;
        sql = "INSERT INTO \"AVPattribute\" (\"DateSaveInDB\",\"Rubric\",\"FilmMaker\",\"YearOfRelease\",\"Age\",\"Duration\",\"ID_AVP\",\"ID_User\") VALUES ('";
        sql += tmp.setNum(m_sDataAVP.dateSaveInDB.date().year());sql += "-";
        sql += tmp.setNum(m_sDataAVP.dateSaveInDB.date().month());sql += "-";
        sql += tmp.setNum(m_sDataAVP.dateSaveInDB.date().day());
        sql+="\',\'";
        sql += m_sDataAVP.rubric;sql+="\',\'";
        sql += m_sDataAVP.filmMaker;sql+="\',\'";
        sql += m_sDataAVP.yearOfRelease;sql+="\',\'";
        sql += m_sDataAVP.age;sql+="\',\'";
        sql += m_sDataAVP.duration;sql+="\',";
        sql += findIdAVP(m_sDataAVP.avpURL) ;sql+=",";
        sql += findIdUser(m_sDataAVP.userSaveInDB) ;sql+=");";
//        qDebug()<<sql;
        if(!query->exec(sql.toStdString().c_str()))
        {
            qDebug()<<sql;
            qDebug()<<query->lastError().text();
//            emit resultReady(query->lastError().text());
            res = false;
        }
        else
        {
            res = true;
        }

        // Save data above imdb
        if(m_sDataAVP.urlIMDB!="")
        {
            sql = "INSERT INTO \"DownloadData\" (\"ResourceName\",\"URL\",\"DownloadStatus\",\"ID_AVP\") VALUES ('";
            sql += "IMDB";sql += "\',\'";
            sql += m_sDataAVP.urlIMDB;sql += "\',\'";
            sql += "No"; sql += "\',";
            sql += findIdAVP(m_sDataAVP.avpURL) ;sql+=");";
            if(!query->exec(sql.toStdString().c_str()))
            {
                qDebug()<<sql;
                qDebug()<<query->lastError().text();
//            emit resultReady(query->lastError().text());
                res = false;
            }
            else
            {
                res = true;
            }
        }

        // Save data about the kinopoisk
        if(m_sDataAVP.urlKinopoisk!="")
        {
            sql = "INSERT INTO \"DownloadData\" (\"ResourceName\",\"URL\",\"DownloadStatus\",\"ID_AVP\") VALUES ('";
            sql += "Kinopoisk";sql += "\',\'";
            sql += m_sDataAVP.urlKinopoisk;sql += "\',\'";
            sql += "No"; sql += "\',";
            sql += findIdAVP(m_sDataAVP.avpURL) ;sql+=");";
//            qDebug()<<sql;
            if(!query->exec(sql.toStdString().c_str()))
            {
                qDebug()<<sql;
                qDebug()<<query->lastError().text();
//            emit resultReady(query->lastError().text());
                res = false;
            }
            else
            {
                res = true;
            }
        }

    }
    return res;
}

//=========================================================
QString CImportData::findIdUser(QString &fio)
{
    QString id_user="";
    QString sql="";
    sql = "SELECT \"ID\" FROM \"User\" WHERE \"FIO\"=\'"+fio+"\';";
//    qDebug()<<sql;
    if(!query->exec(sql))
    {
        emit resultReady(query->lastError().text());
        id_user = "0";
    }
    else
    {
        if(query->next())
        {
            id_user = query->value(0).toString();
        }
    }
    return id_user;
}

//=========================================================
QString CImportData::findIdAVS(QString &url, QString name)
{
    QString id_avs="";
    QString sql="";
    sql = "SELECT \"ID\" FROM avs WHERE \"URL\"=\'"+url+"\' AND \"NameAVS\"=\'"+name+"\';";
//    qDebug()<<sql;
    if(!query->exec(sql))
    {
        emit resultReady(query->lastError().text());
        id_avs = "0";
    }
    else
    {
        if(query->next())
        {
            id_avs = query->value(0).toString();
        }
    }
    return id_avs;
}

//=========================================================
QString CImportData::findIdAVP(QString &url)
{
    QString id_avp="";
    QString sql="";
    sql = "SELECT \"ID\" FROM avp WHERE \"URL\"=\'"+url+"\';";
//    qDebug()<<sql;
    if(!query->exec(sql))
    {
        emit resultReady(query->lastError().text());
        id_avp = "0";
    }
    else
    {
        if(query->next())
        {
            id_avp = query->value(0).toString();
        }
    }
    return id_avp;
}
