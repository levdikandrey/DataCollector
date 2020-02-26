#ifndef ACLIENT_H
#define ACLIENT_H

#pragma once

#include <QObject>
#include <QTcpSocket>

class AClient: public QObject
{
Q_OBJECT

public:
    AClient(qintptr socketDescriptor, QObject* parent = 0);

public slots:
    void onReadyRead();
    void onClientDisconnected();
    void onClientConnected();

    void client2world();
    void world2client();
    void sendAnswerAnalysisAVP(uint32_t idAVP);
    void onWorldDisconnected();
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

private:
    QTcpSocket m_client;
    QTcpSocket m_world;
};

#endif // ACLIENT_H
