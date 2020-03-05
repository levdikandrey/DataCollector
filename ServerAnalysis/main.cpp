#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QSqlDatabase>
#include <QThread>

#include "threadreadqueue.h"
//#include "threadanalysisavp.h"

#include "aserver.h"
#include "aclient.h"

#include <deque>

#ifdef ENABLE_EPOOL
#include "qeventdispatcher_epoll.h"
#endif

constexpr size_t DEFAULT_TREADS = 4;
constexpr size_t DEFAULT_PORT = 50100;

std::deque<SCommand> listAVP;
QSqlDatabase db;

int main(int argc, char** argv)
{
#ifdef ENABLE_EPOOL
    QCoreApplication::setEventDispatcher(new QEventDispatcherEpoll);
#endif
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("ServerAnalysis");
    QCoreApplication::setApplicationVersion("0.1");

    qRegisterMetaType<qintptr>("qintptr");
    qRegisterMetaType<SCommand>("SCommand");
    qRegisterMetaType<uint64_t>("uint64_t");
    setlocale(LC_ALL,"Russian");

    QCommandLineParser parser;
    parser.setApplicationDescription("Multi-threaded server analysis");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption portOption(QStringList() << "p" << "port", "port",
            "port");
    parser.addOption(portOption);

    QCommandLineOption threadOption(QStringList() << "t" << "threads",
            "The number of worker threads", "count");
    parser.addOption(threadOption);

    parser.process(app);

    size_t port = parser.value(portOption).toUInt();
    if (port == 0)
    {
        port = DEFAULT_PORT;
    }

    size_t threads = parser.value(threadOption).toUInt();
    if (threads == 0)
    {
        threads = DEFAULT_TREADS;
    }

//    //Инициализируем поток для чтения из очереди ИД АВП для анализа
//    QThread *m_threadReadQueue = new QThread(this);
//    ThreadReadQueue *m_TRQ = new ThreadReadQueue();
//    m_TRQ->moveToThread(m_threadReadQueue);
//    connect(m_threadReadQueue, &QThread::finished, m_TRQ, &QObject::deleteLater);
//    connect(this, &AServer::operateReadQueue, m_TRQ, &ThreadReadQueue::doWork);
//    connect(m_TRQ, &ThreadReadQueue::resultReady, this, &AServer::handleResultsReadQueue);

//    m_threadReadQueue->start();
//    emit operateReadQueue("");
//    qDebug()<<"The AVP list queue reading thread is started";

//    //Инициализируем поток для анализа АВП из БД АВП
//    m_threadAnalysisAVP = new QThread(this);
//    m_taAVP = new ThreadAnalysisAVP();
//    m_taAVP->moveToThread(m_threadAnalysisAVP);
//    connect(m_threadAnalysisAVP, &QThread::finished, m_taAVP, &QObject::deleteLater);
//    connect(this, &AServer::operateAnalysis, m_taAVP, &ThreadAnalysisAVP::doWork);
//    connect(m_taAVP, &ThreadAnalysisAVP::resultReady, this, &AServer::handleResultsAnalysis);

//    m_threadAnalysisAVP->start();
//    emit operateAnalysis("");

    AServer server(threads);

    if (!server.listen(QHostAddress::Any, port))
    {
        qCritical() << server.errorString();
        exit(1);
    }

    qDebug() << "Run server analysis on" << port << "threads" << threads;
    return app.exec();
}
