#ifndef NETWORK_REQUESTSENDER_H
#define NETWORK_REQUESTSENDER_H

#include "request.h"
#include <QNetworkProxy>

namespace Network
{
    class RequestSender : public QObject
    {
        Q_OBJECT
    public:

        enum RequestError
        {
            NoError,
            TimeoutError
        };

        RequestSender(qint32 maxWaitTime = 35000);
        ~RequestSender();

        void setProxy(const QNetworkProxy& proxy);

        QByteArray get(Request& request);
        QByteArray post(Request& request);
        QByteArray getWhileSuccess(Request& request, int maxCount = 2);
        QByteArray postWhileSuccess(Request& request, int maxCount = 2);

        void setMaxWaitTime(qint32 max);

        qint32 maxWaitTime() const;
        RequestError error() const;

    private:
        QByteArray sendRequest(Request& request, bool getRequest = true);
        QByteArray sendWhileSuccess(Request& request, int maxCount = 2, bool getRequest = true);

    private:
        qint32 _maxWaitTime;
        RequestError _error;
        QNetworkProxy _proxy;
    };
}


#endif // NETWORK_REQUESTSENDER_H
