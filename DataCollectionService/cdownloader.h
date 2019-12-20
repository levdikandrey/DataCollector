#ifndef CDOWNLOADER_H
#define CDOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QUrl>
#include <QDebug>

class CDownLoader : public QObject
{
    Q_OBJECT

public:
    explicit CDownLoader(QObject *parent = nullptr);

signals:
    void onReady();
    void resultReady(const QString &result);

public slots:
    void getData(const QString &url);       // Метод инициализации запроса на получение данных
    void onResult(QNetworkReply *reply);    // Слот обработки ответа о полученных данных

public slots:
    /**
     * @brief doWork - функция выполнения потока
     */
    void doWork(const QString &);


private:
    QNetworkAccessManager *manager;         // менеджер сетевого доступа
    QString currentFileName;
};

#endif // CDOWNLOADER_H
