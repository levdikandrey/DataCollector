#include <QtNetwork>

#include "client.h"
#include "mainwindow.h"

#include <unistd.h>
#include <iostream>

using namespace std;

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
	int length;
	qDebug()<<__PRETTY_FUNCTION__;
	QDataStream in(tcpSocket);
	in.setVersion(QDataStream::Qt_4_0);
	
	length=tcpSocket->bytesAvailable();
    qDebug()<<"length="<<tcpSocket->bytesAvailable();
	

    QByteArray request = tcpSocket->readAll();
    qDebug()<<"request.length() ="<<request.length()<<" HEX: "<<request.toHex();
    for(int i=0; i<request.length(); i++)
    {
        printf(" 0x%02X", (unsigned char)request[i]);
    }
    std::cout<<std::endl;



	//Если пришла первая часть из посланной сервером информации.
	if(blockSize == 0)
	{
		message="";
		//Если первая часть меньше того кол-ва информации что определяет размер всего сообщения...
		if(tcpSocket->bytesAvailable() < (int)sizeof(quint16))
			return;
		in >> start_byte;
	        qDebug()<<"start_byte="<<start_byte;
		for(int j=1; j<=tcpSocket->bytesAvailable();j++)
		{
			if((start_byte==0xd9))
			{
				break;
			}
			else if((start_byte!=0xd9) && (length!=0))
			{
//				length--;
				in >> start_byte;
			}
			else if((start_byte!=0xd9) && (length==j))
			{	
				return;
			}
		}
		message+=start_byte;
	
		//Получить размер посылаемого сервером сообщения.
		in >> blockSize;
	        qDebug()<<"blockSize="<<blockSize;
		message+=blockSize;
	}
	//Если последующие части вместе взятые меньше, чем определенное сервером кол-во...
	if(tcpSocket->bytesAvailable() < blockSize)
		return;
	
	for(int i=0; i<blockSize; i++)
	{
		in >> command_byte;
	//        qDebug()<<"command_byte="<<command_byte;
		message+=command_byte;
	}
	qDebug()<<"message.length()="<<message.length();
//	PrintHexQString(message);
	
	//Обнулить параметр размера посылаемого сервером сообщения.
	blockSize = 0;
	emit receiveMessage(message);
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
//        usleep(250000);
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

