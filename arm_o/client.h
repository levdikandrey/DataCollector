#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include "aprotocol.h"

class QTcpSocket;
class QTimer;

class Client : public QObject
{
   Q_OBJECT
public:
   /**
    * @brief Client::Client - Конструктор.
    * @param server_ip_address - ip-адресс сервера
    * @param server_ip_port - порт сервера
    * @param parent - указатель на родительский объект.
    */
   Client(QString server_ip_address, int server_ip_port, QObject *parent = NULL);
   /**
    * @brief ~Client - деструктор
    */
   virtual ~Client();
   /**
    * @brief reconnect - Функция активации процесса переподключения к серверу.
    */
   void reconnect();
   /**
    * @brief getState - Функция возврата состояния сетевого соединения.
    * @return
    */
   QString getState();
   /**
    * @brief sendMessage -Функция отправки сообщения на сервер.
    * @param message
    */
   void sendMessage(QString message);
   void sendCommandAnalysisAVP(uint64_t idAVP);
//   void sendCommandAnalysisAVPList(uint64_t idAVP);

signals:
   void connected();
   void disconnected();
   void receiveMessage(QString mess_str);
   void state(QString state_str);

private:
   QTcpSocket *tcpSocket;
   QTimer *timer;

   QString serverIPAddress;
   int serverIPPort;

   /**
        Размер посылаемого сервером сообщения.
   */
   quint8 start_byte;
   quint8 blockSize;
   quint8 command_byte;
   QString message;

public slots:
   /**
        Слот реагирования на сигнал о подключении к серверу.
   */
   void slotConnected();
   /**
        Слот реагирования на сигнал об отключении от сервера.
   */
   void slotDisconnected();
   /**
        Слот реагирования на сигнал поступления новой информации от сервера.
   */
   void slotReadyRead();
   /**
        Слот сработки таймера.
   */
   void slotTimerTimeout();

};

#endif
