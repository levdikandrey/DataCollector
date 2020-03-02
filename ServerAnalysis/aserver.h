#ifndef ASERVER_H
#define ASERVER_H
#pragma once

#include <QVector>
#include <QTcpServer>
#include <QDebug>

#include "threadreadqueue.h"
#include "threadanalysisavp.h"

class Worker;
class QThread;

class AServer: public QTcpServer
{
Q_OBJECT

public:
    AServer(size_t threads = 4, QObject * parent = nullptr);
    ~AServer();

protected:
    virtual void incomingConnection(qintptr socketDescriptor);

private:
    void initThreads();
    bool initDB();

signals:
    void operateReadQueue(const QString &);
    void operateAnalysis(const QString &);

public slots:
    /**
     * @brief handleResults
     */
    void handleResultsReadQueue(const QString &);
    void handleResultsAnalysis(const QString &);

private:
    size_t m_threadCount;
    size_t m_rrcounter;

    QThread *m_threadReadQueue;
    ThreadReadQueue *m_TRQ;

    QThread *m_threadAnalysisAVP;
    ThreadAnalysisAVP *m_taAVP;

    QVector<QThread*> m_threads;
    QVector<Worker*> m_workers;
};

#endif // ASERVER_H
