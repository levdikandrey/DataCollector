#ifndef ACLIENT_H
#define ACLIENT_H

#pragma once

#include <QObject>
#include <QTcpSocket>

class AClient;

struct SCommand
{
    uint64_t idAVP;
    AClient *client;
};

class AClient: public QObject
{
Q_OBJECT

public:
    AClient(qintptr socketDescriptor, QObject* parent = 0);
    void sendAnswerAnalysisAVP(uint64_t idAVP);

public slots:
    void onReadyRead();
    void onClientDisconnected();
    void onClientConnected();

    void receiveCommand(QString &command);

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