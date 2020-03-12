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

/**
 * @brief The AClient class
 */
class AClient: public QObject
{
Q_OBJECT

public:
    /**
     * @brief AClient
     * @param socketDescriptor
     * @param threadReadQueue
     * @param parent
     */
    AClient(qintptr socketDescriptor, ThreadReadQueue* threadReadQueue, QObject* parent = 0);

public slots:
    /**
     * @brief onReadyRead
     */
    void onReadyRead();
    /**
     * @brief onClientDisconnected
     */
    void onClientDisconnected();
    /**
     * @brief onClientConnected
     */
    void onClientConnected();
    /**
     * @brief receiveCommand
     * @param command
     */
    void receiveCommand(QString &command);
    /**
     * @brief sendAnswerAnalysisAVP
     * @param command
     */
    void sendAnswerAnalysisAVP(const SCommand &command);

signals:
    /**
    * @brief connected
    */
   void connected();
   /**
    * @brief disconnected
    */
   void disconnected();
   /**
    * @brief receiveMessage
    * @param mess
    */
   void receiveMessage(QString &mess);

private:
   /**
     * @brief done
     */
    void done();
    /**
     * @brief printHex
     * @param request
     */
    void printHex(QByteArray request);

    QString message;
    QByteArray m_request;
    QTcpSocket m_client;

    SCommand m_sCommand;
};

#endif // ACLIENT_H
