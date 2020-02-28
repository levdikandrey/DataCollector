#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QSqlDatabase>

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

    AServer server(threads);

    if (!server.listen(QHostAddress::Any, port))
    {
        qCritical() << server.errorString();
        exit(1);
    }

    qDebug() << "Run server analysis on" << port << "threads" << threads;
    return app.exec();
}
