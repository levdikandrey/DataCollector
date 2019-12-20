#include "cdownloader.h"
#include <QMutex>
#include <QWaitCondition>
#include <QThread>

//extern QMutex m_mutex;
//extern QWaitCondition cond;
//extern bool kvit;

//=========================================================
CDownLoader::CDownLoader(QObject *parent) : QObject(parent)
{
    currentFileName="";
}

//=========================================================
void CDownLoader::doWork(const QString &)
{
    QString result;
    qDebug()<<"Запуск потока!!! getpid() = "<<getpid();
    // Инициализируем менеджер ...
    manager = new QNetworkAccessManager();
    // ... и подключаем сигнал о завершении получения данных к обработчику полученного ответа
    connect(manager, &QNetworkAccessManager::finished, this, &CDownLoader::onResult);
    qDebug()<<"CDataCollectionService::run()1";
    getData("https://www.kinopoisk.ru/film/1045781/");

    emit resultReady(result);
}

//=========================================================
void CDownLoader::getData(const QString &url_path)
{
//    m_mutex.lock();
    qDebug()<<"CDownLoader::getData(const QString &url_path) getpid()="<<getpid();
    currentFileName = "c:\\example\\";
    currentFileName += url_path.section('/',-2,-2);
    currentFileName += ".html";
    qDebug()<<"currentFileName="<<currentFileName;
    QUrl url;
    url.setUrl(url_path);       // URL, к которому будем получать данные
    QNetworkRequest request;    // Отправляемый запрос
    request.setUrl(url);        // Устанавлвиваем URL в запрос
    qDebug()<<"manager->get(request)";
    manager->get(request);      // Выполняем запрос
    qDebug()<<"manager->get(request)1";

//    m_mutex.unlock();
}

//=========================================================
void CDownLoader::onResult(QNetworkReply *reply)
{
    qDebug()<<"CDownLoader::onResult(QNetworkReply *reply) getpid() ="<<getpid();
//    qDebug()<<"reply->readAll()="<<reply->readAll();
    // Если в процесе получения данных произошла ошибка
    if(reply->error())
    {
        // Сообщаем об этом и показываем информацию об ошибках
        qDebug() << "ERROR";
        qDebug() << reply->errorString();
    }
    else
    {
        QString answer = QString::fromUtf8(reply->readAll());
//        qDebug() << "answer------------>"<<answer;
        // В противном случае создаём объект для работы с файлом
        QFile *file = new QFile(currentFileName);
        // Создаём файл или открываем его на перезапись ...
        if(file->open(QFile::WriteOnly))
        {
//            file->write(reply->readAll());  // ... и записываем всю информацию со страницы в файл
            file->write(answer.toStdString().c_str());  // ... и записываем всю информацию со страницы в файл
            file->close();                  // закрываем файл
            qDebug() << "Downloading is completed";
            emit onReady(); // Посылаем сигнал о завершении получения файла
        }
    }
    reply->deleteLater();
//    kvit = true;
//    cond.wakeAll();
}
