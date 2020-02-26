#ifndef WORKER_H
#define WORKER_H

#pragma once

#include <QObject>

class Worker: public QObject
{
Q_OBJECT

public:

    Q_INVOKABLE void addClient(qintptr socketDescriptor);
};

#endif // WORKER_H
