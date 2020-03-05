#include "aclient.h"
#include "threadreadqueue.h"

#include <cstdint>
#include <iostream>
#include <deque>

#include <QDebug>
#include <QtEndian>
#include <QHostAddress>
#include <QtNetwork>

extern std::deque<SCommand> listAVP;

namespace
{
#pragma pack(push, 1)
    /**
    * @brief The header struct - Заголовок сообщений
    */
    struct header
    {
        uint8_t command;        // Номер команды
        uint16_t length;        // Длина команды. Содержит длинну команды без заголовка
    };
    /**
     * @brief The commandAnalysis struct - Команда. Анализ одного АВП
     */
    struct commandAnalysis : header
    {
        uint64_t idAVP;         // ID АВП в БД АВП
    };
    /**
     * @brief The commandAnalysisManyAPV struct - Команда. Анализ нескольких АВП
     */
    struct commandAnalysisManyAPV : header
    {
        uint16_t coountAVP;     // Количество АВП для анализа
        const char* listAVP;    // Список идентификаторов АВП (coountAVP * 4)
    };
    /**
     * @brief The answerAnalysis struct - Ответ на команду анализ одного АВП
     */
    struct answerAnalysis : header
    {
        uint64_t idAVP;      // ID АВП в БД АВП
        uint8_t status;     // ответ на команду анализа 1 - все проанализировано, 2 - нет рецензий
    };
#pragma pack(pop)

    enum class command
    {
        analysisAVP = 0x01,         // Анализ одного АВП
        analysisManyAVP = 0x02,     // Анализ нескольких АВП
        answerAnalysisAVP = 0x03    // Ответ на команду 0х01
    };
}

#define PrintHexQString(s)  for(int i=0;i<s.length();i++){printf(" 0x%02X", (unsigned char)s[i].toAscii());} cout<<endl;
//=========================================================
AClient::AClient(qintptr socketDescriptor, ThreadReadQueue* threadReadQueue, QObject* parent) :
        QObject(parent)
{
//    qDebug()<<__PRETTY_FUNCTION__<<" m_TRQ = "<<threadReadQueue;
    blockSize = 0;

    m_client.setSocketDescriptor(socketDescriptor);

    connect(&m_client, &QTcpSocket::readyRead, this, &AClient::onReadyRead);
    connect(&m_client, &QTcpSocket::disconnected, this, &AClient::onClientDisconnected);
    connect(&m_client, &QTcpSocket::connected, this, &AClient::onClientConnected);

    connect(threadReadQueue, &ThreadReadQueue::sendAnswerAnalysisAVP, this, &AClient::sendAnswerAnalysisAVP);
    connect(this,&AClient::receiveMessage, this, &AClient::receiveCommand);
}

//=========================================================
void AClient::onReadyRead()
{

    qDebug()<<__PRETTY_FUNCTION__;
    QByteArray request = m_client.readAll();
    qDebug()<<"request.length() ="<<request.length()<<" HEX: "<<request.toHex();
    for(int i=0; i<request.length(); i++)
    {
        printf(" 0x%02X", (unsigned char)request[i]);
    }
    std::cout<<std::endl;

    header *headerCommand = reinterpret_cast<header*>(request.data());
    if(headerCommand->command == 1)
    {
        commandAnalysis* command = reinterpret_cast<commandAnalysis*>(request.data());
        qDebug()<<"command->command: "<<command->command<<" command->length:"<< command->length<<"command->idAVP:"<<command->idAVP;
        m_sCommand.idAVP = command->idAVP;
        m_sCommand.client = this;
        listAVP.push_back(m_sCommand);
    }

}

//=========================================================
void AClient::receiveCommand(QString &command)
{
    qDebug()<<"request.length() ="<<command.length();
    for(int i=0; i<command.length(); i++)
    {
        printf(" 0x%02X", (unsigned char)command[i].toLatin1());
    }
    std::cout<<std::endl;

    header *headerCommand = reinterpret_cast<header*>(&command);
    if(headerCommand->command == 1)
    {
        commandAnalysis* comm = reinterpret_cast<commandAnalysis*>(command.data());
        qDebug()<<"command->command: "<<comm->command<<" command->length:"<< comm->length<<"command->idAVP:"<<comm->idAVP;
        m_sCommand.idAVP = comm->idAVP;
        m_sCommand.client = this;
        m_sCommand.answerState = 0x00;
        listAVP.push_back(m_sCommand);
    }
}

//=========================================================
void AClient::sendAnswerAnalysisAVP(const SCommand &command)
{
    if(command.client == this)
    {
        qDebug()<<__PRETTY_FUNCTION__;
        QString name;
        name =  m_client.peerAddress().toString();

        answerAnalysis ans;
        ans.command = (uint8_t)command::answerAnalysisAVP;
        ans.length = sizeof(uint64_t)+1;
        ans.idAVP = command.idAVP;
        ans.status = command.answerState;
        qDebug()<<"Send answer Client IP = "<<name<< "length packet = "<<sizeof(ans);
        m_client.write(reinterpret_cast<char*>(&ans), sizeof(ans));
        m_client.flush();
    }
}

//=========================================================
void AClient::onClientDisconnected()
{
    qDebug()<<__PRETTY_FUNCTION__;
    QString name;
    name =  m_client.peerAddress().toString();
    qDebug()<<"Client disconnect IP = "<<name;
    m_client.flush();
    done();
}

//=========================================================
void AClient::onClientConnected()
{
    qDebug()<<__PRETTY_FUNCTION__;
    QString name;
    name =  m_client.peerAddress().toString();
    qDebug()<<"Подключился клиент IP = "<<name;
}

//=========================================================
void AClient::done()
{
    m_client.close();
    deleteLater();
}
