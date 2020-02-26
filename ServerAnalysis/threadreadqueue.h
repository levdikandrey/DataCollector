#ifndef THREADREADQUEUE_H
#define THREADREADQUEUE_H

#include <QObject>
#include <QSqlQuery>

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
    uint64_t idAVP;
    QSqlQuery *query;
};

#endif // THREADREADQUEUE_H
