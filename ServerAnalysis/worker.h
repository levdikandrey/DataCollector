#ifndef WORKER_H
#define WORKER_H

#pragma once

#include "threadreadqueue.h"

#include <QObject>

class Worker: public QObject
{
Q_OBJECT

public:

    Q_INVOKABLE void addClient(qintptr socketDescriptor, ThreadReadQueue* threadReadQueue);
};

#endif // WORKER_H
