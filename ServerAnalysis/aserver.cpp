#include "aserver.h"
#include "worker.h"

#include <stdlib.h>
#include <QThread>
#include <QSqlDatabase>


#ifdef ENABLE_EPOOL
#include "qeventdispatcher_epoll.h"
#endif

extern QSqlDatabase db;
//=========================================================
AServer::AServer(size_t threads, QObject * parent) :
        QTcpServer(parent),
        m_threadCount(threads),
        m_rrcounter(0)
{
    initDB();
    initThreads();
}

//=========================================================
AServer::~AServer()
{
    for(QThread* thread: m_threads)
    {
        thread->quit();
        thread->wait();
    }
    m_threadReadQueue->quit();
    m_threadReadQueue->wait();
}

//=========================================================
bool AServer::initDB()
{
//    qDebug()<<__PRETTY_FUNCTION__;
    bool res = true;
    try
    {
        db = QSqlDatabase::addDatabase("QPSQL");
        db.setHostName("127.0.0.1");
        db.setDatabaseName("avpDB");
        db.setUserName("postgres");
        db.setPassword("postgres");
        bool ok = db.open();
        if(ok)
        {
            qDebug()<<"Connect to DB AVP";
        }
        else
        {
            qDebug()<<"Error: Connect to DB AVP!";
            res = false;
        }
    }
    catch(std::exception &e)
    {
        qDebug()<<e.what();
    }
    return res;
}

//=========================================================
void AServer::initThreads()
{
        //Инициализируем поток для чтения из очереди ИД АВП для анализа
        QThread *m_threadReadQueue = new QThread(this);
        m_TRQ = new ThreadReadQueue();
//        qDebug()<<"m_TRQ = "<<m_TRQ;
        m_TRQ->moveToThread(m_threadReadQueue);
        connect(m_threadReadQueue, &QThread::finished, m_TRQ, &QObject::deleteLater);
        connect(this, &AServer::operateReadQueue, m_TRQ, &ThreadReadQueue::doWork);
        connect(m_TRQ, &ThreadReadQueue::resultReady, this, &AServer::handleResultsReadQueue);

        m_threadReadQueue->start();
        emit operateReadQueue("");
        qDebug()<<"The AVP list queue reading thread is started";

        //Инициализируем поток для анализа АВП из БД АВП
        m_threadAnalysisAVP = new QThread(this);
        m_taAVP = new ThreadAnalysisAVP();
        m_taAVP->moveToThread(m_threadAnalysisAVP);
        connect(m_threadAnalysisAVP, &QThread::finished, m_taAVP, &QObject::deleteLater);
        connect(this, &AServer::operateAnalysis, m_taAVP, &ThreadAnalysisAVP::doWork);
        connect(m_taAVP, &ThreadAnalysisAVP::resultReady, this, &AServer::handleResultsAnalysis);

        m_threadAnalysisAVP->start();
        emit operateAnalysis("");

    for (size_t i = 0; i < m_threadCount; ++i)
    {
        QThread* thread = new QThread(this);
#ifdef ENABLE_EPOOL
        thread->setEventDispatcher(new QEventDispatcherEpoll);
#endif
        Worker* worker = new Worker();
        worker->moveToThread(thread);
        connect(thread, &QThread::finished, worker, &QObject::deleteLater);

        m_threads.push_back(thread);
        m_workers.push_back(worker);

        thread->start();
    }
    qDebug()<<"The AVP analysis thread is started";
}

//=========================================================
void AServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<__PRETTY_FUNCTION__;
    Worker* worker = m_workers[m_rrcounter % m_threadCount];
    ++m_rrcounter;
    QMetaObject::invokeMethod(worker, "addClient", Qt::QueuedConnection,Q_ARG(qintptr, socketDescriptor),Q_ARG(ThreadReadQueue*, m_TRQ));
}

//=========================================================
void AServer::handleResultsReadQueue(const SCommand &)
{
//    command.client->sendAnswerAnalysisAVP(command.idAVP);
}

//=========================================================
void AServer::handleResultsAnalysis(const QString &)
{
}

