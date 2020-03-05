#ifndef ACLIENT_H
#define ACLIENT_H

#pragma once


//#include "aprotocol.h"

#include <QObject>
#include <QTcpSocket>

//using namespace AProtocol;

class AClient;
class ThreadReadQueue;

struct SCommand
{
    uint64_t idAVP;
    AClient *client;
    uint8_t answerState;
};

class AClient: public QObject
{
Q_OBJECT

public:
    AClient(qintptr socketDescriptor, ThreadReadQueue* threadReadQueue, QObject* parent = 0);

public slots:
    void onReadyRead();
    void onClientDisconnected();
    void onClientConnected();

    void receiveCommand(QString &command);
    void sendAnswerAnalysisAVP(const SCommand &command);

signals:
   void connected();
   void disconnected();
   void receiveMessage(QString &mess);

private:
    void done();

    /**
         Размер посылаемого сервером сообщения.
    */
    uint8_t start_byte;
    uint16_t blockSize;
    uint8_t command_byte;
    QString message;

    SCommand m_sCommand;

private:
    QTcpSocket m_client;

};

#endif // ACLIENT_H
