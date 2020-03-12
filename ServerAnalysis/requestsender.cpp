#include "requestsender.h"

#include <QTimer>
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>

//#define NETWORK_SHOW_SEND_REQUESTS

namespace Network
{
    //=========================================================
    RequestSender::RequestSender(qint32 maxWaitTime)
    {
        setMaxWaitTime(maxWaitTime);
        _error = NoError;
    }

    //=========================================================
    RequestSender::~RequestSender()
    {

    }

    //=========================================================
    void RequestSender::setProxy(const QNetworkProxy& proxy)
    {
        _proxy = proxy;
    }

    //=========================================================
    QByteArray RequestSender::get(Request& request)
    {
        return sendRequest(request, true);
    }

    //=========================================================
    QByteArray RequestSender::post(Request& request)
    {
        return sendRequest(request, false);
    }

    //=========================================================
    QByteArray RequestSender::getWhileSuccess(Request& request, int maxCount /*= 2*/)
    {
        return sendWhileSuccess(request, maxCount, true);
    }

    //=========================================================
    QByteArray RequestSender::postWhileSuccess(Request& request, int maxCount /*= 2*/)
    {
        return sendWhileSuccess(request, maxCount, false);
    }

    //=========================================================
    void RequestSender::setMaxWaitTime(qint32 max)
    {
        _maxWaitTime = max;
    }

    //=========================================================
    qint32 RequestSender::maxWaitTime() const
    {
        return _maxWaitTime;
    }

    //=========================================================
    RequestSender::RequestError RequestSender::error() const
    {
        return _error;
    }

    //=========================================================
    QByteArray RequestSender::sendRequest(Request& request, bool getRequest /*= true*/)
    {
        QTimer timer;
        timer.setInterval(_maxWaitTime);
        timer.setSingleShot(true);

        QEventLoop loop;
        QSharedPointer<QNetworkAccessManager> manager(new QNetworkAccessManager);
        manager->setProxy(_proxy);

        QNetworkReply* reply = getRequest ? manager->get(request.request()) :
                                            manager->post(request.request(false), request.data());

#if defined(NETWORK_SHOW_SEND_REQUESTS)
        if (getRequest)
            qDebug() << "[GET] " <<  request.request().url().toString();
        else
            qDebug() << "[POST]" << request.request(false).url().toString() << request.data();
#endif

        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        QObject::connect(&timer, &QTimer::timeout, reply, &QNetworkReply::abort);

        timer.start();
        loop.exec();

        QByteArray data;

        if (reply->isFinished() && reply->error() == QNetworkReply::NoError)
        {
            data = reply->readAll();
            _error = RequestSender::NoError;
        }
        else
        {
            _error = RequestSender::TimeoutError;
        }
        reply->deleteLater();

#if defined(NETWORK_SHOW_SEND_REQUESTS)
        qDebug() << "[ANSWER]" << data;
#endif

        return data;
    }

    //=========================================================
    QByteArray RequestSender::sendWhileSuccess(Request& request, int maxCount /*= 2*/, bool getRequest /*= true*/)
    {
        if (maxCount < 0)
            throw QString(QString(__LINE__) + " " __FILE__);//ToDo

        int c = 0;
        QByteArray answer;

        while (c < maxCount)
        {
            c++;
            answer = getRequest ? get(request) : post(request);

            if (error() == NoError)
                break;

            qDebug() << "Ошибка при отправке запроса. Код ошибки - " << error() << ". Повторная отправка запроса через 2 секунды\n";
            QThread::currentThread()->msleep(2000);
        }

        return answer;
    }

}
