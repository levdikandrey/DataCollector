#include "aclient.h"

#include <cstdint>
#include <iostream>
#include <deque>

#include <QDebug>
#include <QtEndian>
#include <QHostAddress>
#include <QtNetwork>

extern std::deque<uint64_t> listAVP;
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
        uint64_t idAVP;         // ID АВП в БД АВП
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
AClient::AClient(qintptr socketDescriptor, QObject* parent) :
        QObject(parent)
{
    blockSize = 0;

    m_client.setSocketDescriptor(socketDescriptor);

    connect(&m_client, &QTcpSocket::readyRead, this, &AClient::onReadyRead);
    connect(&m_client, &QTcpSocket::disconnected, this, &AClient::onClientDisconnected);
    connect(&m_client, &QTcpSocket::connected, this, &AClient::onClientConnected);

//    connect(&m_world, &QTcpSocket::connected, this, &AClient::sendSocksAnsver);
    connect(&m_world, &QTcpSocket::readyRead, this, &AClient::world2client);
    connect(&m_world,&QTcpSocket::disconnected, this, &AClient::onWorldDisconnected);

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
        listAVP.push_back(command->idAVP);
        sendAnswerAnalysisAVP(command->idAVP);
    }

//    int length;
//    qDebug()<<__PRETTY_FUNCTION__;
//    QDataStream in(&m_client);
//    in.setVersion(QDataStream::Qt_5_12);

//    length=m_client.bytesAvailable();
//    qDebug()<<"length="<<m_client.bytesAvailable();
//    //Если пришла первая часть из посланной сервером информации.
//    if(blockSize == 0)
//    {
//        message="";
//        //Если первая часть меньше того кол-ва информации что определяет размер всего сообщения...
//        if(m_client.bytesAvailable() < (int)sizeof(uint16_t))
//            return;
//        in >> start_byte;
//            qDebug()<<"start_byte="<<start_byte;
//        for(int j=1; j<=m_client.bytesAvailable();j++)
//        {
//            if((start_byte==0x01) || (start_byte==0x02))
//            {
//                break;
//            }
//            else if( ((start_byte!=0x01) || (start_byte!=0x02)) && (length!=0))
//            {
//                in >> start_byte;
//            }
//            else if(((start_byte!=0x01) || (start_byte!=0x02)) && (length==j))
//            {
//                return;
//            }
//        }
//        message+=start_byte;

//        //Получить размер посылаемого сервером сообщения.
//        in >> blockSize;
//        in >> blockSize;
//            qDebug()<<"blockSize="<<blockSize;
//        message+=blockSize;
//    }
//    //Если последующие части вместе взятые меньше, чем определенное сервером кол-во...
//    if(m_client.bytesAvailable() < blockSize)
//        return;

//    for(int i=0; i<blockSize; i++)
//    {
//        in >> command_byte;
//    //        qDebug()<<"command_byte="<<command_byte;
//        message+=command_byte;
//    }
//    qDebug()<<"message.length()="<<message.length();
////	PrintHexQString(message);

//    //Обнулить параметр размера посылаемого сервером сообщения.
//    blockSize = 0;
//    emit receiveMessage(message);
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
        listAVP.push_back(comm->idAVP);
        sendAnswerAnalysisAVP(comm->idAVP);
    }
}

//=========================================================
void AClient::sendAnswerAnalysisAVP(uint32_t idAVP)
{
    answerAnalysis ans;
    ans.command = (uint8_t)command::answerAnalysisAVP;
    ans.length = sizeof(uint32_t);
    ans.idAVP = idAVP;
    m_client.write(reinterpret_cast<char*>(&ans), sizeof(ans));
    m_client.flush();
}

//=========================================================
void AClient::client2world()
{
    m_world.write(m_client.readAll());
}

//=========================================================
void AClient::world2client()
{
    m_client.write(m_world.readAll());
}

//=========================================================
void AClient::onClientDisconnected()
{
    qDebug()<<__PRETTY_FUNCTION__;
    QString name;
    name =  m_client.peerAddress().toString();
    qDebug()<<"Client disconnect IP = "<<name;
    m_world.flush();
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
void AClient::onWorldDisconnected()
{
    m_client.flush();
    done();
}

//=========================================================
void AClient::done()
{
    m_client.close();
    m_world.close();
    deleteLater();
}
