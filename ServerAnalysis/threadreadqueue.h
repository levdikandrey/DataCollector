#ifndef THREADREADQUEUE_H
#define THREADREADQUEUE_H

#include <QObject>
#include <QSqlQuery>

#include "aclient.h"

class ThreadReadQueue: public QObject
{
    Q_OBJECT

public:
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
    void resultReady(const QString &result);

private:
    void analysisAVP(uint64_t idAVP);
    uint64_t idViolation(QString nameViolation);
    void addAnalysisResult(uint64_t idAVP, uint64_t idViolation, QString percent);

    uint64_t idAVP;
    QSqlQuery *query;
    SCommand m_sCommand;
};

#endif // THREADREADQUEUE_H
