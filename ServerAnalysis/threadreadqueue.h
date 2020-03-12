#ifndef THREADREADQUEUE_H
#define THREADREADQUEUE_H

#include <QObject>
#include <QSqlQuery>

#include "aclient.h"
#include "requestsender.h"

using namespace Network;
/**
 * @brief The ThreadReadQueue class
 */
class ThreadReadQueue: public QObject
{
    Q_OBJECT

public:
    /**
     * @brief ThreadReadQueue
     */
    ThreadReadQueue();
    /**
     * @brief doWork - функция выполнения потока
     */
    void doWork();

signals:
    /**
     * @brief resultReady - функция для возврата значения в вызывающий поток
     * @param result - возращаемое значение
     */
    void resultReady(const SCommand &);
    /**
     * @brief sendAnswerAnalysisAVP
     */
    void sendAnswerAnalysisAVP(const SCommand &);

private:
    /**
     * @brief analysisAVP - функция анализа АВП
     * @param idAVP - идентификатор АВП
     */
    void analysisAVP(uint64_t idAVP);
    /**
     * @brief idViolation - функция возвращает идентификатор нарушения
     * @param nameViolation - наименование нарушения
     * @return - идентификатор нарушения
     */
    uint64_t idViolation(QString nameViolation) const;
    /**
     * @brief addAnalysisResult - функция добавляет проанализированное нарушение в БД АВП
     * @param idAVP - идентификатор АВП
     * @param idViolation - идентификатор нарушения
     * @param percent - процен вероятности нарушения
     */
    void addAnalysisResult(uint64_t idAVP, uint64_t idViolation, QString percent);
    /**
     * @brief idAVS - функция возвращает идентификатор АВС
     * @param idAVP - идентификатор АВП
     * @return - идентификатор АВС
     */
    uint64_t idAVS(uint64_t idAVP) const;
    /**
     * @brief pathOnDisk - функция возвращает путь к файлу рецензии для АВП
     * @param idAVP - идентификатор АВП
     * @return - путь к файлу
     */
    QString getPathOnDisk(uint64_t idAVP) const;
    QString getURL(uint64_t idAVP) const;
    uint64_t getIdDownloadData(uint64_t idAVP) const;
    bool getData(const QString &url_path, const QString &fileName);
    bool changeSaveInDB(uint64_t idAVP, QString &currentFileName);

    uint64_t m_idAVP;
    uint64_t m_idAVS;
    QSqlQuery *query;
    AClient *client;
    SCommand m_sCommand;
    RequestSender *m_requestSender;
    Request m_request;
};

#endif // THREADREADQUEUE_H
