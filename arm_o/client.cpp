#include <QtNetwork>

#include "client.h"
#include "mainwindow.h"
#include "aprotocol.h"

#include <unistd.h>
#include <iostream>

using namespace std;
using namespace   AProtocol;
#define TIMER_PERIOD 1500

#define PrintHex(s)  for(string::iterator pos=s.begin();pos!=s.end() ;++pos ){printf(" 0x%02X", (unsigned char)(*pos));} cout<<endl;
#define PrintHexQString(s)  for(int i=0;i<s.length();i++){printf(" 0x%02X", (unsigned char)s[i].toAscii());} cout<<endl;
//=========================================================
Client::Client(QString server_ip_address, int server_ip_port, QObject *parent) : QObject(parent)
{
    qDebug()<<__PRETTY_FUNCTION__;
    blockSize = 0;

    serverIPAddress = server_ip_address;
    serverIPPort = server_ip_port;

    //Создать объект класса сетевого соединения.
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket,  &QTcpSocket::connected,this, &Client::slotConnected);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &Client::slotDisconnected);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &Client::slotReadyRead);

    //Создать таймер.
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(slotTimerTimeout()));
    timer->start(TIMER_PERIOD);
}

//=========================================================
Client::~Client()
{
    timer->stop();
    if(tcpSocket->state() == QAbstractSocket::ConnectedState)
        tcpSocket->disconnectFromHost();
}

//=========================================================
void Client::reconnect()
{
    timer->stop();
    tcpSocket->abort();
    timer->start(TIMER_PERIOD);
}

//=========================================================
QString Client::getState()
{
//    qDebug()<<__PRETTY_FUNCTION__;
    QString state_connection;

    switch(tcpSocket->state())
   {
       case QAbstractSocket::UnconnectedState: state_connection = tr("Отсутствует сетевое соединение с сервером..."); break;
       case QAbstractSocket::HostLookupState:  state_connection = tr("Поиск сервера..."); break;
       case QAbstractSocket::ConnectingState:  state_connection = tr("Поиск сервера..."); break;
       case QAbstractSocket::ConnectedState:   state_connection = tr("Сетевое соединение с сервером активно..."); break;
       case QAbstractSocket::ClosingState:     state_connection = tr("Производится закрытие сетевого соединения с сервером..."); break;
       default: break;
   }
   return state_connection;
}

//=========================================================
void Client::sendMessage(QString message)
{
    qDebug()<<__PRETTY_FUNCTION__;
    tcpSocket->write(message.toStdString().data(),message.length());
}

//=========================================================
void Client::slotConnected()
{
    qDebug()<<__PRETTY_FUNCTION__<<" установлено соединение с сервером ";
    emit connected();
}

//=========================================================
void Client::slotDisconnected()
{
    qDebug()<<__PRETTY_FUNCTION__<<" разорвано соединение с сервером ";
    emit disconnected();
}

//=========================================================
void Client::slotReadyRead()
{
	qDebug()<<__PRETTY_FUNCTION__;
    QByteArray request = tcpSocket->readAll();
    qDebug()<<"request.length() ="<<request.length()<<" HEX: "<<request.toHex();
    for(int i=0; i<request.length(); i++)
    {
        printf(" 0x%02X", (unsigned char)request[i]);
    }
    std::cout<<std::endl;

    header *headerCommand = reinterpret_cast<header*>(request.data());
    if(headerCommand->command == 3)
    {
        answerAnalysisAPV* command = reinterpret_cast<answerAnalysisAPV*>(request.data());
        qDebug()<<"command->command:"<<command->command<<" command->length:"<<command->length<<"command->idAVP:"<<command->idAVP<<"command->status:"<<command->status;
        if(command->status == 0x01)
            ((MainWindow*)parent())->initTableMyTask();
        else if(command->status == 0x02)
        {
            ((MainWindow*)parent())->changeStatusAVP(command->idAVP, command->status);
        }
    }
}

//=========================================================
void Client::slotTimerTimeout()
{
    //    qDebug()<<__PRETTY_FUNCTION__;
    emit state(getState());

    //Активировать новое сетевое соединение, при условии что оно не активно.
    if(tcpSocket->state() == QAbstractSocket::UnconnectedState)
    {
//        qDebug()<<__PRETTY_FUNCTION__<<" установка соединения ";
        tcpSocket->connectToHost(serverIPAddress, serverIPPort);
    }
//    else if(tcpSocket->state() == QAbstractSocket::ConnectedState)
//    {
//        qDebug()<<__PRETTY_FUNCTION__<<" Соединение установлено!";
//    }
}

//=========================================================
void Client::sendCommandAnalysisAVP(uint64_t idAVP)
{
    qDebug()<<__PRETTY_FUNCTION__;
    AProtocol::commandAnalysis m_commandAnalysis;
    m_commandAnalysis.command = 1;
    m_commandAnalysis.length = sizeof(uint64_t);
    m_commandAnalysis.idAVP = idAVP;
    tcpSocket->write(reinterpret_cast<char*>(&m_commandAnalysis), sizeof(m_commandAnalysis));
}

