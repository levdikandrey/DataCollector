#include <QCoreApplication>
#include <QDebug>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>

#include "cdatacollectionservice.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    setlocale(LC_ALL,"Russian");
    qDebug() << " QSslSocket::supportsSsl() = "<< QSslSocket::supportsSsl() << "\n"
             << "QSslSocket::sslLibraryBuildVersionString() = "<<QSslSocket::sslLibraryBuildVersionString() << "\n"
             << "QSslSocket::sslLibraryVersionString() = "<< QSslSocket::sslLibraryVersionString();

    CDataCollectionService &m_dcs = CDataCollectionService::getInstance();
    qDebug()<<"Сервис сбора данных запушен!";
    qDebug()<<"main getpid()="<<getpid();
    m_dcs.run();
    return a.exec();
}
